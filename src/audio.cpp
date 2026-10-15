#include "thinkgame/audio.hpp"

namespace thinkgame {
Audio::Audio(const std::filesystem::path& assets) {
    const std::array<const char*,6> files={"click.wav","hint.wav","correct.wav","wrong.wav","start.wav","complete.wav"};
    for(std::size_t i=0;i<files.size();++i){
        if(!buffers_[i].loadFromFile(assets/"sounds"/files[i])){available_=false;enabled_=false;return;}
        sounds_[i]=std::make_unique<sf::Sound>(buffers_[i]);
        sounds_[i]->setVolume(i==0?25.f:40.f);
    }
}
void Audio::play(Cue cue){
    if(!enabled_||!available_)return;
    for(auto& sound:sounds_)if(sound)sound->stop();
    sounds_[static_cast<std::size_t>(cue)]->play();
}
void Audio::toggle(){
    if(!available_)return;
    enabled_=!enabled_;
    if(!enabled_)for(auto& sound:sounds_)if(sound)sound->stop();
}
sf::SoundSource::Status Audio::status(Cue cue) const {
    const auto& sound=sounds_[static_cast<std::size_t>(cue)];
    return sound?sound->getStatus():sf::SoundSource::Status::Stopped;
}
}
