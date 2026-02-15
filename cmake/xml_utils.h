/*
 * C++ header for XML utilities functionality
 * This replaces the functionality of core/common/src/xml.rs
 */

#ifndef XML_UTILS_H
#define XML_UTILS_H

#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <optional>

namespace ruffle {

// XML utility functions for handling entity unescaping
class XmlUtils {
private:
    // Regular expressions for matching XML entities
    static const std::regex& get_avm1_entity_regex() {
        static const std::regex avm1_regex(R"(&[^;&]*;)");
        return avm1_regex;
    }

    static const std::regex& get_avm2_entity_regex() {
        static const std::regex avm2_regex(R"(&[^;]*;)");
        return avm2_regex;
    }

    // Helper function to unescape a single XML entity
    static std::string unescape_entity(const std::string& entity) {
        // Handle common XML entities
        if (entity == "&amp;") return "&";
        else if (entity == "&lt;") return "<";
        else if (entity == "&gt;") return ">";
        else if (entity == "&quot;") return "\"";
        else if (entity == "&apos;") return "'";
        
        // Handle numeric character references
        if (entity.length() > 3 && entity[1] == '#') {
            try {
                unsigned int codepoint;
                if (entity[2] == 'x' || entity[2] == 'X') {
                    // Hexadecimal
                    codepoint = std::stoi(entity.substr(3, entity.length() - 4), nullptr, 16);
                } else {
                    // Decimal
                    codepoint = std::stoi(entity.substr(2, entity.length() - 3));
                }
                
                // Convert codepoint to UTF-8
                if (codepoint <= 0x7F) {
                    return std::string(1, static_cast<char>(codepoint));
                } else if (codepoint <= 0x7FF) {
                    std::string result;
                    result += static_cast<char>(0xC0 | (codepoint >> 6));
                    result += static_cast<char>(0x80 | (codepoint & 0x3F));
                    return result;
                } else if (codepoint <= 0xFFFF) {
                    std::string result;
                    result += static_cast<char>(0xE0 | (codepoint >> 12));
                    result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                    result += static_cast<char>(0x80 | (codepoint & 0x3F));
                    return result;
                } else if (codepoint <= 0x10FFFF) {
                    std::string result;
                    result += static_cast<char>(0xF0 | (codepoint >> 18));
                    result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
                    result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                    result += static_cast<char>(0x80 | (codepoint & 0x3F));
                    return result;
                }
            } catch (...) {
                // If conversion fails, return the original entity
                return entity;
            }
        }
        
        // If entity is not recognized, return it as-is
        return entity;
    }

    // Handles flash-specific XML unescaping behavior
    static std::string custom_unescape(const std::string& input, const std::regex& entity_regex) {
        std::string result;
        std::sregex_iterator iter(input.begin(), input.end(), entity_regex);
        std::sregex_iterator end;

        size_t last_end = 0;

        for (; iter != end; ++iter) {
            std::smatch match = *iter;
            size_t start = match.position();
            size_t end_pos = start + match.length();

            // Add text before the entity
            result += input.substr(last_end, start - last_end);

            // Process the entity
            std::string entity = match.str();
            std::string decoded = unescape_entity(entity);
            result += decoded;

            last_end = end_pos;
        }

        // Add remaining text after the last entity
        result += input.substr(last_end);

        return result;
    }

public:
    // AVM1 XML unescaping. Decodes entities individually, even when
    // preceded by a bare '&' (e.g. "&&amp;" becomes "&&").
    static std::string avm1_unescape(const std::vector<uint8_t>& input) {
        // Convert input bytes to string
        std::string input_str(input.begin(), input.end());
        return custom_unescape(input_str, get_avm1_entity_regex());
    }

    // AVM2 E4X XML unescaping. Does not decode entities preceded by a
    // bare '&' (e.g. "&&amp;" is preserved as "&&amp;").
    static std::string avm2_unescape(const std::vector<uint8_t>& input) {
        // Convert input bytes to string
        std::string input_str(input.begin(), input.end());
        return custom_unescape(input_str, get_avm2_entity_regex());
    }

    // Overloaded versions that accept string directly
    static std::string avm1_unescape(const std::string& input) {
        return custom_unescape(input, get_avm1_entity_regex());
    }

    static std::string avm2_unescape(const std::string& input) {
        return custom_unescape(input, get_avm2_entity_regex());
    }
};

} // namespace ruffle

#endif // XML_UTILS_H