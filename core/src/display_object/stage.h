// C++ translation of core/src/display_object/stage.rs
// Root stage implementation

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_STAGE_H
#define RUFFLE_CORE_DISPLAY_OBJECT_STAGE_H

#include <vector>
#include <optional>
#include <memory>
#include <cstdint>
#include <string>
#include <array>

// Forward declarations
namespace ruffle {
namespace core {

template<typename GC>
class Mutation;
template<typename GC>
class Color;
template<typename GC>
class Matrix;
template<typename GC>
class Rectangle;
template<typename GC>
class Twips;

namespace tag_utils {
    template<typename GC>
    class SwfMovie;
}

namespace config {
    enum class Letterbox;
}

namespace render {
    namespace quality {
        enum class StageQuality;
    }
    namespace backend {
        struct ViewportDimensions;
    }
}

namespace avm2 {
    template<typename GC>
    class Object;
    template<typename GC>
    class StageObject;
    template<typename GC>
    class LoaderInfoObject;
}

namespace display_object {

template<typename GC>
class DisplayObjectBase;
template<typename GC>
class InteractiveObjectBase;
template<typename GC>
class ChildContainer;
template<typename GC>
class DisplayObject;
template<typename GC>
class InteractiveObject;

/// Stage alignment options
enum class StageAlign {
    None = 0,
    Top = 1 << 0,
    Bottom = 1 << 1,
    Left = 1 << 2,
    Right = 1 << 3,
    TopLeft = Top | Left,
    TopRight = Top | Right,
    BottomLeft = Bottom | Left,
    BottomRight = Bottom | Right
};

/// Stage scale modes
enum class StageScaleMode {
    ShowAll,
    NoBorder,
    ExactFit,
    NoScale
};

/// Stage display states
enum class StageDisplayState {
    Normal,
    FullScreen,
    FullScreenInteractive
};

/// Window mode for web embedding
enum class WindowMode {
    Window,
    Opaque,
    Transparent,
    Direct,
    Gpu,
    DirectInWindow
};

/// Stage data
template<typename GC>
class StageData {
private:
    /// Base properties for interactive display objects
    InteractiveObjectBase<GC> base_;
    
    /// The list of all children of the stage
    std::unique_ptr<ChildContainer<GC>> child_;
    
    /// The AVM2 view of this stage object
    std::optional<typename avm2::StageObject<GC>> avm2_object_;
    
    /// The AVM2 'LoaderInfo' object for this stage object
    std::optional<typename avm2::LoaderInfoObject<GC>> loader_info_;
    
    /// An array of AVM2 'Stage3D' instances
    std::vector<typename avm2::Object<GC>> stage3ds_;
    
    /// Focus tracker for keyboard focus
    // FocusTracker<GC> focus_tracker_;
    
    /// The SWF movie
    std::shared_ptr<tag_utils::SwfMovie<GC>> movie_;
    
    /// The dimensions of the SWF file
    std::pair<uint32_t, uint32_t> movie_size_;
    
    /// The viewport transformation matrix
    Matrix<GC> viewport_matrix_;
    
    /// Matrix used for rendering the letterbox
    Matrix<GC> letterbox_matrix_;
    
    /// The bounds of the current viewport in twips
    Rectangle<Twips> view_bounds_;
    
    /// The stage background color
    std::optional<Color<GC>> background_color_;
    
    /// Letterbox mode
    config::Letterbox letterbox_;
    
    /// Quality settings
    render::quality::StageQuality quality_;
    
    /// The dimensions of the stage as reported to ActionScript
    std::pair<uint32_t, uint32_t> stage_size_;
    
    /// Scale mode
    StageScaleMode scale_mode_;
    
    /// Whether scale mode is forced
    bool forced_scale_mode_;
    
    /// Display state
    StageDisplayState display_state_;
    
    /// Alignment
    StageAlign align_;
    
    /// Whether alignment is forced
    bool forced_align_;
    
    /// Whether fullscreen is allowed
    bool allow_fullscreen_;
    
    /// Whether the stage is invalidated (needs render)
    bool invalidated_;
    
    /// Whether to use high quality downsampling
    bool use_bitmap_downsampling_;
    
    /// Window mode
    WindowMode window_mode_;
    
    /// Whether objects display focus rect
    bool stage_focus_rect_;
    
    /// Whether to show default context menu
    bool show_menu_;
    
public:
    StageData()
        : movie_size_(0, 0)
        , viewport_matrix_(Matrix<GC>::identity())
        , letterbox_matrix_(Matrix<GC>::identity())
        , letterbox_(config::Letterbox::Fullscreen)
        , quality_(render::quality::StageQuality::Medium)
        , stage_size_(0, 0)
        , scale_mode_(StageScaleMode::ShowAll)
        , forced_scale_mode_(false)
        , display_state_(StageDisplayState::Normal)
        , align_(StageAlign::None)
        , forced_align_(false)
        , allow_fullscreen_(true)
        , invalidated_(false)
        , use_bitmap_downsampling_(false)
        , window_mode_(WindowMode::Window)
        , stage_focus_rect_(true)
        , show_menu_(true) {}
    
    // Getters
    const InteractiveObjectBase<GC>& base() const { return base_; }
    InteractiveObjectBase<GC>& base() { return base_; }
    
    const ChildContainer<GC>& child() const { return *child_; }
    ChildContainer<GC>& child() { return *child_; }
    
    const std::optional<typename avm2::StageObject<GC>>& avm2_object() const { return avm2_object_; }
    const std::optional<typename avm2::LoaderInfoObject<GC>>& loader_info() const { return loader_info_; }
    const std::vector<typename avm2::Object<GC>>& stage3ds() const { return stage3ds_; }
    
    const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie() const { return movie_; }
    std::pair<uint32_t, uint32_t> movie_size() const { return movie_size_; }
    
    const Matrix<GC>& viewport_matrix() const { return viewport_matrix_; }
    const Matrix<GC>& letterbox_matrix() const { return letterbox_matrix_; }
    const Rectangle<Twips>& view_bounds() const { return view_bounds_; }
    
    std::optional<Color<GC>> background_color() const { return background_color_; }
    config::Letterbox letterbox() const { return letterbox_; }
    render::quality::StageQuality quality() const { return quality_; }
    std::pair<uint32_t, uint32_t> stage_size() const { return stage_size_; }
    StageScaleMode scale_mode() const { return scale_mode_; }
    bool forced_scale_mode() const { return forced_scale_mode_; }
    StageDisplayState display_state() const { return display_state_; }
    StageAlign align() const { return align_; }
    bool forced_align() const { return forced_align_; }
    bool allow_fullscreen() const { return allow_fullscreen_; }
    bool invalidated() const { return invalidated_; }
    bool use_bitmap_downsampling() const { return use_bitmap_downsampling_; }
    WindowMode window_mode() const { return window_mode_; }
    bool stage_focus_rect() const { return stage_focus_rect_; }
    bool show_menu() const { return show_menu_; }
    
    // Setters
    void set_avm2_object(typename avm2::StageObject<GC> obj) { avm2_object_ = std::move(obj); }
    void set_loader_info(typename avm2::LoaderInfoObject<GC> obj) { loader_info_ = std::move(obj); }
    void set_stage3ds(std::vector<typename avm2::Object<GC>> objs) { stage3ds_ = std::move(objs); }
    void set_movie(std::shared_ptr<tag_utils::SwfMovie<GC>> movie) { movie_ = std::move(movie); }
    void set_movie_size(uint32_t width, uint32_t height) { movie_size_ = {width, height}; }
    void set_viewport_matrix(Matrix<GC> matrix) { viewport_matrix_ = std::move(matrix); }
    void set_letterbox_matrix(Matrix<GC> matrix) { letterbox_matrix_ = std::move(matrix); }
    void set_view_bounds(Rectangle<Twips> bounds) { view_bounds_ = std::move(bounds); }
    void set_background_color(std::optional<Color<GC>> color) { background_color_ = std::move(color); }
    void set_letterbox(config::Letterbox letterbox) { letterbox_ = letterbox; }
    void set_quality(render::quality::StageQuality quality) { quality_ = quality; }
    void set_stage_size(uint32_t width, uint32_t height) { stage_size_ = {width, height}; }
    void set_scale_mode(StageScaleMode mode) { scale_mode_ = mode; }
    void set_forced_scale_mode(bool forced) { forced_scale_mode_ = forced; }
    void set_display_state(StageDisplayState state) { display_state_ = state; }
    void set_align(StageAlign align) { align_ = align; }
    void set_forced_align(bool forced) { forced_align_ = forced; }
    void set_allow_fullscreen(bool allow) { allow_fullscreen_ = allow; }
    void set_invalidated(bool invalidated) { invalidated_ = invalidated; }
    void set_use_bitmap_downsampling(bool use) { use_bitmap_downsampling_ = use; }
    void set_window_mode(WindowMode mode) { window_mode_ = mode; }
    void set_stage_focus_rect(bool rect) { stage_focus_rect_ = rect; }
    void set_show_menu(bool show) { show_menu_ = show; }
};

/// The Stage is the root of the display object hierarchy
template<typename GC>
class Stage {
private:
    std::shared_ptr<StageData<GC>> data_;
    
public:
    Stage() = default;
    explicit Stage(std::shared_ptr<StageData<GC>> data) : data_(std::move(data)) {}
    
    /// Create an empty stage
    static Stage empty(
        Mutation<GC>* gc_context,
        bool fullscreen,
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie
    ) {
        auto data = std::make_shared<StageData<GC>>();
        data->set_movie(movie);
        data->set_display_state(fullscreen ? StageDisplayState::FullScreen : StageDisplayState::Normal);
        data->set_invalidated(false);
        return Stage(data);
    }
    
    /// Get background color
    std::optional<Color<GC>> background_color() const {
        return data_ ? data_->background_color() : std::nullopt;
    }
    
    /// Set background color
    void set_background_color(std::optional<Color<GC>> color) {
        if (data_) {
            data_->set_background_color(std::move(color));
        }
    }
    
    /// Get inverse view matrix
    Matrix<GC> inverse_view_matrix() const {
        return data_ ? data_->viewport_matrix().inverse() : Matrix<GC>::identity();
    }
    
    /// Get view matrix
    Matrix<GC> view_matrix() const {
        return data_ ? data_->viewport_matrix() : Matrix<GC>::identity();
    }
    
    /// Get letterbox mode
    config::Letterbox letterbox() const {
        return data_ ? data_->letterbox() : config::Letterbox::Fullscreen;
    }
    
    /// Set letterbox mode
    void set_letterbox(config::Letterbox letterbox) {
        if (data_) {
            data_->set_letterbox(letterbox);
        }
    }
    
    /// Get movie size
    std::pair<uint32_t, uint32_t> movie_size() const {
        return data_ ? data_->movie_size() : std::make_pair(0u, 0u);
    }
    
    /// Set movie size
    void set_movie_size(uint32_t width, uint32_t height) {
        if (data_) {
            data_->set_movie_size(width, height);
        }
    }
    
    /// Set movie
    void set_movie(Mutation<GC>* gc_context, std::shared_ptr<tag_utils::SwfMovie<GC>> movie) {
        if (data_) {
            data_->set_movie(std::move(movie));
            // Update child container's AS3 flag
            bool is_as3 = data_->movie() ? data_->movie()->is_action_script_3() : false;
            data_->child().set_is_action_script_3(is_as3);
        }
    }
    
    /// Set loader info
    void set_loader_info(Mutation<GC>* gc_context, typename avm2::LoaderInfoObject<GC> loader_info) {
        if (data_) {
            data_->set_loader_info(std::move(loader_info));
        }
    }
    
    /// Get quality
    render::quality::StageQuality quality() const {
        return data_ ? data_->quality() : render::quality::StageQuality::Medium;
    }
    
    /// Set quality
    void set_quality(render::quality::StageQuality quality) {
        if (data_) {
            data_->set_quality(quality);
        }
    }
    
    /// Get stage size
    std::pair<uint32_t, uint32_t> stage_size() const {
        return data_ ? data_->stage_size() : std::make_pair(0u, 0u);
    }
    
    /// Get scale mode
    StageScaleMode scale_mode() const {
        return data_ ? data_->scale_mode() : StageScaleMode::ShowAll;
    }
    
    /// Set scale mode
    void set_scale_mode(StageScaleMode mode) {
        if (data_) {
            data_->set_scale_mode(mode);
        }
    }
    
    /// Get display state
    StageDisplayState display_state() const {
        return data_ ? data_->display_state() : StageDisplayState::Normal;
    }
    
    /// Set display state
    void set_display_state(StageDisplayState state) {
        if (data_) {
            data_->set_display_state(state);
        }
    }
    
    /// Get alignment
    StageAlign align() const {
        return data_ ? data_->align() : StageAlign::None;
    }
    
    /// Set alignment
    void set_align(StageAlign align) {
        if (data_) {
            data_->set_align(align);
        }
    }
    
    /// Check if fullscreen is allowed
    bool allow_fullscreen() const {
        return data_ ? data_->allow_fullscreen() : true;
    }
    
    /// Set allow fullscreen
    void set_allow_fullscreen(bool allow) {
        if (data_) {
            data_->set_allow_fullscreen(allow);
        }
    }
    
    /// Check if stage is invalidated
    bool invalidated() const {
        return data_ ? data_->invalidated() : false;
    }
    
    /// Set invalidated
    void set_invalidated(bool invalidated) {
        if (data_) {
            data_->set_invalidated(invalidated);
        }
    }
    
    /// Get window mode
    WindowMode window_mode() const {
        return data_ ? data_->window_mode() : WindowMode::Window;
    }
    
    /// Set window mode
    void set_window_mode(WindowMode mode) {
        if (data_) {
            data_->set_window_mode(mode);
        }
    }
    
    /// Check if focus rect is shown
    bool stage_focus_rect() const {
        return data_ ? data_->stage_focus_rect() : true;
    }
    
    /// Set focus rect
    void set_stage_focus_rect(bool rect) {
        if (data_) {
            data_->set_stage_focus_rect(rect);
        }
    }
    
    /// Check if menu is shown
    bool show_menu() const {
        return data_ ? data_->show_menu() : true;
    }
    
    /// Set show menu
    void set_show_menu(bool show) {
        if (data_) {
            data_->set_show_menu(show);
        }
    }
    
    /// Get AVM2 object
    const std::optional<typename avm2::StageObject<GC>>& avm2_object() const {
        static const std::optional<typename avm2::StageObject<GC>> empty;
        return data_ ? data_->avm2_object() : empty;
    }
    
    /// Get stage3D objects
    const std::vector<typename avm2::Object<GC>>& stage3ds() const {
        static const std::vector<typename avm2::Object<GC>> empty;
        return data_ ? data_->stage3ds() : empty;
    }
    
    /// Get child container
    ChildContainer<GC>& child() {
        static ChildContainer<GC> empty;
        return data_ ? data_->child() : empty;
    }
    
    const ChildContainer<GC>& child() const {
        static ChildContainer<GC> empty;
        return data_ ? data_->child() : empty;
    }
    
    /// Equality operators
    bool operator==(const Stage<GC>& other) const {
        return data_ == other.data_;
    }
    
    bool operator!=(const Stage<GC>& other) const {
        return !(*this == other);
    }
    
    /// Check if valid
    explicit operator bool() const {
        return data_ != nullptr;
    }
};

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_STAGE_H
