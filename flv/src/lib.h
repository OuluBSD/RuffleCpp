// C++ translation of flv/src/lib.rs
// FLV module declarations

#ifndef RUFFLE_FLV_LIB_H
#define RUFFLE_FLV_LIB_H

// FLV submodules
// These will be defined in their respective header files
namespace ruffle {
namespace flv {

// Forward declarations for FLV submodules
namespace header {
    // FLV header defined in header.h
    class Header;
}

namespace script {
    // Script data defined in script.h
    class ScriptData;
    class Value;
    class Variable;
}

namespace sound {
    // Sound types defined in sound.h
    class AudioData;
    enum class AudioDataType;
    class SoundFormat;
    enum class SoundRate;
    enum class SoundSize;
    enum class SoundType;
}

namespace tag {
    // Tag types defined in tag.h
    class Tag;
    class TagData;
}

namespace video {
    // Video types defined in video.h
    enum class CodecId;
    class CommandFrame;
    enum class FrameType;
    class VideoData;
    class VideoPacket;
}

namespace reader {
    // FLV reader defined in reader.h
    class FlvReader;
}

namespace error {
    // Error type defined in error.h
    class Error;
}

// Re-exports from submodules
using header::Header;
using reader::FlvReader;
using script::ScriptData;
using script::Value;
using script::Variable;
using sound::AudioData;
using sound::AudioDataType;
using sound::SoundFormat;
using sound::SoundRate;
using sound::SoundSize;
using sound::SoundType;
using tag::Tag;
using tag::TagData;
using video::CodecId;
using video::CommandFrame;
using video::FrameType;
using video::VideoData;
using video::VideoPacket;
using error::Error;

} // namespace flv
} // namespace ruffle

#endif // RUFFLE_FLV_LIB_H
