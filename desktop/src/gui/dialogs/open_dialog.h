// C++ translation of desktop/src/gui/dialogs/open_dialog.rs
// Open dialog for launching SWF files with configuration options

#ifndef RUFFLE_DESKTOP_GUI_DIALOGS_OPEN_DIALOG_H
#define RUFFLE_DESKTOP_GUI_DIALOGS_OPEN_DIALOG_H

#include "../gui.h"
#include "../../player.h"
#include "../../../frontend-utils/src/player_options.h"
#include "../../../frontend-utils/src/content.h"

#include <wx/wx.h>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include <chrono>

namespace ruffle {
namespace desktop {
namespace gui {
namespace dialogs {

// Forward declarations
class PathOrUrlField;
class FilePicker;

/// Optional field wrapper for dialog inputs
template<typename T>
class OptionalField {
public:
    OptionalField(std::optional<typename T::ValueType> initial, T field);
    
    /// Render the field UI
    /// 
    /// @param ui UI context
    /// @param value Output value
    /// @param locale Locale identifier
    /// @return true if field is valid
    bool ui(wxWindow* ui, std::optional<typename T::ValueType>* value, const std::string& locale);
    
    /// Get the result value
    std::optional<typename T::ValueType> result() const;
    
    /// Check if field is valid
    bool is_valid() const;

private:
    T field_;
    std::optional<typename T::ValueType> value_;
    bool enabled_;
};

/// URL input field
class UrlField {
public:
    using ValueType = std::string;
    using ResultType = std::string;  // Would be url::Url in full implementation
    
    explicit UrlField(const std::string& hint);
    
    /// Render the field
    void ui(wxWindow* parent, std::string& value, bool error);
    
    /// Validate and convert to result
    std::optional<ResultType> to_result(const std::string& value) const;

private:
    std::string hint_;
};

/// Cookie input field
class CookieField {
public:
    using ValueType = std::string;
    using ResultType = std::string;
    
    explicit CookieField(const std::string& hint);
    
    void ui(wxWindow* parent, std::string& value, bool error);
    std::optional<ResultType> to_result(const std::string& value) const;

private:
    std::string hint_;
};

/// Duration input field (for script timeout)
class DurationField {
public:
    using ValueType = double;  // seconds
    using ResultType = std::chrono::duration<double>;
    
    DurationField(double min_seconds, double max_seconds, double default_seconds);
    
    void ui(wxWindow* parent, double& value);
    std::optional<ResultType> to_result(double value) const;

private:
    double min_;
    double max_;
    double default_;
};

/// Number field for integer ranges
template<typename T>
class NumberField {
public:
    using ValueType = T;
    using ResultType = T;
    
    NumberField(T min, T max, T default_value);
    
    void ui(wxWindow* parent, T& value);
    std::optional<ResultType> to_result(T value) const;

private:
    T min_;
    T max_;
    T default_;
};

/// Enum dropdown field
template<typename EnumType>
class EnumDropdownField {
public:
    using ValueType = EnumType;
    using ResultType = EnumType;
    
    using LabelFn = std::function<std::string(EnumType, const std::string&)>;
    using TooltipFn = std::function<std::optional<std::string>(EnumType, const std::string&)>;
    
    EnumDropdownField(
        EnumType default_value,
        std::vector<EnumType> options,
        LabelFn label_fn
    );
    
    EnumDropdownField& with_tooltips(TooltipFn tooltip_fn);
    
    void ui(wxWindow* parent, EnumType& value, const std::string& locale);
    std::optional<ResultType> to_result(EnumType value) const;

private:
    EnumType default_;
    std::vector<EnumType> options_;
    LabelFn label_fn_;
    TooltipFn tooltip_fn_;
};

/// Boolean dropdown field (enable/disable)
class BooleanDropdownField {
public:
    using ValueType = bool;
    using ResultType = bool;
    using LabelFn = std::function<std::string(bool, const std::string&)>;
    
    BooleanDropdownField(bool default_value, LabelFn label_fn);
    
    void ui(wxWindow* parent, bool& value, const std::string& locale);
    std::optional<ResultType> to_result(bool value) const;

private:
    bool default_;
    LabelFn label_fn_;
};

/// Field with checkbox for force override
template<typename InnerFieldType>
class FieldWithCheckbox {
public:
    using ValueType = std::pair<typename InnerFieldType::ValueType, bool>;
    using ResultType = std::pair<typename InnerFieldType::ResultType, bool>;
    
    FieldWithCheckbox(
        InnerFieldType inner_field,
        const std::string& checkbox_label,
        bool checkbox_default
    );
    
    FieldWithCheckbox& with_checkbox_tooltip(const std::string& tooltip);
    
    void ui(wxWindow* parent, ValueType& value, const std::string& locale);
    std::optional<ResultType> to_result(const ValueType& value) const;

private:
    InnerFieldType inner_field_;
    std::string checkbox_label_;
    bool checkbox_default_;
    std::optional<std::string> checkbox_tooltip_;
};

/// Open dialog for launching SWF files
class OpenDialog {
public:
    /// Create new open dialog
    /// 
    /// @param defaults Default launch options
    /// @param default_content Optional default content to open
    /// @param picker File picker for browsing
    /// @param event_loop Event loop proxy for sending launch events
    OpenDialog(
        LaunchOptions defaults,
        std::optional<frontend::utils::content::ContentDescriptor> default_content,
        std::shared_ptr<FilePicker> picker,
        wxEventLoop* event_loop
    );
    
    /// Get the selected content descriptor
    std::optional<frontend::utils::content::ContentDescriptor> content_descriptor() const;
    
    /// Get the configured launch options
    const LaunchOptions& options() const;
    
    /// Show the dialog
    /// 
    /// @param locale Locale identifier for localization
    /// @param parent Parent window
    /// @return true if dialog should remain open
    bool show(const std::string& locale, wxWindow* parent);

private:
    LaunchOptions options_;
    wxEventLoop* event_loop_;
    std::shared_ptr<FilePicker> picker_;
    
    // Network settings fields
    OptionalField<UrlField> spoof_url_;
    OptionalField<UrlField> referer_;
    OptionalField<CookieField> cookie_;
    OptionalField<UrlField> base_url_;
    OptionalField<UrlField> proxy_url_;
    OptionalField<BooleanDropdownField> upgrade_to_https_;
    OptionalField<EnumDropdownField<ruffle::core::backend::navigator::SocketMode>> tcp_connections_;
    OptionalField<EnumDropdownField<ruffle::core::LoadBehavior>> load_behavior_;
    
    // Player settings fields
    OptionalField<DurationField> script_timeout_;
    OptionalField<EnumDropdownField<ruffle::render::quality::StageQuality>> quality_;
    OptionalField<FieldWithCheckbox<EnumDropdownField<ruffle::core::StageAlign>>> align_;
    OptionalField<FieldWithCheckbox<EnumDropdownField<ruffle::core::StageScaleMode>>> scale_mode_;
    OptionalField<EnumDropdownField<ruffle::core::config::Letterbox>> letterbox_;
    OptionalField<NumberField<uint8_t>> player_version_;
    OptionalField<EnumDropdownField<ruffle::core::PlayerRuntime>> player_runtime_;
    OptionalField<BooleanDropdownField> dummy_external_interface_;
    
    // Content path field
    PathOrUrlField path_;
    
    // Framerate settings
    double framerate_;
    bool framerate_enabled_;
    
    /// Start/launch the selected content
    /// 
    /// @return true if launch was successful
    bool start();
    
    /// Render network settings panel
    /// 
    /// @param locale Locale identifier
    /// @param parent Parent window
    /// @return true if all fields are valid
    bool network_settings(const std::string& locale, wxWindow* parent);
    
    /// Render player settings panel
    void player_settings(const std::string& locale, wxWindow* parent);
    
    /// Render movie parameters panel
    void movie_parameters(const std::string& locale, wxWindow* parent);
};

} // namespace dialogs
} // namespace gui
} // namespace desktop
} // namespace ruffle

#endif // RUFFLE_DESKTOP_GUI_DIALOGS_OPEN_DIALOG_H
