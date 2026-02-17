// C++ translation of tests/framework/src/options/font.rs
//! Font options for test framework

#ifndef RUFFLE_TESTS_FRAMEWORK_OPTIONS_FONT_H
#define RUFFLE_TESTS_FRAMEWORK_OPTIONS_FONT_H

#include <string>
#include <vector>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
    namespace font {
        enum class FontType;
        class FontQuery;
        enum class DefaultFont;
    }
}
}

namespace ruffle {
namespace tests {
namespace framework {
namespace options {
namespace font {

/// Font options configuration
class FontOptions {
private:
    std::string family_;
    std::string path_;
    bool bold_;
    bool italic_;

public:
    /// Default constructor
    FontOptions()
        : family_()
        , path_()
        , bold_(false)
        , italic_(false)
    {}

    /// Constructor with all parameters
    FontOptions(
        std::string family,
        std::string path,
        bool bold,
        bool italic
    )
        : family_(std::move(family))
        , path_(std::move(path))
        , bold_(bold)
        , italic_(italic)
    {}

    // Getters
    const std::string& family() const { return family_; }
    const std::string& path() const { return path_; }
    bool bold() const { return bold_; }
    bool italic() const { return italic_; }

    // Setters
    void set_family(const std::string& value) { family_ = value; }
    void set_path(const std::string& value) { path_ = value; }
    void set_bold(bool value) { bold_ = value; }
    void set_italic(bool value) { italic_ = value; }

    /// Convert to a FontQuery
    /// @return A FontQuery with the configured options
    core::font::FontQuery to_font_query() const;
};

/// Font sort options configuration
class FontSortOptions {
private:
    std::string family_;
    bool bold_;
    bool italic_;
    std::vector<std::string> sort_;

public:
    /// Default constructor
    FontSortOptions()
        : family_()
        , bold_(false)
        , italic_(false)
        , sort_()
    {}

    /// Constructor with all parameters
    FontSortOptions(
        std::string family,
        bool bold,
        bool italic,
        std::vector<std::string> sort
    )
        : family_(std::move(family))
        , bold_(bold)
        , italic_(italic)
        , sort_(std::move(sort))
    {}

    // Getters
    const std::string& family() const { return family_; }
    bool bold() const { return bold_; }
    bool italic() const { return italic_; }
    const std::vector<std::string>& sort() const { return sort_; }

    // Setters
    void set_family(const std::string& value) { family_ = value; }
    void set_bold(bool value) { bold_ = value; }
    void set_italic(bool value) { italic_ = value; }
    void set_sort(std::vector<std::string> value) { sort_ = std::move(value); }
};

/// Default fonts options configuration
class DefaultFontsOptions {
private:
    std::vector<std::string> sans_;
    std::vector<std::string> serif_;
    std::vector<std::string> typewriter_;
    std::vector<std::string> japanese_gothic_;
    std::vector<std::string> japanese_gothic_mono_;
    std::vector<std::string> japanese_mincho_;

public:
    /// Default constructor
    DefaultFontsOptions()
        : sans_()
        , serif_()
        , typewriter_()
        , japanese_gothic_()
        , japanese_gothic_mono_()
        , japanese_mincho_()
    {}

    /// Constructor with all parameters
    DefaultFontsOptions(
        std::vector<std::string> sans,
        std::vector<std::string> serif,
        std::vector<std::string> typewriter,
        std::vector<std::string> japanese_gothic,
        std::vector<std::string> japanese_gothic_mono,
        std::vector<std::string> japanese_mincho
    )
        : sans_(std::move(sans))
        , serif_(std::move(serif))
        , typewriter_(std::move(typewriter))
        , japanese_gothic_(std::move(japanese_gothic))
        , japanese_gothic_mono_(std::move(japanese_gothic_mono))
        , japanese_mincho_(std::move(japanese_mincho))
    {}

    // Getters
    const std::vector<std::string>& sans() const { return sans_; }
    const std::vector<std::string>& serif() const { return serif_; }
    const std::vector<std::string>& typewriter() const { return typewriter_; }
    const std::vector<std::string>& japanese_gothic() const { return japanese_gothic_; }
    const std::vector<std::string>& japanese_gothic_mono() const { return japanese_gothic_mono_; }
    const std::vector<std::string>& japanese_mincho() const { return japanese_mincho_; }

    // Setters
    void set_sans(std::vector<std::string> value) { sans_ = std::move(value); }
    void set_serif(std::vector<std::string> value) { serif_ = std::move(value); }
    void set_typewriter(std::vector<std::string> value) { typewriter_ = std::move(value); }
    void set_japanese_gothic(std::vector<std::string> value) { japanese_gothic_ = std::move(value); }
    void set_japanese_gothic_mono(std::vector<std::string> value) { japanese_gothic_mono_ = std::move(value); }
    void set_japanese_mincho(std::vector<std::string> value) { japanese_mincho_ = std::move(value); }

    /// Apply the default fonts to a player
    /// @param player The player to apply the fonts to
    void apply(core::Player* player);

private:
    /// Apply a single default font to a player
    /// @param player The player to apply the font to
    /// @param font The default font type
    /// @param names The font names to apply
    void apply_default_font(
        core::Player* player,
        core::font::DefaultFont font,
        const std::vector<std::string>& names
    );
};

} // namespace font
} // namespace options
} // namespace framework
} // namespace tests
} // namespace ruffle

#endif // RUFFLE_TESTS_FRAMEWORK_OPTIONS_FONT_H
