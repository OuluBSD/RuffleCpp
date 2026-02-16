// C++ translation of core/src/orphan_manager.rs
//! Special handling for AVM2 orphan objects

#ifndef RUFFLE_CORE_ORPHAN_MANAGER_H
#define RUFFLE_CORE_ORPHAN_MANAGER_H

#include <memory>
#include <vector>
#include <functional>
#include <algorithm>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename T> class UpdateContext;
    template<typename T> class Mutation;
    namespace display_object {
        template<typename T> class DisplayObject;
        template<typename T> class DisplayObjectWeak;
        template<typename T> class TDisplayObject;
    }
}
}

namespace ruffle {
namespace core {

/// The list of 'orphan' objects - these objects have no parent,
/// so we need to manually run their frames in `run_all_phases_avm2` to match
/// Flash's behavior. Clips are added to this list with `add_orphan_movie`.
/// and are removed automatically by `cleanup_dead_orphans`.
///
/// We store `DisplayObjectWeak`, since we don't want to keep these objects
/// alive if they would otherwise be garbage-collected. The movie will
/// stop ticking whenever garbage collection runs if there are no more
/// strong references around (this matches Flash's behavior).
template<typename GC>
class OrphanManager {
private:
    /// Reference-counted list of weak display object references
    std::shared_ptr<std::vector<display_object::DisplayObjectWeak<GC>>> orphans_;

    /// Get mutable reference to the orphans vector (triggers copy-on-write)
    std::vector<display_object::DisplayObjectWeak<GC>>& orphans_mut() {
        // If we're the only owner, return the vector directly
        // Otherwise, make a copy (simulating Rc::make_mut behavior)
        if (orphans_.use_count() > 1) {
            orphans_ = std::make_shared<std::vector<display_object::DisplayObjectWeak<GC>>>(
                *orphans_
            );
        }
        return *orphans_;
    }

public:
    /// Default constructor - creates empty orphan list
    OrphanManager() : orphans_(std::make_shared<std::vector<display_object::DisplayObjectWeak<GC>>>()) {}

    /// Add a DisplayObject to the orphan list. In AVM2, movies advance their
    /// frames even when they are not on a display list. Unfortunately,
    /// multiple SWFs rely on this behavior, so we need to match Flash's
    /// behavior. This should not be called manually - `movie_clip` will
    /// call it when necessary.
    ///
    /// Note: comparing pointers is correct because GcWeak keeps its allocation alive,
    /// so the pointers can't overlap by accident.
    void add_orphan_obj(display_object::DisplayObject<GC> dobj) {
        // Check if this object is already in the orphan list
        bool already_present = std::any_of(
            orphans_->begin(),
            orphans_->end(),
            [&dobj](const auto& d) {
                return d.as_ptr() == dobj.as_ptr();
            }
        );

        if (!already_present) {
            orphans_mut().push_back(dobj.downgrade());
        }
    }

    /// Iterate over each orphan object and call the provided function
    ///
    /// Clone the shared_ptr before iterating over it. Any modifications must go through
    /// `orphans_mut`, which will leave this copy unmodified.
    /// This ensures that any orphan additions/removals done by `f` will not affect
    /// the iteration in this method.
    static void each_orphan_obj(
        UpdateContext<GC>* context,
        std::function<void(display_object::DisplayObject<GC>, UpdateContext<GC>*)> f
    ) {
        // Clone the shared_ptr before iterating
        auto orphan_objs = context->orphan_manager.orphans_;

        for (const auto& orphan : *orphan_objs) {
            if (auto dobj = valid_orphan(orphan, context->gc())) {
                f(dobj, context);
            }
        }
    }

    /// Called at the end of `run_all_phases_avm2` - removes any movies
    /// that have been garbage collected, or are no longer orphans
    /// (they've since acquired a parent).
    void cleanup_dead_orphans(Mutation<GC>* mc) {
        auto& orphans = orphans_mut();
        orphans.erase(
            std::remove_if(
                orphans.begin(),
                orphans.end(),
                [mc](const display_object::DisplayObjectWeak<GC>& d) {
                    if (auto dobj = valid_orphan(d, mc)) {
                        // All clips that become orphaned (have their parent removed, or start out with no parent)
                        // get added to the orphan list. However, there's a distinction between clips
                        // that are removed from a RemoveObject tag, and clips that are removed from ActionScript.
                        //
                        // Clips removed from a RemoveObject tag only stay on the orphan list until the end
                        // of the frame - this lets them run a framescript (with 'this.parent == null')
                        // before they're removed. After that, they're removed from the orphan list,
                        // and will not be run in any way.
                        //
                        // Clips removed from ActionScript stay on the orphan list, and will be run
                        // indefinitely (if there are no remaining strong references, they will eventually
                        // be garbage collected).
                        //
                        // To detect this, we check 'placed_by_avm2_script'. This flag get set to 'true'
                        // for objects constructed from ActionScript, and for objects moved around
                        // in the timeline (add/remove child, swap depths) by ActionScript. A
                        // RemoveObject tag will only affect objects instantiated by the timeline,
                        // which have not been moved in the displaylist by ActionScript. Therefore,
                        // any orphan we see that has 'placed_by_avm2_script()' should stay on the orphan
                        // list, because it was not removed by a RemoveObject tag.
                        return !dobj.placed_by_avm2_script();
                    }
                    return true;  // Remove if upgrade failed (garbage collected)
                }
            ),
            orphans.end()
        );
    }
};

/// If the provided `DisplayObjectWeak` should have frames run, returns
/// the upgraded DisplayObject.
/// If this returns nullptr, the entry should be removed from the orphan list.
template<typename GC>
display_object::DisplayObject<GC> valid_orphan(
    const display_object::DisplayObjectWeak<GC>& dobj,
    Mutation<GC>* mc
) {
    auto upgraded = dobj.upgrade(mc);
    if (upgraded && upgraded.parent().is_none()) {
        return upgraded;
    }
    return nullptr;
}

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_ORPHAN_MANAGER_H
