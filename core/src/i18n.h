// C++ translation of core/src/i18n.rs
// Internationalization (i18n) support for core text localization

#ifndef RUFFLE_CORE_I18N_H
#define RUFFLE_CORE_I18N_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace ruffle {
namespace core {

/// Fluent value type for localization arguments
///
/// Mirrors fluent_bundle::FluentValue from Rust
using FluentValue = std::variant<
    std::string,
    double,
    int64_t,
    bool
>;

/// Language identifier for localization
///
/// Simplified representation of fluent_templates::LanguageIdentifier
struct LanguageIdentifier {
    std::string language;
    std::string region;

    LanguageIdentifier() : language("en"), region("US") {}
    LanguageIdentifier(const std::string& lang, const std::string& reg = "")
        : language(lang), region(reg) {}

    /// Parse language identifier from string (e.g., "en-US")
    static LanguageIdentifier from_string(const std::string& s);

    /// Convert to string representation
    std::string to_string() const;

    /// Equality comparison
    bool operator==(const LanguageIdentifier& other) const {
        return language == other.language && region == other.region;
    }
};

/// Hash function for LanguageIdentifier
struct LanguageIdentifierHash {
    std::size_t operator()(const LanguageIdentifier& lang) const {
        return std::hash<std::string>{}(lang.language) ^
               (std::hash<std::string>{}(lang.region) << 1);
    }
};

/// Arguments map for localized text with arguments
using TextArgs = std::unordered_map<std::string, FluentValue>;

/// Look up a core text by ID in the specified language
///
/// \param language The language identifier
/// \param id The text ID to look up
/// \return The localized text, or the ID itself if not found
std::string core_text(const LanguageIdentifier& language, std::string_view id);

/// Look up a core text by ID with arguments in the specified language
///
/// \param language The language identifier
/// \param id The text ID to look up
/// \param args The arguments to substitute in the text
/// \return The localized text with arguments substituted, or the ID itself if not found
std::string core_text_with_args(
    const LanguageIdentifier& language,
    std::string_view id,
    const TextArgs& args
);

/// Initialize the i18n system (loads localization files)
///
/// \param locales_path Path to the locales directory
void init_i18n(const std::string& locales_path = "./assets/texts");

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_I18N_H
