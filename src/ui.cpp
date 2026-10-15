#include "thinkgame/ui.hpp"
#include <algorithm>
#include <cmath>
#include <chrono>
#include <sstream>
#include <stdexcept>

namespace thinkgame {
namespace {
constexpr float pi=3.14159265359f;
const sf::Color ink{38,41,39},muted{119,125,114},yellow{255,215,78},pale{255,245,200},border{230,234,223},gold{170,137,35};
const std::array<sf::Color,12> pastel={sf::Color{239,233,255},{228,242,253},{233,244,220},{253,233,225},{255,241,213},{225,242,236},{248,231,242},{236,239,249},{255,244,200},{232,246,246},{245,237,222},{238,243,227}};
sf::FloatRect rect(float x,float y,float w,float h){return {{x,y},{w,h}};}
sf::Vector2f polar(sf::Vector2f centre,float radius,float angle){return centre+sf::Vector2f{radius*std::cos(angle),radius*std::sin(angle)};}
sf::String utf8(const std::string& value){return sf::String::fromUtf8(value.begin(),value.end());}
}
Ui::Ui(Game& game,const std::filesystem::path& assets):game_(game),audio_(assets){
    if(!regular_.openFromFile(assets/"DejaVuSans.ttf")||!bold_.openFromFile(assets/"DejaVuSans-Bold.ttf"))throw std::runtime_error("Could not load bundled fonts");
    if(!logo_.loadFromFile(assets/"logo.png"))throw std::runtime_error("Could not load the supplied logo");
    logo_.setSmooth(true);
    if(!mascot_.loadFromFile(assets/"home-mascot.png"))throw std::runtime_error("Could not load the home illustration");
    mascot_.setSmooth(true);
    // Presentation-only monochrome mask: the supplied image remains unchanged.
    if(sf::Shader::isAvailable()){
        shaderReady_=logoShader_.loadFromMemory("uniform sampler2D texture; void main(){vec4 p=texture2D(texture,gl_TexCoord[0].xy);float l=dot(p.rgb,vec3(0.299,0.587,0.114));float a=1.0-smoothstep(0.35,0.88,l);gl_FragColor=vec4(0.28,0.23,0.10,a);}",sf::Shader::Type::Fragment);
        if(shaderReady_)logoShader_.setUniform("texture",sf::Shader::CurrentTexture);
    }
}
void Ui::text(const std::string& value,float x,float y,unsigned size,sf::Color color,bool bold,bool centre){
    sf::Text t(bold?bold_:regular_,utf8(value),size);t.setFillColor(color);
    const auto bounds=t.getLocalBounds();
    t.setOrigin({centre?bounds.position.x+bounds.size.x/2:bounds.position.x,bounds.position.y});t.setPosition({x,y});target_->draw(t);
}
float Ui::paragraph(const std::string& value,float x,float y,float width,unsigned size,sf::Color color,bool draw){
    std::istringstream words(value);std::string word,row;float top=y;
    while(words>>word){const auto candidate=row.empty()?word:row+" "+word;sf::Text probe(regular_,utf8(candidate),size);
        if(!row.empty()&&probe.getLocalBounds().size.x>width){if(draw)text(row,x,top,size,color);top+=size+7;row=word;}else row=candidate;}
    if(!row.empty()){if(draw)text(row,x,top,size,color);top+=size+7;}
    return top;
}
void Ui::box(sf::FloatRect r,sf::Color fill,float radius,sf::Color outline,float thickness){
    radius=std::min({radius,r.size.x/2,r.size.y/2});sf::ConvexShape shape;shape.setPointCount(36);
    std::array<sf::Vector2f,4> corners={sf::Vector2f{r.position.x+r.size.x-radius,r.position.y+radius},{r.position.x+r.size.x-radius,r.position.y+r.size.y-radius},{r.position.x+radius,r.position.y+r.size.y-radius},{r.position.x+radius,r.position.y+radius}};
    for(int c=0;c<4;++c)for(int i=0;i<9;++i)shape.setPoint(c*9+i,polar(corners[c],radius,(-pi/2+c*pi/2)+i*pi/16));
    shape.setFillColor(fill);shape.setOutlineColor(outline);shape.setOutlineThickness(thickness);target_->draw(shape);
}
void Ui::line(sf::Vector2f a,sf::Vector2f b,sf::Color color,float thickness){
    const auto delta=b-a;sf::RectangleShape s({std::sqrt(delta.x*delta.x+delta.y*delta.y),thickness});s.setOrigin({0,thickness/2});s.setPosition(a);s.setRotation(sf::radians(std::atan2(delta.y,delta.x)));s.setFillColor(color);target_->draw(s);
}
void Ui::circle(sf::Vector2f centre,float radius,sf::Color fill,sf::Color outline,float thickness){sf::CircleShape s(radius,60);s.setOrigin({radius,radius});s.setPosition(centre);s.setFillColor(fill);s.setOutlineColor(outline);s.setOutlineThickness(thickness);target_->draw(s);}
void Ui::lock(float x,float y,sf::Color color,float scale){circle({x+5*scale,y+4*scale},4*scale,sf::Color::Transparent,color,scale);box(rect(x,y+5*scale,10*scale,8*scale),sf::Color::White,2*scale,color,scale);circle({x+5*scale,y+9*scale},scale,color);}
void Ui::bulb(float x,float y,sf::Color color){circle({x+5,y+5},4,sf::Color::Transparent,color,1);line({x+3,y+9},{x+7,y+9},color);line({x+3,y+12},{x+7,y+12},color);}
void Ui::button(sf::FloatRect r,const std::string& label,const std::string& id,bool enabled,bool primary){
    auto fill=primary?yellow:pale;
    if(!enabled)fill=sf::Color{246,241,218};else if(r.contains(mouse_))fill=primary?sf::Color{255,207,51}:sf::Color{255,239,172};
    box(r,fill,12,focus_==id?gold:sf::Color::Transparent,focus_==id?2:1);
    const unsigned font=r.size.y>=52?16:14;
    text(label,r.position.x+r.size.x/2+(id=="hint"&&label=="Hint"?7.f:0.f),r.position.y+(r.size.y-font)/2,font,enabled?ink:sf::Color{160,155,131},true,true);
    controls_.push_back({r,id,enabled});
}
void Ui::logo(sf::Vector2f position,float size){
    box(rect(position.x,position.y,size,size),yellow,size*.3f);
    sf::Sprite sprite(logo_);sprite.setTextureRect(sf::IntRect({38,12},{114,83}));sprite.setScale({size*.87f/114,size*.87f/114});sprite.setPosition(position+sf::Vector2f{size*.065f,size*.18f});
    sf::RenderStates states;if(shaderReady_)states.shader=&logoShader_;target_->draw(sprite,states);
}
void Ui::header(){
    box(rect(0,0,size_.x,90),sf::Color::White,0);line({0,89},{size_.x,89},border);
    logo({30,23},46);
    text("ThinkGame",90,35,22,ink,true);
    const float start=size_.x-330;
    button(rect(start,24,90,44),"Home","home",true,screen_==Screen::Home);
    button(rect(start+100,24,90,44),"Levels","levels",true,screen_==Screen::Levels);
    button(rect(start+200,24,100,44),"Score","score",true,screen_==Screen::Score);
    const float soundWidth=size_.x<850?54.f:126.f;
    button(rect(start-soundWidth-12,24,soundWidth,44),audio_.available()?(size_.x<850?(audio_.enabled()?"♪ On":"♪ Off"):(audio_.enabled()?"♪ Sound on":"♪ Sound off")):"No audio","sound",audio_.available(),false);
    if(size_.x>=1250)text(game_.saveStatus=="Progress saved"?"✓  Progress saved":"Save needs attention",start-soundWidth-196,41,12,game_.saveStatus=="Progress saved"?sf::Color{99,132,111}:sf::Color{162,72,53});
}
void Ui::homeScreen(){
    const float width=std::min(1040.f,size_.x-64),left=(size_.x-width)/2;
    const float top=std::clamp((size_.y-590)/2+105,120.f,205.f);
    const float height=size_.y<700?230.f:300.f;
    const bool compact=size_.x<850;
    box(rect(left,top,width,height),sf::Color{251,248,239},28,sf::Color{240,231,211});
    box(rect(left+32,top+27,155,28),pale,14);
    text("30 LEVELS TO SOLVE",left+109,top+35,10,gold,true,true);
    text("Find the pattern.",left+32,top+72,compact?29:39,ink,true);
    text("Solve the puzzle.",left+32,top+120,compact?29:39,ink,true);
    paragraph("Pick a level and find the missing number.",left+32,top+178,compact?310.f:440.f,15,muted);
    const float artSize=compact?225.f:320.f;
    const sf::Vector2f artCentre{left+width-(compact?132.f:185.f),top+height/2};
    circle(artCentre,compact?95.f:125.f,sf::Color{255,240,188});
    sf::Sprite mascot(mascot_);const float artScale=artSize/mascot_.getSize().x;
    mascot.setScale({artScale,artScale});mascot.setPosition(artCentre-sf::Vector2f{artSize/2,artSize/2});target_->draw(mascot);
    const int next=std::min(game_.progress().completed+1,30);
    const float statsY=top+height+20,cardW=(width-24)/3;
    const std::array<std::string,3> labels={"LEVEL TO PLAY","LEVELS SOLVED","TOTAL SCORE"};
    const std::array<std::string,3> valuesText={std::to_string(next)+" / 30",std::to_string(game_.progress().completed)+" / 30",std::to_string(game_.score())};
    for(int i=0;i<3;++i){const float x=left+i*(cardW+12);box(rect(x,statsY,cardW,88),sf::Color::White,16,border);text(labels[i],x+18,statsY+17,11,muted,true);text(valuesText[i],x+18,statsY+41,25,ink,true);}
    const float actionsY=statsY+108;
    button(rect(left,actionsY,width*.46f,56),game_.progress().completed==0?"Play →":game_.progress().completed==30?"Play again →":"Continue playing →","play");
    const float remaining=width*.54f-24;
    button(rect(left+width*.46f+12,actionsY,remaining/2,56),"See levels","levels",true,false);
    button(rect(left+width*.46f+24+remaining/2,actionsY,remaining/2,56),"See score","score",true,false);
    text("Your progress is saved automatically.",size_.x/2,actionsY+78,12,muted,false,true);
    if(game_.saveStatus!="Progress saved")paragraph(game_.saveStatus,left,actionsY+105,width,12,sf::Color{170,75,56});
}
void Ui::scoreScreen(){
    const float centre=size_.x/2,width=std::min(700.f,size_.x-64),left=centre-width/2;
    const float top=size_.y<700?118.f:155.f;
    text("Your score",centre,top,30,ink,true,true);
    text("Every pattern you solve is a little win.",centre,top+47,15,muted,false,true);
    box(rect(left,top+85,width,180),sf::Color{255,249,224},24,sf::Color{240,226,180});
    text("TOTAL POINTS",centre,top+108,12,gold,true,true);
    text(std::to_string(game_.score()),centre,top+137,58,ink,true,true);
    text("100 points for each completed level",centre,top+227,14,muted,false,true);
    const float y=top+285;
    text("Levels solved",left,y,16,ink,true);
    text(std::to_string(game_.progress().completed)+" / 30",left+width-90,y,16,ink,true);
    box(rect(left,y+34,width,12),border,6);
    if(game_.progress().completed>0)box(rect(left,y+34,width*game_.progress().completed/30.f,12),yellow,6);
    text(game_.progress().completed==30?"All 30 patterns discovered. Beautiful work!":"Keep going. Your next discovery is waiting.",centre,y+65,14,muted,false,true);
    button(rect(centre-125,y+109,250,56),game_.progress().completed==30?"Play again →":"Keep playing →","play");
    text("Your score and progress are saved automatically.",centre,y+184,12,muted,false,true);
    if(game_.saveStatus!="Progress saved")paragraph(game_.saveStatus,left,y+205,width,12,sf::Color{170,75,56});
}
void Ui::levelNodes(sf::FloatRect area,int columns){
    const int rows=30/columns;
    const float step=area.size.y/rows;
    const float centre=area.position.x+area.size.x/2;
    line({centre,area.position.y+step/2},{centre,area.position.y+step*(rows-.5f)},sf::Color{217,220,212},1.5f);
    for(int row=0;row<rows;++row){const float y=area.position.y+step*(row+.5f);line({area.position.x+area.size.x/(columns*2),y},{area.position.x+area.size.x*(1-1.f/(columns*2)),y},sf::Color{217,220,212},1.5f);
        for(int col=0;col<columns;++col){const int level=row*columns+col+1;const float x=area.position.x+area.size.x*(col+.5f)/columns;
            const bool done=game_.progress().solved(level),current=level==game_.progress().current,available=game_.progress().unlocked(level);
            circle({x,y},21,done?yellow:sf::Color::White,done?sf::Color{230,188,54}:current?ink:sf::Color{190,195,185},current?2:1);
            if(rect(x-22,y-22,44,44).contains(mouse_)||focus_=="level:"+std::to_string(level))circle({x,y},24,sf::Color::Transparent,done?pale:border,2);
            text(std::to_string(level),x,y-7,13,ink,current,true);
            if(done){circle({x+16,y+16},7,pale);text("✓",x+16,y+11,9,gold,false,true);}
            else if(!available){circle({x+17,y+17},8,sf::Color::White);lock(x+12,y+11,sf::Color{139,145,131},.7f);}
            controls_.push_back({rect(x-24,y-24,48,48),"level:"+std::to_string(level),true});
        }
    }
}
void Ui::sidebar(){
    box(rect(0,90,234,size_.y-90),sf::Color{250,251,246},0);line({234,90},{234,size_.y},border);
    text("Levels",25,119,14,ink,true);
    circle({38,160},4,yellow,gold);text("Solved",49,154,11,muted);circle({111,160},4,sf::Color::White,sf::Color{193,198,189});text("Unsolved",122,154,11,muted);
    const float treeHeight=std::min(560.f,size_.y-280);
    levelNodes(rect(24,185,186,treeHeight));
    text(std::to_string(game_.progress().completed)+" of 30 solved",25,treeHeight+206,11,muted);
    paragraph("Solve each level to unlock the next.",25,treeHeight+240,190,11,muted);
}
void Ui::levelScreen(){
    text("Levels",size_.x/2,123,26,ink,true,true);
    text(std::to_string(game_.progress().completed)+" of 30 solved · Complete each level to unlock the next.",size_.x/2,166,14,muted,false,true);
    circle({size_.x/2-66,201},4,yellow,gold);text("Solved",size_.x/2-56,195,11,muted);
    circle({size_.x/2+20,201},4,sf::Color::White,sf::Color{193,198,189});text("Unsolved",size_.x/2+30,195,11,muted);
    levelNodes(rect(size_.x/2-280,220,560,std::min(580.f,size_.y-240)),5);
}
void Ui::dash(sf::FloatRect r){
    const float x=r.position.x+4,y=r.position.y+4,w=r.size.x-8,h=r.size.y-8;
    for(float k=6;k<w-6;k+=13){line({x+k,y},{x+std::min(k+7,w-6),y},gold,1.5f);line({x+k,y+h},{x+std::min(k+7,w-6),y+h},gold,1.5f);}
    for(float k=6;k<h-6;k+=13){line({x,y+k},{x,y+std::min(k+7,h-6)},gold,1.5f);line({x+w,y+k},{x+w,y+std::min(k+7,h-6)},gold,1.5f);}
}
void Ui::diagram(sf::FloatRect area){
    const auto& p=game_.puzzle();const auto middle=area.position+area.size/2.f;
    auto number=[&](Number value,bool missing,float x,float y,unsigned font=27){text(missing?"?":std::to_string(value),x,y-font*.48f,font,missing?gold:ink,false,true);};
    if(p.shape==Shape::Square||p.shape==Shape::Rectangle){
        const int cols=static_cast<int>(p.examples[0].inputs.size()+1);const float cell=std::min({136.f,(area.size.x-24-(cols-1)*10)/cols,(area.size.y-16-20)/3});
        const float cellH=p.shape==Shape::Square?cell:cell*.72f;
        const float left=middle.x-(cols*cell+(cols-1)*10)/2,top=middle.y-(3*cellH+20)/2;
        for(int row=0;row<3;++row)for(int col=0;col<cols;++col){const bool output=col==cols-1,missing=output&&row==2;const auto value=output?p.examples[row].output:p.examples[row].inputs[col];const auto r=rect(left+col*(cell+10),top+row*(cellH+10),cell,cellH);box(r,missing?pale:pastel[(row*cols+col)%12],14);if(missing)dash(r);number(value,missing,r.position.x+cell/2,r.position.y+cellH/2,cell<70?26:cell<100?32:38);}
        return;
    }
    if(p.examples[0].inputs.size()==1){
        const float radius=std::min(210.f,area.size.y*.46f);
        if(p.shape==Shape::Hexagon){
            for(int i=0;i<6;++i){const auto a=polar(middle,radius,-pi/2+i*pi/3),b=polar(middle,radius,-pi/2+(i+1)*pi/3);sf::ConvexShape wedge;wedge.setPointCount(3);wedge.setPoint(0,middle);wedge.setPoint(1,a);wedge.setPoint(2,b);wedge.setFillColor(i==5?pale:pastel[i]);wedge.setOutlineColor(sf::Color::White);wedge.setOutlineThickness(2);target_->draw(wedge);const auto pos=(middle+a+b)/3.f;const bool output=i%2;number(output?p.examples[i/2].output:p.examples[i/2].inputs[0],i==5,pos.x,pos.y-8,38);text(output?"b":"a",pos.x,pos.y+24,12,muted,false,true);}
        }else{
            for(int k=0;k<2;++k){sf::ConvexShape triangle;triangle.setPointCount(3);for(int i=0;i<3;++i)triangle.setPoint(i,polar(middle,radius,-pi/2+k*pi+i*2*pi/3));triangle.setFillColor(k?sf::Color{232,244,236,180}:sf::Color{255,246,209});triangle.setOutlineColor(sf::Color{209,215,200});triangle.setOutlineThickness(1.4f);target_->draw(triangle);}
            for(int i=0;i<6;++i){const auto pos=polar(middle,radius*.94f,-pi/2+i*pi/3);circle(pos,29,i==5?pale:pastel[i]);number(i%2?p.examples[i/2].output:p.examples[i/2].inputs[0],i==5,pos.x,pos.y-6,28);text(i%2?"b":"a",pos.x,pos.y+18,10,muted,false,true);}
        }
        return;
    }
    // Three completed/incomplete shape examples collectively form one puzzle.
    const int sectors=static_cast<int>(p.examples[0].inputs.size()+1);
    const float groupWidth=area.size.x/3,radius=std::min({132.f,groupWidth*.42f,area.size.y*.44f});
    for(int example=0;example<3;++example){
        const sf::Vector2f centre{area.position.x+groupWidth*(example+.5f),middle.y+2};
        if(p.shape==Shape::Circle){
            for(int sector=0;sector<sectors;++sector){sf::ConvexShape wedge;wedge.setPointCount(22);wedge.setPoint(0,centre);const float start=-pi/2+sector*2*pi/sectors;for(int i=0;i<21;++i)wedge.setPoint(i+1,polar(centre,radius,start+i*2*pi/sectors/20));const bool output=sector==sectors-1,missing=output&&example==2;wedge.setFillColor(missing?pale:pastel[(example*sectors+sector)%12]);wedge.setOutlineColor(sf::Color::White);wedge.setOutlineThickness(2);target_->draw(wedge);const auto pos=polar(centre,radius*.62f,start+pi/sectors);number(output?p.examples[example].output:p.examples[example].inputs[sector],missing,pos.x,pos.y+1,sectors==5?25:29);text(std::string(1,static_cast<char>('a'+sector)),pos.x,pos.y-21,11,muted,false,true);}
        }else{
            for(int k=0;k<2;++k){sf::ConvexShape triangle;triangle.setPointCount(3);for(int i=0;i<3;++i)triangle.setPoint(i,polar(centre,radius,-pi/2+k*pi+i*2*pi/3));triangle.setFillColor(k?sf::Color{234,246,240,150}:sf::Color{255,246,215});triangle.setOutlineColor(sf::Color{216,221,207});triangle.setOutlineThickness(1);target_->draw(triangle);}
            for(int sector=0;sector<3;++sector){const auto pos=polar(centre,radius*.8f,-pi/2+sector*2*pi/3);const bool output=sector==2,missing=output&&example==2;circle(pos,26,missing?pale:pastel[example*3+sector]);number(output?p.examples[example].output:p.examples[example].inputs[sector],missing,pos.x,pos.y-6,26);text(std::string(1,static_cast<char>('a'+sector)),pos.x,pos.y+13,10,muted,false,true);}
        }
    }
}
void Ui::puzzleScreen(){
    const float left=28.f,width=size_.x-left-28,centre=left+width/2;
    const auto& p=game_.puzzle();const bool solved=game_.progress().solved(p.level);
    const float headingY=119-scroll_;
    const auto label="LEVEL "+(p.level<10?std::string("0"):std::string())+std::to_string(p.level)+" / 30";
    text(label,centre,headingY,12,gold,true,true);text("Find the missing number.",centre,headingY+29,30,ink,true,true);text("One empty space. One pattern to discover.",centre,headingY+72,14,muted,false,true);
    const float panelW=std::min(1100.f,width-20),panelH=std::clamp(size_.y*.46f,280.f,480.f),top=217-scroll_;
    const auto panel=rect(centre-panelW/2,top,panelW,panelH);
    box(panel,sf::Color{253,254,251},22,border,1);
    text("NUMBER PATTERN",panel.position.x+23,top+25,11,sf::Color{137,143,126});
    const bool exhausted=game_.hintsLeft()==0,visible=game_.hintVisible();
    const float hintW=130.f,hintX=panel.position.x+panelW-hintW-19;
    const std::string hintLabel=solved?"Level solved":!visible&&game_.hintsUsed()>0?(game_.hintsUsed()==1?"View hint":"View hints"):exhausted?(game_.hintsUsed()==1?"View hint":"View hints"):game_.hintsUsed()>0?"Next hint":"Hint";
    button(rect(hintX,top+13,hintW,42),hintLabel,"hint",!solved,false);
    if(!solved)text(std::to_string(game_.hintsUsed())+" / "+std::to_string(game_.hintBudget())+" used",hintX-91,top+30,12,sf::Color{144,132,93});
    diagram(rect(panel.position.x+22,top+67,panelW-44,panelH-83));
    const float answerY=top+panelH+34;
    text("Your answer",centre,answerY-22,13,muted,false,true);
    const auto input=rect(centre-209,answerY,245,48);
    box(input,sf::Color::White,12,focus_=="answer"&&!solved?gold:sf::Color{213,220,203},focus_=="answer"&&!solved?1.8f:1);
    const auto displayed=solved?std::to_string(p.answer()):game_.answer;
    text(displayed.empty()?"Enter a number":displayed,input.position.x+15,answerY+15,16,displayed.empty()?muted:ink);
    if(focus_=="answer"&&!solved&&(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()/500)%2==0){sf::Text measure(regular_,utf8(game_.answer),16);const float x=input.position.x+15+measure.getLocalBounds().size.x;line({x,answerY+13},{x,answerY+34},gold);}
    controls_.push_back({input,"answer",!solved});
    button(rect(centre+45,answerY,164,48),"Submit answer →","submit",!solved);
    text("Enter to submit · Backspace to edit",centre,answerY+66,12,sf::Color{145,152,134},false,true);
    text(game_.feedback,centre,answerY+96,14,solved?sf::Color{65,132,87}:game_.feedbackError?sf::Color{174,71,60}:muted,false,true);
    float bottom=answerY+157;
    if(solved){
        const float sy=answerY+131;
        box(rect(centre-panelW/2,sy,panelW,115),sf::Color{240,248,239},15);
        text("The pattern, explained",centre-panelW/2+18,sy+14,13,sf::Color{63,105,66},true);
        paragraph(p.explanation,centre-panelW/2+18,sy+39,panelW-36,12,sf::Color{78,116,80});
        text(substitution(p,p.examples.back()),centre-panelW/2+18,sy+80,14,sf::Color{55,94,61});
        bottom=sy+137;
        if(p.level<30){button(rect(centre-80,bottom,160,44),"Next level →","next");bottom+=60;}
    }else{lock(centre-167,answerY+133,sf::Color{146,154,135},.8f);text("Solve this level to unlock the next.",centre+6,answerY+136,11,sf::Color{146,154,135},false,true);}
    if(game_.saveStatus!="Progress saved")paragraph(game_.saveStatus,left+10,bottom,width-20,11,sf::Color{170,75,56});
    maxScroll_=std::max(0.f,bottom+scroll_+24-size_.y);scroll_=std::min(scroll_,maxScroll_);
    if(game_.hintVisible()){
        const float cardW=std::min(420.f,panelW-32),hintX=panel.position.x+panelW-cardW-16;
        float clueBottom=48;
        for(int i=0;i<game_.hintsUsed();++i)clueBottom=paragraph(i==0?p.hint:p.extraHints.at(i-1),hintX+18,clueBottom,cardW-36,16,sf::Color{85,75,33},false)+12;
        const float cardH=std::max(132.f,clueBottom+4),hintY=std::clamp(top+66,106.f,std::max(106.f,size_.y-cardH-20));
        const auto hintRect=rect(hintX,hintY,cardW,cardH);hintCard_=hintRect;
        box(hintRect,sf::Color{255,249,224},14,sf::Color{235,214,151});
        text("PATTERN HELP",hintX+18,hintY+17,12,gold,true);
        button(rect(hintX+cardW-42,hintY+9,30,30),"×","close-hint",true,false);
        float clueY=hintY+48;
        for(int i=0;i<game_.hintsUsed();++i)clueY=paragraph(i==0?p.hint:p.extraHints.at(i-1),hintX+18,clueY,cardW-36,16,sf::Color{85,75,33})+12;
    }
}
void Ui::popup(){
    controls_.clear();box(rect(0,0,size_.x,size_.y),sf::Color{24,29,23,65},0);
    const float x=size_.x/2-190,y=std::max(124.f,size_.y/2-140);
    box(rect(x,y,380,250),sf::Color::White,20);
    box(rect(x+168,y+24,44,44),pale,12);lock(x+183,y+35,gold,1.5f);
    const auto n=*game_.lockedLevel;
    text("Level "+std::to_string(n)+" is locked",size_.x/2,y+87,20,ink,true,true);
    text("Complete Level "+std::to_string(n-1)+" to unlock this level.",size_.x/2,y+130,12,muted,false,true);
    button(rect(size_.x/2-56,y+170,112,44),"Got it","dismiss");
}
void Ui::draw(sf::RenderTarget& target,sf::Vector2u size){
    if(size!=pixelSize_){scroll_=0;pixelSize_=size;}
    mouse_*=scale_;
    scale_=std::clamp(std::min(size.x/1200.f,size.y/860.f),1.f,2.f);
    target_=&target;size_=sf::Vector2f(size)/scale_;mouse_/=scale_;controls_.clear();hintCard_.reset();target_->setView(sf::View(rect(0,0,size_.x,size_.y)));target_->clear(sf::Color::White);
    switch(screen_){case Screen::Home:homeScreen();break;case Screen::Puzzle:puzzleScreen();break;case Screen::Levels:levelScreen();break;case Screen::Score:scoreScreen();break;}
    header();
    if(game_.lockedLevel)popup();
}
void Ui::activate(const std::string& id){
    if(id=="dismiss"){game_.lockedLevel.reset();focus_=screen_==Screen::Levels?"levels":"answer";return;}
    if(game_.lockedLevel)return;
    if(id=="sound"){audio_.toggle();audio_.play(Audio::Cue::Click);return;}
    if(id!="hint"&&id!="submit"&&id!="play")audio_.play(Audio::Cue::Click);
    if(id=="home"){screen_=Screen::Home;focus_="play";scroll_=0;return;}
    if(id=="score"){screen_=Screen::Score;focus_="play";return;}
    if(id=="levels"){
        if(screen_==Screen::Levels)screen_=levelsBack_;
        else {levelsBack_=screen_;screen_=Screen::Levels;}
        focus_=screen_==Screen::Puzzle?"answer":"levels";return;
    }
    if(id=="play"){
        int level=game_.progress().current;
        if(game_.progress().solved(level))level=game_.progress().completed==30?1:game_.progress().completed+1;
        if(game_.select(level)){audio_.play(Audio::Cue::Start);screen_=Screen::Puzzle;scroll_=0;focus_="answer";}return;
    }
    if(id=="answer"){if(screen_==Screen::Puzzle)focus_="answer";return;}
    if(screen_!=Screen::Puzzle&&id.rfind("level:",0)!=0)return;
    if(id=="close-hint"){game_.hideHint();focus_="answer";return;}
    if(id=="hint"){
        const bool opened=!game_.hintVisible()&&game_.hintsUsed()>0?game_.reviewHint():game_.hintsLeft()>0?game_.hint():game_.reviewHint();
        if(opened)audio_.play(Audio::Cue::Hint);
        focus_="answer";return;
    }
    if(id=="submit"){const bool correct=game_.submit();if(correct||game_.feedbackError)audio_.play(correct?(game_.progress().completed==30?Audio::Cue::Complete:Audio::Cue::Correct):Audio::Cue::Wrong);return;}
    if(id=="next"){if(game_.progress().solved(game_.progress().current)&&game_.select(game_.progress().current+1)){scroll_=0;focus_="answer";}return;}
    if(id.rfind("level:",0)==0){const int n=std::stoi(id.substr(6));if(game_.select(n)){screen_=Screen::Puzzle;scroll_=0;focus_="answer";}else focus_="dismiss";}
}
void Ui::event(const sf::Event& e,sf::Vector2f mouse){
    mouse_=mouse/scale_;
    if(const auto* click=e.getIf<sf::Event::MouseButtonPressed>();click&&click->button==sf::Mouse::Button::Left){
        if(hintCard_&&hintCard_->contains(mouse_)&&!game_.lockedLevel){
            for(const auto& control:controls_)if(control.id=="close-hint"&&control.rect.contains(mouse_)){activate(control.id);break;}
            return;
        }
        for(auto i=controls_.rbegin();i!=controls_.rend();++i)if(i->enabled&&i->rect.contains(mouse_)){if(mouse_.y<90&&i->id!="levels"&&i->id!="home"&&i->id!="score"&&i->id!="sound"&&i->id!="dismiss")continue;const auto id=i->id;focus_=id;activate(id);break;}
    }
    if(const auto* wheel=e.getIf<sf::Event::MouseWheelScrolled>();wheel&&screen_==Screen::Puzzle&&!game_.lockedLevel)scroll_=std::clamp(scroll_-wheel->delta*40.f,0.f,maxScroll_);
    if(const auto* key=e.getIf<sf::Event::KeyPressed>()){
        if(key->code==sf::Keyboard::Key::Escape){if(game_.lockedLevel)activate("dismiss");else if(screen_==Screen::Puzzle&&game_.hintVisible())activate("close-hint");else if(screen_==Screen::Levels)activate("levels");else activate("home");}
        else if(key->code==sf::Keyboard::Key::Tab){
            std::vector<const Control*> available;for(const auto& control:controls_)if(control.enabled)available.push_back(&control);
            auto found=std::find_if(available.begin(),available.end(),[&](const Control* c){return c->id==focus_;});
            if(!available.empty()){int index=found==available.end()?0:static_cast<int>(std::distance(available.begin(),found))+(key->shift?-1:1);index=(index+static_cast<int>(available.size()))%static_cast<int>(available.size());focus_=available[index]->id;
                if(screen_==Screen::Puzzle && focus_!="levels" && focus_!="home" && focus_!="score" && focus_!="sound" && focus_.rfind("level:",0)!=0){
                    const auto& r=available[index]->rect;
                    if(r.position.y<110)scroll_=std::max(0.f,scroll_-(110-r.position.y));
                    else if(r.position.y+r.size.y>size_.y-20)scroll_=std::min(maxScroll_,scroll_+r.position.y+r.size.y-(size_.y-20));
                }}
        }else if(key->code==sf::Keyboard::Key::Enter){if(game_.lockedLevel)activate("dismiss");else if(focus_=="answer"&&screen_==Screen::Puzzle)activate("submit");else{const auto found=std::find_if(controls_.begin(),controls_.end(),[&](const Control& c){return c.id==focus_&&c.enabled;});if(found!=controls_.end())activate(found->id);}}
        else if(key->code==sf::Keyboard::Key::Backspace&&screen_==Screen::Puzzle&&!game_.lockedLevel&&focus_=="answer")game_.backspace();
    }
    if(const auto* entered=e.getIf<sf::Event::TextEntered>();entered&&screen_==Screen::Puzzle&&!game_.lockedLevel&&focus_=="answer"&&entered->unicode<128)game_.input(static_cast<char>(entered->unicode));
}
}
