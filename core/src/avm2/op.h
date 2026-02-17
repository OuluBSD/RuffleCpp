// C++ translation of core/src/avm2/op.rs
//! AVM2 Operation codes

#ifndef RUFFLE_CORE_AVM2_OP_H
#define RUFFLE_CORE_AVM2_OP_H

#include <cstdint>
#include <memory>
#include <optional>
#include <variant>
#include <vector>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC>
    class AvmString;
    template<typename GC>
    class AvmAtom;
    template<typename GC>
    class Class;
    template<typename GC>
    class Method;
    template<typename GC>
    class Multiname;
    template<typename GC>
    class Namespace;
    template<typename GC>
    class Script;

    namespace gc_arena {
        template<typename T>
        class Gc;
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

using NativeMethodImpl = void(*)();

/// Lookup switch data structure
/// This has interior mutability so that we can rewrite switch offsets from the
/// optimizer when we need to
template<typename GC>
class LookupSwitch {
public:
    std::cell::Cell<size_t> default_offset;
    std::vector<std::cell::Cell<size_t>> case_offsets;
};

/// AVM2 Operation codes
/// Represents all possible bytecode operations in the AVM2 virtual machine
template<typename GC>
class Op {
public:
    /// Operation variants
    struct Add {};
    struct AddI {};
    struct ApplyType { uint32_t num_types; };
    struct AsType { Class<GC> class_; };
    struct AsTypeLate {};
    struct BitAnd {};
    struct BitNot {};
    struct BitOr {};
    struct BitXor {};
    struct Bkpt {};
    struct BkptLine { uint32_t line_num; };
    struct Call { uint32_t num_args; };
    struct CallMethod { uint32_t index; uint32_t num_args; bool push_return_value; };
    struct CallNative { NativeMethodImpl method; uint32_t num_args; bool push_return_value; };
    struct CallProperty { std::shared_ptr<Multiname<GC>> multiname; uint32_t num_args; };
    struct CallPropLex { std::shared_ptr<Multiname<GC>> multiname; uint32_t num_args; };
    struct CallPropVoid { std::shared_ptr<Multiname<GC>> multiname; uint32_t num_args; };
    struct CallStatic { Method<GC> method; uint32_t num_args; };
    struct CallSuper { std::shared_ptr<Multiname<GC>> multiname; uint32_t num_args; };
    struct CheckFilter {};
    struct Coerce { Class<GC> class_; };
    struct CoerceSwapPop { Class<GC> class_; };
    struct CoerceA {};
    struct CoerceB {};
    struct CoerceD {};
    struct CoerceDSwapPop {};
    struct CoerceI {};
    struct CoerceISwapPop {};
    struct CoerceO {};
    struct CoerceS {};
    struct CoerceU {};
    struct CoerceUSwapPop {};
    struct Construct { uint32_t num_args; };
    struct ConstructProp { std::shared_ptr<Multiname<GC>> multiname; uint32_t num_args; };
    struct ConstructSlot { uint32_t index; uint32_t num_args; };
    struct ConstructSuper { uint32_t num_args; };
    struct ConvertO {};
    struct ConvertS {};
    struct Debug { bool is_local_register; AvmAtom<GC> register_name; uint8_t register_; };
    struct DebugFile { AvmAtom<GC> file_name; };
    struct DebugLine { uint32_t line_num; };
    struct DecLocal { uint32_t index; };
    struct DecLocalI { uint32_t index; };
    struct Decrement {};
    struct DecrementI {};
    struct DeleteProperty { std::shared_ptr<Multiname<GC>> multiname; };
    struct Divide {};
    struct Dup {};
    struct Dxns { AvmAtom<GC> string; };
    struct DxnsLate {};
    struct Equals {};
    struct EscXAttr {};
    struct EscXElem {};
    struct FindDef { std::shared_ptr<Multiname<GC>> multiname; };
    struct FindProperty { std::shared_ptr<Multiname<GC>> multiname; };
    struct FindPropStrict { std::shared_ptr<Multiname<GC>> multiname; };
    struct GetDescendants { std::shared_ptr<Multiname<GC>> multiname; };
    struct GetLocal { uint32_t index; };
    struct GetOuterScope { size_t index; };
    struct GetPropertyStatic { std::shared_ptr<Multiname<GC>> multiname; };
    struct GetPropertyFast { std::shared_ptr<Multiname<GC>> multiname; };
    struct GetPropertySlow { std::shared_ptr<Multiname<GC>> multiname; };
    struct GetScopeObject { size_t index; };
    struct GetScriptGlobals { Script<GC> script; };
    struct GetSlot { uint32_t index; };
    struct GetSuper { std::shared_ptr<Multiname<GC>> multiname; };
    struct GreaterEquals {};
    struct GreaterThan {};
    struct HasNext {};
    struct HasNext2 { uint32_t object_register; uint32_t index_register; };
    struct IfFalse { size_t offset; };
    struct IfTrue { size_t offset; };
    struct In {};
    struct IncLocal { uint32_t index; };
    struct IncLocalI { uint32_t index; };
    struct Increment {};
    struct IncrementI {};
    struct InitProperty { std::shared_ptr<Multiname<GC>> multiname; };
    struct InstanceOf {};
    struct IsType { Class<GC> class_; };
    struct IsTypeLate {};
    struct Jump { size_t offset; };
    struct Kill { uint32_t index; };
    struct LessEquals {};
    struct LessThan {};
    struct Lf32 {};
    struct Lf64 {};
    struct Li16 {};
    struct Li32 {};
    struct Li8 {};
    struct LookupSwitch { std::shared_ptr<LookupSwitch<GC>> data; };
    struct LShift {};
    struct Modulo {};
    struct Multiply {};
    struct MultiplyI {};
    struct Negate {};
    struct NegateI {};
    struct NewActivation { Class<GC> activation_class; };
    struct NewArray { uint32_t num_args; };
    struct NewCatch { size_t index; };
    struct NewClass { Class<GC> class_; };
    struct NewFunction { Method<GC> method; };
    struct NewObject { uint32_t num_args; };
    struct NextName {};
    struct NextValue {};
    struct Nop {};
    struct Not {};
    struct Pop {};
    struct PopJump { size_t offset; };
    struct PopScope {};
    struct PushDouble { double value; };
    struct PushFalse {};
    struct PushInt { int32_t value; };
    struct PushNamespace { Namespace<GC> namespace_; };
    struct PushNull {};
    struct PushScope {};
    struct PushShort { int16_t value; };
    struct PushString { AvmAtom<GC> string; };
    struct PushTrue {};
    struct PushUint { uint32_t value; };
    struct PushUndefined {};
    struct PushWith {};
    struct ReturnValue { std::optional<Class<GC>> return_type; };
    struct ReturnVoid { std::optional<Class<GC>> return_type; };
    struct RShift {};
    struct SetGlobalSlot { uint32_t index; };
    struct SetLocal { uint32_t index; };
    struct SetPropertyStatic { std::shared_ptr<Multiname<GC>> multiname; };
    struct SetPropertyFast { std::shared_ptr<Multiname<GC>> multiname; };
    struct SetPropertySlow { std::shared_ptr<Multiname<GC>> multiname; };
    struct SetSlot { uint32_t index; };
    struct SetSlotNoCoerce { uint32_t index; };
    struct SetSuper { std::shared_ptr<Multiname<GC>> multiname; };
    struct Sf32 {};
    struct Sf64 {};
    struct Si16 {};
    struct Si32 {};
    struct Si8 {};
    struct StrictEquals {};
    struct StoreLocal { uint32_t index; };
    struct Subtract {};
    struct SubtractI {};
    struct Swap {};
    struct Sxi1 {};
    struct Sxi16 {};
    struct Sxi8 {};
    struct Throw {};
    struct TypeOf {};
    struct Timestamp {};
    struct URShift {};

private:
    std::variant<
        Add, AddI, ApplyType, AsType, AsTypeLate, BitAnd, BitNot, BitOr, BitXor,
        Bkpt, BkptLine, Call, CallMethod, CallNative, CallProperty, CallPropLex,
        CallPropVoid, CallStatic, CallSuper, CheckFilter, Coerce, CoerceSwapPop,
        CoerceA, CoerceB, CoerceD, CoerceDSwapPop, CoerceI, CoerceISwapPop,
        CoerceO, CoerceS, CoerceU, CoerceUSwapPop, Construct, ConstructProp,
        ConstructSlot, ConstructSuper, ConvertO, ConvertS, Debug, DebugFile,
        DebugLine, DecLocal, DecLocalI, Decrement, DecrementI, DeleteProperty,
        Divide, Dup, Dxns, DxnsLate, Equals, EscXAttr, EscXElem, FindDef,
        FindProperty, FindPropStrict, GetDescendants, GetLocal, GetOuterScope,
        GetPropertyStatic, GetPropertyFast, GetPropertySlow, GetScopeObject,
        GetScriptGlobals, GetSlot, GetSuper, GreaterEquals, GreaterThan, HasNext,
        HasNext2, IfFalse, IfTrue, In, IncLocal, IncLocalI, Increment, IncrementI,
        InitProperty, InstanceOf, IsType, IsTypeLate, Jump, Kill, LessEquals,
        LessThan, Lf32, Lf64, Li16, Li32, Li8, LookupSwitch, LShift, Modulo,
        Multiply, MultiplyI, Negate, NegateI, NewActivation, NewArray, NewCatch,
        NewClass, NewFunction, NewObject, NextName, NextValue, Nop, Not, Pop,
        PopJump, PopScope, PushDouble, PushFalse, PushInt, PushNamespace, PushNull,
        PushScope, PushShort, PushString, PushTrue, PushUint, PushUndefined,
        PushWith, ReturnValue, ReturnVoid, RShift, SetGlobalSlot, SetLocal,
        SetPropertyStatic, SetPropertyFast, SetPropertySlow, SetSlot, SetSlotNoCoerce,
        SetSuper, Sf32, Sf64, Si16, Si32, Si8, StrictEquals, StoreLocal, Subtract,
        SubtractI, Swap, Sxi1, Sxi16, Sxi8, Throw, TypeOf, Timestamp, URShift
    > data_;

public:
    // Constructors for each operation type
    Op(Add) : data_(Add{}) {}
    Op(AddI) : data_(AddI{}) {}
    Op(ApplyType t) : data_(t) {}
    Op(AsType t) : data_(t) {}
    Op(AsTypeLate) : data_(AsTypeLate{}) {}
    Op(BitAnd) : data_(BitAnd{}) {}
    Op(BitNot) : data_(BitNot{}) {}
    Op(BitOr) : data_(BitOr{}) {}
    Op(BitXor) : data_(BitXor{}) {}
    Op(Bkpt) : data_(Bkpt{}) {}
    Op(BkptLine t) : data_(t) {}
    Op(Call t) : data_(t) {}
    Op(CallMethod t) : data_(t) {}
    Op(CallNative t) : data_(t) {}
    Op(CallProperty t) : data_(t) {}
    Op(CallPropLex t) : data_(t) {}
    Op(CallPropVoid t) : data_(t) {}
    Op(CallStatic t) : data_(t) {}
    Op(CallSuper t) : data_(t) {}
    Op(CheckFilter) : data_(CheckFilter{}) {}
    Op(Coerce t) : data_(t) {}
    Op(CoerceSwapPop t) : data_(t) {}
    Op(CoerceA) : data_(CoerceA{}) {}
    Op(CoerceB) : data_(CoerceB{}) {}
    Op(CoerceD) : data_(CoerceD{}) {}
    Op(CoerceDSwapPop) : data_(CoerceDSwapPop{}) {}
    Op(CoerceI) : data_(CoerceI{}) {}
    Op(CoerceISwapPop) : data_(CoerceISwapPop{}) {}
    Op(CoerceO) : data_(CoerceO{}) {}
    Op(CoerceS) : data_(CoerceS{}) {}
    Op(CoerceU) : data_(CoerceU{}) {}
    Op(CoerceUSwapPop) : data_(CoerceUSwapPop{}) {}
    Op(Construct t) : data_(t) {}
    Op(ConstructProp t) : data_(t) {}
    Op(ConstructSlot t) : data_(t) {}
    Op(ConstructSuper t) : data_(t) {}
    Op(ConvertO) : data_(ConvertO{}) {}
    Op(ConvertS) : data_(ConvertS{}) {}
    Op(Debug t) : data_(t) {}
    Op(DebugFile t) : data_(t) {}
    Op(DebugLine t) : data_(t) {}
    Op(DecLocal t) : data_(t) {}
    Op(DecLocalI t) : data_(t) {}
    Op(Decrement) : data_(Decrement{}) {}
    Op(DecrementI) : data_(DecrementI{}) {}
    Op(DeleteProperty t) : data_(t) {}
    Op(Divide) : data_(Divide{}) {}
    Op(Dup) : data_(Dup{}) {}
    Op(Dxns t) : data_(t) {}
    Op(DxnsLate) : data_(DxnsLate{}) {}
    Op(Equals) : data_(Equals{}) {}
    Op(EscXAttr) : data_(EscXAttr{}) {}
    Op(EscXElem) : data_(EscXElem{}) {}
    Op(FindDef t) : data_(t) {}
    Op(FindProperty t) : data_(t) {}
    Op(FindPropStrict t) : data_(t) {}
    Op(GetDescendants t) : data_(t) {}
    Op(GetLocal t) : data_(t) {}
    Op(GetOuterScope t) : data_(t) {}
    Op(GetPropertyStatic t) : data_(t) {}
    Op(GetPropertyFast t) : data_(t) {}
    Op(GetPropertySlow t) : data_(t) {}
    Op(GetScopeObject t) : data_(t) {}
    Op(GetScriptGlobals t) : data_(t) {}
    Op(GetSlot t) : data_(t) {}
    Op(GetSuper t) : data_(t) {}
    Op(GreaterEquals) : data_(GreaterEquals{}) {}
    Op(GreaterThan) : data_(GreaterThan{}) {}
    Op(HasNext) : data_(HasNext{}) {}
    Op(HasNext2 t) : data_(t) {}
    Op(IfFalse t) : data_(t) {}
    Op(IfTrue t) : data_(t) {}
    Op(In) : data_(In{}) {}
    Op(IncLocal t) : data_(t) {}
    Op(IncLocalI t) : data_(t) {}
    Op(Increment) : data_(Increment{}) {}
    Op(IncrementI) : data_(IncrementI{}) {}
    Op(InitProperty t) : data_(t) {}
    Op(InstanceOf) : data_(InstanceOf{}) {}
    Op(IsType t) : data_(t) {}
    Op(IsTypeLate) : data_(IsTypeLate{}) {}
    Op(Jump t) : data_(t) {}
    Op(Kill t) : data_(t) {}
    Op(LessEquals) : data_(LessEquals{}) {}
    Op(LessThan) : data_(LessThan{}) {}
    Op(Lf32) : data_(Lf32{}) {}
    Op(Lf64) : data_(Lf64{}) {}
    Op(Li16) : data_(Li16{}) {}
    Op(Li32) : data_(Li32{}) {}
    Op(Li8) : data_(Li8{}) {}
    Op(LookupSwitch t) : data_(t) {}
    Op(LShift) : data_(LShift{}) {}
    Op(Modulo) : data_(Modulo{}) {}
    Op(Multiply) : data_(Multiply{}) {}
    Op(MultiplyI) : data_(MultiplyI{}) {}
    Op(Negate) : data_(Negate{}) {}
    Op(NegateI) : data_(NegateI{}) {}
    Op(NewActivation t) : data_(t) {}
    Op(NewArray t) : data_(t) {}
    Op(NewCatch t) : data_(t) {}
    Op(NewClass t) : data_(t) {}
    Op(NewFunction t) : data_(t) {}
    Op(NewObject t) : data_(t) {}
    Op(NextName) : data_(NextName{}) {}
    Op(NextValue) : data_(NextValue{}) {}
    Op(Nop) : data_(Nop{}) {}
    Op(Not) : data_(Not{}) {}
    Op(Pop) : data_(Pop{}) {}
    Op(PopJump t) : data_(t) {}
    Op(PopScope) : data_(PopScope{}) {}
    Op(PushDouble t) : data_(t) {}
    Op(PushFalse) : data_(PushFalse{}) {}
    Op(PushInt t) : data_(t) {}
    Op(PushNamespace t) : data_(t) {}
    Op(PushNull) : data_(PushNull{}) {}
    Op(PushScope) : data_(PushScope{}) {}
    Op(PushShort t) : data_(t) {}
    Op(PushString t) : data_(t) {}
    Op(PushTrue) : data_(PushTrue{}) {}
    Op(PushUint t) : data_(t) {}
    Op(PushUndefined) : data_(PushUndefined{}) {}
    Op(PushWith) : data_(PushWith{}) {}
    Op(ReturnValue t) : data_(t) {}
    Op(ReturnVoid t) : data_(t) {}
    Op(RShift) : data_(RShift{}) {}
    Op(SetGlobalSlot t) : data_(t) {}
    Op(SetLocal t) : data_(t) {}
    Op(SetPropertyStatic t) : data_(t) {}
    Op(SetPropertyFast t) : data_(t) {}
    Op(SetPropertySlow t) : data_(t) {}
    Op(SetSlot t) : data_(t) {}
    Op(SetSlotNoCoerce t) : data_(t) {}
    Op(SetSuper t) : data_(t) {}
    Op(Sf32) : data_(Sf32{}) {}
    Op(Sf64) : data_(Sf64{}) {}
    Op(Si16) : data_(Si16{}) {}
    Op(Si32) : data_(Si32{}) {}
    Op(Si8) : data_(Si8{}) {}
    Op(StrictEquals) : data_(StrictEquals{}) {}
    Op(StoreLocal t) : data_(t) {}
    Op(Subtract) : data_(Subtract{}) {}
    Op(SubtractI) : data_(SubtractI{}) {}
    Op(Swap) : data_(Swap{}) {}
    Op(Sxi1) : data_(Sxi1{}) {}
    Op(Sxi16) : data_(Sxi16{}) {}
    Op(Sxi8) : data_(Sxi8{}) {}
    Op(Throw) : data_(Throw{}) {}
    Op(TypeOf) : data_(TypeOf{}) {}
    Op(Timestamp) : data_(Timestamp{}) {}
    Op(URShift) : data_(URShift{}) {}

    /// Check if this operation can throw an error
    bool can_throw_error() const {
        return !std::holds_alternative<AsType>(data_) &&
               !std::holds_alternative<Bkpt>(data_) &&
               !std::holds_alternative<BkptLine>(data_) &&
               !std::holds_alternative<CoerceO>(data_) &&
               !std::holds_alternative<Dup>(data_) &&
               !std::holds_alternative<GetScopeObject>(data_) &&
               !std::holds_alternative<GetOuterScope>(data_) &&
               !std::holds_alternative<GetLocal>(data_) &&
               !std::holds_alternative<IfTrue>(data_) &&
               !std::holds_alternative<IfFalse>(data_) &&
               !std::holds_alternative<IsType>(data_) &&
               !std::holds_alternative<Jump>(data_) &&
               !std::holds_alternative<Kill>(data_) &&
               !std::holds_alternative<LookupSwitch>(data_) &&
               !std::holds_alternative<Nop>(data_) &&
               !std::holds_alternative<Not>(data_) &&
               !std::holds_alternative<Pop>(data_) &&
               !std::holds_alternative<PopJump>(data_) &&
               !std::holds_alternative<PopScope>(data_) &&
               !std::holds_alternative<PushDouble>(data_) &&
               !std::holds_alternative<PushFalse>(data_) &&
               !std::holds_alternative<PushInt>(data_) &&
               !std::holds_alternative<PushNamespace>(data_) &&
               !std::holds_alternative<PushNull>(data_) &&
               !std::holds_alternative<PushShort>(data_) &&
               !std::holds_alternative<PushString>(data_) &&
               !std::holds_alternative<PushTrue>(data_) &&
               !std::holds_alternative<PushUint>(data_) &&
               !std::holds_alternative<PushUndefined>(data_) &&
               !std::holds_alternative<SetLocal>(data_) &&
               !std::holds_alternative<StrictEquals>(data_) &&
               !std::holds_alternative<StoreLocal>(data_) &&
               !std::holds_alternative<Swap>(data_) &&
               !std::holds_alternative<Timestamp>(data_) &&
               !std::holds_alternative<TypeOf>(data_) &&
               !std::holds_alternative<ReturnVoid>(data_);
    }

    /// Check if this is a no-op operation
    bool is_nop() const {
#ifdef AVM_DEBUG
        return std::holds_alternative<Nop>(data_);
#else
        return std::holds_alternative<Nop>(data_) ||
               std::holds_alternative<Debug>(data_) ||
               std::holds_alternative<DebugFile>(data_) ||
               std::holds_alternative<DebugLine>(data_);
#endif
    }

    /// Whether all this op does is push a single value to the stack, possibly
    /// reading from stack or locals, but never, e.g., throwing an error or
    /// calling a method.
    bool is_pure_push() const {
        return std::holds_alternative<PushTrue>(data_) ||
               std::holds_alternative<PushFalse>(data_) ||
               std::holds_alternative<PushUndefined>(data_) ||
               std::holds_alternative<PushNull>(data_) ||
               std::holds_alternative<PushDouble>(data_) ||
               std::holds_alternative<PushInt>(data_) ||
               std::holds_alternative<PushShort>(data_) ||
               std::holds_alternative<PushUint>(data_) ||
               std::holds_alternative<GetLocal>(data_) ||
               std::holds_alternative<Dup>(data_);
    }
};

// Size assertion for 64-bit systems (matching Rust's size_of check)
#ifdef __SIZEOF_POINTER__
#if __SIZEOF_POINTER__ == 8
static_assert(sizeof(Op<void*>) == 16, "Op should be 16 bytes on 64-bit systems");
#endif
#endif

} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_OP_H
