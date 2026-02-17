// C++ translation of desktop/src/gui/locale.rs
// Localization support for Ruffle desktop

#ifndef RUFFLE_DESKTOP_GUI_LOCALE_H
#define RUFFLE_DESKTOP_GUI_LOCALE_H

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>

// Forward declarations
namespace unic_langid {
    class LanguageIdentifier;
    enum class CharacterDirection;
}

namespace fluent_bundle {
    class FluentValue;
}

namespace ruffle {
namespace desktop {
namespace gui {

/// Get localized text for the given ID.
///
/// @param locale The locale to use for localization
/// @param id The text ID to look up
/// @return The localized text, or the ID if not found
std::string text(const unic_langid::LanguageIdentifier& locale, std::string_view id);

/// Get localized text for the given ID, returning nullopt if not found.
///
/// @param locale The locale to use for localization
/// @param id The text ID to look up
/// @return The localized text, or std::nullopt if not found
std::optional<std::string> optional_text(const unic_langid::LanguageIdentifier& locale, std::string_view id);

/// Get all available languages.
///
/// @return Vector of available language identifiers
std::vector<const unic_langid::LanguageIdentifier*> available_languages();

/// Get localized text with arguments.
///
/// @param locale The locale to use for localization
/// @param id The text ID to look up
/// @param args Arguments to substitute into the text
/// @return The localized text with arguments substituted
std::string text_with_args(
    const unic_langid::LanguageIdentifier& locale,
    std::string_view id,
    const std::unordered_map<std::string, fluent_bundle::FluentValue>& args
);

/// Reorder BiDi text so that RTL text runs are reversed.
///
/// @param locale The locale to determine base direction
/// @param text The text to reorder
/// @return The reordered text
std::string reorder_bidi(const unic_langid::LanguageIdentifier& locale, std::string_view text);

/// Mirror a character for RTL display.
///
/// @param c The character to mirror
/// @return The mirrored character
char mirror_char(char c);

/// A text that may or may not need localization.
class LocalizableText {
public:
    /// Create from non-localized text.
    explicit LocalizableText(std::string text);

    /// Create from localized text ID.
    explicit LocalizableText(std::string_view id, bool localized = true);

    /// Localize this text.
    ///
    /// @param locale The locale to use for localization
    /// @return The localized text
    std::string localize(const unic_langid::LanguageIdentifier& locale) const;

private:
    enum class Type {
        NonLocalized,
        Localized
    };

    Type type_;
    std::string value_;
};

} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_LOCALE_H
