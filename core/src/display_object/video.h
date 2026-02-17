// C++ translation of core/src/display_object/video.rs
// Video display object

#ifndef RUFFLE_CORE_DISPLAY_OBJECT_VIDEO_H
#define RUFFLE_CORE_DISPLAY_OBJECT_VIDEO_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <set>
#include <map>
#include <atomic>
#include <mutex>

// Forward declarations
namespace ruffle {
namespace swf {
    using CharacterId = uint16_t;
    class DefineVideoStream;
    class VideoFrame;
    enum class VideoCodec;
}

namespace common {
namespace tag_utils {
    class SwfMovie;
    class SwfSlice;
}
namespace utils {
    template<typename Self, typename Inner>
    struct HasPrefixField;
}
}

namespace render {
namespace bitmap {
    class BitmapInfo;
    enum class PixelSnapping;
}
namespace commands {
    template<typename T> class CommandHandler;
}
namespace quality {
    enum class StageQuality;
}
}

namespace video {
    using VideoStreamHandle = uint32_t;
    namespace error {
        class Error;
    }
    class EncodedFrame;
}

namespace core {
namespace avm1 {
    template<typename T> class NativeObject;
    template<typename T> class Object;
}

namespace avm2 {
    template<typename T> class StageObject;
}

namespace context {
    template<typename T> class RenderContext;
    template<typename T> class UpdateContext;
}

namespace streams {
    template<typename T> class NetStream;
}

namespace vminterface {
    template<typename T> class AvmObject;
    enum class Instantiator;
}

namespace display_object {
    enum class BoundsMode;
    struct Rectangle;
    struct Twips;
    struct Point;
    struct Matrix;
    class DisplayObjectBase;
    template<typename T> class DisplayObjectPtr;
    template<typename T> class DisplayObjectWeak;
    template<typename T> class TDisplayObject;
    struct Avm1TextFieldBinding;
    struct RenderOptions;

    template<typename GC> class VideoWeak;

    /// Video stream state
    enum class VideoStreamState {
        /// An uninstantiated video stream
        Uninstantiated,

        /// An instantiated video stream
        Instantiated
    };

    /// Video source types
    template<typename GC>
    class VideoSource {
    public:
        enum class Type {
            /// Video bitstream embedded in SWF
            Swf,

            /// Attached NetStream
            NetStream,

            /// Unconnected video
            Unconnected
        };

        VideoSource() : type_(Type::Unconnected) {}

        static VideoSource swf(std::shared_ptr<SwfVideoSource> source) {
            VideoSource vs;
            vs.type_ = Type::Swf;
            vs.swf_source_ = source;
            return vs;
        }

        static VideoSource netstream(streams::NetStream<GC> stream) {
            VideoSource vs;
            vs.type_ = Type::NetStream;
            vs.netstream_ = stream;
            return vs;
        }

        static VideoSource unconnected() {
            return VideoSource();
        }

        Type type() const { return type_; }

        std::shared_ptr<SwfVideoSource> swf_source() const { return swf_source_; }
        streams::NetStream<GC> netstream() const { return netstream_; }

    private:
        Type type_;
        std::shared_ptr<SwfVideoSource> swf_source_;
        streams::NetStream<GC> netstream_;
    };

    /// SWF video source data
    class SwfVideoSource {
    public:
        swf::DefineVideoStream streamdef;

        /// Frame locations: frame_id -> (start, end) in the movie data
        std::map<uint32_t, std::pair<size_t, size_t>> frames;
    };

    /// Video display object data
    /// Uses HasPrefixField pattern - base must be first member
    template<typename GC>
    class VideoData {
    public:
        VideoData() = default;

        // Base display object - must be first for HasPrefixField pattern
        DisplayObjectBase<GC> base;

        /// AVM1 text field bindings
        std::vector<Avm1TextFieldBinding<GC>> avm1_text_field_bindings;

        /// The source of the video data
        VideoSource<GC> source;

        /// The decoder stream
        VideoStreamState stream_state;
        uint32_t stream_index;
        video::VideoStreamHandle stream_handle;

        /// AVM representation of this video player
        std::optional<vminterface::AvmObject<GC>> object;

        /// List of keyframes for seeking
        std::set<uint32_t> keyframes;

        /// The movie that created this Video object
        std::shared_ptr<common::tag_utils::SwfMovie> movie;

        /// The last decoded frame
        std::optional<std::pair<uint32_t, render::bitmap::BitmapInfo>> decoded_frame;

        /// Video size (width, height)
        std::pair<int32_t, int32_t> size;

        // Friend classes for accessing private members
        friend class Video<GC>;
        friend class VideoWeak<GC>;
    };

    /// A Video display object for playing video content
    template<typename GC>
    class Video {
    public:
        using DataType = VideoData<GC>;
        using WeakType = VideoWeak<GC>;

        Video() = default;
        explicit Video(std::shared_ptr<DataType> data) : data_(std::move(data)) {}

        /// Construct a Video from SWF tag
        static Video from_swf_tag(
            std::shared_ptr<common::tag_utils::SwfMovie> movie,
            const swf::DefineVideoStream& streamdef
        );

        /// Create a new empty Video
        static Video create(
            std::shared_ptr<common::tag_utils::SwfMovie> movie,
            int32_t width,
            int32_t height,
            std::optional<vminterface::AvmObject<GC>> object
        );

        /// Set the video size
        void set_size(int32_t width, int32_t height);

        /// Attach a NetStream to this video
        void attach_netstream(context::UpdateContext<GC>* context, streams::NetStream<GC> stream);

        /// Preload frame data from SWF
        void preload_swf_frame(const swf::VideoFrame& tag);

        /// Seek to a particular frame
        void seek(context::UpdateContext<GC>* context, uint32_t frame_id);

        /// Get the base display object (for TDisplayObject trait)
        const DisplayObjectBase<GC>& base() const {
            return data_->base;
        }

        /// Get the character ID
        swf::CharacterId id() const;

        /// Get self bounds
        Rectangle<Twips> self_bounds(BoundsMode mode) const;

        /// Render this display object
        void render_self(context::RenderContext<GC>* context);

        /// Render with options
        void render_with_options(context::RenderContext<GC>* context, RenderOptions options);

        /// Get the movie
        std::shared_ptr<common::tag_utils::SwfMovie> movie() const {
            return data_->movie;
        }

        /// Get AVM1 object
        std::optional<avm1::Object<GC>> object1() const;

        /// Get AVM2 object
        std::optional<avm2::StageObject<GC>> object2() const;

        /// Set AVM2 object
        void set_object2(context::UpdateContext<GC>* context, avm2::StageObject<GC> to);

        /// Post instantiation handler
        void post_instantiation(
            context::UpdateContext<GC>* context,
            std::optional<avm1::Object<GC>> init_object,
            vminterface::Instantiator instantiated_by,
            bool run_frame
        );

        /// Construct frame handler
        void construct_frame(context::UpdateContext<GC>* context);

        /// Handler for ratio change (seek)
        void on_ratio_changed(context::UpdateContext<GC>* context, uint16_t new_ratio);

        /// Get AVM1 text field bindings
        std::optional<std::vector<Avm1TextFieldBinding<GC>>*> avm1_text_field_bindings();

        /// Get mutable AVM1 text field bindings
        std::optional<std::vector<Avm1TextFieldBinding<GC>>*> avm1_text_field_bindings_mut();

        /// Create a weak reference to this Video
        VideoWeak<GC> downgrade() const;

    private:
        std::shared_ptr<DataType> data_;

        /// Set the AVM object
        void set_object(context::UpdateContext<GC>* context, vminterface::AvmObject<GC> to);

        /// Set the video source
        void set_source(context::UpdateContext<GC>* context, VideoSource<GC> to);

        /// Internal seek to a frame
        void seek_internal(context::UpdateContext<GC>* context, uint32_t frame_id);
    };

    /// Weak reference to Video
    template<typename GC>
    class VideoWeak {
    public:
        VideoWeak() = default;
        explicit VideoWeak(std::weak_ptr<VideoData<GC>> data)
            : data_(std::move(data)) {}

        /// Try to upgrade the weak reference to a strong reference
        std::optional<Video<GC>> upgrade() const {
            if (auto locked = data_.lock()) {
                return std::make_optional(Video<GC>(locked));
            }
            return std::nullopt;
        }

    private:
        std::weak_ptr<VideoData<GC>> data_;
    };

} // namespace display_object
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DISPLAY_OBJECT_VIDEO_H
