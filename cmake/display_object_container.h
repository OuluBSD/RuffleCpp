/*
 * C++ header for display object container functionality
 * This replaces the functionality of core/src/display_object/container.rs
 */

#ifndef DISPLAY_OBJECT_CONTAINER_H
#define DISPLAY_OBJECT_CONTAINER_H

#include "display_object.h"
#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "string.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <optional>
#include <algorithm>
#include <functional>

namespace ruffle {

// Forward declarations
class UpdateContext;
class RenderContext;
class Stage;
class Avm1Button;
class MovieClip;
class LoaderDisplay;
class TabOrder;

// Enum for different container types
enum class ContainerType {
    STAGE,
    AVM1_BUTTON,
    MOVIE_CLIP,
    LOADER_DISPLAY
};

// Structure that stores child display objects
class ChildContainer {
private:
    // The list of all children in render order
    std::vector<std::shared_ptr<DisplayObject>> render_list_;
    
    // The mapping from timeline Depths to child display objects
    std::map<int, std::shared_ptr<DisplayObject>> depth_list_;
    
    // Whether this container has AVM1 objects pending removal
    bool pending_removals_;
    
    // Whether this is an AVM1 container
    bool is_action_script_3_;
    
    // Tab children property for AVM2
    bool tab_children_;

public:
    ChildContainer() : pending_removals_(false), is_action_script_3_(false), tab_children_(true) {}

    // Getters
    const std::vector<std::shared_ptr<DisplayObject>>& render_list() const { return render_list_; }
    const std::map<int, std::shared_ptr<DisplayObject>>& depth_list() const { return depth_list_; }
    bool pending_removals() const { return pending_removals_; }
    bool is_action_script_3() const { return is_action_script_3_; }
    bool tab_children() const { return tab_children_; }

    // Setters
    void set_pending_removals(bool pending) { pending_removals_ = pending; }
    void set_is_action_script_3(bool is_as3) { is_action_script_3_ = is_as3; }
    void set_tab_children(bool tab) { tab_children_ = tab; }

    // Get a child by its position in the render list
    std::shared_ptr<DisplayObject> get_by_index(size_t index) const {
        if (index < render_list_.size()) {
            return render_list_[index];
        }
        return nullptr;
    }

    // Get a child by its depth
    std::shared_ptr<DisplayObject> get_by_depth(int depth) const {
        auto it = depth_list_.find(depth);
        if (it != depth_list_.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Check if there's a child at a given depth
    bool has_depth(int depth) const {
        return depth_list_.find(depth) != depth_list_.end();
    }

    // Get a child by its name
    std::shared_ptr<DisplayObject> get_by_name(const std::string& name, bool case_sensitive) const {
        for (const auto& child : render_list_) {
            if (case_sensitive) {
                if (child->name() == name) {
                    return child;
                }
            } else {
                // Case-insensitive comparison
                std::string lower_name = child->name();
                std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
                std::string lower_query = name;
                std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);
                
                if (lower_name == lower_query) {
                    return child;
                }
            }
        }
        return nullptr;
    }

    // Get the number of children
    size_t num_children() const { return render_list_.size(); }

    // Get the highest depth among children
    int highest_depth() const {
        if (depth_list_.empty()) return 0;
        return depth_list_.rbegin()->first;
    }

    // Replace a child at a specific depth
    std::shared_ptr<DisplayObject> replace_at_depth(std::shared_ptr<DisplayObject> child, int depth) {
        // Find if there's already a child at this depth
        auto existing_it = depth_list_.find(depth);
        std::shared_ptr<DisplayObject> removed_child = nullptr;
        
        if (existing_it != depth_list_.end()) {
            removed_child = existing_it->second;
            
            // Remove from render list if it's there
            auto render_it = std::find(render_list_.begin(), render_list_.end(), removed_child);
            if (render_it != render_list_.end()) {
                render_list_.erase(render_it);
            }
            
            // Remove from depth list
            depth_list_.erase(existing_it);
        }

        // Add the new child
        depth_list_[depth] = child;
        
        // Insert into render list in the appropriate position based on depth
        auto insert_pos = std::upper_bound(render_list_.begin(), render_list_.end(), depth,
            [](int depth, const std::shared_ptr<DisplayObject>& obj) {
                return depth < obj->depth();
            });
        render_list_.insert(insert_pos, child);

        return removed_child;
    }

    // Insert a child at a specific index in the render list
    void insert_at_index(std::shared_ptr<DisplayObject> child, size_t index) {
        if (index <= render_list_.size()) {
            render_list_.insert(render_list_.begin() + index, child);
        } else {
            render_list_.push_back(child);
        }
    }

    // Swap two children in the render list
    void swap_at_index(size_t index1, size_t index2) {
        if (index1 < render_list_.size() && index2 < render_list_.size()) {
            std::swap(render_list_[index1], render_list_[index2]);
        }
    }

    // Remove a child from the depth list
    void remove_child_from_depth_list(std::shared_ptr<DisplayObject> child) {
        // Remove from depth list
        for (auto it = depth_list_.begin(); it != depth_list_.end();) {
            if (it->second == child) {
                it = depth_list_.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Remove a child from the render list
    bool remove_child_from_render_list(std::shared_ptr<DisplayObject> child) {
        auto it = std::find(render_list_.begin(), render_list_.end(), child);
        if (it != render_list_.end()) {
            render_list_.erase(it);
            return true;
        }
        return false;
    }

    // Insert a child into the depth list
    void insert_child_into_depth_list(int depth, std::shared_ptr<DisplayObject> child) {
        depth_list_[depth] = child;
    }

    // Check if container is empty
    bool is_empty() const { return render_list_.empty(); }

    // Remove a range of children by index
    void remove_range(const std::vector<size_t>& indices) {
        // Sort indices in descending order to avoid index shifting during removal
        std::vector<size_t> sorted_indices = indices;
        std::sort(sorted_indices.rbegin(), sorted_indices.rend());
        
        for (size_t index : sorted_indices) {
            if (index < render_list_.size()) {
                auto child = render_list_[index];
                render_list_.erase(render_list_.begin() + index);
                
                // Also remove from depth list
                for (auto it = depth_list_.begin(); it != depth_list_.end();) {
                    if (it->second == child) {
                        it = depth_list_.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }
    }
};

// Base class for display object containers
class DisplayObjectContainer : public DisplayObject {
protected:
    std::shared_ptr<ChildContainer> container_;

public:
    DisplayObjectContainer(DisplayObjectType type, uint16_t id) 
        : DisplayObject(type, id), container_(std::make_shared<ChildContainer>()) {}

    // Getters
    std::shared_ptr<ChildContainer> container() const { return container_; }

    // Get a child by its position in the render list
    std::shared_ptr<DisplayObject> child_by_index(size_t index) const {
        return container_->get_by_index(index);
    }

    // Get a child by its position in the depth list
    std::shared_ptr<DisplayObject> child_by_depth(int depth) const {
        return container_->get_by_depth(depth);
    }

    // Check if there's a child at a given depth
    bool has_child_at_depth(int depth) const {
        return container_->has_depth(depth);
    }

    // Get a child by its instance/timeline name
    std::shared_ptr<DisplayObject> child_by_name(const std::string& name, bool case_sensitive) const {
        return container_->get_by_name(name, case_sensitive);
    }

    // Get the number of children
    size_t num_children() const {
        return container_->num_children();
    }

    // Get the highest depth among children
    int highest_depth() const {
        return container_->highest_depth();
    }

    // Insert a child display object into the container at a specific position
    // in the depth list, removing any child already at that position
    std::shared_ptr<DisplayObject> replace_at_depth(
        std::shared_ptr<UpdateContext> context,
        std::shared_ptr<DisplayObject> child,
        int depth) {
        
        auto removed_child = container_->replace_at_depth(child, depth);
        
        child->set_parent(context, shared_from_this());
        child->set_place_frame(0);
        child->set_depth(depth);
        
        if (removed_child) {
            if (!container_->is_action_script_3()) {
                removed_child->avm1_unload(context);
            }
            removed_child->set_parent(context, nullptr);
        }
        
        invalidate_cached_bitmap();
        
        return removed_child;
    }

    // Move a child display object around in the container's depth list
    void swap_at_depth(std::shared_ptr<UpdateContext> context,
                      std::shared_ptr<DisplayObject> child,
                      int depth) {
        // Verify this is actually our child
        if (child->parent() != shared_from_this()) {
            return; // Not our child
        }
        
        // Implementation would swap the child at the specified depth
        // For now, we'll just call replace_at_depth to move it
        container_->replace_at_depth(child, depth);
        invalidate_cached_bitmap();
    }

    // Insert a child display object into the container at a specific position
    // in the render list
    void insert_at_index(std::shared_ptr<UpdateContext> context,
                        std::shared_ptr<DisplayObject> child,
                        size_t index) {
        
        bool parent_changed = false;
        if (auto old_parent = child->parent()) {
            if (old_parent != shared_from_this()) {
                if (auto old_container = old_parent->as_container()) {
                    old_container->remove_child(context, child);
                }
                parent_changed = true;
            }
        } else {
            parent_changed = true;
        }

        bool child_was_on_stage = child->is_on_stage(context);

        child->set_place_frame(0);
        child->set_parent(context, shared_from_this());
        if (!container_->is_action_script_3()) {
            child->set_avm1_removed(false);
        }

        container_->insert_at_index(child, index);

        if (parent_changed) {
            dispatch_added_event(shared_from_this(), child, child_was_on_stage, context);
        }

        invalidate_cached_bitmap();
    }

    // Swap two children in the render list
    void swap_at_index(std::shared_ptr<UpdateContext> context, size_t index1, size_t index2) {
        container_->swap_at_index(index1, index2);
        invalidate_cached_bitmap();
    }

    // Remove a child display object from this container
    void remove_child(std::shared_ptr<UpdateContext> context,
                     std::shared_ptr<DisplayObject> child) {
        
        // Verify this is our child
        if (child->parent() != shared_from_this()) {
            return; // Not our child
        }

        // Check if this child should have delayed removal (AVM1 only)
        if (!container_->is_action_script_3()) {
            // For now, we'll skip the AVM1 delayed removal logic
            // In a real implementation, this would check for unload listeners
        }

        remove_child_directly(context, child);
    }

    // Remove a child directly from this container
    void remove_child_directly(std::shared_ptr<UpdateContext> context,
                              std::shared_ptr<DisplayObject> child) {
        
        dispatch_removed_event(child, context);
        
        container_->remove_child_from_depth_list(child);
        container_->remove_child_from_render_list(child);

        if (!container_->is_action_script_3()) {
            child->avm1_unload(context);
        } else if (child->object2()) {
            child->set_parent(context, nullptr);
        }

        invalidate_cached_bitmap();
    }

    // Insert a child directly into this container's depth list
    void insert_child_into_depth_list(std::shared_ptr<UpdateContext> context,
                                     int depth,
                                     std::shared_ptr<DisplayObject> child) {
        
        child->set_depth(depth);
        child->set_parent(context, shared_from_this());
        container_->insert_child_into_depth_list(depth, child);

        invalidate_cached_bitmap();
    }

    // Remove a set of children identified by their render list indices
    void remove_range(std::shared_ptr<UpdateContext> context,
                     const std::vector<size_t>& indices) {
        
        std::vector<std::shared_ptr<DisplayObject>> removed_list;
        for (size_t index : indices) {
            if (index < container_->num_children()) {
                auto child = container_->get_by_index(index);
                if (child) {
                    removed_list.push_back(child);
                }
            }
        }

        for (const auto& removed : removed_list) {
            dispatch_removed_event(removed, context);
            removed->set_placed_by_avm2_script(true);
            container_->remove_child_from_depth_list(removed);
            container_->remove_child_from_render_list(removed);

            if (!container_->is_action_script_3()) {
                removed->avm1_unload(context);
            } else if (removed->object2()) {
                removed->set_parent(context, nullptr);
            }
        }

        invalidate_cached_bitmap();
    }

    // Check if the container is empty
    bool is_empty() const {
        return container_->is_empty();
    }

    // Iterate over children in render order
    const std::vector<std::shared_ptr<DisplayObject>>& children() const {
        return container_->render_list();
    }

    // Tab children property
    bool is_tab_children(std::shared_ptr<UpdateContext> context) const {
        if (container_->is_action_script_3()) {
            return container_->tab_children();
        } else {
            return is_tab_children_avm1(context);
        }
    }

    void set_tab_children(std::shared_ptr<UpdateContext> context, bool value) {
        if (container_->is_action_script_3()) {
            container_->set_tab_children(value);
        } else {
            // For AVM1, set the property on the object
            if (auto obj = object1()) {
                obj->set_property("tabChildren", std::make_shared<Value>(value), context);
            }
        }
    }

    // Fill tab order for accessibility
    void fill_tab_order(std::shared_ptr<TabOrder> tab_order,
                       std::shared_ptr<UpdateContext> context) const {
        
        if (!is_tab_children(context)) {
            return;
        }

        for (const auto& child : children()) {
            if (!child->visible()) {
                continue; // Non-visible objects are excluded from tab ordering
            }
            
            if (auto interactive_child = child->as_interactive()) {
                if (interactive_child->is_tabbable(context)) {
                    tab_order->add_object(interactive_child);
                }
            }
            
            if (auto container_child = child->as_container()) {
                container_child->fill_tab_order(tab_order, context);
            }
        }
    }

    // Render children in render order
    void render_children(std::shared_ptr<RenderContext> context) {
        int clip_depth = 0;
        std::vector<std::pair<int, std::shared_ptr<DisplayObject>>> clip_depth_stack;

        for (const auto& child : children()) {
            int depth = child->depth();

            child->pre_render(context);

            // Check if we need to pop off a mask
            while (clip_depth > 0 && depth > clip_depth) {
                auto [prev_clip_depth, clip_child] = clip_depth_stack.back();
                clip_depth_stack.pop_back();
                clip_depth = prev_clip_depth;
                
                context->commands.deactivate_mask();
                clip_child->render(context);
                context->commands.pop_mask();
            }
            
            if (child->clip_depth() > 0 && child->allow_as_mask()) {
                // Push and render the mask
                clip_depth_stack.emplace_back(clip_depth, child);
                clip_depth = child->clip_depth();
                context->commands.push_mask();
                child->render(context);
                context->commands.activate_mask();
            } else if (child->visible() || context->commands.drawing_mask()) {
                // Either a normal visible child, or a descendant of a mask object
                child->render(context);
            }
        }

        // Pop any remaining masks
        for (auto it = clip_depth_stack.rbegin(); it != clip_depth_stack.rend(); ++it) {
            context->commands.deactivate_mask();
            it->second->render(context);
            context->commands.pop_mask();
        }
    }

    // Helper methods for event dispatching
    static void dispatch_removed_from_stage_event(std::shared_ptr<DisplayObject> child,
                                                 std::shared_ptr<UpdateContext> context) {
        if (auto object = child->object2()) {
            auto removed_evt = Avm2EventObject::create_default_event(context, "removedFromStage");
            Avm2::dispatch_event(context, removed_evt, object);
        }

        if (auto child_container = child->as_container()) {
            for (const auto& grandchild : child_container->children()) {
                dispatch_removed_from_stage_event(grandchild, context);
            }
        }
    }

    static void dispatch_removed_event(std::shared_ptr<DisplayObject> child,
                                      std::shared_ptr<UpdateContext> context) {
        if (auto object = child->object2()) {
            auto removed_evt = Avm2EventObject::create_event(context, "removed", true, false);
            Avm2::dispatch_event(context, removed_evt, object);

            if (child->is_on_stage(context)) {
                dispatch_removed_from_stage_event(child, context);
            }
        }
    }

    static void dispatch_added_to_stage_event_only(std::shared_ptr<DisplayObject> child,
                                                  std::shared_ptr<UpdateContext> context) {
        if (auto object = child->object2()) {
            auto added_evt = Avm2EventObject::create_default_event(context, "addedToStage");
            Avm2::dispatch_event(context, added_evt, object);
        }
    }

    static void dispatch_added_to_stage_event(std::shared_ptr<DisplayObject> child,
                                             std::shared_ptr<UpdateContext> context) {
        dispatch_added_to_stage_event_only(child, context);

        if (auto child_container = child->as_container()) {
            for (const auto& grandchild : child_container->children()) {
                dispatch_added_to_stage_event(grandchild, context);
            }
        }
    }

    static void dispatch_added_event_only(std::shared_ptr<DisplayObject> child,
                                         std::shared_ptr<UpdateContext> context) {
        if (auto object = child->object2()) {
            auto added_evt = Avm2EventObject::create_event(context, "added", true, false);
            Avm2::dispatch_event(context, added_evt, object);
        }
    }

    static void dispatch_added_event(std::shared_ptr<DisplayObject> parent,
                                    std::shared_ptr<DisplayObject> child,
                                    bool child_was_on_stage,
                                    std::shared_ptr<UpdateContext> context) {
        dispatch_added_event_only(child, context);

        if (parent->is_on_stage(context) && !child_was_on_stage) {
            dispatch_added_to_stage_event(child, context);
        }
    }

    // Check if this object is a container
    bool is_container() const override { return true; }
    
    // Cast to container
    std::shared_ptr<DisplayObjectContainer> as_container() override {
        return std::static_pointer_cast<DisplayObjectContainer>(shared_from_this());
    }
};

} // namespace ruffle

#endif // DISPLAY_OBJECT_CONTAINER_H