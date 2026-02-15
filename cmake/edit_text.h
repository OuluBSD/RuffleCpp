/*
 * C++ header for editable text functionality
 * This replaces the functionality of core/src/display_object/edit_text.rs
 */

#ifndef EDIT_TEXT_H
#define EDIT_TEXT_H

#include "display_object.h"
#include "display_object/interactive.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "font.h"
#include "html.h"
#include "string.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <functional>

namespace ruffle {

// Forward declarations
class Avm1Object;
class Avm2StageObject;
class UpdateContext;
class RenderContext;
class TextFormat;
class Layout;
class LayoutBox;
class StyleSheet;
class MouseCursor;

// Enum for text field types
enum class TextFieldType {
    DYNAMIC,
    INPUT
};

// Enum for text alignment
enum class TextAlign {
    LEFT,
    RIGHT,
    CENTER,
    JUSTIFY
};

// Enum for border type
enum class BorderType {
    NONE,
    SINGLE_LINE
};

// Enum for embed font status
enum class EmbedStatus {
    NOT_EMBEDDED,
    EMBEDDED_NO_OUTLINE,
    EMBEDDED_WITH_OUTLINE
};

// Structure for text field properties
struct EditTextProperties {
    std::string initial_text;
    std::string variable_name;
    std::string border_color;
    std::string background_color;
    std::string text_color;
    std::string font_name;
    double font_height;
    bool is_html;
    bool is_word_wrap;
    bool is_multiline;
    bool is_password;
    bool is_read_only;
    bool is_auto_size;
    bool has_border;
    bool has_layout;
    bool is_selectable;
    bool is_device_font;
    TextFieldType field_type;
    TextAlign align;
    BorderType border_type;
    EmbedStatus embed_status;
    
    EditTextProperties() 
        : font_height(12.0), is_html(false), is_word_wrap(false), is_multiline(false),
          is_password(false), is_read_only(false), is_auto_size(false), 
          has_border(false), has_layout(false), is_selectable(true), 
          is_device_font(true), field_type(TextFieldType::DYNAMIC),
          align(TextAlign::LEFT), border_type(BorderType::NONE),
          embed_status(EmbedStatus::NOT_EMBEDDED) {}
};

// Editable text display object
class EditText : public InteractiveObject {
private:
    EditTextProperties properties_;
    std::shared_ptr<Layout> layout_;
    std::shared_ptr<StyleSheet> style_sheet_;
    std::vector<Avm1TextFieldBinding> text_field_bindings_;
    std::shared_ptr<Font> font_;
    std::string text_;
    std::string html_text_;
    std::vector<std::shared_ptr<TextFormat>> text_formats_;
    int max_characters_;
    int caret_index_;
    std::pair<int, int> selection_;
    bool is_focused_;
    bool is_dragged_;
    bool is_render_valid_;
    std::vector<std::shared_ptr<DisplayObject>> children_;

public:
    EditText(uint16_t id, EditTextProperties props)
        : InteractiveObject(DisplayObjectType::EDIT_TEXT, id), 
          properties_(std::move(props)), max_characters_(0), 
          caret_index_(-1), selection_({-1, -1}), is_focused_(false),
          is_dragged_(false), is_render_valid_(false) {}

    // Static factory method
    static std::shared_ptr<EditText> create(uint16_t id, EditTextProperties props) {
        return std::make_shared<EditText>(id, std::move(props));
    }

    // Getters
    const std::string& text() const { return text_; }
    const std::string& html_text() const { return html_text_; }
    const std::vector<std::shared_ptr<TextFormat>>& text_formats() const { return text_formats_; }
    int max_characters() const { return max_characters_; }
    int caret_index() const { return caret_index_; }
    std::pair<int, int> selection() const { return selection_; }
    bool is_focused() const { return is_focused_; }
    bool is_dragged() const { return is_dragged_; }
    bool is_html() const { return properties_.is_html; }
    bool is_word_wrap() const { return properties_.is_word_wrap; }
    bool is_multiline() const { return properties_.is_multiline; }
    bool is_password() const { return properties_.is_password; }
    bool is_read_only() const { return properties_.is_read_only; }
    bool is_auto_size() const { return properties_.is_auto_size; }
    bool has_border() const { return properties_.has_border; }
    bool is_selectable() const { return properties_.is_selectable; }
    TextFieldType field_type() const { return properties_.field_type; }
    TextAlign align() const { return properties_.align; }
    std::shared_ptr<Font> font() const { return font_; }
    std::shared_ptr<StyleSheet> style_sheet() const { return style_sheet_; }
    const std::vector<Avm1TextFieldBinding>& text_field_bindings() const { return text_field_bindings_; }

    // Setters
    void set_text(const std::string& text) { 
        text_ = text; 
        is_render_valid_ = false; 
        if (auto obj = object1()) {
            obj->set_property("text", std::make_shared<Value>(text), nullptr);
        }
    }
    
    void set_html_text(const std::string& html) { 
        html_text_ = html; 
        properties_.is_html = true;
        is_render_valid_ = false; 
    }
    
    void set_max_characters(int max_chars) { max_characters_ = max_chars; }
    void set_caret_index(int index) { caret_index_ = index; }
    void set_selection(int start, int end) { selection_ = {start, end}; }
    void set_focused(bool focused) { is_focused_ = focused; }
    void set_dragged(bool dragged) { is_dragged_ = dragged; }
    void set_font(std::shared_ptr<Font> font) { font_ = std::move(font); }
    void set_style_sheet(std::shared_ptr<StyleSheet> sheet) { style_sheet_ = std::move(sheet); }
    
    void set_word_wrap(bool wrap) { 
        properties_.is_word_wrap = wrap; 
        is_render_valid_ = false;
    }
    
    void set_multiline(bool multiline) { 
        properties_.is_multiline = multiline; 
        is_render_valid_ = false;
    }
    
    void set_password(bool password) { 
        properties_.is_password = password; 
        is_render_valid_ = false;
    }
    
    void set_read_only(bool read_only) { 
        properties_.is_read_only = read_only; 
    }
    
    void set_auto_size(bool auto_size) { 
        properties_.is_auto_size = auto_size; 
        is_render_valid_ = false;
    }
    
    void set_selectable(bool selectable) { 
        properties_.is_selectable = selectable; 
    }
    
    void set_field_type(TextFieldType type) { 
        properties_.field_type = type; 
    }
    
    void set_align(TextAlign align) { 
        properties_.align = align; 
        is_render_valid_ = false;
    }

    // Add a text field binding
    void add_text_field_binding(Avm1TextFieldBinding binding) {
        text_field_bindings_.push_back(std::move(binding));
    }

    // Remove all text field bindings
    void clear_text_field_bindings() {
        text_field_bindings_.clear();
    }

    // Check if text control is applicable
    bool is_text_control_applicable(TextControlCode code, std::shared_ptr<UpdateContext> context) const {
        switch (code) {
            case TextControlCode::CUT:
            case TextControlCode::COPY:
                return !selection_.first == selection_.second; // Has selection
            case TextControlCode::PASTE:
                return field_type() == TextFieldType::INPUT && !is_read_only();
            case TextControlCode::DELETE:
                return field_type() == TextFieldType::INPUT && !is_read_only();
            case TextControlCode::SELECT_ALL:
                return true; // Always applicable
            default:
                return false;
        }
    }

    // Apply text control operation
    void apply_text_control(TextControlCode code, std::shared_ptr<UpdateContext> context) {
        switch (code) {
            case TextControlCode::CUT:
                if (field_type() == TextFieldType::INPUT && !is_read_only()) {
                    // Cut selected text to clipboard
                    auto selected_text = get_selected_text();
                    // In a real implementation, this would copy to clipboard and delete selection
                    delete_selected_text();
                }
                break;
            case TextControlCode::COPY:
                // Copy selected text to clipboard
                auto selected_text = get_selected_text();
                // In a real implementation, this would copy to clipboard
                break;
            case TextControlCode::PASTE:
                if (field_type() == TextFieldType::INPUT && !is_read_only()) {
                    // Paste from clipboard
                    // In a real implementation, this would paste from clipboard
                }
                break;
            case TextControlCode::DELETE:
                if (field_type() == TextFieldType::INPUT && !is_read_only()) {
                    delete_selected_text();
                }
                break;
            case TextControlCode::SELECT_ALL:
                select_all();
                break;
        }
    }

    // Get selected text
    std::string get_selected_text() const {
        if (selection_.first != selection_.second && 
            selection_.first >= 0 && selection_.first < static_cast<int>(text_.size()) &&
            selection_.second >= 0 && selection_.second <= static_cast<int>(text_.size())) {
            
            int start = std::min(selection_.first, selection_.second);
            int end = std::max(selection_.first, selection_.second);
            return text_.substr(start, end - start);
        }
        return "";
    }

    // Delete selected text
    void delete_selected_text() {
        if (selection_.first != selection_.second && 
            selection_.first >= 0 && selection_.first < static_cast<int>(text_.size()) &&
            selection_.second >= 0 && selection_.second <= static_cast<int>(text_.size())) {
            
            int start = std::min(selection_.first, selection_.second);
            int end = std::max(selection_.first, selection_.second);
            
            text_.erase(start, end - start);
            caret_index_ = start;
            selection_ = {start, start};
            is_render_valid_ = false;
        }
    }

    // Select all text
    void select_all() {
        selection_ = {0, static_cast<int>(text_.size())};
    }

    // Handle events for the text field
    ClipEventResult handle_event(std::shared_ptr<UpdateContext> context, ClipEvent event) override {
        switch (event.type()) {
            case EventType::KEY_DOWN:
                if (field_type() == TextFieldType::INPUT && !is_read_only()) {
                    // Handle key input
                    handle_key_input(context, event.key_code());
                    return ClipEventResult::HANDLED;
                }
                break;
            case EventType::KEY_UP:
                return ClipEventResult::HANDLED;
            case EventType::PRESS:
                if (is_selectable()) {
                    // Handle mouse press for selection
                    handle_mouse_press(context, event.mouse_x(), event.mouse_y());
                    return ClipEventResult::HANDLED;
                }
                break;
            case EventType::RELEASE:
                if (is_selectable()) {
                    // Handle mouse release
                    handle_mouse_release(context, event.mouse_x(), event.mouse_y());
                    return ClipEventResult::HANDLED;
                }
                break;
            default:
                break;
        }
        return InteractiveObject::handle_event(context, event);
    }

    // Get mouse cursor for this text field
    MouseCursor mouse_cursor(std::shared_ptr<UpdateContext> context) const override {
        if (field_type() == TextFieldType::INPUT && !is_read_only()) {
            return MouseCursor::IBEAM; // Text cursor for input fields
        } else {
            return MouseCursor::ARROW; // Arrow cursor for dynamic text
        }
    }

    // Render the text field
    void render_self(std::shared_ptr<RenderContext> context) override {
        if (!is_render_valid_) {
            // Re-layout the text if needed
            update_layout();
            is_render_valid_ = true;
        }

        // Render the background if needed
        if (properties_.has_border) {
            // Render border/background
            // This would use the context's renderer to draw the background
        }

        // Render the text content
        if (layout_) {
            layout_->render(context);
        }
    }

    // Update the text layout
    void update_layout() {
        // In a real implementation, this would create or update the layout
        // based on the current text, font, and formatting properties
        // For now, we'll just mark that the layout needs to be updated
    }

    // Get bounds of the text field
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        // Calculate bounds based on text content and properties
        // This would be more complex in a real implementation
        return Rectangle<Twips>(
            Twips(0),
            Twips(0),
            Twips::from_pixels(properties_.font_height * text_.length()), // Approximate width
            Twips::from_pixels(properties_.font_height * 1.2) // Approximate height
        );
    }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return true; // Text fields can be used as masks
    }

    // Post instantiation setup
    void post_instantiation(
        std::shared_ptr<UpdateContext> context,
        std::shared_ptr<Object> init_object,
        Instantiator instantiated_by,
        bool run_frame) override {
        
        InteractiveObject::post_instantiation(context, init_object, instantiated_by, run_frame);

        if (movie()->is_action_script_3()) {
            set_default_instance_name(context);

            if (!instantiated_by.is_avm()) {
                // Create AVM2 object for this text field
                auto text_field_cls = context->avm2->classes().text_field;
                auto text_field_obj = Avm2StageObject::for_display_object(
                    context->gc(), shared_from_this(), text_field_cls);

                // Call the constructor
                try {
                    text_field_cls->call_constructor(
                        text_field_obj, 
                        {}, 
                        context);
                } catch (const std::exception& e) {
                    printf("Error running AVM2 construction for text field: %s\n", e.what());
                }

                set_object2(context, text_field_obj);
            }

            on_construction_complete(context);
        }

        // Set initial text if provided
        if (!properties_.initial_text.empty()) {
            set_text(properties_.initial_text);
        }

        // Set variable name if provided
        if (!properties_.variable_name.empty()) {
            set_variable_name(context, properties_.variable_name);
        }
    }

    // Set variable name for AVM1 binding
    void set_variable_name(std::shared_ptr<UpdateContext> context, const std::string& var_name) {
        if (auto obj = object1()) {
            obj->set_property("variable", std::make_shared<Value>(var_name), context);
            
            // Add to unbound text fields list for binding
            context->unbound_text_fields.push_back(shared_from_this());
        }
    }

    // AVM1 unload
    void avm1_unload(std::shared_ptr<UpdateContext> context) override {
        // Remove from focus tracker if focused
        if (is_focused_) {
            context->focus_tracker->remove_focus(shared_from_this());
            is_focused_ = false;
        }

        // Remove from any masking relationships
        if (auto maskee = this->maskee()) {
            maskee->set_masker(context->gc(), nullptr, true);
        } else if (auto masker = this->masker()) {
            masker->set_maskee(context->gc(), nullptr, true);
        }

        // Remove text field bindings
        clear_text_field_bindings();

        InteractiveObject::avm1_unload(context);
        set_avm1_removed(true);
    }

    // Cast to specific types
    std::shared_ptr<EditText> as_edit_text() override { 
        return std::static_pointer_cast<EditText>(shared_from_this()); 
    }
    
    bool is_edit_text() const override { return true; }

    // Check if empty
    bool is_empty() const override {
        return text_.empty();
    }

private:
    // Handle key input for input text fields
    void handle_key_input(std::shared_ptr<UpdateContext> context, int key_code) {
        // In a real implementation, this would handle keyboard input
        // adding characters, handling backspace, etc.
        // For now, we'll just mark that we need to update the layout
        is_render_valid_ = false;
    }

    // Handle mouse press for text selection
    void handle_mouse_press(std::shared_ptr<UpdateContext> context, int x, int y) {
        // In a real implementation, this would handle mouse clicks
        // for text selection and caret positioning
        // For now, we'll just set focus
        is_focused_ = true;
        context->focus_tracker->set_focus(shared_from_this());
    }

    // Handle mouse release for text selection
    void handle_mouse_release(std::shared_ptr<UpdateContext> context, int x, int y) {
        // In a real implementation, this would handle mouse releases
        // for text selection
    }
};

} // namespace ruffle

#endif // EDIT_TEXT_H