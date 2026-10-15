#include "thinkgame/game.hpp"
#include "thinkgame/assets.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace thinkgame;
void check(bool condition,const char* message){if(!condition)throw std::runtime_error(message);}
int main(){try{
    validateCatalogue();
    for(const auto& puzzle:puzzles()){
        check(!puzzle.hint.empty()&&puzzle.hint.find_first_of("=+*/^%0123456789")==std::string::npos,"Hints must use words without formulas or numeric answers");
        for(const auto& clue:puzzle.extraHints)check(!clue.empty()&&clue.find_first_of("=+*/^%0123456789")==std::string::npos,"Extra clues must use words without formulas or numeric answers");
    }
    const std::array<Number,30> answers={17,25,9,20,18,11,42,16,2,36,28,13,22,120,52,81,75,9,210,27,29,5,49,42,16,30,10,27,74,4};
    for(int n=1;n<=30;++n)check(puzzles()[n-1].answer()==answers[n-1],"Independent authored answer mismatch");
    // Behavioural equivalence screening: distinct same-arity rules must diverge.
    for(int first=1;first<=30;++first)for(int second=first+1;second<=30;++second){
        auto arity=puzzles()[first-1].examples[0].inputs.size();
        if(arity!=puzzles()[second-1].examples[0].inputs.size())continue;
        bool different=false;int jointlyValid=0;
        for(int n=2;n<=99&&!different;++n){
            std::vector<Number> v(arity);
            for(std::size_t j=0;j<arity;++j)v[j]=(n+static_cast<int>(j)*7)%97+2;
            if(arity==1)v[0]=n;
            try{const auto x=evaluate(first,v),y=evaluate(second,v);++jointlyValid;different=x!=y;}catch(const std::domain_error&){}
        }
        check(different||jointlyValid==0,"Potential equivalent rules require review");
    }
    auto directory=std::filesystem::temp_directory_path()/"thinkgame-core-tests";
    std::filesystem::remove_all(directory);
    const auto install = directory / "install";
    const auto installedAssets = install / "share/thinkgame/assets";
    std::filesystem::create_directories(install / "bin");
    std::filesystem::create_directories(installedAssets);
    std::ofstream(install / "bin/thinkgame") << "test executable";
    for (const auto* name : {"DejaVuSans.ttf", "DejaVuSans-Bold.ttf", "logo.png"})
        std::ofstream(installedAssets / name) << "test asset";
    check(findAssets("thinkgame", directory / "missing", (install / "bin").string()) ==
          install / "bin/../share/thinkgame/assets", "PATH launch did not locate installed assets");
    std::filesystem::create_directories(install / "bin/assets");
    std::ofstream(install / "bin/assets/DejaVuSans.ttf") << "partial bundle";
    check(findAssets(install / "bin/thinkgame", directory / "missing", "") ==
          install / "bin/../share/thinkgame/assets", "Partial asset bundle hid valid installed assets");
#ifndef _WIN32
    std::filesystem::create_symlink(install / "bin/thinkgame", directory / "game-link");
    check(findAssets(directory / "game-link", directory / "missing", "") ==
          install / "bin/../share/thinkgame/assets", "Symlink launch did not locate installed assets");
#endif
    std::filesystem::remove_all(install);
    std::filesystem::remove(directory / "game-link");
    SaveStore store(directory/"progress.save");
    Game g(store);
    check(g.score()==0,"Initial score must be zero");
    check(g.progress().unlocked(1)&&!g.progress().unlocked(2),"Initial locks");
    check(!g.select(30)&&g.progress().current==1&&g.lockedLevel==30,"Locked selection changed level");
    g.lockedLevel.reset();
    check(!g.submit()&&g.feedbackError,"Empty input accepted");
    g.answer="-";check(!g.submit(),"Minus alone accepted");
    g.answer="17x";check(!g.submit(),"Trailing junk accepted");
    g.answer="99999999999999999999999";check(!g.submit(),"Overflow accepted");
    g.answer="18";check(!g.submit()&&!g.progress().unlocked(2),"Wrong answer unlocked next");
    g.answer.clear();g.input('1');g.input('8');g.backspace();g.input('7');check(g.answer=="17","Keyboard edit failed");
    check(g.hintBudget()==1&&g.hint()&&g.hintsLeft()==0&&g.hintVisible(),"Level one hint budget failed");
    check(!g.hint()&&g.hintVisible(),"Extra hint accepted or persistent hint disappeared");
    Game reopened(store);check(reopened.hintsLeft()==0&&!reopened.hintVisible(),"Hint quota reset on reopening");
    check(reopened.reviewHint()&&reopened.hintVisible()&&reopened.hintsUsed()==1,"Saved clue could not be viewed");
    reopened.hideHint();check(!reopened.hintVisible(),"Close hint failed");
    check(reopened.reviewHint()&&reopened.hintsLeft()==0,"Viewing unlocked clues changed quota");
    check(g.submit()&&g.progress().completed==1&&g.progress().unlocked(2),"Correct answer not applied");
    check(!g.submit()&&g.progress().completed==1,"Repeated submission advanced");
    check(!g.hint(),"Hint on completed level");
    check(!g.reviewHint(),"Review on completed level");
    Game restored(store);check(restored.progress().completed==1,"Completion not restored");
    check(restored.score()==100,"Score not restored from completed levels");
    for(int level=2;level<=30;++level){check(restored.select(level),"Sequential selection blocked");restored.answer=std::to_string(answers[level-1]);check(restored.submit(),"Sequential answer blocked");}
    check(restored.progress().completed==30&&restored.feedback.find("All 30")!=std::string::npos,"Final completion missing");
    check(restored.select(1)&&restored.hintsLeft()==0&&restored.progress().completed==30,"Replay reset progress");
    check(restored.score()==3000&&!restored.submit()&&restored.score()==3000,"Replay awarded duplicate points");
    std::ofstream(store.path(),std::ios::trunc)<<"corrupt save";
    std::string notice;auto backup=store.load(notice);check(backup.completed==30&&!notice.empty(),"Backup not recovered");
    std::filesystem::remove(store.path().string()+".bak");auto fresh=store.load(notice);check(fresh.completed==0&&fresh.current==1,"Corruption not handled");
    Progress invalid;invalid.completed=31;std::string error;check(!store.save(invalid,error),"Invalid save accepted");
    SaveStore blocked(directory);check(!blocked.save(Progress{},error),"Save failure not reported");
    try{evaluate(30,{2,3,2,4});check(false,"Zero denominator accepted");}catch(const std::domain_error&){}
    check(store.save(Progress{},error),"Fresh save failed");notice="old notice";store.load(notice);check(notice.empty(),"Valid load retained an old notice");
    // Check every level's allowance, including the tier boundaries and persisted exhaustion.
    const std::array<int,30> budgets={1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3};
    for(int level=1;level<=30;++level){
        SaveStore tierStore(directory/("tier-"+std::to_string(level)+".save"));
        Progress tier;tier.completed=level-1;tier.current=level;
        check(tierStore.save(tier,error),"Tier fixture save failed");Game tierGame(tierStore);
        check(tierGame.hintBudget()==budgets[level-1]&&!tierGame.reviewHint(),"Wrong tier or unseen clue opened");
        for(int clue=0;clue<budgets[level-1];++clue){
            check(tierGame.hint()&&tierGame.hintsUsed()==clue+1&&tierGame.hintVisible(),"Progressive clue not unlocked");
        }
        check(!tierGame.hint()&&tierGame.hintsLeft()==0,"Tier limit bypassed");
        Game tierReload(tierStore);check(tierReload.hintsLeft()==0&&tierReload.reviewHint(),"Tier limit or clues not restored");
        tierReload.answer=std::to_string(puzzles()[level-1].answer());check(tierReload.submit()&&!tierReload.hintVisible(),"Solving did not close hints");
    }
    Progress legacy;legacy.hints[0]=3;check(store.save(legacy,error),"Legacy fixture failed");Game legacyGame(store);
    check(legacyGame.hintsUsed()==1&&legacyGame.hintsLeft()==0&&legacyGame.reviewHint(),"Older three-use save was not safely capped");
    std::filesystem::remove_all(directory);
    std::cout<<"PASS: 90 examples, 30 independent answers, rule equivalence screening, all progression, input, persistent progressive hints, all tier boundaries, legacy quota compatibility, reload, backups, corruption, save failure, and installed asset discovery.\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
