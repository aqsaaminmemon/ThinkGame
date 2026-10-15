#pragma once
#include "thinkgame/game.hpp"
#include "thinkgame/audio.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

namespace thinkgame {
class Ui {
public:
    enum class Screen { Home, Puzzle, Levels, Score };
    Ui(Game& game,const std::filesystem::path& assets);
    void draw(sf::RenderTarget& target,sf::Vector2u size);
    void event(const sf::Event& event,sf::Vector2f mouse);
    void showLevels(bool value) { if(value) { screen_=Screen::Levels; focus_="levels"; } }
    void showPuzzle() { screen_=Screen::Puzzle; focus_="answer"; }
    Screen screen() const { return screen_; }
    bool soundEnabled() const { return audio_.enabled(); }
private:
    struct Control { sf::FloatRect rect; std::string id; bool enabled; };
    Game& game_;
    Audio audio_;
    sf::Font regular_,bold_;
    sf::Texture logo_,mascot_;
    sf::Shader logoShader_;
    bool shaderReady_=false;
    Screen screen_=Screen::Home,levelsBack_=Screen::Home;
    float scale_=1;
    float scroll_=0,maxScroll_=0;
    sf::Vector2f mouse_{};
    std::string focus_="play";
    std::vector<Control> controls_;
    sf::RenderTarget* target_=nullptr;
    sf::Vector2f size_{};
    sf::Vector2u pixelSize_{};
    std::optional<sf::FloatRect> hintCard_;
    void text(const std::string& value,float x,float y,unsigned size,sf::Color color,bool bold=false,bool centre=false);
    float paragraph(const std::string& value,float x,float y,float width,unsigned size,sf::Color color,bool draw=true);
    void box(sf::FloatRect rect,sf::Color fill,float radius=12,sf::Color outline=sf::Color::Transparent,float thickness=1);
    void line(sf::Vector2f a,sf::Vector2f b,sf::Color color,float thickness=1);
    void circle(sf::Vector2f centre,float radius,sf::Color fill,sf::Color outline=sf::Color::Transparent,float thickness=1);
    void lock(float x,float y,sf::Color color,float scale=1);
    void bulb(float x,float y,sf::Color color);
    void button(sf::FloatRect rect,const std::string& label,const std::string& id,bool enabled=true,bool primary=true);
    void activate(const std::string& id);
    void header();
    void logo(sf::Vector2f position,float size);
    void homeScreen();
    void scoreScreen();
    void levelNodes(sf::FloatRect area,int columns=3);
    void sidebar();
    void levelScreen();
    void puzzleScreen();
    void diagram(sf::FloatRect area);
    void popup();
    void dash(sf::FloatRect rect);
};
}
