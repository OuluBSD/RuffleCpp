/*
 * C++ header for character functionality
 * This replaces the functionality of core/src/character.rs
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>
#include <variant>
#include <optional>
#include <vector>
#include <string>
#include <functional>

namespace ruffle {

// Forward declarations
class EditText;
class Graphic;
class MovieClip;
class BitmapCharacter;
class Avm1Button;
class Avm2Button;
class Font;
class MorphShape;
class Text;
class Video;
class SoundHandle;
class BinaryData;
class BitmapClass;
class RenderBackend;
class BitmapHandle;
class BitmapSize;
class DefineBitsLossless;

// Enum for character types
enum class CharacterType {
    EDIT_TEXT,
    GRAPHIC,
    MOVIE_CLIP,
    BITMAP,
    AVM1_BUTTON,
    AVM2_BUTTON,
    FONT,
    MORPH_SHAPE,
    TEXT,
    SOUND,
    VIDEO,
    BINARY_DATA
};

// Character class representing different types of display objects
class Character {
private:
    std::variant<
        std::shared_ptr<EditText>,
        std::shared_ptr<Graphic>,
        std::shared_ptr<MovieClip>,
        std::shared_ptr<BitmapCharacter>,
        std::shared_ptr<Avm1Button>,
        std::shared_ptr<Avm2Button>,
        std::shared_ptr<Font>,
        std::shared_ptr<MorphShape>,
        std::shared_ptr<Text>,
        std::shared_ptr<SoundHandle>,
        std::shared_ptr<Video>,
        std::shared_ptr<BinaryData>
    > data_;

public:
    // Constructors for different character types
    explicit Character(std::shared_ptr<EditText> edit_text) : data_(std::move(edit_text)) {}
    explicit Character(std::shared_ptr<Graphic> graphic) : data_(std::move(graphic)) {}
    explicit Character(std::shared_ptr<MovieClip> movie_clip) : data_(std::move(movie_clip)) {}
    explicit Character(std::shared_ptr<BitmapCharacter> bitmap) : data_(std::move(bitmap)) {}
    explicit Character(std::shared_ptr<Avm1Button> avm1_button) : data_(std::move(avm1_button)) {}
    explicit Character(std::shared_ptr<Avm2Button> avm2_button) : data_(std::move(avm2_button)) {}
    explicit Character(std::shared_ptr<Font> font) : data_(std::move(font)) {}
    explicit Character(std::shared_ptr<MorphShape> morph_shape) : data_(std::move(morph_shape)) {}
    explicit Character(std::shared_ptr<Text> text) : data_(std::move(text)) {}
    explicit Character(std::shared_ptr<SoundHandle> sound) : data_(std::move(sound)) {}
    explicit Character(std::shared_ptr<Video> video) : data_(std::move(video)) {}
    explicit Character(std::shared_ptr<BinaryData> binary_data) : data_(std::move(binary_data)) {}

    // Getters for different character types
    std::shared_ptr<EditText> as_edit_text() const {
        if (auto ptr = std::get_if<std::shared_ptr<EditText>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<Graphic> as_graphic() const {
        if (auto ptr = std::get_if<std::shared_ptr<Graphic>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<MovieClip> as_movie_clip() const {
        if (auto ptr = std::get_if<std::shared_ptr<MovieClip>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<BitmapCharacter> as_bitmap() const {
        if (auto ptr = std::get_if<std::shared_ptr<BitmapCharacter>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<Avm1Button> as_avm1_button() const {
        if (auto ptr = std::get_if<std::shared_ptr<Avm1Button>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<Avm2Button> as_avm2_button() const {
        if (auto ptr = std::get_if<std::shared_ptr<Avm2Button>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<Font> as_font() const {
        if (auto ptr = std::get_if<std::shared_ptr<Font>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<MorphShape> as_morph_shape() const {
        if (auto ptr = std::get_if<std::shared_ptr<MorphShape>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<Text> as_text() const {
        if (auto ptr = std::get_if<std::shared_ptr<Text>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<SoundHandle> as_sound() const {
        if (auto ptr = std::get_if<std::shared_ptr<SoundHandle>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<Video> as_video() const {
        if (auto ptr = std::get_if<std::shared_ptr<Video>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    std::shared_ptr<BinaryData> as_binary_data() const {
        if (auto ptr = std::get_if<std::shared_ptr<BinaryData>>(&data_)) {
            return *ptr;
        }
        return nullptr;
    }

    // Get the character type
    CharacterType type() const {
        return std::visit([](const auto& ptr) -> CharacterType {
            using T = std::decay_t<decltype(ptr)>;
            if constexpr (std::is_same_v<T, std::shared_ptr<EditText>>) {
                return CharacterType::EDIT_TEXT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Graphic>>) {
                return CharacterType::GRAPHIC;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MovieClip>>) {
                return CharacterType::MOVIE_CLIP;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<BitmapCharacter>>) {
                return CharacterType::BITMAP;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Avm1Button>>) {
                return CharacterType::AVM1_BUTTON;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Avm2Button>>) {
                return CharacterType::AVM2_BUTTON;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Font>>) {
                return CharacterType::FONT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<MorphShape>>) {
                return CharacterType::MORPH_SHAPE;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Text>>) {
                return CharacterType::TEXT;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<SoundHandle>>) {
                return CharacterType::SOUND;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<Video>>) {
                return CharacterType::VIDEO;
            } else if constexpr (std::is_same_v<T, std::shared_ptr<BinaryData>>) {
                return CharacterType::BINARY_DATA;
            } else {
                return CharacterType::EDIT_TEXT; // Default case
            }
        }, data_);
    }
};

// Compressed bitmap enum
class CompressedBitmap {
public:
    enum class Type {
        JPEG,
        LOSSLESS
    };

private:
    Type type_;
    std::vector<uint8_t> jpeg_data_;
    std::optional<std::vector<uint8_t>> alpha_data_;
    uint32_t width_;
    uint32_t height_;
    DefineBitsLossless lossless_data_;

public:
    // Constructor for JPEG
    CompressedBitmap(const std::vector<uint8_t>& data, 
                    std::optional<std::vector<uint8_t>> alpha,
                    uint32_t width, uint32_t height)
        : type_(Type::JPEG), jpeg_data_(data), alpha_data_(std::move(alpha)),
          width_(width), height_(height) {}

    // Constructor for lossless
    explicit CompressedBitmap(const DefineBitsLossless& lossless)
        : type_(Type::LOSSLESS), lossless_data_(lossless) {}

    Type type() const { return type_; }
    uint32_t width() const { return width_; }
    uint32_t height() const { return height_; }

    const std::vector<uint8_t>& jpeg_data() const { return jpeg_data_; }
    const std::optional<std::vector<uint8_t>>& alpha_data() const { return alpha_data_; }
    const DefineBitsLossless& lossless_data() const { return lossless_data_; }

    // Get the size of the bitmap
    BitmapSize size() const {
        if (type_ == Type::JPEG) {
            return BitmapSize{width_, height_};
        } else {
            return BitmapSize{
                static_cast<uint32_t>(lossless_data_.width),
                static_cast<uint32_t>(lossless_data_.height)
            };
        }
    }

    // Decode the bitmap
    std::shared_ptr<RenderBitmap> decode() const {
        if (type_ == Type::JPEG) {
            // In a real implementation, this would decode the JPEG data
            return decode_jpeg(jpeg_data_, alpha_data_);
        } else {
            // In a real implementation, this would decode the lossless data
            return decode_lossless(lossless_data_);
        }
    }

private:
    // Helper methods for decoding
    std::shared_ptr<RenderBitmap> decode_jpeg(const std::vector<uint8_t>& data,
                                            const std::optional<std::vector<uint8_t>>& alpha) const {
        // In a real implementation, this would use the actual decoder
        return std::make_shared<RenderBitmap>();
    }

    std::shared_ptr<RenderBitmap> decode_lossless(const DefineBitsLossless& data) const {
        // In a real implementation, this would use the actual decoder
        return std::make_shared<RenderBitmap>();
    }
};

// Bitmap character class
class BitmapCharacter {
private:
    CompressedBitmap compressed_;
    std::optional<BitmapHandle> handle_;
    std::shared_ptr<BitmapClass> avm2_class_;

public:
    explicit BitmapCharacter(CompressedBitmap compressed)
        : compressed_(std::move(compressed)), avm2_class_(nullptr) {}

    static std::shared_ptr<BitmapCharacter> create(CompressedBitmap compressed) {
        return std::make_shared<BitmapCharacter>(std::move(compressed));
    }

    const CompressedBitmap& compressed() const { return compressed_; }
    std::shared_ptr<BitmapClass> avm2_class() const { return avm2_class_; }

    void set_avm2_class(std::shared_ptr<BitmapClass> bitmap_class) {
        avm2_class_ = std::move(bitmap_class);
    }

    // Get the bitmap handle for rendering
    std::optional<BitmapHandle> bitmap_handle(RenderBackend* backend) {
        if (handle_.has_value()) {
            return handle_.value();
        }

        auto decoded = compressed_.decode();
        if (!decoded) {
            return std::nullopt;
        }

        auto new_handle = backend->register_bitmap(decoded);
        if (new_handle) {
            handle_ = new_handle.value();
            return handle_.value();
        }

        return std::nullopt;
    }
};

} // namespace ruffle

#endif // CHARACTER_H