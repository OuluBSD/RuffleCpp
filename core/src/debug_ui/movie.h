// C++ translation of core/src/debug_ui/movie.rs
// Debug UI for movie list and movie information

#ifndef RUFFLE_CORE_DEBUG_UI_MOVIE_H
#define RUFFLE_CORE_DEBUG_UI_MOVIE_H

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

// Forward declarations
namespace ruffle {
namespace core {

namespace tag_utils {
    template<typename GC>
    class SwfMovie;
}

namespace character {
    enum class Character;
}

namespace library {
    template<typename GC>
    class Library;
}

namespace debug_ui {
    enum class Message;
    struct ItemToSave;
}

namespace display_object {
    template<typename GC>
    class DisplayObject;
}

} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {
namespace debug_ui {
namespace movie {

/// Panel types for movie window
enum class Panel {
    Information,
    Characters
};

/// Window for displaying list of known movies
class MovieListWindow {
private:
    std::string url_search_;

public:
    MovieListWindow() = default;

    /// Show the movie list window
    /// Returns true if window should stay open
    template<typename GC>
    bool show(
        void* egui_ctx,
        UpdateContext<GC>* context,
        std::vector<Message>* messages
    );
};

/// Window for displaying movie details
class MovieWindow {
private:
    Panel open_panel_ = Panel::Information;
    std::string character_search_;

public:
    MovieWindow() = default;

    /// Show the movie window
    /// Returns true if window should stay open
    template<typename GC>
    bool show(
        void* egui_ctx,
        UpdateContext<GC>* context,
        std::shared_ptr<tag_utils::SwfMovie<GC>> movie,
        std::vector<Message>* messages
    );

private:
    /// Show characters panel
    template<typename GC>
    void show_characters(
        void* ui,
        UpdateContext<GC>* context,
        const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie
    );

    /// Show information panel
    template<typename GC>
    void show_information(
        void* ui,
        const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie,
        std::vector<Message>* messages
    );
};

/// Get a display name for a movie
template<typename GC>
std::string movie_name(const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie);

/// Show button to open movie details
template<typename GC>
void open_movie_button(
    void* ui,
    const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie,
    std::vector<Message>* messages
);

/// Show button to open character details
void open_character_button(void* ui, character::Character character);

/// Save SWF file
template<typename GC>
void save_swf(
    const std::shared_ptr<tag_utils::SwfMovie<GC>>& movie,
    std::vector<Message>* messages
);

} // namespace movie
} // namespace debug_ui
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DEBUG_UI_MOVIE_H
