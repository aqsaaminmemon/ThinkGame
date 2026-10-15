#pragma once
#include "thinkgame/progress.hpp"
#include "thinkgame/puzzle.hpp"
#include <optional>
#include <algorithm>

namespace thinkgame {
class Game {
public:
    explicit Game(SaveStore store);
    const Progress& progress() const { return progress_; }
    int score() const { return progress_.completed * 100; }
    const Puzzle& puzzle() const { return puzzles().at(progress_.current - 1); }
    bool select(int level);
    bool submit();
    bool hint();
    bool reviewHint();
    void hideHint() { hintOpen_=false; }
    bool hintVisible() const { return hintOpen_; }
    int hintBudget() const { return hintLimit(progress_.current); }
    int hintsUsed() const { return std::min(hintBudget(),progress_.hints[progress_.current-1]); }
    int hintsLeft() const { return hintBudget()-hintsUsed(); }
    void input(char c);
    void backspace();
    void persist();
    std::string answer, feedback, saveStatus;
    bool feedbackError = false;
    std::optional<int> lockedLevel;
private:
    Progress progress_;
    SaveStore store_;
    bool hintOpen_=false;
};
}
