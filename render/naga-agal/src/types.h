// C++ translation of render/naga-agal/src/types.rs
// AGAL (Adobe Graphics Assembly Language) types

#ifndef RUFFLE_RENDER_NAGA_AGAL_TYPES_H
#define RUFFLE_RENDER_NAGA_AGAL_TYPES_H

#include <cstdint>
#include <array>
#include <optional>
#include <stdexcept>

namespace ruffle {
namespace render {
namespace naga_agal {

// Error type for parsing errors
class Error : public std::runtime_error {
public:
    explicit Error(const std::string& msg) : std::runtime_error(msg) {}
};

/// AGAL Opcode enumeration
/// https://github.com/adobe/avmplus/blob/master/core/AGALOpcode.h
enum class Opcode : uint8_t {
    Mov = 0x00,
    Add = 0x01,
    Sub = 0x02,
    Mul = 0x03,
    Div = 0x04,
    Rcp = 0x05,
    Min = 0x06,
    Max = 0x07,
    Frc = 0x08,
    Sqt = 0x09,
    Rsq = 0x0a,
    Pow = 0x0b,
    Log = 0x0c,
    Exp = 0x0d,
    Nrm = 0x0e,
    Sin = 0x0f,
    Cos = 0x10,
    Crs = 0x11,
    Dp3 = 0x12,
    Dp4 = 0x13,
    Abs = 0x14,
    Neg = 0x15,
    Sat = 0x16,
    M33 = 0x17,
    M44 = 0x18,
    M34 = 0x19,
    Ddx = 0x1a,
    Ddy = 0x1b,
    Ife = 0x1c,
    Ine = 0x1d,
    Ifg = 0x1e,
    Ifl = 0x1f,
    Els = 0x20,
    Eif = 0x21,
    Kil = 0x27,
    Tex = 0x28,
    Sge = 0x29,
    Slt = 0x2a,
    Seq = 0x2c,
    Sne = 0x2d,
};

/// Register type enumeration
enum class RegisterType : uint8_t {
    Attribute = 0,
    Constant = 1,
    Temporary = 2,
    Output = 3,
    Varying = 4,
    Sampler = 5,
    FragmentRegister = 6,
};

/// Direct mode enumeration
enum class DirectMode : uint8_t {
    Direct = 0,
    Indirect = 1,
};

/// Write mask for destination registers
enum class Mask : uint8_t {
    None = 0b0000,
    X = 0b0001,
    Y = 0b0010,
    Z = 0b0100,
    W = 0b1000,
    XY = X | Y,
    XZ = X | Z,
    XW = X | W,
    YZ = Y | Z,
    YW = Y | W,
    ZW = Z | W,
    XYZ = X | Y | Z,
    XYW = X | Y | W,
    XZW = X | Z | W,
    YZW = Y | Z | W,
    XYZW = X | Y | Z | W,
};

inline Mask operator|(Mask a, Mask b) {
    return static_cast<Mask>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline Mask operator&(Mask a, Mask b) {
    return static_cast<Mask>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

/// Destination field structure
struct DestField {
    RegisterType register_type;
    Mask write_mask;
    uint16_t reg_num;
    
    static DestField parse(uint32_t val) {
        uint16_t reg_num = static_cast<uint16_t>(val & 0xFFFF);
        Mask write_mask = static_cast<Mask>((val >> 16) & 0xF);
        RegisterType reg_type = static_cast<RegisterType>((val >> 24) & 0xF);
        
        return DestField {
            .register_type = reg_type,
            .write_mask = write_mask,
            .reg_num = reg_num
        };
    }
};

/// Source field structure
struct SourceField {
    DirectMode direct_mode;
    uint8_t index_select;
    RegisterType index_type;
    RegisterType register_type;
    uint8_t swizzle;
    uint8_t indirect_offset;
    uint16_t reg_num;
    
    static SourceField parse(uint64_t val) {
        uint16_t reg_num = static_cast<uint16_t>(val & 0xFFFF);
        uint8_t indirect_offset = static_cast<uint8_t>((val >> 16) & 0xFF);
        uint8_t swizzle = static_cast<uint8_t>((val >> 24) & 0xFF);
        RegisterType register_type = static_cast<RegisterType>((val >> 32) & 0xF);
        RegisterType index_type = static_cast<RegisterType>((val >> 40) & 0xF);
        uint8_t index_select = static_cast<uint8_t>((val >> 48) & 0x3);
        DirectMode direct_mode = static_cast<DirectMode>((val >> 63) & 0x1);
        
        return SourceField {
            .direct_mode = direct_mode,
            .index_select = index_select,
            .index_type = index_type,
            .register_type = register_type,
            .swizzle = swizzle,
            .indirect_offset = indirect_offset,
            .reg_num = reg_num
        };
    }
};

/// Texture filter mode
enum class Filter : uint8_t {
    Nearest = 0,
    Linear = 1,
    Anisotropic2x = 2,
    Anisotropic4x = 3,
    Anisotropic8x = 4,
    Anisotropic16x = 5,
};

/// Mipmap mode
enum class Mipmap : uint8_t {
    Disable = 0,
    Nearest = 1,
    Linear = 2,
};

/// Texture wrapping mode
enum class Wrapping : uint8_t {
    Clamp = 0,
    Repeat = 1,
    ClampURepeatV = 2,
    RepeatUClampV = 3,
};

/// Texture dimension
enum class Dimension : uint8_t {
    TwoD = 0,
    Cube = 1,
};

/// Special sampler flags
struct Special {
    bool ignore_sampler;
    
    static Special parse(uint8_t val) {
        return Special {
            .ignore_sampler = (val & 0x4) != 0
        };
    }
};

/// Sampler field structure
struct SamplerField {
    Filter filter;
    Mipmap mipmap;
    Wrapping wrapping;
    Dimension dimension;
    Special special;
    int8_t texture_lod_bias;  // Texture level-of-detail (LOD) bias
    uint16_t reg_num;
    RegisterType reg_type;
    
    static SamplerField parse(uint64_t val) {
        uint16_t reg_num = static_cast<uint16_t>(val & 0xFFFF);
        int8_t load_bias = static_cast<int8_t>((val >> 16) & 0xFF);
        RegisterType reg_type = static_cast<RegisterType>((val >> 32) & 0xF);
        Dimension dimension = static_cast<Dimension>((val >> 44) & 0xF);
        Special special = Special::parse(static_cast<uint8_t>((val >> 48) & 0xF));
        Wrapping wrapping = static_cast<Wrapping>((val >> 52) & 0xF);
        Mipmap mipmap = static_cast<Mipmap>((val >> 56) & 0xF);
        Filter filter = static_cast<Filter>((val >> 60) & 0xF);
        
        return SamplerField {
            .filter = filter,
            .mipmap = mipmap,
            .wrapping = wrapping,
            .dimension = dimension,
            .texture_lod_bias = load_bias,
            .special = special,
            .reg_num = reg_num,
            .reg_type = reg_type
        };
    }
};

/// Sampler configuration
struct SamplerConfig {
    Wrapping wrapping;
    Filter filter;
    Mipmap mipmap;
    
    static SamplerConfig default_config() {
        return SamplerConfig {
            .wrapping = Wrapping::Clamp,
            .filter = Filter::Nearest,
            .mipmap = Mipmap::Disable
        };
    }
};

/// Source operand (either SourceField or SamplerField)
class Source2 {
public:
    enum class Type {
        SourceField,
        Sampler
    };
    
private:
    Type type_;
    std::optional<SourceField> source_field_;
    std::optional<SamplerField> sampler_;
    
public:
    explicit Source2(const SourceField& sf) 
        : type_(Type::SourceField), source_field_(sf) {}
    
    explicit Source2(const SamplerField& s) 
        : type_(Type::Sampler), sampler_(s) {}
    
    const SourceField& assert_source_field() const {
        if (type_ != Type::SourceField) {
            throw std::runtime_error("Expected SourceField");
        }
        return *source_field_;
    }
    
    const SamplerField& assert_sampler() const {
        if (type_ != Type::Sampler) {
            throw std::runtime_error("Expected SamplerField");
        }
        return *sampler_;
    }
    
    Type type() const { return type_; }
};

} // namespace naga_agal
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_NAGA_AGAL_TYPES_H
