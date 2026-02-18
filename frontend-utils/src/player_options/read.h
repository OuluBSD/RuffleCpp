// C++ translation of frontend-utils/src/player_options/read.rs
// Player options reading functionality from TOML configuration

#ifndef RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_READ_H
#define RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_READ_H

#include "../player_options.h"
#include "../parse.h"

#include <chrono>
#include <toml_edit.hpp>

namespace ruffle {
namespace frontend {
namespace utils {
namespace player_options {
namespace read {

/// Parse context for reading player options
struct ParseContext {
    // Warnings collected during parsing
    std::vector<ParseWarning> warnings;
    
    // Key path for error reporting
    std::vector<std::string> key_path;
    
    void push_key(const std::string& key);
    void pop_key();
};

/// Read player options from a TOML table
/// 
/// @param cx Parse context for error reporting
/// @param table The TOML table containing player options
/// @return PlayerOptions structure with parsed values
PlayerOptions read_player_options(ParseContext* cx, const toml_edit::table& table);

/// Parse parameters (FlashVars) from a TOML table
void parse_parameters(ParseContext* cx, const toml_edit::table& parameters, PlayerOptions& result);

/// Parse script timeout duration from TOML value
std::optional<std::chrono::duration<double>> parse_script_timeout(const toml_edit::table& table);

/// Parse base URL from TOML value
std::optional<url::Url> parse_base_url(ParseContext* cx, const toml_edit::table& table);

/// Parse quality setting from TOML value
std::optional<ruffle::render::quality::StageQuality> parse_quality(ParseContext* cx, const toml_edit::table& table);

/// Parse align setting from TOML value
std::optional<ruffle::core::StageAlign> parse_align(ParseContext* cx, const toml_edit::table& table);

/// Parse scale mode from TOML value
std::optional<ruffle::core::StageScaleMode> parse_scale_mode(ParseContext* cx, const toml_edit::table& table);

/// Parse load behavior from TOML value
std::optional<ruffle::core::LoadBehavior> parse_load_behavior(ParseContext* cx, const toml_edit::table& table);

/// Parse letterbox setting from TOML value
std::optional<ruffle::core::config::Letterbox> parse_letterbox(ParseContext* cx, const toml_edit::table& table);

/// Parse player runtime from TOML value
std::optional<ruffle::core::PlayerRuntime> parse_runtime(ParseContext* cx, const toml_edit::table& table);

} // namespace read
} // namespace player_options
} // namespace utils
} // namespace frontend
} // namespace ruffle

#endif // RUFFLE_FRONTEND_UTILS_PLAYER_OPTIONS_READ_H
