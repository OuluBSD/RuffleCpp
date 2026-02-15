/*
 * C++ header for context menu functionality
 * This replaces the functionality of core/src/context_menu.rs
 */

#ifndef CONTEXT_MENU_H
#define CONTEXT_MENU_H

#include "avm1.h"
#include "avm2.h"
#include "context.h"
#include "display_object.h"
#include "events.h"
#include "i18n.h"
#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace ruffle {

// Forward declarations
class EditText;
class Stage;
class UpdateContext;
class TextControlCode;

// Structure holding the temporary state of an open context menu.
//
// The context menu items and callbacks set to `object.menu`
// are stored aside when the menu is open. This way the context menu
// items work even if the movie changed `object.menu` in the meantime.
class ContextMenuState {
private:
    std::vector<ContextMenuItem> info_;
    std::vector<ContextMenuCallback> callbacks_;
    std::shared_ptr<DisplayObject> object_;

public:
    ContextMenuState() = default;

    // Create a new context menu state
    static std::shared_ptr<ContextMenuState> create() {
        return std::make_shared<ContextMenuState>();
    }

    // Add an item and callback to the context menu
    void push(ContextMenuItem item, ContextMenuCallback callback) {
        info_.push_back(std::move(item));
        callbacks_.push_back(std::move(callback));
    }

    // Get the context menu items
    const std::vector<ContextMenuItem>& info() const {
        return info_;
    }

    // Get a callback by index
    const ContextMenuCallback& callback(size_t index) const {
        if (index < callbacks_.size()) {
            return callbacks_[index];
        }
        throw std::out_of_range("Callback index out of range");
    }

    // Get the display object associated with this context menu
    std::shared_ptr<DisplayObject> get_display_object() const {
        return object_;
    }

    // Set the display object associated with this context menu
    void set_display_object(std::shared_ptr<DisplayObject> object) {
        object_ = std::move(object);
    }

    // Build built-in context menu items based on the flags
    void build_builtin_items(BuiltInItemFlags item_flags, std::shared_ptr<UpdateContext> context) {
        auto stage = context->stage();
        auto language = context->ui()->language();

        // When a text field is focused and the mouse is hovering it,
        // show the copy/paste menu.
        auto focused_text = context->focus_tracker().get_as_edit_text();
        auto hovered_obj = context->mouse_data().hovered_object();
        if (focused_text && 
            DisplayObject::equals(hovered_obj, std::static_pointer_cast<DisplayObject>(focused_text))) {
            build_text_items(focused_text, context);
            return;
        }

        auto root_mc = stage->root_clip();
        if (!root_mc) return;

        if (item_flags.play) {
            bool is_playing_root_movie = root_mc->playing();
            push(
                ContextMenuItem{
                    true,  // enabled
                    true,  // separator_before
                    core_text(language, "context-menu-play"),
                    is_playing_root_movie  // checked
                },
                ContextMenuCallback::Play()
            );
        }
        
        if (item_flags.rewind) {
            bool is_first_frame = root_mc->current_frame() <= 1;
            push(
                ContextMenuItem{
                    !is_first_frame,  // enabled
                    true,             // separator_before
                    core_text(language, "context-menu-rewind"),
                    false  // checked
                },
                ContextMenuCallback::Rewind()
            );
        }
        
        if (item_flags.forward_and_back) {
            bool is_first_frame = root_mc->current_frame() <= 1;
            push(
                ContextMenuItem{
                    true,  // enabled
                    false, // separator_before
                    core_text(language, "context-menu-forward"),
                    false  // checked
                },
                ContextMenuCallback::Forward()
            );
            push(
                ContextMenuItem{
                    !is_first_frame,  // enabled
                    false,            // separator_before
                    core_text(language, "context-menu-back"),
                    false  // checked
                },
                ContextMenuCallback::Back()
            );
        }
        
        if (item_flags.quality) {
            // TODO: This should be a submenu, but at time of writing those aren't supported
            auto stage_quality = stage->quality();
            push(
                ContextMenuItem{
                    stage_quality != StageQuality::LOW,  // enabled
                    true,                               // separator_before
                    stage_quality == StageQuality::LOW, // checked
                    core_text(language, "context-menu-quality-low")
                },
                ContextMenuCallback::QualityLow()
            );
            push(
                ContextMenuItem{
                    stage_quality != StageQuality::MEDIUM,  // enabled
                    false,                                  // separator_before
                    stage_quality == StageQuality::MEDIUM,  // checked
                    core_text(language, "context-menu-quality-medium")
                },
                ContextMenuCallback::QualityMedium()
            );
            push(
                ContextMenuItem{
                    stage_quality != StageQuality::HIGH,  // enabled
                    false,                                // separator_before
                    stage_quality == StageQuality::HIGH,  // checked
                    core_text(language, "context-menu-quality-high")
                },
                ContextMenuCallback::QualityHigh()
            );
        }
    }

private:
    // Build text-related context menu items
    void build_text_items(std::shared_ptr<EditText> text, std::shared_ptr<UpdateContext> context) {
        auto language = context->ui()->language();
        
        push(
            ContextMenuItem{
                text->is_text_control_applicable(TextControlCode::CUT, context),  // enabled
                true,  // separator_before
                core_text(language, "context-menu-cut"),
                false  // checked
            },
            ContextMenuCallback::TextControl(TextControlCode::CUT, text)
        );
        
        push(
            ContextMenuItem{
                text->is_text_control_applicable(TextControlCode::COPY, context),  // enabled
                false,  // separator_before
                core_text(language, "context-menu-copy"),
                false  // checked
            },
            ContextMenuCallback::TextControl(TextControlCode::COPY, text)
        );
        
        push(
            ContextMenuItem{
                text->is_text_control_applicable(TextControlCode::PASTE, context),  // enabled
                false,  // separator_before
                core_text(language, "context-menu-paste"),
                false  // checked
            },
            ContextMenuCallback::TextControl(TextControlCode::PASTE, text)
        );
        
        push(
            ContextMenuItem{
                text->is_text_control_applicable(TextControlCode::DELETE, context),  // enabled
                false,  // separator_before
                core_text(language, "context-menu-delete"),
                false  // checked
            },
            ContextMenuCallback::TextControl(TextControlCode::DELETE, text)
        );
        
        push(
            ContextMenuItem{
                text->is_text_control_applicable(TextControlCode::SELECT_ALL, context),  // enabled
                true,  // separator_before
                core_text(language, "context-menu-select-all"),
                false  // checked
            },
            ContextMenuCallback::TextControl(TextControlCode::SELECT_ALL, text)
        );
    }
};

// Context menu item structure
struct ContextMenuItem {
    bool enabled;
    bool separator_before;
    bool checked;
    std::string caption;

    ContextMenuItem(bool en, bool sep, const std::string& cap, bool chk)
        : enabled(en), separator_before(sep), checked(chk), caption(cap) {}
};

// Callback types for context menu items
class ContextMenuCallback {
public:
    enum class Type {
        ZOOM,
        QUALITY_LOW,
        QUALITY_MEDIUM,
        QUALITY_HIGH,
        PLAY,
        LOOP,
        REWIND,
        FORWARD,
        BACK,
        PRINT,
        AVM1,
        AVM2,
        TEXT_CONTROL
    };

private:
    Type type_;
    std::shared_ptr<Object> avm1_item_;
    std::shared_ptr<Object> avm1_callback_;
    std::shared_ptr<Object> avm2_item_;
    TextControlCode text_control_code_;
    std::shared_ptr<EditText> text_control_text_;

public:
    // Constructors for different callback types
    ContextMenuCallback() : type_(Type::ZOOM) {}
    
    static ContextMenuCallback Zoom() { return ContextMenuCallback(Type::ZOOM); }
    static ContextMenuCallback QualityLow() { return ContextMenuCallback(Type::QUALITY_LOW); }
    static ContextMenuCallback QualityMedium() { return ContextMenuCallback(Type::QUALITY_MEDIUM); }
    static ContextMenuCallback QualityHigh() { return ContextMenuCallback(Type::QUALITY_HIGH); }
    static ContextMenuCallback Play() { return ContextMenuCallback(Type::PLAY); }
    static ContextMenuCallback Loop() { return ContextMenuCallback(Type::LOOP); }
    static ContextMenuCallback Rewind() { return ContextMenuCallback(Type::REWIND); }
    static ContextMenuCallback Forward() { return ContextMenuCallback(Type::FORWARD); }
    static ContextMenuCallback Back() { return ContextMenuCallback(Type::BACK); }
    static ContextMenuCallback Print() { return ContextMenuCallback(Type::PRINT); }
    
    static ContextMenuCallback Avm1(std::shared_ptr<Object> item, std::shared_ptr<Object> callback) {
        ContextMenuCallback cb(Type::AVM1);
        cb.avm1_item_ = std::move(item);
        cb.avm1_callback_ = std::move(callback);
        return cb;
    }
    
    static ContextMenuCallback Avm2(std::shared_ptr<Object> item) {
        ContextMenuCallback cb(Type::AVM2);
        cb.avm2_item_ = std::move(item);
        return cb;
    }
    
    static ContextMenuCallback TextControl(TextControlCode code, std::shared_ptr<EditText> text) {
        ContextMenuCallback cb(Type::TEXT_CONTROL);
        cb.text_control_code_ = code;
        cb.text_control_text_ = std::move(text);
        return cb;
    }

    Type type() const { return type_; }
    
    // Getters for AVM1 callback
    std::shared_ptr<Object> avm1_item() const { 
        if (type_ != Type::AVM1) throw std::runtime_error("Not an AVM1 callback");
        return avm1_item_; 
    }
    
    std::shared_ptr<Object> avm1_callback() const { 
        if (type_ != Type::AVM1) throw std::runtime_error("Not an AVM1 callback");
        return avm1_callback_; 
    }
    
    // Getters for AVM2 callback
    std::shared_ptr<Object> avm2_item() const { 
        if (type_ != Type::AVM2) throw std::runtime_error("Not an AVM2 callback");
        return avm2_item_; 
    }
    
    // Getters for text control callback
    TextControlCode text_control_code() const { 
        if (type_ != Type::TEXT_CONTROL) throw std::runtime_error("Not a text control callback");
        return text_control_code_; 
    }
    
    std::shared_ptr<EditText> text_control_text() const { 
        if (type_ != Type::TEXT_CONTROL) throw std::runtime_error("Not a text control callback");
        return text_control_text_; 
    }

private:
    explicit ContextMenuCallback(Type type) : type_(type) {}
};

// Built-in item flags structure
struct BuiltInItemFlags {
    bool forward_and_back;
    bool loop_;
    bool play;
    bool print;
    bool quality;
    bool rewind;
    bool save;
    bool zoom;

    BuiltInItemFlags() 
        : forward_and_back(false), loop_(false), play(false), print(false),
          quality(false), rewind(false), save(false), zoom(false) {}

    // Create flags for a stage
    static BuiltInItemFlags for_stage(std::shared_ptr<Stage> stage) {
        auto root_mc = stage->root_clip();
        bool is_multiframe_movie = false;
        if (root_mc) {
            is_multiframe_movie = root_mc->header_frames() > 1;
        }
        
        if (is_multiframe_movie) {
            return BuiltInItemFlags{
                true,   // forward_and_back
                true,   // loop_
                true,   // play
                true,   // print
                true,   // quality
                true,   // rewind
                false,  // save
                true    // zoom
            };
        } else {
            return BuiltInItemFlags{
                false,  // forward_and_back
                false,  // loop_
                false,  // play
                true,   // print
                true,   // quality
                false,  // rewind
                false,  // save
                true    // zoom
            };
        }
    }
};

} // namespace ruffle

#endif // CONTEXT_MENU_H