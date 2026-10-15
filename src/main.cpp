#include "thinkgame/ui.hpp"
#include "thinkgame/assets.hpp"
#include <cstdlib>
#include <iostream>
int main(int argc,char** argv) {
    try {
        thinkgame::validateCatalogue();
        std::filesystem::path screenshot,save=thinkgame::SaveStore::defaultPath();
        bool levels=false,puzzle=false;sf::Vector2u size{1120,800};
        for(int i=1;i<argc;++i) {
            const std::string arg=argv[i];
            if(arg=="--screenshot"&&i+1<argc)screenshot=argv[++i];
            else if(arg=="--save-file"&&i+1<argc)save=argv[++i];
            else if(arg=="--levels")levels=true;
            else if(arg=="--puzzle")puzzle=true;
            else if(arg=="--size"&&i+2<argc){size.x=static_cast<unsigned>(std::stoul(argv[++i]));size.y=static_cast<unsigned>(std::stoul(argv[++i]));if(size.x<720||size.y<600||size.x>3840||size.y>2160)throw std::runtime_error("Supported window sizes start at 720 x 600.");}
            else if(arg=="--help"){std::cout<<"ThinkGame\n  --screenshot file.png  Render the real game without showing a window\n  --levels               Start on Levels\n  --puzzle               Start directly in the game\n  --size width height    Window dimensions (minimum 720 x 600)\n  --save-file path       Use a separate save, useful for testing\n";return 0;}
            else throw std::runtime_error("Unknown or incomplete argument: "+arg);
        }
        thinkgame::Game game{thinkgame::SaveStore(save)};
        const char* searchPath=std::getenv("PATH");
        thinkgame::Ui ui(game,thinkgame::findAssets(argv[0],THINKGAME_ASSET_DIR,searchPath?searchPath:""));
        if(puzzle)ui.showPuzzle();ui.showLevels(levels);
        if(!screenshot.empty()) {
            sf::RenderTexture texture(size);
            ui.draw(texture,size);texture.display();
            if(!texture.getTexture().copyToImage().saveToFile(screenshot))throw std::runtime_error("Could not save screenshot");
            std::cout<<"Rendered "<<screenshot<<'\n';return 0;
        }
        sf::RenderWindow window(sf::VideoMode(size),"ThinkGame",sf::Style::Default);
        window.setMinimumSize(sf::Vector2u{720,600});window.setFramerateLimit(60);
        while(window.isOpen()) {
            while(const auto event=window.pollEvent()) {
                if(event->is<sf::Event::Closed>()){game.persist();window.close();}
                if(const auto* resize=event->getIf<sf::Event::Resized>())window.setView(sf::View(sf::FloatRect({0,0},sf::Vector2f(resize->size))));
                if(const auto* click=event->getIf<sf::Event::MouseButtonPressed>())ui.event(*event,sf::Vector2f(click->position));
                else if(const auto* move=event->getIf<sf::Event::MouseMoved>())ui.event(*event,sf::Vector2f(move->position));
                else ui.event(*event,sf::Vector2f(sf::Mouse::getPosition(window)));
            }
            if(window.isOpen()){ui.draw(window,window.getSize());window.display();}
        }
        return 0;
    }catch(const std::exception& e){std::cerr<<"ThinkGame: "<<e.what()<<'\n';return 1;}
}
