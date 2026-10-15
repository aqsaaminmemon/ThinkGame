#pragma once
#include <SFML/Audio.hpp>
#include <array>
#include <filesystem>
#include <memory>

namespace thinkgame {
class Audio {
public:
    enum class Cue { Click, Hint, Correct, Wrong, Start, Complete };
    explicit Audio(const std::filesystem::path& assets);
    void play(Cue cue);
    void toggle();
    bool enabled() const { return enabled_; }
    bool available() const { return available_; }
    sf::SoundSource::Status status(Cue cue) const;
private:
    // Buffers must outlive the sounds that reference them.
    std::array<sf::SoundBuffer,6> buffers_;
    std::array<std::unique_ptr<sf::Sound>,6> sounds_;
    bool enabled_=true,available_=true;
};
}
