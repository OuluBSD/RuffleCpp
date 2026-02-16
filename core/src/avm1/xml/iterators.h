// C++ translation of core/src/avm1/xml/iterators.rs
// Iterator types for XML trees

#ifndef RUFFLE_CORE_AVM1_XML_ITERATORS_H
#define RUFFLE_CORE_AVM1_XML_ITERATORS_H

#include <optional>
#include <cstddef>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm1 {
namespace xml {
    template<typename T> class XmlNode;
}
}
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace xml {
namespace iterators {

/// Iterator that yields direct children of an XML node.
/// 
/// This class implements both forward and bidirectional iteration
/// over the children of an XML node.
template<typename GC>
class ChildIter {
private:
    XmlNode<GC> base_;
    size_t index_;
    size_t back_index_;

public:
    /// Construct a new ChildIter that lists the children of an XML node.
    /// 
    /// \param base The XML node to iterate over
    explicit ChildIter(XmlNode<GC> base)
        : base_(base)
        , index_(0)
        , back_index_(base.children_len())
    {
    }

    /// Get the next child node in forward iteration.
    /// 
    /// \return The next XmlNode, or std::nullopt if no more children
    std::optional<XmlNode<GC>> next() {
        if (index_ < back_index_) {
            auto item = base_.get_child_by_index(index_);
            index_++;
            return item;
        }
        return std::nullopt;
    }

    /// Get the next child node in backward iteration.
    /// 
    /// \return The next XmlNode from the back, or std::nullopt if no more children
    std::optional<XmlNode<GC>> next_back() {
        if (index_ < back_index_) {
            back_index_--;
            auto item = base_.get_child_by_index(back_index_);
            return item;
        }
        return std::nullopt;
    }

    /// Check if there are more children to iterate.
    /// 
    /// \return true if more children exist, false otherwise
    bool has_next() const {
        return index_ < back_index_;
    }
};

} // namespace iterators
} // namespace xml
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_XML_ITERATORS_H
