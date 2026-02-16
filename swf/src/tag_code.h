// C++ translation of swf/src/tag_code.rs
// SWF Tag Code definitions

#ifndef RUFFLE_SWF_TAG_CODE_H
#define RUFFLE_SWF_TAG_CODE_H

#include <cstdint>
#include <string>
#include <optional>

namespace ruffle {
namespace swf {

/// SWF Tag Codes
/// Each tag code represents a specific type of tag in a SWF file
enum class TagCode : uint16_t {
    End = 0,
    ShowFrame = 1,
    DefineShape = 2,

    PlaceObject = 4,
    RemoveObject = 5,
    DefineBits = 6,
    DefineButton = 7,
    JpegTables = 8,
    SetBackgroundColor = 9,
    DefineFont = 10,
    DefineText = 11,
    DoAction = 12,
    DefineFontInfo = 13,
    DefineSound = 14,
    StartSound = 15,

    DefineButtonSound = 17,
    SoundStreamHead = 18,
    SoundStreamBlock = 19,
    DefineBitsLossless = 20,
    DefineBitsJpeg2 = 21,
    DefineShape2 = 22,
    DefineButtonCxform = 23,
    Protect = 24,

    PlaceObject2 = 26,

    RemoveObject2 = 28,

    DefineShape3 = 32,
    DefineText2 = 33,
    DefineButton2 = 34,
    DefineBitsJpeg3 = 35,
    DefineBitsLossless2 = 36,
    DefineEditText = 37,

    DefineSprite = 39,
    NameCharacter = 40,
    ProductInfo = 41,

    FrameLabel = 43,

    SoundStreamHead2 = 45,
    DefineMorphShape = 46,

    DefineFont2 = 48,

    ExportAssets = 56,
    ImportAssets = 57,
    EnableDebugger = 58,
    DoInitAction = 59,
    DefineVideoStream = 60,
    VideoFrame = 61,
    DefineFontInfo2 = 62,

    DebugId = 63,
    EnableDebugger2 = 64,
    ScriptLimits = 65,
    SetTabIndex = 66,

    FileAttributes = 69,

    PlaceObject3 = 70,
    ImportAssets2 = 71,
    DoAbc = 72,
    DefineFontAlignZones = 73,
    CsmTextSettings = 74,
    DefineFont3 = 75,
    SymbolClass = 76,
    Metadata = 77,
    DefineScalingGrid = 78,

    DoAbc2 = 82,
    DefineShape4 = 83,
    DefineMorphShape2 = 84,

    DefineSceneAndFrameLabelData = 86,
    DefineBinaryData = 87,
    DefineFontName = 88,
    StartSound2 = 89,
    DefineBitsJpeg4 = 90,
    DefineFont4 = 91,

    EnableTelemetry = 93,
    PlaceObject4 = 94,
};

/// TagCode utility functions
namespace tag_code {

/// Convert a u16 value to TagCode
/// @param n The numeric tag code value
/// @return Optional TagCode if valid, nullopt otherwise
inline std::optional<TagCode> from_u16(uint16_t n) {
    switch (n) {
        case 0: return TagCode::End;
        case 1: return TagCode::ShowFrame;
        case 2: return TagCode::DefineShape;
        case 4: return TagCode::PlaceObject;
        case 5: return TagCode::RemoveObject;
        case 6: return TagCode::DefineBits;
        case 7: return TagCode::DefineButton;
        case 8: return TagCode::JpegTables;
        case 9: return TagCode::SetBackgroundColor;
        case 10: return TagCode::DefineFont;
        case 11: return TagCode::DefineText;
        case 12: return TagCode::DoAction;
        case 13: return TagCode::DefineFontInfo;
        case 14: return TagCode::DefineSound;
        case 15: return TagCode::StartSound;
        case 17: return TagCode::DefineButtonSound;
        case 18: return TagCode::SoundStreamHead;
        case 19: return TagCode::SoundStreamBlock;
        case 20: return TagCode::DefineBitsLossless;
        case 21: return TagCode::DefineBitsJpeg2;
        case 22: return TagCode::DefineShape2;
        case 23: return TagCode::DefineButtonCxform;
        case 24: return TagCode::Protect;
        case 26: return TagCode::PlaceObject2;
        case 28: return TagCode::RemoveObject2;
        case 32: return TagCode::DefineShape3;
        case 33: return TagCode::DefineText2;
        case 34: return TagCode::DefineButton2;
        case 35: return TagCode::DefineBitsJpeg3;
        case 36: return TagCode::DefineBitsLossless2;
        case 37: return TagCode::DefineEditText;
        case 39: return TagCode::DefineSprite;
        case 40: return TagCode::NameCharacter;
        case 41: return TagCode::ProductInfo;
        case 43: return TagCode::FrameLabel;
        case 45: return TagCode::SoundStreamHead2;
        case 46: return TagCode::DefineMorphShape;
        case 48: return TagCode::DefineFont2;
        case 56: return TagCode::ExportAssets;
        case 57: return TagCode::ImportAssets;
        case 58: return TagCode::EnableDebugger;
        case 59: return TagCode::DoInitAction;
        case 60: return TagCode::DefineVideoStream;
        case 61: return TagCode::VideoFrame;
        case 62: return TagCode::DefineFontInfo2;
        case 63: return TagCode::DebugId;
        case 64: return TagCode::EnableDebugger2;
        case 65: return TagCode::ScriptLimits;
        case 66: return TagCode::SetTabIndex;
        case 69: return TagCode::FileAttributes;
        case 70: return TagCode::PlaceObject3;
        case 71: return TagCode::ImportAssets2;
        case 72: return TagCode::DoAbc;
        case 73: return TagCode::DefineFontAlignZones;
        case 74: return TagCode::CsmTextSettings;
        case 75: return TagCode::DefineFont3;
        case 76: return TagCode::SymbolClass;
        case 77: return TagCode::Metadata;
        case 78: return TagCode::DefineScalingGrid;
        case 82: return TagCode::DoAbc2;
        case 83: return TagCode::DefineShape4;
        case 84: return TagCode::DefineMorphShape2;
        case 86: return TagCode::DefineSceneAndFrameLabelData;
        case 87: return TagCode::DefineBinaryData;
        case 88: return TagCode::DefineFontName;
        case 89: return TagCode::StartSound2;
        case 90: return TagCode::DefineBitsJpeg4;
        case 91: return TagCode::DefineFont4;
        case 93: return TagCode::EnableTelemetry;
        case 94: return TagCode::PlaceObject4;
        default: return std::nullopt;
    }
}

/// Format a tag code as a string
/// @param tag_code The numeric tag code value
/// @return String representation of the tag code
inline std::string format(uint16_t tag_code) {
    if (auto code = from_u16(tag_code)) {
        switch (*code) {
            case TagCode::End: return "End";
            case TagCode::ShowFrame: return "ShowFrame";
            case TagCode::DefineShape: return "DefineShape";
            case TagCode::PlaceObject: return "PlaceObject";
            case TagCode::RemoveObject: return "RemoveObject";
            case TagCode::DefineBits: return "DefineBits";
            case TagCode::DefineButton: return "DefineButton";
            case TagCode::JpegTables: return "JpegTables";
            case TagCode::SetBackgroundColor: return "SetBackgroundColor";
            case TagCode::DefineFont: return "DefineFont";
            case TagCode::DefineText: return "DefineText";
            case TagCode::DoAction: return "DoAction";
            case TagCode::DefineFontInfo: return "DefineFontInfo";
            case TagCode::DefineSound: return "DefineSound";
            case TagCode::StartSound: return "StartSound";
            case TagCode::DefineButtonSound: return "DefineButtonSound";
            case TagCode::SoundStreamHead: return "SoundStreamHead";
            case TagCode::SoundStreamBlock: return "SoundStreamBlock";
            case TagCode::DefineBitsLossless: return "DefineBitsLossless";
            case TagCode::DefineBitsJpeg2: return "DefineBitsJpeg2";
            case TagCode::DefineShape2: return "DefineShape2";
            case TagCode::DefineButtonCxform: return "DefineButtonCxform";
            case TagCode::Protect: return "Protect";
            case TagCode::PlaceObject2: return "PlaceObject2";
            case TagCode::RemoveObject2: return "RemoveObject2";
            case TagCode::DefineShape3: return "DefineShape3";
            case TagCode::DefineText2: return "DefineText2";
            case TagCode::DefineButton2: return "DefineButton2";
            case TagCode::DefineBitsJpeg3: return "DefineBitsJpeg3";
            case TagCode::DefineBitsLossless2: return "DefineBitsLossless2";
            case TagCode::DefineEditText: return "DefineEditText";
            case TagCode::DefineSprite: return "DefineSprite";
            case TagCode::NameCharacter: return "NameCharacter";
            case TagCode::ProductInfo: return "ProductInfo";
            case TagCode::FrameLabel: return "FrameLabel";
            case TagCode::SoundStreamHead2: return "SoundStreamHead2";
            case TagCode::DefineMorphShape: return "DefineMorphShape";
            case TagCode::DefineFont2: return "DefineFont2";
            case TagCode::ExportAssets: return "ExportAssets";
            case TagCode::ImportAssets: return "ImportAssets";
            case TagCode::EnableDebugger: return "EnableDebugger";
            case TagCode::DoInitAction: return "DoInitAction";
            case TagCode::DefineVideoStream: return "DefineVideoStream";
            case TagCode::VideoFrame: return "VideoFrame";
            case TagCode::DefineFontInfo2: return "DefineFontInfo2";
            case TagCode::DebugId: return "DebugId";
            case TagCode::EnableDebugger2: return "EnableDebugger2";
            case TagCode::ScriptLimits: return "ScriptLimits";
            case TagCode::SetTabIndex: return "SetTabIndex";
            case TagCode::FileAttributes: return "FileAttributes";
            case TagCode::PlaceObject3: return "PlaceObject3";
            case TagCode::ImportAssets2: return "ImportAssets2";
            case TagCode::DoAbc: return "DoAbc";
            case TagCode::DefineFontAlignZones: return "DefineFontAlignZones";
            case TagCode::CsmTextSettings: return "CsmTextSettings";
            case TagCode::DefineFont3: return "DefineFont3";
            case TagCode::SymbolClass: return "SymbolClass";
            case TagCode::Metadata: return "Metadata";
            case TagCode::DefineScalingGrid: return "DefineScalingGrid";
            case TagCode::DoAbc2: return "DoAbc2";
            case TagCode::DefineShape4: return "DefineShape4";
            case TagCode::DefineMorphShape2: return "DefineMorphShape2";
            case TagCode::DefineSceneAndFrameLabelData: return "DefineSceneAndFrameLabelData";
            case TagCode::DefineBinaryData: return "DefineBinaryData";
            case TagCode::DefineFontName: return "DefineFontName";
            case TagCode::StartSound2: return "StartSound2";
            case TagCode::DefineBitsJpeg4: return "DefineBitsJpeg4";
            case TagCode::DefineFont4: return "DefineFont4";
            case TagCode::EnableTelemetry: return "EnableTelemetry";
            case TagCode::PlaceObject4: return "PlaceObject4";
        }
    }
    return "Unknown(" + std::to_string(tag_code) + ")";
}

} // namespace tag_code

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_TAG_CODE_H
