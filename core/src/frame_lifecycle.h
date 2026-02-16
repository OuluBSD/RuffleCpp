// C++ translation of core/src/frame_lifecycle.rs
//! Frame events management
//!
//! This module aids in keeping track of which frame execution phase we are in.
//!
//! For AVM2 code, display objects execute a series of discrete phases, and
//! each object is notified about the current frame phase in rendering order.
//! When objects are created, they are 'caught up' to the current frame phase
//! to ensure correct order of operations.
//!
//! AVM1 code (presumably, either on an AVM1 stage or within an AVM1Movie)
//! runs in one phase, with timeline operations executing with all phases
//! inline in the order that clips were originally created.

#ifndef RUFFLE_CORE_FRAME_LIFECYCLE_H
#define RUFFLE_CORE_FRAME_LIFECYCLE_H

#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename T> class Avm2;
    template<typename T> class EventObject;
}
namespace display_object {
    template<typename T> class DisplayObject;
    template<typename T> class MovieClip;
    template<typename T> class TDisplayObject;
}
namespace loader {
    class LoadManager;
}
namespace orphan_manager {
    class OrphanManager;
}
template<typename T> class UpdateContext;
}
}

namespace ruffle {
namespace core {

/// Which phase of the frame we're currently in.
///
/// AVM2 frames exist in one of four phases: Enter, Construct,
/// FrameScripts, or Exit. An additional Idle phase covers rendering and
/// event processing.
enum class FramePhase {
    /// We're entering the next frame.
    ///
    /// When movie clips enter a new frame, they must do two things:
    ///
    ///  - Remove all children that should not exist on the next frame.
    ///  - Increment their current frame number.
    ///
    /// Once this phase ends, we fire enterFrame on the broadcast list.
    Enter,

    /// We're constructing children of existing display objects.
    ///
    /// All PlaceObject tags should execute at this time.
    ///
    /// Once we construct the frame, we fire frameConstructed on the
    /// broadcast list.
    Construct,

    /// We're running all queued frame scripts.
    ///
    /// Frame scripts are the AS3 equivalent of old-style DoAction tags. They
    /// are queued in the Update phase if the current timeline frame number
    /// differs from the prior frame's one.
    FrameScripts,

    /// We're finishing frame processing.
    ///
    /// When we exit a completed frame, we fire exitFrame on the broadcast
    /// list.
    Exit,

    /// We're not currently executing any frame code.
    ///
    /// At this point in time, event handlers are expected to run. No frame
    /// catch-up work should execute.
    Idle,
};

/// Run one frame according to AVM2 frame order.
/// NOTE: The each_orphan_movie calls are in really odd places,
/// but this is needed to match Flash Player's output. There may
/// still be lurking bugs, but the current code matches Flash's
/// output exactly for two complex test cases (see avm2/orphan_movie*)
template<typename GC>
void run_all_phases_avm2(UpdateContext<GC>* context);

/// Like run_all_phases_avm2, but specialized for the "nested frame" triggered
/// by a goto. This is different enough to not be worth combining into a single
/// method with extra parameters.
///
/// During a goto, we run frame construction, framescripts, and frame exits for the *entire stage*.
/// This even extends to orphans - for example, calling gotoAndStop on an orphan will
/// cause frame construction to get run for the *current frame* of other objects on the timeline
/// (even if the goto was called from an enterFrame event handler).
template<typename GC>
void run_inner_goto_frame(
    UpdateContext<GC>* context,
    const std::vector<display_object::DisplayObject<GC>*>& removed_frame_scripts,
    display_object::MovieClip<GC>* initial_clip
);

/// Broadcast a enterFrame event to all DisplayObjects.
template<typename GC>
void broadcast_frame_entered(UpdateContext<GC>* context);

/// Broadcast a frameConstructed event to all DisplayObjects.
template<typename GC>
void broadcast_frame_constructed(UpdateContext<GC>* context);

/// Broadcast a exitFrame event to all DisplayObjects.
template<typename GC>
void broadcast_frame_exited(UpdateContext<GC>* context);

/// Empty the context.frame_script_cleanup_queue by running frame scripts for
/// each clip in the queue.
template<typename GC>
void run_frame_script_cleanup(UpdateContext<GC>* context);

/// Run all previously-executed frame phases on a newly-constructed display
/// object.
///
/// This is a no-op on AVM1, which has it's own catch-up logic.
template<typename GC>
void catchup_display_object_to_frame(
    UpdateContext<GC>* context,
    display_object::DisplayObject<GC>* dobj
);

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_FRAME_LIFECYCLE_H
