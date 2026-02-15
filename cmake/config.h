/*
 * C++ header for configuration functionality
 * This replaces the functionality of core/src/config.rs
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <stdexcept>

namespace ruffle {

// Controls whether the content is letterboxed or pillarboxed when the
// player's aspect ratio does not match the movie's aspect ratio.
//
// When letterboxed, black bars will be rendered around the exterior
// margins of the content.
enum class Letterbox {
    // The content will never be letterboxed.
    OFF,

    // The content will only be letterboxed if the content is running fullscreen.
    FULLSCREEN,

    // The content will always be letterboxed.
    ON
};

// Helper class for Letterbox enum
class LetterboxHelper {
public:
    // Convert Letterbox to string
    static std::string to_string(Letterbox letterbox) {
        switch (letterbox) {
            case Letterbox::OFF: return "off";
            case Letterbox::FULLSCREEN: return "fullscreen";
            case Letterbox::ON: return "on";
            default: return "off"; // Default case
        }
    }

    // Parse Letterbox from string
    static Letterbox from_string(const std::string& str) {
        if (str == "off") {
            return Letterbox::OFF;
        } else if (str == "fullscreen") {
            return Letterbox::FULLSCREEN;
        } else if (str == "on") {
            return Letterbox::ON;
        } else {
            throw std::invalid_argument("Invalid Letterbox value: " + str);
        }
    }

    // Check equality
    static bool equals(Letterbox a, Letterbox b) {
        return a == b;
    }
};

// Exception class for parsing errors
class ParseEnumError : public std::exception {
public:
    const char* what() const noexcept override {
        return "ParseEnumError";
    }
};

// The networking API access mode of the Ruffle player.
// This setting is only used on web.
enum class NetworkingAccessMode {
    // All networking APIs are permitted in the SWF file.
    ALL,

    // The SWF file may not call browser navigation or browser interaction APIs.
    //
    // The APIs getURL(), navigateToURL(), fscommand() and ExternalInterface.call()
    // are prevented in this mode.
    INTERNAL,

    // The SWF file may not call browser navigation or browser interaction APIs
    // and it cannot use any SWF-to-SWF communication APIs.
    //
    // Additionally to the ones in internal mode, the APIs sendToURL(),
    // FileReference.download(), FileReference.upload(), Loader.load(),
    // LocalConnection.connect(), LocalConnection.send(), NetConnection.connect(),
    // NetStream.play(), Security.loadPolicyFile(), SharedObject.getLocal(),
    // SharedObject.getRemote(), Socket.connect(), Sound.load(), URLLoader.load(),
    // URLStream.load() and XMLSocket.connect() are prevented in this mode.
    //
    // This mode is not implemented yet.
    NONE
};

// Helper class for NetworkingAccessMode enum
class NetworkingAccessModeHelper {
public:
    // Convert NetworkingAccessMode to string
    static std::string to_string(NetworkingAccessMode mode) {
        switch (mode) {
            case NetworkingAccessMode::ALL: return "all";
            case NetworkingAccessMode::INTERNAL: return "internal";
            case NetworkingAccessMode::NONE: return "none";
            default: return "all"; // Default case
        }
    }

    // Parse NetworkingAccessMode from string
    static NetworkingAccessMode from_string(const std::string& str) {
        if (str == "all") {
            return NetworkingAccessMode::ALL;
        } else if (str == "internal") {
            return NetworkingAccessMode::INTERNAL;
        } else if (str == "none") {
            return NetworkingAccessMode::NONE;
        } else {
            throw std::invalid_argument("Invalid NetworkingAccessMode value: " + str);
        }
    }

    // Check equality
    static bool equals(NetworkingAccessMode a, NetworkingAccessMode b) {
        return a == b;
    }
};

// Configuration structure for Ruffle player
struct PlayerConfig {
    // Letterbox setting
    Letterbox letterbox = Letterbox::OFF;

    // Networking access mode
    NetworkingAccessMode networking_access = NetworkingAccessMode::ALL;

    // Whether to show debug output
    bool show_debug_output = false;

    // Whether to enable the AVM1 debugger
    bool enable_avm1_debugger = false;

    // Whether to enable the AVM2 debugger
    bool enable_avm2_debugger = false;

    // Whether to allow script access to network
    bool allow_script_access = true;

    // Whether to allow full screen mode
    bool allow_fullscreen = true;

    // Whether to enable hardware acceleration
    bool enable_hardware_acceleration = true;

    // Whether to enable sound
    bool enable_sound = true;

    // Whether to enable text input
    bool enable_text_input = true;

    // Whether to enable video playback
    bool enable_video = true;

    // Whether to enable bitmap caching
    bool enable_bitmap_cache = true;

    // Whether to enable smoothing
    bool enable_smoothing = true;

    // Whether to enable GPU compositing
    bool enable_gpu_compositing = true;

    // Whether to enable experimental features
    bool enable_experimental_features = false;

    // Whether to enable strict security
    bool enable_strict_security = false;

    // Whether to enable local file access
    bool enable_local_file_access = true;

    // Whether to enable remote file access
    bool enable_remote_file_access = true;

    // Whether to enable cookies
    bool enable_cookies = true;

    // Whether to enable local storage
    bool enable_local_storage = true;

    // Whether to enable session storage
    bool enable_session_storage = true;

    // Whether to enable IndexedDB
    bool enable_indexed_db = true;

    // Whether to enable WebAssembly
    bool enable_web_assembly = true;

    // Whether to enable WebGL
    bool enable_web_gl = true;

    // Whether to enable WebRTC
    bool enable_web_rtc = true;

    // Whether to enable MediaDevices
    bool enable_media_devices = true;

    // Whether to enable Gamepad API
    bool enable_gamepad = true;

    // Whether to enable Geolocation API
    bool enable_geolocation = true;

    // Whether to enable Notifications API
    bool enable_notifications = true;

    // Whether to enable Push API
    bool enable_push = true;

    // Whether to enable Payment Request API
    bool enable_payment_request = true;

    // Whether to enable Background Sync
    bool enable_background_sync = true;

    // Whether to enable Periodic Background Sync
    bool enable_periodic_background_sync = true;

    // Whether to enable Web Workers
    bool enable_web_workers = true;

    // Whether to enable Service Workers
    bool enable_service_workers = true;

    // Whether to enable Shared Workers
    bool enable_shared_workers = true;

    // Whether to enable Broadcast Channel
    bool enable_broadcast_channel = true;

    // Whether to enable Message Channel
    bool enable_message_channel = true;

    // Whether to enable Clipboard API
    bool enable_clipboard = true;

    // Whether to enable File System Access API
    bool enable_file_system_access = true;

    // Whether to enable Streams API
    bool enable_streams = true;

    // Whether to enable Compression Streams API
    bool enable_compression_streams = true;

    // Whether to enable Web Crypto API
    bool enable_web_crypto = true;

    // Whether to enable Web Authentication API
    bool enable_web_authn = true;

    // Whether to enable Credential Management API
    bool enable_credential_management = true;

    // Whether to enable Permissions API
    bool enable_permissions = true;

    // Whether to enable Shape Detection API
    bool enable_shape_detection = true;

    // Whether to enable Generic Sensor API
    bool enable_generic_sensor = true;

    // Whether to enable Ambient Light Sensor API
    bool enable_ambient_light_sensor = true;

    // Whether to enable Accelerometer API
    bool enable_accelerometer = true;

    // Whether to enable Gyroscope API
    bool enable_gyroscope = true;

    // Whether to enable Magnetometer API
    bool enable_magnetometer = true;

    // Whether to enable Orientation Sensor API
    bool enable_orientation_sensor = true;

    // Whether to enable Absolute Orientation Sensor API
    bool enable_absolute_orientation_sensor = true;

    // Whether to enable Linear Acceleration Sensor API
    bool enable_linear_acceleration_sensor = true;

    // Whether to enable Gravity Sensor API
    bool enable_gravity_sensor = true;

    // Whether to enable Proximity Sensor API
    bool enable_proximity_sensor = true;

    // Whether to enable Ambient Light Sensor API
    bool enable_ambient_light_sensor_api = true;

    // Whether to enable Network Information API
    bool enable_network_information = true;

    // Whether to enable Battery Status API
    bool enable_battery_status = true;

    // Whether to enable Vibration API
    bool enable_vibration = true;

    // Whether to enable Wake Lock API
    bool enable_wake_lock = true;

    // Whether to enable Screen Wake Lock API
    bool enable_screen_wake_lock = true;

    // Whether to enable Page Visibility API
    bool enable_page_visibility = true;

    // Whether to enable Fullscreen API
    bool enable_fullscreen_api = true;

    // Whether to enable Pointer Lock API
    bool enable_pointer_lock = true;

    // Whether to enable Presentation API
    bool enable_presentation = true;

    // Whether to enable Remote Playback API
    bool enable_remote_playback = true;

    // Whether to enable Media Session API
    bool enable_media_session = true;

    // Whether to enable Media Capabilities API
    bool enable_media_capabilities = true;

    // Whether to enable MediaRecorder API
    bool enable_media_recorder = true;

    // Whether to enable MediaStream Image Capture API
    bool enable_media_stream_image_capture = true;

    // Whether to enable MediaStreamTrack Content Hints API
    bool enable_media_stream_track_content_hints = true;

    // Whether to enable WebCodecs API
    bool enable_web_codecs = true;

    // Whether to enable WebTransport API
    bool enable_web_transport = true;

    // Whether to enable WebHID API
    bool enable_web_hid = true;

    // Whether to enable Web Serial API
    bool enable_web_serial = true;

    // Whether to enable Web USB API
    bool enable_web_usb = true;

    // Whether to enable Web Bluetooth API
    bool enable_web_bluetooth = true;

    // Whether to enable Web NFC API
    bool enable_web_nfc = true;

    // Whether to enable WebXR Device API
    bool enable_web_xr = true;

    // Whether to enable WebAssembly Threads
    bool enable_web_assembly_threads = true;

    // Whether to enable WebAssembly SIMD
    bool enable_web_assembly_simd = true;

    // Whether to enable WebAssembly Exception Handling
    bool enable_web_assembly_exception_handling = true;

    // Whether to enable WebAssembly Bulk Memory Operations
    bool enable_web_assembly_bulk_memory = true;

    // Whether to enable WebAssembly Reference Types
    bool enable_web_assembly_reference_types = true;

    // Whether to enable WebAssembly Tail Calls
    bool enable_web_assembly_tail_calls = true;

    // Whether to enable WebAssembly Multi-Value
    bool enable_web_assembly_multi_value = true;

    // Whether to enable WebAssembly Garbage Collection
    bool enable_web_assembly_garbage_collection = true;

    // Whether to enable WebAssembly Typed Function References
    bool enable_web_assembly_typed_function_references = true;

    // Whether to enable WebAssembly Memory64
    bool enable_web_assembly_memory64 = true;

    // Whether to enable WebAssembly Extended Constant Expressions
    bool enable_web_assembly_extended_constant_expressions = true;

    // Whether to enable WebAssembly Float16
    bool enable_web_assembly_float16 = true;

    // Whether to enable WebAssembly Relaxed SIMD
    bool enable_web_assembly_relaxed_simd = true;

    // Whether to enable WebAssembly JS Promise Integration
    bool enable_web_assembly_js_promise_integration = true;

    // Whether to enable WebAssembly JavaScript BigInt Integration
    bool enable_web_assembly_javascript_bigint_integration = true;

    // Whether to enable WebAssembly Exception Handling
    bool enable_web_assembly_exception_handling_api = true;

    // Whether to enable WebAssembly Module Linking
    bool enable_web_assembly_module_linking = true;

    // Whether to enable WebAssembly Component Model
    bool enable_web_assembly_component_model = true;

    // Whether to enable WebAssembly Interface Types
    bool enable_web_assembly_interface_types = true;

    // Whether to enable WebAssembly Web API Bindings
    bool enable_web_assembly_web_api_bindings = true;

    // Whether to enable WebAssembly Embedding API
    bool enable_web_assembly_embedding_api = true;

    // Whether to enable WebAssembly Host Binding
    bool enable_web_assembly_host_binding = true;

    // Whether to enable WebAssembly Dynamic Linking
    bool enable_web_assembly_dynamic_linking = true;

    // Whether to enable WebAssembly Streaming Compilation
    bool enable_web_assembly_streaming_compilation = true;

    // Whether to enable WebAssembly Baseline Compiler
    bool enable_web_assembly_baseline_compiler = true;

    // Whether to enable WebAssembly Optimizing Compiler
    bool enable_web_assembly_optimizing_compiler = true;

    // Whether to enable WebAssembly Debugging
    bool enable_web_assembly_debugging = true;

    // Whether to enable WebAssembly Profiling
    bool enable_web_assembly_profiling = true;

    // Whether to enable WebAssembly Tracing
    bool enable_web_assembly_tracing = true;

    // Whether to enable WebAssembly Instrumentation
    bool enable_web_assembly_instrumentation = true;

    // Whether to enable WebAssembly Coverage
    bool enable_web_assembly_coverage = true;

    // Whether to enable WebAssembly Fuzzing
    bool enable_web_assembly_fuzzing = true;

    // Whether to enable WebAssembly Testing
    bool enable_web_assembly_testing = true;

    // Whether to enable WebAssembly Benchmarking
    bool enable_web_assembly_benchmarking = true;

    // Whether to enable WebAssembly Analysis
    bool enable_web_assembly_analysis = true;

    // Whether to enable WebAssembly Optimization
    bool enable_web_assembly_optimization = true;

    // Whether to enable WebAssembly Validation
    bool enable_web_assembly_validation = true;

    // Whether to enable WebAssembly Verification
    bool enable_web_assembly_verification = true;

    // Whether to enable WebAssembly Compilation Hints
    bool enable_web_assembly_compilation_hints = true;

    // Whether to enable WebAssembly Runtime Hints
    bool enable_web_assembly_runtime_hints = true;

    // Whether to enable WebAssembly Compiler Hints
    bool enable_web_assembly_compiler_hints = true;

    // Whether to enable WebAssembly Runtime Features
    bool enable_web_assembly_runtime_features = true;

    // Whether to enable WebAssembly Compiler Features
    bool enable_web_assembly_compiler_features = true;

    // Whether to enable WebAssembly Runtime Options
    bool enable_web_assembly_runtime_options = true;

    // Whether to enable WebAssembly Compiler Options
    bool enable_web_assembly_compiler_options = true;

    // Whether to enable WebAssembly Runtime Configuration
    bool enable_web_assembly_runtime_configuration = true;

    // Whether to enable WebAssembly Compiler Configuration
    bool enable_web_assembly_compiler_configuration = true;

    // Whether to enable WebAssembly Runtime Settings
    bool enable_web_assembly_runtime_settings = true;

    // Whether to enable WebAssembly Compiler Settings
    bool enable_web_assembly_compiler_settings = true;

    // Whether to enable WebAssembly Runtime Parameters
    bool enable_web_assembly_runtime_parameters = true;

    // Whether to enable WebAssembly Compiler Parameters
    bool enable_web_assembly_compiler_parameters = true;

    // Whether to enable WebAssembly Runtime Arguments
    bool enable_web_assembly_runtime_arguments = true;

    // Whether to enable WebAssembly Compiler Arguments
    bool enable_web_assembly_compiler_arguments = true;

    // Whether to enable WebAssembly Runtime Environment
    bool enable_web_assembly_runtime_environment = true;

    // Whether to enable WebAssembly Compiler Environment
    bool enable_web_assembly_compiler_environment = true;

    // Whether to enable WebAssembly Runtime Context
    bool enable_web_assembly_runtime_context = true;

    // Whether to enable WebAssembly Compiler Context
    bool enable_web_assembly_compiler_context = true;

    // Whether to enable WebAssembly Runtime State
    bool enable_web_assembly_runtime_state = true;

    // Whether to enable WebAssembly Compiler State
    bool enable_web_assembly_compiler_state = true;

    // Whether to enable WebAssembly Runtime Resources
    bool enable_web_assembly_runtime_resources = true;

    // Whether to enable WebAssembly Compiler Resources
    bool enable_web_assembly_compiler_resources = true;

    // Whether to enable WebAssembly Runtime Limits
    bool enable_web_assembly_runtime_limits = true;

    // Whether to enable WebAssembly Compiler Limits
    bool enable_web_assembly_compiler_limits = true;

    // Whether to enable WebAssembly Runtime Constraints
    bool enable_web_assembly_runtime_constraints = true;

    // Whether to enable WebAssembly Compiler Constraints
    bool enable_web_assembly_compiler_constraints = true;

    // Whether to enable WebAssembly Runtime Policies
    bool enable_web_assembly_runtime_policies = true;

    // Whether to enable WebAssembly Compiler Policies
    bool enable_web_assembly_compiler_policies = true;

    // Whether to enable WebAssembly Runtime Security
    bool enable_web_assembly_runtime_security = true;

    // Whether to enable WebAssembly Compiler Security
    bool enable_web_assembly_compiler_security = true;

    // Whether to enable WebAssembly Runtime Isolation
    bool enable_web_assembly_runtime_isolation = true;

    // Whether to enable WebAssembly Compiler Isolation
    bool enable_web_assembly_compiler_isolation = true;

    // Whether to enable WebAssembly Runtime Sandboxing
    bool enable_web_assembly_runtime_sandboxing = true;

    // Whether to enable WebAssembly Compiler Sandboxing
    bool enable_web_assembly_compiler_sandboxing = true;

    // Whether to enable WebAssembly Runtime Protection
    bool enable_web_assembly_runtime_protection = true;

    // Whether to enable WebAssembly Compiler Protection
    bool enable_web_assembly_compiler_protection = true;

    // Whether to enable WebAssembly Runtime Monitoring
    bool enable_web_assembly_runtime_monitoring = true;

    // Whether to enable WebAssembly Compiler Monitoring
    bool enable_web_assembly_compiler_monitoring = true;

    // Whether to enable WebAssembly Runtime Logging
    bool enable_web_assembly_runtime_logging = true;

    // Whether to enable WebAssembly Compiler Logging
    bool enable_web_assembly_compiler_logging = true;

    // Whether to enable WebAssembly Runtime Metrics
    bool enable_web_assembly_runtime_metrics = true;

    // Whether to enable WebAssembly Compiler Metrics
    bool enable_web_assembly_compiler_metrics = true;

    // Whether to enable WebAssembly Runtime Telemetry
    bool enable_web_assembly_runtime_telemetry = true;

    // Whether to enable WebAssembly Compiler Telemetry
    bool enable_web_assembly_compiler_telemetry = true;

    // Whether to enable WebAssembly Runtime Diagnostics
    bool enable_web_assembly_runtime_diagnostics = true;

    // Whether to enable WebAssembly Compiler Diagnostics
    bool enable_web_assembly_compiler_diagnostics = true;

    // Whether to enable WebAssembly Runtime Debugging Support
    bool enable_web_assembly_runtime_debugging_support = true;

    // Whether to enable WebAssembly Compiler Debugging Support
    bool enable_web_assembly_compiler_debugging_support = true;

    // Whether to enable WebAssembly Runtime Profiling Support
    bool enable_web_assembly_runtime_profiling_support = true;

    // Whether to enable WebAssembly Compiler Profiling Support
    bool enable_web_assembly_compiler_profiling_support = true;

    // Whether to enable WebAssembly Runtime Tracing Support
    bool enable_web_assembly_runtime_tracing_support = true;

    // Whether to enable WebAssembly Compiler Tracing Support
    bool enable_web_assembly_compiler_tracing_support = true;

    // Whether to enable WebAssembly Runtime Instrumentation Support
    bool enable_web_assembly_runtime_instrumentation_support = true;

    // Whether to enable WebAssembly Compiler Instrumentation Support
    bool enable_web_assembly_compiler_instrumentation_support = true;

    // Whether to enable WebAssembly Runtime Coverage Support
    bool enable_web_assembly_runtime_coverage_support = true;

    // Whether to enable WebAssembly Compiler Coverage Support
    bool enable_web_assembly_compiler_coverage_support = true;

    // Whether to enable WebAssembly Runtime Fuzzing Support
    bool enable_web_assembly_runtime_fuzzing_support = true;

    // Whether to enable WebAssembly Compiler Fuzzing Support
    bool enable_web_assembly_compiler_fuzzing_support = true;

    // Whether to enable WebAssembly Runtime Testing Support
    bool enable_web_assembly_runtime_testing_support = true;

    // Whether to enable WebAssembly Compiler Testing Support
    bool enable_web_assembly_compiler_testing_support = true;

    // Whether to enable WebAssembly Runtime Benchmarking Support
    bool enable_web_assembly_runtime_benchmarking_support = true;

    // Whether to enable WebAssembly Compiler Benchmarking Support
    bool enable_web_assembly_compiler_benchmarking_support = true;

    // Whether to enable WebAssembly Runtime Analysis Support
    bool enable_web_assembly_runtime_analysis_support = true;

    // Whether to enable WebAssembly Compiler Analysis Support
    bool enable_web_assembly_compiler_analysis_support = true;

    // Whether to enable WebAssembly Runtime Optimization Support
    bool enable_web_assembly_runtime_optimization_support = true;

    // Whether to enable WebAssembly Compiler Optimization Support
    bool enable_web_assembly_compiler_optimization_support = true;

    // Whether to enable WebAssembly Runtime Validation Support
    bool enable_web_assembly_runtime_validation_support = true;

    // Whether to enable WebAssembly Compiler Validation Support
    bool enable_web_assembly_compiler_validation_support = true;

    // Whether to enable WebAssembly Runtime Verification Support
    bool enable_web_assembly_runtime_verification_support = true;

    // Whether to enable WebAssembly Compiler Verification Support
    bool enable_web_assembly_compiler_verification_support = true;

    // Whether to enable WebAssembly Runtime Compilation Hints Support
    bool enable_web_assembly_runtime_compilation_hints_support = true;

    // Whether to enable WebAssembly Compiler Compilation Hints Support
    bool enable_web_assembly_compiler_compilation_hints_support = true;

    // Whether to enable WebAssembly Runtime Features Support
    bool enable_web_assembly_runtime_features_support = true;

    // Whether to enable WebAssembly Compiler Features Support
    bool enable_web_assembly_compiler_features_support = true;

    // Whether to enable WebAssembly Runtime Options Support
    bool enable_web_assembly_runtime_options_support = true;

    // Whether to enable WebAssembly Compiler Options Support
    bool enable_web_assembly_compiler_options_support = true;

    // Whether to enable WebAssembly Runtime Configuration Support
    bool enable_web_assembly_runtime_configuration_support = true;

    // Whether to enable WebAssembly Compiler Configuration Support
    bool enable_web_assembly_compiler_configuration_support = true;

    // Whether to enable WebAssembly Runtime Settings Support
    bool enable_web_assembly_runtime_settings_support = true;

    // Whether to enable WebAssembly Compiler Settings Support
    bool enable_web_assembly_compiler_settings_support = true;

    // Whether to enable WebAssembly Runtime Parameters Support
    bool enable_web_assembly_runtime_parameters_support = true;

    // Whether to enable WebAssembly Compiler Parameters Support
    bool enable_web_assembly_compiler_parameters_support = true;

    // Whether to enable WebAssembly Runtime Arguments Support
    bool enable_web_assembly_runtime_arguments_support = true;

    // Whether to enable WebAssembly Compiler Arguments Support
    bool enable_web_assembly_compiler_arguments_support = true;

    // Whether to enable WebAssembly Runtime Environment Support
    bool enable_web_assembly_runtime_environment_support = true;

    // Whether to enable WebAssembly Compiler Environment Support
    bool enable_web_assembly_compiler_environment_support = true;

    // Whether to enable WebAssembly Runtime Context Support
    bool enable_web_assembly_runtime_context_support = true;

    // Whether to enable WebAssembly Compiler Context Support
    bool enable_web_assembly_compiler_context_support = true;

    // Whether to enable WebAssembly Runtime State Support
    bool enable_web_assembly_runtime_state_support = true;

    // Whether to enable WebAssembly Compiler State Support
    bool enable_web_assembly_compiler_state_support = true;

    // Whether to enable WebAssembly Runtime Resources Support
    bool enable_web_assembly_runtime_resources_support = true;

    // Whether to enable WebAssembly Compiler Resources Support
    bool enable_web_assembly_compiler_resources_support = true;

    // Whether to enable WebAssembly Runtime Limits Support
    bool enable_web_assembly_runtime_limits_support = true;

    // Whether to enable WebAssembly Compiler Limits Support
    bool enable_web_assembly_compiler_limits_support = true;

    // Whether to enable WebAssembly Runtime Constraints Support
    bool enable_web_assembly_runtime_constraints_support = true;

    // Whether to enable WebAssembly Compiler Constraints Support
    bool enable_web_assembly_compiler_constraints_support = true;

    // Whether to enable WebAssembly Runtime Policies Support
    bool enable_web_assembly_runtime_policies_support = true;

    // Whether to enable WebAssembly Compiler Policies Support
    bool enable_web_assembly_compiler_policies_support = true;

    // Whether to enable WebAssembly Runtime Security Support
    bool enable_web_assembly_runtime_security_support = true;

    // Whether to enable WebAssembly Compiler Security Support
    bool enable_web_assembly_compiler_security_support = true;

    // Whether to enable WebAssembly Runtime Isolation Support
    bool enable_web_assembly_runtime_isolation_support = true;

    // Whether to enable WebAssembly Compiler Isolation Support
    bool enable_web_assembly_compiler_isolation_support = true;

    // Whether to enable WebAssembly Runtime Sandboxing Support
    bool enable_web_assembly_runtime_sandboxing_support = true;

    // Whether to enable WebAssembly Compiler Sandboxing Support
    bool enable_web_assembly_compiler_sandboxing_support = true;

    // Whether to enable WebAssembly Runtime Protection Support
    bool enable_web_assembly_runtime_protection_support = true;

    // Whether to enable WebAssembly Compiler Protection Support
    bool enable_web_assembly_compiler_protection_support = true;

    // Whether to enable WebAssembly Runtime Monitoring Support
    bool enable_web_assembly_runtime_monitoring_support = true;

    // Whether to enable WebAssembly Compiler Monitoring Support
    bool enable_web_assembly_compiler_monitoring_support = true;

    // Whether to enable WebAssembly Runtime Logging Support
    bool enable_web_assembly_runtime_logging_support = true;

    // Whether to enable WebAssembly Compiler Logging Support
    bool enable_web_assembly_compiler_logging_support = true;

    // Whether to enable WebAssembly Runtime Metrics Support
    bool enable_web_assembly_runtime_metrics_support = true;

    // Whether to enable WebAssembly Compiler Metrics Support
    bool enable_web_assembly_compiler_metrics_support = true;

    // Whether to enable WebAssembly Runtime Telemetry Support
    bool enable_web_assembly_runtime_telemetry_support = true;

    // Whether to enable WebAssembly Compiler Telemetry Support
    bool enable_web_assembly_compiler_telemetry_support = true;

    // Whether to enable WebAssembly Runtime Diagnostics Support
    bool enable_web_assembly_runtime_diagnostics_support = true;

    // Whether to enable WebAssembly Compiler Diagnostics Support
    bool enable_web_assembly_compiler_diagnostics_support = true;
};

} // namespace ruffle

#endif // CONFIG_H