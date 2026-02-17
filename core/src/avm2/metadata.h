// C++ translation of core/src/avm2/metadata.rs
//! AVM2 Metadata representation

#ifndef RUFFLE_CORE_AVM2_METADATA_H
#define RUFFLE_CORE_AVM2_METADATA_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class UpdateContext;

    namespace avm2 {
        template<typename T> class Activation;
        template<typename T> class Object;
        template<typename T> class ArrayObject;
        template<typename T> class ScriptObject;
        template<typename T> class AvmString;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class Gc;
        struct TranslationUnit;

        namespace swf {
            namespace avm2 {
                namespace types {
                    template<typename T> class Index;
                    struct Metadata;
                }
            }
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Represents a single key-value pair for a trait metadata
template<typename GC>
class MetadataItem {
public:
    MetadataItem() = default;
    MetadataItem(AvmString<GC> key, AvmString<GC> value)
        : key_(key), value_(value) {}

    const AvmString<GC>& key() const { return key_; }
    const AvmString<GC>& value() const { return value_; }

private:
    AvmString<GC> key_;
    AvmString<GC> value_;
};

/// Represents a single metadata item for a trait
template<typename GC>
class Metadata {
public:
    Metadata() = default;
    Metadata(AvmString<GC> name, std::vector<MetadataItem<GC>> items)
        : name_(name), items_(std::move(items)) {}

    /// Converts an AbcMetadata into a Metadata by resolving all the indexes
    ///
    /// @param activation The current activation
    /// @param translation_unit The translation unit
    /// @param metadata The metadata indexes from ABC
    /// @return Optional vector of Metadata objects
    static std::optional<std::vector<Metadata<GC>>> from_abc_index(
        Activation<GC>* activation,
        TranslationUnit<GC> translation_unit,
        const std::vector<swf::avm2::types::Index<swf::avm2::types::Metadata>>& metadata
    );

    /// Converts the Metadata to an Object of the form used in avmplus:describeTypeJSON()
    ///
    /// @param context The update context
    /// @return Object representing the metadata
    Object<GC> as_json_object(UpdateContext<GC>* context) const;

    const AvmString<GC>& name() const { return name_; }
    const std::vector<MetadataItem<GC>>& items() const { return items_; }

private:
    AvmString<GC> name_;
    std::vector<MetadataItem<GC>> items_;
};

// Template method implementations

template<typename GC>
std::optional<std::vector<Metadata<GC>>> Metadata<GC>::from_abc_index(
    Activation<GC>* activation,
    TranslationUnit<GC> translation_unit,
    const std::vector<swf::avm2::types::Index<swf::avm2::types::Metadata>>& metadata
) {
    if (metadata.empty()) {
        return std::nullopt;
    }

    auto abc = translation_unit.abc();
    std::vector<Metadata<GC>> trait_metadata_list;

    for (const auto& single_metadata_idx : metadata) {
        // Lookup the Index<Metadata> to convert it into a Metadata
        auto idx = static_cast<size_t>(single_metadata_idx.index());
        if (idx >= abc->metadata.size()) {
            // Error handling would go here in real implementation
            return std::nullopt;
        }

        const auto& single_metadata = abc->metadata[idx];

        auto name = translation_unit.pool_string(
            static_cast<uint32_t>(single_metadata.name.index()),
            activation->strings()
        );

        std::vector<MetadataItem<GC>> current_metadata_items;
        for (const auto& metadata_item : single_metadata.items) {
            auto key = translation_unit.pool_string(
                static_cast<uint32_t>(metadata_item.key.index()),
                activation->strings()
            );

            auto value = translation_unit.pool_string(
                static_cast<uint32_t>(metadata_item.value.index()),
                activation->strings()
            );

            current_metadata_items.emplace_back(
                AvmString<GC>::from_string(activation->gc(), key),
                AvmString<GC>::from_string(activation->gc(), value)
            );
        }

        trait_metadata_list.emplace_back(
            AvmString<GC>::from_string(activation->gc(), name),
            std::move(current_metadata_items)
        );
    }

    return trait_metadata_list;
}

template<typename GC>
Object<GC> Metadata<GC>::as_json_object(UpdateContext<GC>* context) const {
    auto name_str = context->strings()->name;
    auto key_str = context->strings()->key;
    auto value_str = context->strings()->value;

    auto object = ScriptObject<GC>::new_object(context);
    object->set_dynamic_property(name_str, Value<GC>::from_string(name_), context->gc());

    std::vector<Object<GC>> storage;
    for (const auto& item : items_) {
        auto value_object = ScriptObject<GC>::new_object(context);
        value_object->set_dynamic_property(key_str, Value<GC>::from_string(item.key()), context->gc());
        value_object->set_dynamic_property(value_str, Value<GC>::from_string(item.value()), context->gc());
        storage.push_back(value_object);
    }

    auto values_array = ArrayObject<GC>::from_storage(context, std::move(storage));
    object->set_dynamic_property(value_str, Value<GC>::from_object(values_array), context->gc());

    return object;
}

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_METADATA_H
