/*
 * C++ header for AVM1 stage object functionality
 * This replaces the functionality of core/src/avm1/object/stage_object.rs
 */

#ifndef AVM1_STAGE_OBJECT_H
#define AVM1_STAGE_OBJECT_H

#include "avm1.h"
#include "avm1/value.h"
#include "avm1/object.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include "avm1/property_map.h"
#include "avm1/clamp.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace ruffle {

// Forward declarations
class DisplayObject;
class MovieClip;

// Stage object functionality for AVM1
class StageObject {
public:
    // Get a property from a display object
    static std::shared_ptr<Value> get_property(
        std::shared_ptr<DisplayObject> dobj,
        const std::string& name,
        std::shared_ptr<Activation> activation) {
        
        // Property search order for DisplayObjects:

        // 1) Path properties such as `_root`, `_parent`, `_levelN` (obeys case sensitivity)
        bool magic_property = name.length() > 0 && name[0] == '_';
        if (magic_property) {
            auto object = resolve_path_property(dobj, name, activation);
            if (object) {
                return object;
            }
        }

        // 2) Child display objects with the given instance name
        auto child = dobj->child_by_name(name, activation->is_case_sensitive());
        if (child) {
            auto child_obj = child->object1();
            if (child_obj) {
                return child_obj->as_value();
            } else {
                // If an object doesn't have an object representation, e.g. Graphic,
                // then trying to access it returns the parent instead
                auto parent = child->parent();
                if (parent) {
                    auto parent_obj = parent->object1();
                    if (parent_obj) {
                        return parent_obj->as_value();
                    }
                }
            }
            return std::make_shared<Value>(Value::UNDEFINED);
        }

        // 3) Display object properties such as `_x`, `_y` (never case sensitive)
        if (magic_property) {
            auto property = activation->context()->avm1.get_display_property(name);
            if (property) {
                return property;
            }
        }

        // 4) Properties of the underlying object
        if (auto obj = dobj->object1()) {
            return obj->get(name, activation);
        }

        return nullptr; // Property not found
    }

    // Set a property on a display object
    static bool set_property(
        std::shared_ptr<DisplayObject> dobj,
        const std::string& name,
        std::shared_ptr<Value> value,
        std::shared_ptr<Activation> activation) {
        
        // Check if it's a magic property (starts with underscore)
        if (name.length() > 0 && name[0] == '_') {
            return set_magic_property(dobj, name, value, activation);
        }

        // Otherwise, set it as a regular property
        if (auto obj = dobj->object1()) {
            obj->set(name, value, activation);
            return true;
        }

        return false;
    }

    // Resolve path properties like _root, _parent, _levelN
    static std::shared_ptr<Value> resolve_path_property(
        std::shared_ptr<DisplayObject> dobj,
        const std::string& name,
        std::shared_ptr<Activation> activation) {
        
        if (name == "_root") {
            return dobj->root()->object1()->as_value();
        } else if (name == "_parent") {
            auto parent = dobj->parent();
            if (parent) {
                if (auto parent_obj = parent->object1()) {
                    return parent_obj->as_value();
                }
            }
            return std::make_shared<Value>(Value::UNDEFINED);
        } else if (name.substr(0, 6) == "_level") {
            // Handle level references like _level0, _level1, etc.
            try {
                int level_num = std::stoi(name.substr(6));
                auto level_obj = activation->context()->get_level(level_num);
                if (level_obj) {
                    return level_obj->as_value();
                }
            } catch (...) {
                // Not a valid level number
            }
        }

        return nullptr; // Not a recognized path property
    }

    // Set magic properties (those starting with underscore)
    static bool set_magic_property(
        std::shared_ptr<DisplayObject> dobj,
        const std::string& name,
        std::shared_ptr<Value> value,
        std::shared_ptr<Activation> activation) {
        
        // Handle display object properties like _x, _y, _xscale, _yscale, etc.
        if (name == "_x") {
            double x = value->coerce_to_number(activation);
            dobj->set_x(x);
            return true;
        } else if (name == "_y") {
            double y = value->coerce_to_number(activation);
            dobj->set_y(y);
            return true;
        } else if (name == "_xscale") {
            double xscale = value->coerce_to_number(activation);
            dobj->set_scale_x(xscale / 100.0); // Scale is in percentage
            return true;
        } else if (name == "_yscale") {
            double yscale = value->coerce_to_number(activation);
            dobj->set_scale_y(yscale / 100.0); // Scale is in percentage
            return true;
        } else if (name == "_alpha") {
            double alpha = value->coerce_to_number(activation);
            dobj->set_alpha(alpha / 100.0); // Alpha is in percentage
            return true;
        } else if (name == "_rotation") {
            double rotation = value->coerce_to_number(activation);
            dobj->set_rotation(rotation);
            return true;
        } else if (name == "_visible") {
            bool visible = value->coerce_to_boolean(activation);
            dobj->set_visible(visible);
            return true;
        } else if (name == "_width") {
            double width = value->coerce_to_number(activation);
            dobj->set_width(width);
            return true;
        } else if (name == "_height") {
            double height = value->coerce_to_number(activation);
            dobj->set_height(height);
            return true;
        } else if (name == "_name") {
            std::string name_str = value->coerce_to_string(activation);
            dobj->set_name(name_str);
            return true;
        }

        // Property not handled
        return false;
    }

    // Coerce a value according to property index (for SetProperty action)
    static std::shared_ptr<Value> action_property_coerce(
        std::shared_ptr<Activation> activation,
        int index,
        std::shared_ptr<Value> value) {
        
        switch (index) {
            // Coerce to a number. This affects the following properties:
            // _x, _y, _xscale, _yscale, _currentframe, _totalframes, _alpha, _visible, _width, _height, _rotation, _framesloaded.
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 12:
                {
                    auto num_value = property_coerce_to_number(activation, value);
                    if (num_value.has_value()) {
                        return std::make_shared<Value>(num_value.value());
                    }
                    return value;
                }
            // Coerce to a number. This affects the following properties:
            // _highquality, _soundbuftime, _xmouse, _ymouse.
            case 16: case 18: case 20: case 21:
                return std::make_shared<Value>(value->coerce_to_number(activation));
            // Coerce to a string. This affects the following properties:
            // _name, _quality.
            case 13: case 19:
                return std::make_shared<Value>(value->coerce_to_string(activation));
            // No coercion for other properties
            default:
                return value;
        }
    }

private:
    // Helper function to coerce property values to numbers
    static std::optional<double> property_coerce_to_number(
        std::shared_ptr<Activation> activation,
        std::shared_ptr<Value> value) {
        
        if (*value != Value::UNDEFINED && *value != Value::NULL_VAL) {
            double n = value->coerce_to_number(activation);
            if (std::isfinite(n)) {
                return n;
            }
        }
        // Invalid value; return no value
        return std::nullopt;
    }
};

// Helper class for display object properties
class DisplayObjectPropertyHelper {
public:
    // Get a display object property by name
    static std::shared_ptr<Value> get_display_property(
        std::shared_ptr<DisplayObject> dobj,
        const std::string& name,
        std::shared_ptr<Activation> activation) {
        
        if (name == "_x") {
            return std::make_shared<Value>(dobj->x());
        } else if (name == "_y") {
            return std::make_shared<Value>(dobj->y());
        } else if (name == "_xscale") {
            return std::make_shared<Value>(dobj->scale_x() * 100.0); // Scale is in percentage
        } else if (name == "_yscale") {
            return std::make_shared<Value>(dobj->scale_y() * 100.0); // Scale is in percentage
        } else if (name == "_alpha") {
            return std::make_shared<Value>(dobj->alpha() * 100.0); // Alpha is in percentage
        } else if (name == "_rotation") {
            return std::make_shared<Value>(dobj->rotation());
        } else if (name == "_visible") {
            return std::make_shared<Value>(dobj->visible());
        } else if (name == "_width") {
            return std::make_shared<Value>(dobj->width());
        } else if (name == "_height") {
            return std::make_shared<Value>(dobj->height());
        } else if (name == "_name") {
            return std::make_shared<Value>(dobj->name());
        } else if (name == "_currentframe") {
            return std::make_shared<Value>(static_cast<double>(dobj->current_frame()));
        } else if (name == "_totalframes") {
            return std::make_shared<Value>(static_cast<double>(dobj->total_frames()));
        } else if (name == "_target") {
            return std::make_shared<Value>(dobj->target_path());
        } else if (name == "_framesloaded") {
            return std::make_shared<Value>(static_cast<double>(dobj->frames_loaded()));
        } else if (name == "_url") {
            return std::make_shared<Value>(dobj->url());
        } else if (name == "_xmouse") {
            return std::make_shared<Value>(x_mouse(activation, dobj));
        } else if (name == "_ymouse") {
            return std::make_shared<Value>(y_mouse(activation, dobj));
        }

        return nullptr; // Not a recognized display property
    }

private:
    // Helper functions for mouse coordinates
    static double x_mouse(std::shared_ptr<Activation> activation,
                         std::shared_ptr<DisplayObject> this_obj) {
        auto local = this_obj->local_mouse_position(activation->context());
        return local.x; // Assuming local position is in pixels
    }

    static double y_mouse(std::shared_ptr<Activation> activation,
                         std::shared_ptr<DisplayObject> this_obj) {
        auto local = this_obj->local_mouse_position(activation->context());
        return local.y; // Assuming local position is in pixels
    }
};

} // namespace ruffle

#endif // AVM1_STAGE_OBJECT_H