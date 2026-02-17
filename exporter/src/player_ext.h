// C++ translation of exporter/src/player_ext.rs
// Player exporter extension

#ifndef RUFFLE_EXPORTER_PLAYER_EXT_H
#define RUFFLE_EXPORTER_PLAYER_EXT_H

#include <memory>
#include <mutex>
#include <optional>
#include <cstdint>
#include <any>

// Forward declarations
namespace ruffle {
namespace core {
    class Player;
}

namespace render {
namespace wgpu {
namespace backend {
    class WgpuRenderBackend;
}
namespace target {
    class TextureTarget;
}
}
}
}

// Forward declare image library types
namespace image {
    class RgbaImage;
}

namespace ruffle {
namespace exporter {

/// Extension trait for Player to support frame capture and export
class PlayerExporterExt {
public:
    virtual ~PlayerExporterExt() = default;

    /// Capture the current frame as an image
    /// @return Optional RGBA image if capture succeeded
    virtual std::shared_ptr<image::RgbaImage> capture_frame() = 0;

    /// Get the number of header frames in the SWF
    /// @return Number of header frames
    virtual uint16_t header_frames() = 0;

    /// Force the root clip to play
    virtual void force_root_clip_play() = 0;
};

/// Implementation of PlayerExporterExt for Arc<Mutex<Player>>
class PlayerExporterImpl : public PlayerExporterExt {
private:
    std::shared_ptr<std::mutex<Player>> player_;

public:
    explicit PlayerExporterImpl(std::shared_ptr<std::mutex<Player>> player)
        : player_(player)
    {}

    /// Capture the current frame as an image
    std::shared_ptr<image::RgbaImage> capture_frame() override {
        // In Rust:
        // let mut player = self.lock().unwrap();
        // let renderer = <dyn Any>::downcast_mut::<WgpuRenderBackend<TextureTarget>>(player.renderer_mut()).unwrap();
        // renderer.capture_frame()

        std::unique_lock<std::mutex> lock(*player_);
        
        // Get the player and downcast renderer to WgpuRenderBackend
        // This is a placeholder - actual implementation would do the downcast
        // and call capture_frame() on the renderer
        return nullptr;  // Placeholder
    }

    /// Get the number of header frames in the SWF
    uint16_t header_frames() override {
        // In Rust:
        // self.lock().unwrap().mutate_with_update_context(|ctx| ctx.root_swf.num_frames())

        std::unique_lock<std::mutex> lock(*player_);
        
        // Placeholder - actual implementation would call mutate_with_update_context
        // and get num_frames() from root_swf
        return 0;
    }

    /// Force the root clip to play
    void force_root_clip_play() override {
        // In Rust:
        // let mut player = self.lock().unwrap();
        // if !player.is_playing() { player.set_is_playing(true); }
        // player.mutate_with_update_context(|ctx| {
        //     if let Some(root_clip) = ctx.stage.root_clip()
        //         && let Some(movie_clip) = root_clip.as_movie_clip()
        //         && !movie_clip.playing()
        //     {
        //         movie_clip.play();
        //     }
        // });

        std::unique_lock<std::mutex> lock(*player_);
        
        // Check and resume if suspended
        // Placeholder - actual implementation would check is_playing() and set it
        bool is_playing = false;  // Placeholder
        if (!is_playing) {
            // set_is_playing(true);
        }

        // Also resume the root MovieClip if stopped
        // Placeholder - actual implementation would mutate context and play root clip
    }
};

} // namespace exporter
} // namespace ruffle

#endif // RUFFLE_EXPORTER_PLAYER_EXT_H
