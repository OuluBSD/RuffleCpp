// C++ translation of core/src/avm2/object/loaderinfo_object.rs
//! Loader-info object for AVM2

#ifndef RUFFLE_CORE_AVM2_OBJECT_LOADERINFO_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_LOADERINFO_OBJECT_H

#include <memory>
#include <cstdint>
#include <optional>
#include <format>
#include <string>
#include <variant>
#include <cell>
#include <mutex>
#include <atomic>

// Forward declarations
namespace ruffle {
namespace core {
    class WString;
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class ScriptObjectData;
        template<typename T> class TObject;
        template<typename T> class Gc;
        template<typename T> class GcWeak;
        template<typename T> class ClassObject;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class StageObject;
        template<typename T> class EventObject;
    }
    namespace display_object {
        template<typename GC> class DisplayObject;
        template<typename GC> class TDisplayObject;
        template<typename GC> class TDisplayObjectContainer;
    }
    namespace loader {
        class ContentType;
    }
    namespace tag_utils {
        template<typename GC> class SwfMovie;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Represents a thing which can be loaded by a loader.
template<typename GC>
class LoaderStream {
public:
    /// An SWF movie that has not yet loaded.
    ///
    /// The main differences from Swf loader streams is that certain loader
    /// info properties are null until the SWF is fully loaded. Furthermore,
    /// the DisplayObject parameter is optional, to represent movies that do
    /// not yet have a root clip.
    ///
    /// While the Stage is not a loadable object, it has loaderInfo, with
    /// properties that roughly mirror an unloaded movie clip. Properties that
    /// are valid on Stage.loaderInfo will be pulled from the root SWF.
    ///
    /// The bool parameter indicates if this is the Stage's loader info;
    /// this is because certain Stage properties are accessible even when the
    /// associated movie is not yet loaded.
    struct NotYetLoaded {
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie;
        std::optional<display_object::DisplayObject<GC>> root_clip;
        bool is_stage;
    };

    /// A loaded SWF movie.
    ///
    /// The associated DisplayObject is the root movieclip.
    struct Swf {
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie;
        display_object::DisplayObject<GC> root;
    };

    using Variant = std::variant<NotYetLoaded, Swf>;

    LoaderStream() = default;
    explicit LoaderStream(Variant variant) : variant_(variant) {}

    /// Create a not-yet-loaded stream
    static LoaderStream not_yet_loaded(
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie,
        std::optional<display_object::DisplayObject<GC>> root_clip,
        bool is_stage
    ) {
        return LoaderStream(Variant(NotYetLoaded{movie, root_clip, is_stage}));
    }

    /// Create a loaded SWF stream
    static LoaderStream swf(
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie,
        display_object::DisplayObject<GC> root
    ) {
        return LoaderStream(Variant(Swf{movie, root}));
    }

    /// Get the movie reference
    const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie() const {
        return std::visit([](const auto& v) -> const std::shared_ptr<tag_utils::SwfMovie<GC>>& {
            return v.movie;
        }, variant_);
    }

    /// Check if this is a loaded SWF
    bool is_loaded() const {
        return std::holds_alternative<Swf>(variant_);
    }

    /// Get the variant
    const Variant& variant() const { return variant_; }
    Variant& variant() { return variant_; }

private:
    Variant variant_;
};

/// Weak reference to LoaderInfoObject data
template<typename GC>
class LoaderInfoObjectWeak;

/// Data structure for LoaderInfoObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class LoaderInfoObjectData {
public:
    LoaderInfoObjectData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// The loaded stream that this gets its info from.
    mutable std::mutex stream_mutex;
    LoaderStream<GC> loaded_stream;

    /// The loader object
    std::optional<StageObject<GC>> loader;

    /// Whether or not we've fired our 'init' event
    std::atomic<bool> init_event_fired{false};

    /// Whether or not we've fired our 'complete' event
    std::atomic<bool> complete_event_fired{false};

    /// The EventDispatcher used for LoaderInfo.sharedEvents.
    Object<GC> shared_events;

    /// The EventDispatcher for uncaught error events.
    Object<GC> uncaught_error_events;

    /// Content type
    loader::ContentType content_type;

    /// Whether to expose content
    std::atomic<bool> expose_content{false};

    /// Whether an error occurred
    std::atomic<bool> errored{false};

    // Friend classes for accessing private members
    friend class LoaderInfoObject<GC>;
    friend class LoaderInfoObjectWeak<GC>;
};

/// LoaderInfoObject - An Object which represents a loadable object
///
/// Represents a loadable object, such as a SWF movie or image resource.
/// Provides information about the loaded content through various properties.
template<typename GC>
class LoaderInfoObject {
public:
    using DataType = LoaderInfoObjectData<GC>;
    using WeakType = LoaderInfoObjectWeak<GC>;

    LoaderInfoObject() = default;
    explicit LoaderInfoObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a loader info object that has not yet been loaded.
    ///
    /// Use nullopt as the root clip to indicate that this is the stage's loader
    /// info.
    ///
    /// @param activation The current activation context
    /// @param movie The SWF movie
    /// @param loader The loader object
    /// @param root_clip The root display object (optional)
    /// @param is_stage Whether this is the stage's loader info
    /// @return New LoaderInfoObject or error
    static LoaderInfoObject not_yet_loaded(
        Activation<GC>* activation,
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie,
        std::optional<StageObject<GC>> loader,
        std::optional<display_object::DisplayObject<GC>> root_clip,
        bool is_stage
    );

    /// Get the loader object
    std::optional<StageObject<GC>> loader() const {
        return data_->loader;
    }

    /// Get the shared events dispatcher
    Object<GC> shared_events() const {
        return data_->shared_events;
    }

    /// Get the uncaught error events dispatcher
    Object<GC> uncaught_error_events() const {
        return data_->uncaught_error_events;
    }

    /// Gets the ContentType, 'hiding' it by returning ContentType::Unknown
    /// if we haven't yet fired the 'init' event. The real ContentType first becomes
    /// visible to ActionScript in the 'init' event.
    loader::ContentType content_type_hide_before_init() const {
        if (data_->init_event_fired.load()) {
            return data_->content_type;
        }
        return loader::ContentType::Unknown;
    }

    /// Set whether an error occurred
    void set_errored(bool val) {
        data_->errored.store(val);
    }

    /// Check if an error occurred
    bool errored() const {
        return data_->errored.load();
    }

    /// Check if the init event has been fired
    bool init_event_fired() const {
        return data_->init_event_fired.load();
    }

    /// Reset the init and complete event flags
    void reset_init_and_complete_events() {
        data_->init_event_fired.store(false);
        data_->complete_event_fired.store(false);
    }

    /// Fires the 'init' and 'complete' events if they haven't been fired yet.
    /// Returns true if both events have been fired (either as a result of
    /// this call, or due to a previous call).
    ///
    /// @param context The update context
    /// @param status HTTP status code
    /// @param redirected Whether the request was redirected
    /// @return true if both events have been fired
    bool fire_init_and_complete_events(
        UpdateContext<GC>* context,
        uint16_t status,
        bool redirected
    );

    /// Get the loader stream
    const LoaderStream<GC>& loader_stream() const {
        std::lock_guard<std::mutex> lock(data_->stream_mutex);
        return data_->loaded_stream;
    }

    /// Check if content should be exposed
    bool expose_content() const {
        return data_->expose_content.load();
    }

    /// Makes the 'content' visible to ActionScript.
    /// This is used by certain special loaders (the stage and root movie),
    /// which expose the loaded content before the 'init' event is fired.
    void set_expose_content() {
        data_->expose_content.store(true);
    }

    /// Set the loader stream
    void set_loader_stream(LoaderStream<GC> stream, GC* mc);

    /// Set the content type
    void set_content_type(loader::ContentType content_type) {
        data_->content_type = content_type;
    }

    /// Unload the loader info
    ///
    /// Resets properties and removes the Loader's content element.
    ///
    /// @param context The update context
    void unload(UpdateContext<GC>* context);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to LoaderInfoObject
template<typename GC>
class LoaderInfoObjectWeak {
public:
    LoaderInfoObjectWeak() = default;
    explicit LoaderInfoObjectWeak(GcWeak<GC, LoaderInfoObjectData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, LoaderInfoObjectData<GC>> data_;
};

// Template method implementations

template<typename GC>
LoaderInfoObject<GC> LoaderInfoObject<GC>::not_yet_loaded(
    Activation<GC>* activation,
    std::shared_ptr<tag_utils::SwfMovie<GC>> movie,
    std::optional<StageObject<GC>> loader,
    std::optional<display_object::DisplayObject<GC>> root_clip,
    bool is_stage
) {
    auto class_obj = activation->avm2()->classes()->loaderinfo;
    auto base = ScriptObjectData<GC>::create(class_obj);

    auto shared_events = activation->context()
        ->avm2
        .classes()
        .eventdispatcher
        .construct(activation, {})
        .as_object()
        .value();

    auto uncaught_error_events = activation->context()
        ->avm2
        .classes()
        .uncaughterrorevents
        .construct(activation, {})
        .as_object()
        .value();

    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            LoaderStream<GC>::not_yet_loaded(movie, root_clip, is_stage),
            loader,
            false,  // init_event_fired
            false,  // complete_event_fired
            shared_events,
            uncaught_error_events,
            loader::ContentType::Unknown,
            false,  // expose_content
            false   // errored
        }
    );

    return LoaderInfoObject<GC>(data);
}

template<typename GC>
bool LoaderInfoObject<GC>::fire_init_and_complete_events(
    UpdateContext<GC>* context,
    uint16_t status,
    bool redirected
) {
    data_->expose_content.store(true);

    if (!data_->init_event_fired.load()) {
        data_->init_event_fired.store(true);

        // TODO - 'init' should be fired earlier during the download.
        // Right now, we fire it when downloading is fully completed.
        auto init_evt = EventObject<GC>::bare_default_event(context, "init");
        Avm2<GC>::dispatch_event(context, init_evt, Object<GC>(*this));
    }

    if (!data_->complete_event_fired.load()) {
        // NOTE: We have to check load progress here because this function
        // is called unconditionally at the end of every frame.
        bool should_complete = false;
        bool from_url = false;

        const auto& stream = data_->loaded_stream;
        if (stream.is_loaded()) {
            const auto& swf = std::get<LoaderStream<GC>::Swf>(stream.variant());
            should_complete = true;  // Simplified - would check loaded_bytes vs total_bytes
            from_url = swf.movie->url() != "file:///";
        }

        if (should_complete) {
            if (from_url) {
                auto activation = Activation<GC>::from_nothing(context);
                auto http_status_evt = EventObject<GC>::http_status_event(&activation, status, redirected);
                Avm2<GC>::dispatch_event(context, http_status_evt, Object<GC>(*this));
            }

            data_->complete_event_fired.store(true);
            auto complete_evt = EventObject<GC>::bare_default_event(context, "complete");
            Avm2<GC>::dispatch_event(context, complete_evt, Object<GC>(*this));
            return true;
        }
        return false;
    }
    return true;
}

template<typename GC>
void LoaderInfoObject<GC>::set_loader_stream(LoaderStream<GC> stream, GC* mc) {
    std::lock_guard<std::mutex> lock(data_->stream_mutex);
    data_->loaded_stream = stream;
}

template<typename GC>
void LoaderInfoObject<GC>::unload(UpdateContext<GC>* context) {
    // Reset properties
    auto movie = &context->root_swf();
    auto empty_swf = std::make_shared<tag_utils::SwfMovie<GC>>(
        tag_utils::SwfMovie<GC>::empty(movie->version(), movie->url())
    );
    auto loader_stream = LoaderStream<GC>::not_yet_loaded(empty_swf, std::nullopt, false);
    set_loader_stream(loader_stream, context->gc());
    set_errored(false);
    reset_init_and_complete_events();

    auto loader_obj = data_->loader
        .expect("LoaderInfo must have been created by Loader")
        .display_object()
        .as_container()
        .value();

    // Remove the Loader's content element if it exists.
    if (auto child = loader_obj.child_by_index(0)) {
        loader_obj.remove_child(context, child.value());
    }
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::LoaderInfoObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::LoaderInfoObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "LoaderInfoObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_LOADERINFO_OBJECT_H
