/*
 * C++ header for video display object functionality
 * This replaces the functionality of core/src/display_object/video.rs
 */

#ifndef VIDEO_DISPLAY_OBJECT_H
#define VIDEO_DISPLAY_OBJECT_H

#include "display_object.h"
#include "display_object/interactive.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "net_stream.h"
#include "swf_movie.h"
#include "matrix.h"
#include "rectangle.h"
#include "twips.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include <set>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class NetStream;
class VideoStreamHandle;
class EncodedFrame;
class BitmapInfo;

// Enum for video stream states
enum class VideoStreamState {
    UNINSTANTIATED,
    INSTANTIATED
};

// Structure for video stream information
struct VideoStream {
    VideoStreamState state;
    std::optional<uint32_t> handle;  // Only valid when state is INSTANTIATED
    uint32_t seek_to_frame;         // Frame to seek to when instantiated

    VideoStream() : state(VideoStreamState::UNINSTANTIATED), seek_to_frame(0) {}
    VideoStream(uint32_t handle_val) : state(VideoStreamState::INSTANTIATED), handle(handle_val), seek_to_frame(0) {}
    VideoStream(uint32_t frame) : state(VideoStreamState::UNINSTANTIATED), seek_to_frame(frame) {}
};

// Enum for video source types
enum class VideoSourceType {
    SWF,
    NET_STREAM,
    UNCONNECTED
};

// Structure for SWF video source
struct SwfVideoSource {
    swf::DefineVideoStream streamdef;
    std::unordered_map<uint32_t, std::pair<size_t, size_t>> frames;  // frame_id -> (start, end) in movie data

    SwfVideoSource(swf::DefineVideoStream def) : streamdef(std::move(def)) {}
};

// Union for video source
class VideoSource {
private:
    VideoSourceType type_;
    std::shared_ptr<SwfVideoSource> swf_source_;
    std::shared_ptr<NetStream> net_stream_;

public:
    VideoSource() : type_(VideoSourceType::UNCONNECTED) {}
    
    static VideoSource from_swf(std::shared_ptr<SwfVideoSource> source) {
        VideoSource vs;
        vs.type_ = VideoSourceType::SWF;
        vs.swf_source_ = std::move(source);
        return vs;
    }
    
    static VideoSource from_net_stream(std::shared_ptr<NetStream> stream) {
        VideoSource vs;
        vs.type_ = VideoSourceType::NET_STREAM;
        vs.net_stream_ = std::move(stream);
        return vs;
    }
    
    static VideoSource unconnected() {
        return VideoSource();
    }

    VideoSourceType type() const { return type_; }
    std::shared_ptr<SwfVideoSource> swf_source() const { return swf_source_; }
    std::shared_ptr<NetStream> net_stream() const { return net_stream_; }
};

// Video display object class
class Video : public DisplayObject {
private:
    VideoSource source_;
    VideoStream stream_;
    std::shared_ptr<Object> object_;
    std::set<uint32_t> keyframes_;
    std::shared_ptr<SwfMovie> movie_;
    std::pair<int32_t, int32_t> size_;
    std::optional<std::pair<uint32_t, BitmapInfo>> decoded_frame_;
    std::vector<Avm1TextFieldBinding> avm1_text_field_bindings_;

public:
    Video(std::shared_ptr<SwfVideoSource> source, 
          std::shared_ptr<SwfMovie> movie,
          int32_t width, int32_t height)
        : DisplayObject(DisplayObjectType::VIDEO, source->streamdef.id),
          source_(VideoSource::from_swf(std::move(source))),
          stream_(0),  // Uninstantiated at frame 0 initially
          movie_(std::move(movie)),
          size_({width, height}) {}

    // Create a video object from an SWF tag
    static std::shared_ptr<Video> from_swf_tag(std::shared_ptr<SwfMovie> movie,
                                             swf::DefineVideoStream streamdef,
                                             std::shared_ptr<MutationContext> mc) {
        auto size = std::make_pair(static_cast<int32_t>(streamdef.width), 
                                  static_cast<int32_t>(streamdef.height));
        
        auto swf_source = std::make_shared<SwfVideoSource>(std::move(streamdef));
        auto video = std::make_shared<Video>(std::move(swf_source), std::move(movie), size.first, size.second);
        
        video->size_ = size;
        return video;
    }

    // Create a new video object
    static std::shared_ptr<Video> create(std::shared_ptr<MutationContext> mc,
                                       std::shared_ptr<SwfMovie> movie,
                                       int32_t width, int32_t height,
                                       std::shared_ptr<Object> object = nullptr) {
        auto swf_source = std::make_shared<SwfVideoSource>(swf::DefineVideoStream{});
        auto video = std::make_shared<Video>(std::move(swf_source), std::move(movie), width, height);
        
        video->source_ = VideoSource::unconnected();
        video->object_ = std::move(object);
        return video;
    }

    // Set the size of the video
    void set_size(int32_t width, int32_t height) {
        size_ = std::make_pair(width, height);
    }

    // Convert this Video into a NetStream sourced video
    void attach_net_stream(std::shared_ptr<UpdateContext> context,
                          std::shared_ptr<NetStream> stream) {
        source_ = VideoSource::from_net_stream(std::move(stream));
        stream_ = VideoStream(0);  // Uninstantiated at frame 0
        keyframes_.clear();
    }

    // Preload frame data from an SWF
    void preload_swf_frame(const swf::VideoFrame& tag) {
        if (source_.type() != VideoSourceType::SWF) {
            return; // Only valid for SWF sources
        }

        auto swf_source = source_.swf_source();
        if (!swf_source) return;

        // Create a subslice of the movie data for this frame
        size_t start = tag.data.start;
        size_t end = tag.data.end;

        auto frames = swf_source->frames;
        if (frames.find(tag.frame_num) != frames.end()) {
            printf("Warning: Duplicate frame %d\n", tag.frame_num);
        }

        frames[tag.frame_num] = std::make_pair(start, end);
    }

    // Seek to a particular frame in the video stream
    void seek(std::shared_ptr<UpdateContext> context, uint32_t frame_id) {
        if (stream_.state == VideoStreamState::UNINSTANTIATED) {
            stream_ = VideoStream(frame_id);
            return;
        }

        uint32_t num_frames = 0;
        switch (source_.type()) {
            case VideoSourceType::SWF: {
                auto swf_source = source_.swf_source();
                if (swf_source) {
                    num_frames = swf_source->streamdef.num_frames;
                }
                break;
            }
            case VideoSourceType::NET_STREAM:
                return; // NetStream videos don't use this seeking mechanism
            case VideoSourceType::UNCONNECTED:
                return; // Unconnected videos can't seek
        }

        // Wrap frame_id to video boundaries
        if (num_frames > 0) {
            frame_id = frame_id % num_frames;
        } else {
            frame_id = 0;
        }

        // Check if we're already at this frame
        if (decoded_frame_ && decoded_frame_->first == frame_id) {
            return; // Already there, no-op
        }

        // Determine if this is an ordered seek (next frame) or not
        bool is_ordered_seek = false;
        if (decoded_frame_) {
            uint32_t last_frame = decoded_frame_->first;
            is_ordered_seek = (frame_id == 0) || (frame_id == last_frame + 1);
        } else {
            is_ordered_seek = (frame_id == 0);
        }

        // Determine where to start seeking from
        uint32_t sweep_from;
        if (is_ordered_seek) {
            sweep_from = frame_id; // No need to sweep
        } else {
            // Find the previous keyframe
            uint32_t prev_keyframe_id = 0;
            for (const auto& kf : keyframes_) {
                if (kf <= frame_id) {
                    prev_keyframe_id = kf;
                } else {
                    break;
                }
            }

            // Start from either the previous keyframe or last frame (whichever is closer)
            if (decoded_frame_) {
                uint32_t last_frame = decoded_frame_->first;
                if (frame_id > last_frame) {
                    // When seeking forward, continue from last frame if it's closer
                    sweep_from = std::max(prev_keyframe_id, last_frame + 1);
                } else {
                    sweep_from = prev_keyframe_id;
                }
            } else {
                sweep_from = prev_keyframe_id;
            }
        }

        // Seek through all frames from sweep_from to frame_id
        for (uint32_t fr = sweep_from; fr <= frame_id; ++fr) {
            seek_internal(context, fr);
        }
    }

    // Getters
    const VideoSource& source() const { return source_; }
    const VideoStream& video_stream() const { return stream_; }
    std::shared_ptr<Object> object1() const override { return object_; }
    std::shared_ptr<Avm2StageObject> object2() const override { 
        if (object_ && object_->is_avm2_object()) {
            return object_->as_avm2_object();
        }
        return nullptr;
    }
    std::shared_ptr<SwfMovie> movie() const override { return movie_; }
    std::pair<int32_t, int32_t> size() const { return size_; }
    const std::set<uint32_t>& keyframes() const { return keyframes_; }
    const std::optional<std::pair<uint32_t, BitmapInfo>>& decoded_frame() const { return decoded_frame_; }

    // Setters
    void set_object2(std::shared_ptr<UpdateContext> context, 
                    std::shared_ptr<Avm2StageObject> obj) override {
        if (!object_) {
            object_ = std::make_shared<Object>(obj);
        } else {
            object_->set_avm2_object(obj);
        }
    }

    // Post instantiation setup
    void post_instantiation(std::shared_ptr<UpdateContext> context,
                           std::shared_ptr<Object> init_object,
                           Instantiator instantiated_by,
                           bool run_frame) override {
        DisplayObject::post_instantiation(context, init_object, instantiated_by, run_frame);

        if (movie()->is_action_script_3()) {
            set_default_instance_name(context);
        }

        // Handle different video source types
        std::pair<std::optional<uint32_t>, std::set<uint32_t>> stream_and_keyframes;
        
        switch (source_.type()) {
            case VideoSourceType::SWF: {
                auto swf_source = source_.swf_source();
                if (!swf_source) break;

                auto& streamdef = swf_source->streamdef;
                if (streamdef.codec == swf::VideoCodec::NONE) {
                    // No codec means no frames
                    stream_and_keyframes = std::make_pair(std::nullopt, std::set<uint32_t>{});
                } else {
                    // Register the video stream with the video backend
                    auto stream_handle = context->video->register_video_stream(
                        streamdef.num_frames,
                        std::make_pair(streamdef.width, streamdef.height),
                        streamdef.codec,
                        streamdef.deblocking
                    );

                    std::set<uint32_t> keyframes;
                    
                    // Preload frames
                    for (const auto& [frame_id, frame_range] : swf_source->frames) {
                        auto encoded_frame = EncodedFrame{
                            streamdef.codec,
                            movie_->data().substr(frame_range.first, frame_range.second - frame_range.first),
                            frame_id
                        };
                        
                        auto result = context->video->preload_video_stream_frame(stream_handle.value(), encoded_frame);
                        
                        if (result && result->is_keyframe()) {
                            keyframes.insert(frame_id);
                        }
                    }
                    
                    stream_and_keyframes = std::make_pair(stream_handle, keyframes);
                }
                break;
            }
            case VideoSourceType::NET_STREAM:
            case VideoSourceType::UNCONNECTED:
                return; // Nothing to do for these sources
        }

        // Update stream and keyframes
        uint32_t starting_seek = 0;
        if (stream_.state == VideoStreamState::UNINSTANTIATED) {
            starting_seek = stream_.seek_to_frame;
        } else {
            printf("Warning: Reinstantiating already-instantiated video stream!\n");
        }

        if (stream_and_keyframes.first.has_value()) {
            stream_ = VideoStream(stream_and_keyframes.first.value());
        }
        keyframes_ = stream_and_keyframes.second;

        // Create AVM1 object if needed
        if (!object_ && !movie()->is_action_script_3()) {
            auto avm1_proto = context->avm1->prototypes(movie()->version()).video;
            auto video_obj = Object::new_with_native(
                &context->strings,
                avm1_proto,
                NativeObject::Video(shared_from_this())
            );
            object_ = std::move(video_obj);
        }

        // Seek to the starting frame
        seek(context, starting_seek);
    }

    // Construct frame for AVM2
    void construct_frame(std::shared_ptr<UpdateContext> context) override {
        if (movie()->is_action_script_3() && !object2()) {
            auto video_class = context->avm2->classes().video;
            auto video_obj = Avm2StageObject::for_display_object(
                context->gc(), shared_from_this(), video_class);
            
            // We don't need to call the initializer method, as AVM2 can't link
            // a custom class to a Video, and the initializer method for Video
            // itself only sets the size of the Video - the Video already has the
            // correct size at this point.
            
            set_object2(context, video_obj);
            on_construction_complete(context);
        }
    }

    // Handle ratio changes (for morphing)
    void on_ratio_changed(std::shared_ptr<UpdateContext> context, uint16_t new_ratio) override {
        seek(context, static_cast<uint32_t>(new_ratio));
    }

    // Get the character ID
    uint16_t id() const override {
        switch (source_.type()) {
            case VideoSourceType::SWF: {
                auto swf_source = source_.swf_source();
                return swf_source ? swf_source->streamdef.id : 0;
            }
            case VideoSourceType::NET_STREAM:
            case VideoSourceType::UNCONNECTED:
                return 0;
        }
    }

    // Get the bounds of this video object
    Rectangle<Twips> self_bounds(BoundsMode mode) const override {
        auto [width, height] = size_;
        return Rectangle<Twips>(
            Twips(0),
            Twips(0),
            Twips::from_pixels_i32(width),
            Twips::from_pixels_i32(height)
        );
    }

    // Render the video
    void render_with_options(std::shared_ptr<RenderContext> context, 
                           RenderOptions options) override {
        // Check if off-screen and should be culled
        if (!context->is_offscreen && 
            !world_bounds(BoundsMode::ENGINE).intersects(context->stage->view_bounds())) {
            return; // Off-screen, culled
        }

        // Apply transform if needed
        if (options.apply_transform) {
            auto transform = base_transform(options.apply_matrix);
            context->transform_stack.push(transform);
        }

        auto transform = context->transform_stack.current_transform();
        auto bounds = self_bounds(BoundsMode::ENGINE);

        // Determine rendering parameters based on source type
        bool smoothed_flag = false;
        std::optional<size_t> num_frames;
        uint8_t version = movie()->version();
        std::optional<BitmapInfo> decoded_frame;
        std::optional<swf::VideoCodec> codec;

        switch (source_.type()) {
            case VideoSourceType::SWF: {
                auto swf_source = source_.swf_source();
                if (swf_source) {
                    smoothed_flag = swf_source->streamdef.is_smoothed;
                    num_frames = swf_source->frames.size();
                    decoded_frame = decoded_frame_ ? std::make_optional(decoded_frame_->second) : std::nullopt;
                    codec = swf_source->streamdef.codec;
                }
                break;
            }
            case VideoSourceType::NET_STREAM: {
                if (auto net_stream = source_.net_stream()) {
                    smoothed_flag = false;
                    decoded_frame = net_stream->last_decoded_bitmap();
                    codec = std::nullopt;
                }
                break;
            }
            case VideoSourceType::UNCONNECTED:
                if (options.apply_transform) {
                    context->transform_stack.pop();
                }
                return;
        }

        // Determine smoothing based on quality settings
        bool smoothing = true;
        auto quality = context->stage->quality();
        if (quality == StageQuality::LOW) {
            smoothing = false;
        } else if (version >= 8) {
            smoothing = smoothed_flag;
        } else if (quality == StageQuality::MEDIUM) {
            smoothing = false;
        } else if (quality == StageQuality::HIGH) {
            smoothing = (num_frames == 1);
        }

        // Render the bitmap if available
        if (decoded_frame.has_value()) {
            // Adjust scale to match declared bounds vs actual decoded frame size
            auto bitmap = decoded_frame.value();
            float scaleX = bounds.width().to_pixels() / static_cast<float>(bitmap.width);
            float scaleY = bounds.height().to_pixels() / static_cast<float>(bitmap.height);
            
            transform.matrix = transform.matrix * Matrix::scale(scaleX, scaleY);

            context->commands.render_bitmap(
                bitmap.handle,
                transform,
                smoothing,
                PixelSnapping::NEVER
            );
        } else if (codec != swf::VideoCodec::NONE) {
            printf("Warning: Video has no decoded frame to render.\n");
        }

        // Pop transform if applied
        if (options.apply_transform) {
            context->transform_stack.pop();
        }
    }

    // Cast to specific types
    std::shared_ptr<Video> as_video() override {
        return std::static_pointer_cast<Video>(shared_from_this());
    }
    
    bool is_video() const override { return true; }

    // Check if this object allows being used as a mask
    bool allow_as_mask() const override {
        return !is_empty(); // Video can be used as mask if not empty
    }

    // Check if this object is empty
    bool is_empty() const override {
        return source_.type() == VideoSourceType::UNCONNECTED;
    }

private:
    // Internal seek function (does not validate keyframes)
    void seek_internal(std::shared_ptr<UpdateContext> context, uint32_t frame_id) {
        if (stream_.state != VideoStreamState::INSTANTIATED || !stream_.handle.has_value()) {
            printf("Error: Attempted to seek uninstantiated video stream.\n");
            return;
        }

        auto stream_handle = stream_.handle.value();

        switch (source_.type()) {
            case VideoSourceType::SWF: {
                auto swf_source = source_.swf_source();
                if (!swf_source) return;

                auto frame_it = swf_source->frames.find(frame_id);
                if (frame_it != swf_source->frames.end()) {
                    auto [start, end] = frame_it->second;
                    auto data = movie_->data().substr(start, end - start);
                    
                    auto encoded_frame = EncodedFrame{
                        swf_source->streamdef.codec,
                        data,
                        frame_id
                    };
                    
                    auto result = context->video->decode_video_stream_frame(stream_handle, encoded_frame, context->renderer);
                    
                    if (result) {
                        decoded_frame_ = std::make_pair(frame_id, result.value());
                        invalidate_cached_bitmap();
                        context->needs_render = true;
                    } else {
                        printf("Error when seeking to video frame %d\n", frame_id);
                    }
                } else {
                    // If frame not found, try to reuse the last decoded frame
                    if (decoded_frame_) {
                        // Just update the frame ID
                        decoded_frame_ = std::make_pair(frame_id, decoded_frame_.value().second);
                    } else {
                        printf("Error: Seeking before decoding any frames\n");
                    }
                }
                break;
            }
            case VideoSourceType::NET_STREAM:
            case VideoSourceType::UNCONNECTED:
                return; // Nothing to do for these sources
        }
    }
};

} // namespace ruffle

#endif // VIDEO_DISPLAY_OBJECT_H