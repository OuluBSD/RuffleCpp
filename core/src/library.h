// C++ translation of core/src/library.rs
// Symbol library for SWF movies

#ifndef RUFFLE_CORE_LIBRARY_H
#define RUFFLE_CORE_LIBRARY_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include <functional>

// Forward declarations
namespace ruffle {
namespace swf {
    struct Rectangle;
    class Twips;
}
namespace render {
    class RenderBackend;
    class BitmapHandle;
    struct BitmapSize;
    namespace bitmap {
        class BitmapSource;
    }
}
namespace core {
    class AvmString;
    class Font;
    class FontDescriptor;
    class FontQuery;
    enum class FontType;
    enum class DefaultFont;
    class WStr;
    class WString;
    
    namespace avm1 {
        template<typename T>
        class PropertyMap;
    }
    namespace avm2 {
        template<typename GC>
        class Class;
        template<typename GC>
        class Domain;
    }
    namespace backend {
        namespace audio {
            using SoundHandle = uint64_t;
        }
        namespace ui {
            struct FontDefinition;
            class UiBackend;
        }
    }
    namespace display_object {
        class DisplayObject;
        class Bitmap;
        class Graphic;
        class MorphShape;
        class Text;
    }
    namespace character {
        class Character;
    }
    namespace tag_utils {
        class SwfMovie;
    }
}
}

namespace ruffle {
namespace core {

using CharacterId = uint16_t;

/// The mappings between class objects and library characters defined by SymbolClass.
template<typename GC>
class Avm2ClassRegistry {
private:
    // Using std::unordered_map as approximation for WeakValueHashMap
    std::unordered_map<avm2::Class<GC>*, std::pair<std::weak_ptr<tag_utils::SwfMovie>, CharacterId>> class_map_;

public:
    Avm2ClassRegistry() = default;

    /// Retrieve the library symbol for a given AVM2 class object.
    std::optional<std::pair<std::shared_ptr<tag_utils::SwfMovie>, CharacterId>> 
    class_symbol(avm2::Class<GC>* class_def) const {
        auto it = class_map_.find(class_def);
        if (it != class_map_.end()) {
            if (auto movie = it->second.first.lock()) {
                return std::make_pair(movie, it->second.second);
            }
        }
        return std::nullopt;
    }

    /// Associate an AVM2 class definition with a given library symbol.
    void set_class_symbol(avm2::Class<GC>* class_def, 
                         std::shared_ptr<tag_utils::SwfMovie> movie, 
                         CharacterId symbol) {
        // Check if already exists
        auto it = class_map_.find(class_def);
        if (it != class_map_.end()) {
            if (auto existing = it->second.first.lock()) {
                if (existing == movie && symbol != it->second.second) {
                    // Flash allows same class with different symbol IDs, first one wins
                    // Log warning but don't overwrite
                    return;
                }
                // Different movie, ignore
                return;
            }
        }
        class_map_[class_def] = {movie, symbol};
    }
};

/// Symbol library for a single given SWF.
template<typename GC>
class MovieLibrary {
private:
    std::shared_ptr<tag_utils::SwfMovie> swf_;
    std::unordered_map<CharacterId, character::Character> characters_;
    avm1::PropertyMap<CharacterId> export_characters_;
    std::unordered_map<AvmString, CharacterId> imported_assets_;
    std::optional<std::vector<uint8_t>> jpeg_tables_;
    // FontMap would be a custom type
    // Using placeholder
    std::vector<Font> fonts_;
    std::optional<avm2::Domain<GC>> avm2_domain_;

public:
    explicit MovieLibrary(std::shared_ptr<tag_utils::SwfMovie> swf)
        : swf_(std::move(swf)) {}

    /// Registers a character; returns true if successful, or false if a character with
    /// the given ID already exists.
    bool register_character(CharacterId id, const character::Character& character) {
        auto result = characters_.emplace(id, character);
        if (result.second) {
            // Successfully inserted
            // TODO: Register font if character is a font
            return true;
        }
        // Character already exists
        return false;
    }

    /// Registers an export name for a given character ID.
    void register_export(CharacterId id, AvmString export_name) {
        // Debug assert that character exists
        if (!contains_character(id)) {
            // Log error
            return;
        }
        export_characters_.insert(export_name, id, false);
    }

    const std::unordered_map<CharacterId, character::Character>& characters() const {
        return characters_;
    }

    const avm1::PropertyMap<CharacterId>& export_characters() const {
        return export_characters_;
    }

    bool contains_character(CharacterId id) const {
        return characters_.find(id) != characters_.end();
    }

    std::optional<character::Character> character_by_id(CharacterId id) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::optional<std::pair<CharacterId, character::Character>> 
    character_by_export_name(AvmString name) const {
        auto id = export_characters_.get(name, false);
        if (id.has_value()) {
            auto it = characters_.find(id.value());
            if (it != characters_.end()) {
                return std::make_pair(*id, it->second);
            }
        }
        return std::nullopt;
    }

    std::optional<CharacterId> character_id_by_import_name(AvmString name) const {
        auto it = imported_assets_.find(name);
        if (it != imported_assets_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void register_import(AvmString name, CharacterId id) {
        imported_assets_[name] = id;
    }

    /// Instantiates the library item with the given character ID into a display object.
    std::optional<display_object::DisplayObject> instantiate_by_id(
        CharacterId id, void* mc /* Mutation<GC>* */) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            return instantiate_display_object(id, it->second, mc);
        }
        return std::nullopt;
    }

    /// Instantiates the library item with the given export name into a display object.
    std::optional<display_object::DisplayObject> instantiate_by_export_name(
        AvmString export_name, void* mc /* Mutation<GC>* */) const {
        auto result = character_by_export_name(export_name);
        if (result.has_value()) {
            return instantiate_display_object(result->first, result->second, mc);
        }
        return std::nullopt;
    }

    std::optional<Font> get_font(CharacterId id) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            // Check if character is a font
            // TODO: Implement proper type checking
        }
        return std::nullopt;
    }

    std::vector<Font> embedded_fonts() const {
        return fonts_;
    }

    /// Returns the Graphic with the given character ID.
    std::optional<display_object::Graphic> get_graphic(CharacterId id) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            // Check if character is a graphic
            // TODO: Implement proper type checking
        }
        return std::nullopt;
    }

    /// Returns the MorphShape with the given character ID.
    std::optional<display_object::MorphShape> get_morph_shape(CharacterId id) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            // Check if character is a morph shape
            // TODO: Implement proper type checking
        }
        return std::nullopt;
    }

    std::optional<backend::audio::SoundHandle> get_sound(CharacterId id) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            // Check if character is a sound
            // TODO: Implement proper type checking
        }
        return std::nullopt;
    }

    /// Returns the Text with the given character ID.
    std::optional<display_object::Text> get_text(CharacterId id) const {
        auto it = characters_.find(id);
        if (it != characters_.end()) {
            // Check if character is a text
            // TODO: Implement proper type checking
        }
        return std::nullopt;
    }

    void set_jpeg_tables(const std::vector<uint8_t>& data) {
        if (jpeg_tables_.has_value()) {
            // Multiple JPEGTables - log warning
            return;
        }
        if (data.empty()) {
            jpeg_tables_ = std::nullopt;
        } else {
            // TODO: Remove invalid JPEG data
            jpeg_tables_ = data;
        }
    }

    std::optional<const std::vector<uint8_t>&> jpeg_tables() const {
        return jpeg_tables_;
    }

    void set_avm2_domain(avm2::Domain<GC> avm2_domain) {
        avm2_domain_ = std::move(avm2_domain);
    }

    /// Get the AVM2 domain this movie runs under.
    avm2::Domain<GC> avm2_domain() const {
        return avm2_domain_.value();
    }

    std::optional<avm2::Domain<GC>> try_avm2_domain() const {
        return avm2_domain_;
    }

private:
    std::optional<display_object::DisplayObject> instantiate_display_object(
        CharacterId id, const character::Character& character, void* mc) const {
        // TODO: Implement proper instantiation based on character type
        return std::nullopt;
    }
};

/// Bitmap source for movie library
class MovieLibrarySource {
private:
    const void* library_;  // MovieLibrary<GC>*

public:
    explicit MovieLibrarySource(const void* library) : library_(library) {}

    std::optional<render::BitmapSize> bitmap_size(uint16_t id) const;
    std::optional<render::BitmapHandle> bitmap_handle(uint16_t id, render::RenderBackend* backend) const;
};

/// Symbol library for multiple movies.
template<typename GC>
class Library {
private:
    // Using std::unordered_map with weak pointers for movie libraries
    std::unordered_map<std::string, MovieLibrary<GC>> movie_libraries_;
    
    // Font-related caches
    std::vector<Font> device_fonts_;
    std::vector<Font> global_fonts_;
    std::unordered_map<std::string, bool> font_lookup_cache_;
    std::unordered_map<std::string, std::vector<Font>> font_sort_cache_;
    std::unordered_map<DefaultFont, std::vector<std::string>> default_font_names_;
    std::unordered_map<std::tuple<DefaultFont, bool, bool>, std::vector<Font>> default_font_cache_;
    
    Avm2ClassRegistry<GC> avm2_class_registry_;

public:
    static Library empty() {
        return Library();
    }

    std::optional<const MovieLibrary<GC>&> library_for_movie(
        const std::shared_ptr<tag_utils::SwfMovie>& movie) const {
        // TODO: Implement proper lookup
        return std::nullopt;
    }

    MovieLibrary<GC>& library_for_movie_mut(
        const std::shared_ptr<tag_utils::SwfMovie>& movie) {
        // TODO: Implement proper lookup or insertion
        static MovieLibrary<GC> dummy(std::make_shared<tag_utils::SwfMovie>());
        return dummy;
    }

    /// Returns the default Font implementations behind the built in names
    std::vector<Font> default_font(
        DefaultFont name, bool is_bold, bool is_italic,
        const backend::ui::UiBackend* ui, render::RenderBackend* renderer,
        void* gc_context /* Mutation<GC>* */) {
        // Check cache first
        auto key = std::make_tuple(name, is_bold, is_italic);
        auto it = default_font_cache_.find(key);
        if (it != default_font_cache_.end()) {
            return it->second;
        }

        std::vector<Font> result;
        // TODO: Implement font lookup
        default_font_cache_[key] = result;
        return result;
    }

    void set_default_font(DefaultFont font, std::vector<std::string> names) {
        default_font_names_[font] = std::move(names);
        default_font_cache_.clear();
    }

    void register_device_font(void* gc_context, render::RenderBackend* renderer,
                             const backend::ui::FontDefinition& definition);

    const Avm2ClassRegistry<GC>& avm2_class_registry() const {
        return avm2_class_registry_;
    }

    Avm2ClassRegistry<GC>& avm2_class_registry_mut() {
        return avm2_class_registry_;
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_LIBRARY_H
