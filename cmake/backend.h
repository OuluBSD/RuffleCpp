/*
 * C++ header for backend functionality
 * This replaces the functionality of core/src/backend.rs
 */

#ifndef BACKEND_H
#define BACKEND_H

#include "backend/audio.h"
#include "backend/log.h"
#include "backend/navigator.h"
#include "backend/storage.h"
#include "backend/ui.h"

// Backend namespace for grouping backend functionality
namespace ruffle::backend {

// Audio backend interface
using AudioBackend = ruffle::backend::audio::AudioBackend;

// Logging backend interface
using LogBackend = ruffle::backend::log::LogBackend;

// Navigator backend interface
using NavigatorBackend = ruffle::backend::navigator::NavigatorBackend;

// Storage backend interface
using StorageBackend = ruffle::backend::storage::StorageBackend;

// UI backend interface
using UiBackend = ruffle::backend::ui::UiBackend;

// Combined backend interface that aggregates all backend services
class Backend {
public:
    std::shared_ptr<AudioBackend> audio;
    std::shared_ptr<LogBackend> log;
    std::shared_ptr<NavigatorBackend> navigator;
    std::shared_ptr<StorageBackend> storage;
    std::shared_ptr<UiBackend> ui;

    Backend(std::shared_ptr<AudioBackend> audio_backend,
            std::shared_ptr<LogBackend> log_backend,
            std::shared_ptr<NavigatorBackend> navigator_backend,
            std::shared_ptr<StorageBackend> storage_backend,
            std::shared_ptr<UiBackend> ui_backend)
        : audio(std::move(audio_backend)),
          log(std::move(log_backend)),
          navigator(std::move(navigator_backend)),
          storage(std::move(storage_backend)),
          ui(std::move(ui_backend)) {}

    // Static factory method to create a backend instance
    static std::shared_ptr<Backend> create(
        std::shared_ptr<AudioBackend> audio_backend,
        std::shared_ptr<LogBackend> log_backend,
        std::shared_ptr<NavigatorBackend> navigator_backend,
        std::shared_ptr<StorageBackend> storage_backend,
        std::shared_ptr<UiBackend> ui_backend) {
        
        return std::make_shared<Backend>(
            std::move(audio_backend),
            std::move(log_backend),
            std::move(navigator_backend),
            std::move(storage_backend),
            std::move(ui_backend)
        );
    }
};

} // namespace ruffle::backend

#endif // BACKEND_H