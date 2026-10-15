#include "thinkgame/ui.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace thinkgame;
void require(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
void click(Ui& ui,float x,float y){ui.event(sf::Event(sf::Event::MouseButtonPressed{sf::Mouse::Button::Left,{static_cast<int>(x),static_cast<int>(y)}}),{x,y});}
void key(Ui& ui,sf::Keyboard::Key code){sf::Event::KeyPressed pressed{};pressed.code=code;ui.event(sf::Event(pressed),{});}
void type(Ui& ui,const std::string& value){for(unsigned char c:value)ui.event(sf::Event(sf::Event::TextEntered{c}),{});}
int main(){try{
    const auto temp=std::filesystem::temp_directory_path()/"thinkgame-ui-tests";
    std::filesystem::remove_all(temp);std::filesystem::create_directories(temp/"screens");
    Game game{SaveStore(temp/"progress.save")};Ui ui(game,THINKGAME_ASSET_DIR);
    sf::RenderTexture texture({1200,960});
    auto render=[&](const std::string& name,sf::Vector2u size=sf::Vector2u{1200,960}){
        require(texture.resize(size),"Render resize failed");ui.draw(texture,size);texture.display();
        require(texture.getTexture().copyToImage().saveToFile(temp/"screens"/(name+".png")),"Screenshot failed");
    };
    render("home");require(ui.screen()==Ui::Screen::Home&&game.score()==0,"Game did not start at home");
    require(ui.soundEnabled(),"Bundled sound effects did not load");
    click(ui,790,46);require(!ui.soundEnabled(),"Sound mute control failed");
    render("home-muted");key(ui,sf::Keyboard::Key::Escape);
    type(ui,"17");key(ui,sf::Keyboard::Key::Backspace);require(game.answer.empty(),"Home accepted puzzle input");
    key(ui,sf::Keyboard::Key::Enter);require(ui.screen()==Ui::Screen::Puzzle,"Home Play did not enter game");
    render("initial");
    click(ui,1018,46);render("initial-levels");
    click(ui,488,268);require(game.lockedLevel==2&&game.progress().current==1,"Locked node bypassed progression");render("locked-popup");
    key(ui,sf::Keyboard::Key::Escape);require(!game.lockedLevel,"Popup escape failed");render("initial-levels");
    key(ui,sf::Keyboard::Key::Escape);render("initial");
    click(ui,1100,250);require(game.hintVisible()&&game.hintsLeft()==0,"Hint click failed");render("hint");
    type(ui,"18");key(ui,sf::Keyboard::Key::Backspace);type(ui,"7");require(game.answer=="17","Keyboard editing failed");
    key(ui,sf::Keyboard::Key::Enter);require(game.progress().completed==1&&game.score()==100,"Enter did not submit or award score");render("solution");
    click(ui,1118,46);require(ui.screen()==Ui::Screen::Score,"Score navigation failed");render("score");
    type(ui,"99");require(game.answer=="17","Score accepted puzzle input");
    click(ui,910,46);require(ui.screen()==Ui::Screen::Home,"Home navigation failed");render("home-progress");
    key(ui,sf::Keyboard::Key::Enter);require(ui.screen()==Ui::Screen::Puzzle&&game.progress().current==2,"Continue did not open next unsolved level");
    require(game.select(1),"Replay selection failed");render("solution");
    render("small-solution",{720,760});
    key(ui,sf::Keyboard::Key::Tab);render("small-focused-next",{720,760});
    key(ui,sf::Keyboard::Key::Enter);require(game.progress().current==2,"Small-window keyboard Next failed");
    require(game.select(1),"Replay selection failed");render("solution");
    click(ui,1018,46);render("levels");
    click(ui,824,752);require(game.lockedLevel==30,"Levels map lock failed");render("levels-locked");key(ui,sf::Keyboard::Key::Escape);render("levels");
    key(ui,sf::Keyboard::Key::Escape);require(ui.screen()==Ui::Screen::Puzzle,"Levels did not return to its origin");render("solution");
    for(int level=2;level<=30;++level){
        key(ui,sf::Keyboard::Key::Tab);render("focused-next");key(ui,sf::Keyboard::Key::Enter);require(game.progress().current==level,"Next level navigation failed");
        render("level-"+std::to_string(level));
        render("small-"+std::to_string(level),{720,760});
        render("level-"+std::to_string(level));
        for(int clue=1;clue<=hintLimit(level);++clue){
            click(ui,1100,250);require(game.hintVisible()&&game.hintsUsed()==clue,"Progressive word-clue click failed");
            render("hint-"+std::to_string(level)+"-"+std::to_string(clue));
        }
        click(ui,1100,250);require(game.hintsLeft()==0&&game.hintsUsed()==hintLimit(level),"UI tier limit bypassed");
        render("hint-"+std::to_string(level));
        render("small-hint-"+std::to_string(level),{720,600});
        render("level-"+std::to_string(level));
        type(ui,std::to_string(puzzles()[level-1].answer()));
        if(level%2)key(ui,sf::Keyboard::Key::Enter);else click(ui,720,715);
        require(game.progress().completed==level,"UI submission failed");
        render("solved-"+std::to_string(level));
    }
    render("complete");
    click(ui,1118,46);render("score-complete");require(game.score()==3000,"Final score incorrect");
    key(ui,sf::Keyboard::Key::Escape);render("home-complete");
    render("small-home",{720,600});key(ui,sf::Keyboard::Key::Enter);require(ui.screen()==Ui::Screen::Puzzle,"Small home Play failed");
    render("smallest-puzzle",{720,600});
    click(ui,530,46);render("smallest-levels",{720,600});require(ui.screen()==Ui::Screen::Levels,"Small levels navigation failed");
    click(ui,584,550);require(!game.lockedLevel&&game.progress().current==30,"Small level selection failed");
    render("large-puzzle",{1920,1080});
    click(ui,1570,55);require(ui.screen()==Ui::Screen::Home,"Scaled Home hit target failed");render("large-home",{1920,1080});
    click(ui,1824,55);require(ui.screen()==Ui::Screen::Score,"Scaled Score hit target failed");render("large-score",{1920,1080});
    Game restored{SaveStore(temp/"progress.save")};require(restored.progress().completed==30&&restored.progress().hints[0]==1&&restored.score()==3000,"UI save or score was not restored");
    // Reproduce a real player's exhausted, saved hint quota at the default window size.
    SaveStore exhaustedStore(temp/"exhausted.save");Progress exhausted;exhausted.hints[0]=3;std::string error;
    require(exhaustedStore.save(exhausted,error),"Could not create exhausted save");
    Game reviewGame(exhaustedStore);Ui reviewUi(reviewGame,THINKGAME_ASSET_DIR);reviewUi.showPuzzle();
    require(texture.resize({1120,800}),"Review texture resize failed");
    auto reviewRender=[&](const char* name,sf::Vector2u size){
        require(texture.resize(size),"Review resize failed");reviewUi.draw(texture,size);texture.display();
        require(texture.getTexture().copyToImage().saveToFile(temp/"screens"/(std::string(name)+".png")),"Review screenshot failed");
    };
    reviewRender("exhausted-before",{1120,800});click(reviewUi,715,46);require(!reviewUi.soundEnabled(),"Review mute failed");
    reviewRender("exhausted-before",{1120,800});click(reviewUi,1010,250);
    require(reviewGame.hintVisible()&&reviewGame.hintsLeft()==0,"Default-size Review hint failed");reviewRender("review-hint",{1120,800});
    sf::sleep(sf::milliseconds(3200));require(reviewGame.hintVisible(),"Hint still expires automatically");
    click(reviewUi,1039,307);require(!reviewGame.hintVisible(),"Close hint click failed");
    reviewRender("exhausted-before",{1120,800});click(reviewUi,1010,250);require(reviewGame.hintVisible()&&reviewGame.hintsLeft()==0,"Repeated review was blocked");
    reviewRender("smallest-review-hint",{720,600});
    reviewRender("large-review-hint",{1920,1080});
    click(reviewUi,1595,385);require(!reviewGame.hintVisible(),"Scaled Close hint click failed");
    reviewRender("large-review-before",{1920,1080});click(reviewUi,1550,300);require(reviewGame.hintVisible()&&reviewGame.hintsLeft()==0,"Scaled Review hint click failed");
    Game reviewRestored(exhaustedStore);require(reviewRestored.hintsLeft()==0,"Review changed saved quota");
    Audio audio(THINKGAME_ASSET_DIR);require(audio.available()&&audio.enabled(),"Audio files did not load");
    for(auto cue:{Audio::Cue::Click,Audio::Cue::Hint,Audio::Cue::Correct,Audio::Cue::Wrong,Audio::Cue::Start,Audio::Cue::Complete}){
        audio.play(cue);require(audio.status(cue)==sf::SoundSource::Status::Playing,"Audio cue did not start");
        sf::sleep(sf::milliseconds(25));
    }
    audio.toggle();
    for(auto cue:{Audio::Cue::Click,Audio::Cue::Hint,Audio::Cue::Correct,Audio::Cue::Wrong,Audio::Cue::Start,Audio::Cue::Complete})require(audio.status(cue)==sf::SoundSource::Status::Stopped,"Mute did not stop all cues");
    audio.play(Audio::Cue::Correct);require(audio.status(Audio::Cue::Correct)==sf::SoundSource::Status::Stopped,"Muted sound started");
    audio.toggle();audio.play(Audio::Cue::Hint);require(audio.status(Audio::Cue::Hint)==sf::SoundSource::Status::Playing,"Unmute did not resume sound effects");
    std::cout<<"PASS: saved clues remain open without a timer and can be viewed and closed at default and scaled sizes; all tiered progressive word hints and six audio cues work, mute stops playback, and unmute works; home, Play/Continue, Levels/Score navigation, saved scoring, scaled mouse targets, actual SFML mouse/keyboard events, locked popups, hint display, both submit methods, all 30 screens, five shapes, small-window renders, solutions and final completion.\nScreenshots: "<<temp/"screens"<<'\n';
    return 0;
}catch(const std::exception& e){std::cerr<<"UI FAIL: "<<e.what()<<'\n';return 1;}}
