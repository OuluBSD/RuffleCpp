// C++ translation of core/src/system_properties.rs
//! System properties for the player

#ifndef RUFFLE_CORE_SYSTEM_PROPERTIES_H
#define RUFFLE_CORE_SYSTEM_PROPERTIES_H

#include <cstdint>
#include <string>
#include <format>
#include <bitset>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class UpdateContext;
}
}

namespace ruffle {
namespace core {

/// Available cpu architectures
enum class CpuArchitecture {
    PowerPc,
    X86,
    Sparc,
    Arm
};

/// Display formatter for CpuArchitecture
inline std::string to_string(CpuArchitecture arch) {
    switch (arch) {
        case CpuArchitecture::PowerPc:
            return "PowerPC";
        case CpuArchitecture::X86:
            return "x86";
        case CpuArchitecture::Sparc:
            return "SPARC";
        case CpuArchitecture::Arm:
            return "ARM";
    }
    return "Unknown";
}

/// The available host operating systems
enum class OperatingSystem {
    WindowsXp,
    Windows2k,
    WindowsNt,
    Windows98,
    Windows95,
    WindowsCe,
    WindowsUnknown,
    Linux,
    MacOS
};

/// Display formatter for OperatingSystem
inline std::string to_string(OperatingSystem os) {
    switch (os) {
        case OperatingSystem::WindowsXp:
            return "Windows XP";
        case OperatingSystem::Windows2k:
            return "Windows 2000";
        case OperatingSystem::WindowsNt:
            return "Windows NT";
        case OperatingSystem::Windows98:
            return "Windows 98/ME";
        case OperatingSystem::Windows95:
            return "Windows 95";
        case OperatingSystem::WindowsCe:
            return "Windows CE";
        case OperatingSystem::WindowsUnknown:
            return "Windows";
        case OperatingSystem::Linux:
            return "Linux";
        case OperatingSystem::MacOS:
            return "MacOS";
    }
    return "Unknown";
}

/// The available player manufacturers
enum class Manufacturer {
    Windows,
    Macintosh,
    Linux,
    Other
};

/// Manufacturer with additional methods
class ManufacturerExt {
public:
    ManufacturerExt(Manufacturer type, const std::string& name = "")
        : type_(type), name_(name) {}

    /// Get the manufacturer string based on player version
    std::string get_manufacturer_string(uint8_t version) const {
        std::string os_part;
        switch (type_) {
            case Manufacturer::Windows:
                os_part = "Windows";
                break;
            case Manufacturer::Macintosh:
                os_part = "Macintosh";
                break;
            case Manufacturer::Linux:
                os_part = "Linux";
                break;
            case Manufacturer::Other:
                os_part = name_;
                break;
        }

        if (version <= 8) {
            return "Macromedia " + os_part;
        } else {
            return "Adobe " + os_part;
        }
    }

    /// Get the platform name
    std::string get_platform_name() const {
        switch (type_) {
            case Manufacturer::Windows:
                return "WIN";
            case Manufacturer::Macintosh:
                return "MAC";
            case Manufacturer::Linux:
                return "LNX";
            default:
                return "";
        }
    }

private:
    Manufacturer type_;
    std::string name_;
};

/// The language of the host os
enum class Language {
    Czech,
    Danish,
    Dutch,
    English,
    Finnish,
    French,
    German,
    Hungarian,
    Italian,
    Japanese,
    Korean,
    Norwegian,
    Unknown,
    Polish,
    Portuguese,
    Russian,
    SimplifiedChinese,
    Spanish,
    Swedish,
    TraditionalChinese,
    Turkish
};

/// Language with additional methods
class LanguageExt {
public:
    LanguageExt(Language lang) : language_(lang) {}

    /// Get the language code based on player version
    std::string get_language_code(uint8_t player_version) const {
        switch (language_) {
            case Language::Czech:
                return "cs";
            case Language::Danish:
                return "da";
            case Language::Dutch:
                return "nl";
            case Language::English:
                return player_version < 7 ? "en-US" : "en";
            case Language::Finnish:
                return "fi";
            case Language::French:
                return "fr";
            case Language::German:
                return "de";
            case Language::Hungarian:
                return "hu";
            case Language::Italian:
                return "it";
            case Language::Japanese:
                return "ja";
            case Language::Korean:
                return "ko";
            case Language::Norwegian:
                return "no";
            case Language::Unknown:
                return "xu";
            case Language::Polish:
                return "pl";
            case Language::Portuguese:
                return "pt";
            case Language::Russian:
                return "ru";
            case Language::SimplifiedChinese:
                return "zh-CN";
            case Language::Spanish:
                return "es";
            case Language::Swedish:
                return "sv";
            case Language::TraditionalChinese:
                return "zh-TW";
            case Language::Turkish:
                return "tr";
        }
        return "en";
    }

    /// Create Language from language identifier string
    static Language from_language_identifier(const std::string& lang_id) {
        // Parse language identifier (e.g., "en-US", "zh-TW")
        std::string lang = lang_id.substr(0, 2);

        if (lang == "da") return Language::Danish;
        if (lang == "nl") return Language::Dutch;
        if (lang == "en") return Language::English;
        if (lang == "fi") return Language::Finnish;
        if (lang == "fr") return Language::French;
        if (lang == "de") return Language::German;
        if (lang == "hu") return Language::Hungarian;
        if (lang == "it") return Language::Italian;
        if (lang == "ja") return Language::Japanese;
        if (lang == "ko") return Language::Korean;
        if (lang == "no") return Language::Norwegian;
        if (lang == "und") return Language::Unknown;
        if (lang == "pl") return Language::Polish;
        if (lang == "pt") return Language::Portuguese;
        if (lang == "ru") return Language::Russian;
        if (lang == "zh") {
            // Check for Traditional Chinese
            if (lang_id.length() >= 5 && lang_id.substr(3, 2) == "TW") {
                return Language::TraditionalChinese;
            }
            return Language::SimplifiedChinese;
        }
        if (lang == "es") return Language::Spanish;
        if (lang == "sv") return Language::Swedish;
        if (lang == "tr") return Language::Turkish;

        // Fallback to English instead of Unknown for better compatibility
        return Language::English;
    }

private:
    Language language_;
};

/// The supported colors of the screen
enum class ScreenColor {
    Color,
    Gray,
    BlackWhite
};

/// Display formatter for ScreenColor
inline std::string to_string(ScreenColor color) {
    switch (color) {
        case ScreenColor::Color:
            return "color";
        case ScreenColor::Gray:
            return "gray";
        case ScreenColor::BlackWhite:
            return "bw";
    }
    return "color";
}

/// The type of the player
enum class PlayerType {
    StandAlone,
    External,
    PlugIn,
    ActiveX
};

/// Display formatter for PlayerType
inline std::string to_string(PlayerType type) {
    switch (type) {
        case PlayerType::StandAlone:
            return "StandAlone";
        case PlayerType::External:
            return "External";
        case PlayerType::PlugIn:
            return "PlugIn";
        case PlayerType::ActiveX:
            return "ActiveX";
    }
    return "StandAlone";
}

/// System capabilities bitflags
class SystemCapabilities {
public:
    SystemCapabilities() : flags_(0) {}

    enum Flag : uint32_t {
        AV_HARDWARE      = 1 << 0,
        ACCESSIBILITY    = 1 << 1,
        AUDIO            = 1 << 2,
        AUDIO_ENCODER    = 1 << 3,
        EMBEDDED_VIDEO   = 1 << 4,
        IME              = 1 << 5,
        MP3              = 1 << 6,
        PRINTING         = 1 << 7,
        SCREEN_BROADCAST = 1 << 8,
        SCREEN_PLAYBACK  = 1 << 9,
        STREAMING_AUDIO  = 1 << 10,
        STREAMING_VIDEO  = 1 << 11,
        VIDEO_ENCODER    = 1 << 12,
        DEBUGGER         = 1 << 13,
        LOCAL_FILE_READ  = 1 << 14,
        PROCESS_64_BIT   = 1 << 15,
        PROCESS_32_BIT   = 1 << 16,
        ACROBAT_EMBEDDED = 1 << 17,
        TLS              = 1 << 18,
        WINDOW_LESS      = 1 << 19
    };

    void set(Flag flag, bool value = true) {
        if (value) {
            flags_ |= static_cast<uint32_t>(flag);
        } else {
            flags_ &= ~static_cast<uint32_t>(flag);
        }
    }

    bool contains(Flag flag) const {
        return (flags_ & static_cast<uint32_t>(flag)) != 0;
    }

    void clear() {
        flags_ = 0;
    }

    uint32_t value() const {
        return flags_;
    }

private:
    uint32_t flags_;
};

/// The properties modified by 'System'
class SystemProperties {
public:
    SystemProperties()
        : exact_settings_(true)
        , use_codepage_(false)
        , capabilities_()
        , player_type_(PlayerType::StandAlone)
        , screen_color_(ScreenColor::Color)
        , pixel_aspect_ratio_(1.0f)
        , dpi_(72.0f)
        , language_(Language::English)
        , manufacturer_(Manufacturer::Linux)
        , os_(OperatingSystem::Linux)
        , cpu_architecture_(CpuArchitecture::X86)
        , idc_level_("5.1")
    {}

    /// Create SystemProperties with a specific language
    ///
    /// @param language_id The language identifier (e.g., "en-US")
    /// @return New SystemProperties
    static SystemProperties new_(const std::string& language_id) {
        SystemProperties props;
        // TODO: default to true on fp>=7, false <= 6
        props.exact_settings_ = true;
        // TODO: default to false on fp>=7, true <= 6
        props.use_codepage_ = false;
        props.capabilities_.clear();
        props.player_type_ = PlayerType::StandAlone;
        props.screen_color_ = ScreenColor::Color;
        // TODO: note for fp <7 this should be the locale and the ui lang for >= 7, on windows
        props.language_ = LanguageExt::from_language_identifier(language_id);
        // source: https://web.archive.org/web/20230611050355/https://flylib.com/books/en/4.13.1.272/1/
        props.pixel_aspect_ratio_ = 1.0f;
        // source: https://tracker.adobe.com/#/view/FP-3949775
        props.dpi_ = 72.0f;
        props.manufacturer_ = Manufacturer::Linux;
        props.os_ = OperatingSystem::Linux;
        props.cpu_architecture_ = CpuArchitecture::X86;
        props.idc_level_ = "5.1";
        return props;
    }

    /// Get the version string
    ///
    /// @param player_version The player version
    /// @return Version string (e.g., "WIN 32,0,0,0")
    std::string get_version_string(uint8_t player_version) const {
        return get_manufacturer_string().get_platform_name() + " " +
               std::to_string(player_version) + ",0,0,0";
    }

    /// Check if a capability is enabled
    ///
    /// @param cap The capability to check
    /// @return true if enabled, false otherwise
    bool has_capability(SystemCapabilities::Flag cap) const {
        return capabilities_.contains(cap);
    }

    /// Encode a capability as "t" or "f"
    std::string encode_capability(SystemCapabilities::Flag cap) const {
        return has_capability(cap) ? "t" : "f";
    }

    /// Encode a negated capability as "t" or "f"
    std::string encode_not_capability(SystemCapabilities::Flag cap) const {
        return has_capability(cap) ? "f" : "t";
    }

    /// Encode a string for URL
    std::string encode_string(const std::string& s) const {
        // Simple percent encoding for non-alphanumeric characters
        std::string result;
        for (char c : s) {
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
                result += c;
            } else {
                char hex[4];
                snprintf(hex, sizeof(hex), "%%%02X", static_cast<unsigned char>(c));
                result += hex;
            }
        }
        return result;
    }

    /// Get the server string for analytics
    ///
    /// @param context The update context
    /// @return URL-encoded server string
    std::string get_server_string(const UpdateContext<GC>& context) const;

    // Getters
    bool exact_settings() const { return exact_settings_; }
    void set_exact_settings(bool value) { exact_settings_ = value; }

    bool use_codepage() const { return use_codepage_; }
    void set_use_codepage(bool value) { use_codepage_ = value; }

    const SystemCapabilities& capabilities() const { return capabilities_; }
    SystemCapabilities& capabilities() { return capabilities_; }

    PlayerType player_type() const { return player_type_; }
    void set_player_type(PlayerType type) { player_type_ = type; }

    ScreenColor screen_color() const { return screen_color_; }
    void set_screen_color(ScreenColor color) { screen_color_ = color; }

    float pixel_aspect_ratio() const { return pixel_aspect_ratio_; }
    void set_pixel_aspect_ratio(float ratio) { pixel_aspect_ratio_ = ratio; }

    float dpi() const { return dpi_; }
    void set_dpi(float dpi) { dpi_ = dpi; }

    Language language() const { return language_; }
    void set_language(Language lang) { language_ = lang; }

    Manufacturer manufacturer() const { return manufacturer_; }
    void set_manufacturer(Manufacturer mfr) { manufacturer_ = mfr; }

    OperatingSystem os() const { return os_; }
    void set_os(OperatingSystem os) { os_ = os; }

    CpuArchitecture cpu_architecture() const { return cpu_architecture_; }
    void set_cpu_architecture(CpuArchitecture arch) { cpu_architecture_ = arch; }

    const std::string& idc_level() const { return idc_level_; }
    void set_idc_level(const std::string& level) { idc_level_ = level; }

private:
    ManufacturerExt get_manufacturer_string() const {
        return ManufacturerExt(manufacturer_);
    }

    /// If true then settings should be saved and read from the exact same domain of the player
    /// If false then they should be saved to the super domain
    bool exact_settings_;

    /// If true, the system codepage should be used for text files
    /// If false, UTF-8 should be used for SWF version >= 6 and ISO Latin-1 for SWF version <= 5
    bool use_codepage_;

    /// The capabilities of the player
    SystemCapabilities capabilities_;

    /// The type of the player
    PlayerType player_type_;

    /// The type of screen available to the player
    ScreenColor screen_color_;

    /// The aspect ratio of the screens pixels
    float pixel_aspect_ratio_;

    /// The dpi of the screen
    float dpi_;

    /// The language of the host os
    Language language_;

    /// The manufacturer of the player
    Manufacturer manufacturer_;

    /// The os of the host
    OperatingSystem os_;

    /// The cpu architecture of the platform
    CpuArchitecture cpu_architecture_;

    /// The highest supported h264 decoder level
    std::string idc_level_;
};

// SystemProperties method implementations

template<typename GC>
std::string SystemProperties::get_server_string(const UpdateContext<GC>& context) const {
    auto viewport_dimensions = context.renderer().viewport_dimensions();

    std::string result;
    auto append_pair = [&result](const std::string& key, const std::string& value) {
        if (!result.empty()) result += "&";
        result += key + "=" + value;
    };

    append_pair("A", encode_capability(SystemCapabilities::AUDIO));
    append_pair("SA", encode_capability(SystemCapabilities::STREAMING_AUDIO));
    append_pair("SV", encode_capability(SystemCapabilities::STREAMING_VIDEO));
    append_pair("EV", encode_capability(SystemCapabilities::EMBEDDED_VIDEO));
    append_pair("MP3", encode_capability(SystemCapabilities::MP3));
    append_pair("AE", encode_capability(SystemCapabilities::AUDIO_ENCODER));
    append_pair("VE", encode_capability(SystemCapabilities::VIDEO_ENCODER));
    append_pair("ACC", encode_not_capability(SystemCapabilities::ACCESSIBILITY));
    append_pair("PR", encode_capability(SystemCapabilities::PRINTING));
    append_pair("SP", encode_capability(SystemCapabilities::SCREEN_PLAYBACK));
    append_pair("SB", encode_capability(SystemCapabilities::SCREEN_BROADCAST));
    append_pair("DEB", encode_capability(SystemCapabilities::DEBUGGER));
    append_pair("M", encode_string(get_manufacturer_string().get_manufacturer_string(context.player_version())));
    append_pair("R", std::to_string(viewport_dimensions.width) + "x" + std::to_string(viewport_dimensions.height));
    append_pair("COL", to_string(screen_color_));
    append_pair("AR", std::to_string(pixel_aspect_ratio_));
    append_pair("OS", encode_string(to_string(os_)));
    append_pair("L", LanguageExt(language_).get_language_code(context.player_version()));
    append_pair("IME", encode_capability(SystemCapabilities::IME));
    append_pair("PT", to_string(player_type_));
    append_pair("AVD", encode_not_capability(SystemCapabilities::AV_HARDWARE));
    append_pair("LFD", encode_not_capability(SystemCapabilities::LOCAL_FILE_READ));
    append_pair("DP", std::to_string(dpi_));

    return result;
}

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_SYSTEM_PROPERTIES_H
