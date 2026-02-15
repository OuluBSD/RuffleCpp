/*
 * C++ header for text display object functionality
 * This replaces the functionality of core/src/display_object/text.rs
 */

#ifndef TEXT_DISPLAY_OBJECT_H
#define TEXT_DISPLAY_OBJECT_H

#include "display_object.h"
#include "display_object/interactive.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "font.h"
#include "swf_movie.h"
#include "matrix.h"
#include "rectangle.h"
#include "twips.h"
#include "wstr.h"
#include <memory>
#include <vector>
#include <optional>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Font;
class TextSnapshot;

// Structure for shared text data between all instances
struct TextShared {
    std::shared_ptr<SwfMovie> swf;
    uint16_t id;
    Rectangle<Twips> bounds;
    Matrix text_transform;
    std::vector<swf::TextRecord> text_blocks;

    TextShared(std::shared_ptr<SwfMovie> movie, 
               uint16_t text_id,
               Rectangle<Twips> bounds_val,
               Matrix transform,
               std::vector<swf::TextRecord> blocks)
        : swf(std::move(movie)), id(text_id), bounds(bounds_val), 
          text_transform(transform), text_blocks(std::move(blocks)) {}
};

// Structure for text data
struct TextData {
    DisplayObjectBase base;
    std::shared_ptr<TextShared> shared;
    TextRenderSettings render_settings;
    std::shared_ptr<Avm2StageObject> avm2_object;

    TextData() : render_settings(TextRenderSettings::default()) {}
};

// Text display object class
class Text : public DisplayObject {
private:
    std::shared_ptr<TextData> data_;

public:
    Text(std::shared_ptr<TextShared> shared_data)
        : DisplayObject(DisplayObjectType::TEXT, shared_data->id), 
          data_(std::make_shared<TextData>()) {
        data_->shared = std::move(shared_data);
    }

    // Create a text object from an SWF tag
    static std::shared_ptr<Text> from_swf_tag(std::shared_ptr<UpdateContext> context,
                                            std::shared_ptr<SwfMovie> swf,
                                            const swf::Text& tag) {
        auto shared = std::make_shared<TextShared>(
            std::move(swf),
            tag.id,
            tag.bounds,
            Matrix(tag.matrix),  // Convert SWF matrix to our Matrix
            tag.records  // Copy text records
        );

        auto text_obj = std::make_shared<Text>(std::move(shared));
        return text_obj;
    }

    // Set the shared data
    void set_shared(std::shared_ptr<UpdateContext> context,
                   std::shared_ptr<TextShared> shared) {
        data_->shared = std::move(shared);
        invalidate_cached_bitmap();
    }

    // Set the render settings
    void set_render_settings(TextRenderSettings settings) {
        data_->render_settings = settings;
        invalidate_cached_bitmap();
    }

    // Get the text content
    std::optional<WString> text(std::shared_ptr<UpdateContext> context) const {
        WString result;
        std::optional<uint16_t> font_id;

        for (const auto& block : data_->shared->text_blocks) {
            if (block.font_id.has_value()) {
                font_id = block.font_id.value();
            }

            if (!font_id.has_value()) {
                continue; // Need a font ID to render text
            }

            auto font = context->library->library_for_movie(movie())->get_font(font_id.value());
            if (!font) {
                continue; // Font not found
            }

            for (const auto& glyph : block.glyphs) {
                if (auto glyph_data = font->get_glyph(glyph.index)) {
                    result.push_char(glyph_data->character());
                }
            }
        }

        return result.empty() ? std::nullopt : std::make_optional(std::move(result));
    }

    // Getters
    uint16_t id() const override { return data_->shared->id; }
    std::shared_ptr<SwfMovie> movie() const override { return data_->shared->swf; }
    std::shared_ptr<Avm2StageObject> object2() const override { return data_->avm2_object; }
    const std::shared_ptr<TextShared>& shared() const { return data_->shared; }
    const TextRenderSettings& render_settings() const { return data_->render_settings; }

    // Setters
    void set_object2(std::shared_ptr<UpdateContext> context, 
                    std::shared_ptr<Avm2StageObject> obj) override {
        data_->avm2_object = std::move(obj);
    }

    // Replace with another text object
    void replace_with(std::shared_ptr<UpdateContext> context, uint16_t id) override {
        if (auto new_text = context->library->library_for_movie_mut(movie())->get_text(id)) {
            set_shared(context, new_text->shared());
        } else {
            printf("PlaceObject: expected text at character ID %d\n", id);
        }
        invalidate_cached_bitmap();
    }

    // Render the text
    void render_self(std::shared_ptr<RenderContext> context) override {
        auto shared = data_->shared;
        
        // Push the text transform to the transform stack
        context->transform_stack.push(Transform{
            shared->text_transform,
            ColorTransform::IDENTITY,
            // In a real implementation, this would handle perspective projection
        });

        // Render each text block
        swf::Color color{0, 0, 0, 0};  // Default color
        uint16_t font_id = 0;
        Twips height = Twips(0);
        Transform transform = Transform::IDENTITY;

        for (const auto& block : shared->text_blocks) {
            if (block.x_offset.has_value()) {
                transform.matrix.tx = block.x_offset.value();
            }
            if (block.y_offset.has_value()) {
                transform.matrix.ty = block.y_offset.value();
            }
            
            color = block.color.value_or(color);
            font_id = block.font_id.value_or(font_id);
            height = block.height.value_or(height);
            
            if (auto font = context->library->library_for_movie(movie())->get_font(font_id)) {
                float scale = static_cast<float>(height.get()) / font->scale();
                transform.matrix.a = scale;
                transform.matrix.d = scale;
                transform.color_transform.set_mult_color(color);
                
                for (const auto& glyph_info : block.glyphs) {
                    if (auto glyph = font->get_glyph(glyph_info.index)) {
                        if (glyph->is_renderable(context)) {
                            context->transform_stack.push(transform);
                            glyph->render(context);
                            context->transform_stack.pop();
                        }
                        
                        transform.matrix.tx += Twips(glyph_info.advance);
                    }
                }
            }
        }
        
        context->transform_stack.pop();
    }

    // Get the bounds of this text object
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        return data_->shared->bounds;
    }

    // Hit test for this text object
    bool hit_test_shape(std::shared_ptr<UpdateContext> context,
                       Point<Twips> point,
                       HitTestOptions options) const override {
        if ((!options.contains(HitTestOptions::SKIP_INVISIBLE) || visible()) &&
            world_bounds(BoundsMode::ENGINE).contains(point)) {
            
            // Texts using "Advanced text rendering" always hit test using their bounding box
            if (data_->render_settings.is_advanced()) {
                return true;
            }

            auto shared = data_->shared;

            // Transform the point into the text's local space
            auto local_matrix_opt = global_to_local_matrix();
            if (!local_matrix_opt.has_value()) {
                return false;
            }
            auto text_matrix_opt = shared->text_transform.inverse();
            if (!text_matrix_opt.has_value()) {
                return false;
            }
            
            Point<Twips> local_point = text_matrix_opt.value() * local_matrix_opt.value() * point;

            uint16_t current_font_id = 0;
            Twips current_height = Twips(0);
            Matrix glyph_matrix = Matrix::IDENTITY;

            for (const auto& block : shared->text_blocks) {
                if (block.x_offset.has_value()) {
                    glyph_matrix.tx = block.x_offset.value();
                }
                if (block.y_offset.has_value()) {
                    glyph_matrix.ty = block.y_offset.value();
                }
                
                current_font_id = block.font_id.value_or(current_font_id);
                current_height = block.height.value_or(current_height);

                if (auto font = context->library->library_for_movie(movie())->get_font(current_font_id)) {
                    float scale = static_cast<float>(current_height.get()) / font->scale();
                    glyph_matrix.a = scale;
                    glyph_matrix.d = scale;
                    
                    for (const auto& glyph_info : block.glyphs) {
                        if (auto glyph = font->get_glyph(glyph_info.index)) {
                            // Transform the point into glyph space and test
                            auto inv_matrix_opt = glyph_matrix.inverse();
                            if (!inv_matrix_opt.has_value()) {
                                return false;
                            }
                            
                            Point<Twips> glyph_point = inv_matrix_opt.value() * local_point;
                            if (glyph->hit_test(glyph_point)) {
                                return true;
                            }

                            glyph_matrix.tx += Twips(glyph_info.advance);
                        }
                    }
                }
            }
        }

        return false;
    }

    // Construct frame for AVM2
    void construct_frame(std::shared_ptr<UpdateContext> context) override {
        if (movie()->is_action_script_3() && !object2()) {
            auto static_text_class = context->avm2->classes().static_text;

            auto object = Avm2StageObject::for_display_object(
                context->gc(), shared_from_this(), static_text_class);
            
            // We don't need to call the initializer method, as AVM2 can't link
            // a custom class to a StaticText, and the initializer method for
            // StaticText itself is a no-op
            set_object2(context, object);

            on_construction_complete(context);
        }
    }

    // Post instantiation setup
    void post_instantiation(std::shared_ptr<UpdateContext> context,
                           std::shared_ptr<Object> init_object,
                           Instantiator instantiated_by,
                           bool run_frame) override {
        DisplayObject::post_instantiation(context, init_object, instantiated_by, run_frame);
        
        if (movie()->is_action_script_3()) {
            set_default_instance_name(context);
        }
    }

    // AVM1 object is always null for text
    std::shared_ptr<Object> object1() const override {
        return nullptr;
    }

    // Cast to specific types
    std::shared_ptr<Text> as_text() override {
        return std::static_pointer_cast<Text>(shared_from_this());
    }
    
    bool is_text() const override { return true; }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return !is_empty(); // Text can be used as mask if not empty
    }

    // Check if this object is empty
    bool is_empty() const override {
        return data_->shared->text_blocks.empty();
    }
};

// Chunk of text in a text snapshot
struct TextSnapshotChunk {
    std::shared_ptr<Text> object;
    WString text;
    size_t global_index;

    TextSnapshotChunk(std::shared_ptr<Text> obj, WString txt, size_t index)
        : object(std::move(obj)), text(std::move(txt)), global_index(index) {}

    // Get a substring of this chunk
    WString sub_string(size_t start, size_t end) const {
        size_t chunk_start = std::max(0, static_cast<int>(start) - static_cast<int>(global_index));
        size_t chunk_end = std::min(text.length(), 
                                   std::max(chunk_start, 
                                           static_cast<size_t>(std::max(0, static_cast<int>(end) - static_cast<int>(global_index)))));
        return text.substr(chunk_start, chunk_end - chunk_start);
    }
};

// Text snapshot class for capturing text content
class TextSnapshot {
private:
    std::vector<TextSnapshotChunk> chunks_;

public:
    TextSnapshot(std::vector<TextSnapshotChunk> chunks) : chunks_(std::move(chunks)) {}

    // Create a text snapshot from a movie clip
    static std::shared_ptr<TextSnapshot> create(std::shared_ptr<UpdateContext> context,
                                              std::shared_ptr<MovieClip> target) {
        std::vector<TextSnapshotChunk> chunks;
        size_t index = 0;

        for (const auto& child : target->children()) {
            if (auto text_obj = child->as_text()) {
                if (auto text_content = text_obj->text(context)) {
                    size_t len = text_content->length();
                    chunks.emplace_back(text_obj, std::move(*text_content), index);
                    index += len;
                }
            }
        }

        return std::make_shared<TextSnapshot>(std::move(chunks));
    }

    // Get the total character count
    size_t count() const {
        size_t total = 0;
        for (const auto& chunk : chunks_) {
            total += chunk.text.length();
        }
        return total;
    }

    // Get text from a range
    WString get_text(int32_t from, int32_t to, bool include_newlines) const {
        size_t text_count = count();
        if (text_count == 0) {
            return WString();
        }

        size_t start = std::min(static_cast<size_t>(std::max(0, static_cast<int>(from))), text_count - 1);
        size_t end = std::min(
            static_cast<size_t>(std::max(0, static_cast<int>(to))),
            std::max(start + 1, text_count)
        );

        // Filter chunks that intersect with the requested range
        std::vector<WString> relevant_chunks;
        for (const auto& chunk : chunks_) {
            if (chunk.global_index < end && (chunk.global_index + chunk.text.length()) > start) {
                relevant_chunks.push_back(chunk.sub_string(start, end));
            }
        }

        WString result;
        if (!relevant_chunks.empty()) {
            result = relevant_chunks[0];
        }

        for (size_t i = 1; i < relevant_chunks.size(); ++i) {
            if (include_newlines) {
                result.push_back('\n');
            }
            result += relevant_chunks[i];
        }

        return result;
    }

    // Find text in the snapshot
    int32_t find_text(int32_t from, const WString& search_text, bool case_sensitive) const {
        if (search_text.empty()) {
            return -1;
        }

        size_t start_idx = static_cast<size_t>(std::max(0, static_cast<int>(from)));
        size_t text_count = count();

        // Build the full text from relevant chunks
        WString full_text;
        for (const auto& chunk : chunks_) {
            if (chunk.global_index + chunk.text.length() > start_idx) {
                size_t chunk_start = std::max(0, static_cast<int>(start_idx) - static_cast<int>(chunk.global_index));
                full_text += chunk.text.substr(chunk_start);
            }
        }

        // Perform the search
        if (case_sensitive) {
            size_t pos = full_text.find(search_text);
            return pos != std::string::npos ? static_cast<int32_t>(pos + start_idx) : -1;
        } else {
            // Case-insensitive search (simplified)
            WString lower_full_text = full_text.to_lower();
            WString lower_search_text = search_text.to_lower();
            size_t pos = lower_full_text.find(lower_search_text);
            return pos != std::string::npos ? static_cast<int32_t>(pos + start_idx) : -1;
        }
    }
};

} // namespace ruffle

#endif // TEXT_DISPLAY_OBJECT_H