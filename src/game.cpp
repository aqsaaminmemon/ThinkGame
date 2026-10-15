#include "thinkgame/game.hpp"
#include <charconv>
#include <utility>

namespace thinkgame {
Game::Game(SaveStore store) : store_(std::move(store)) {
    std::string notice;
    progress_ = store_.load(notice);
    feedback = notice.empty() ? "Take your time. Look for the connection." : notice;
    if (!notice.empty()) saveStatus = notice;
    else persist();
}
void Game::persist() {
    std::string error;
    saveStatus = store_.save(progress_, error) ? "Progress saved" : "Could not save: " + error;
}
bool Game::select(int level) {
    if (level < 1 || level > 30) return false;
    if (!progress_.unlocked(level)) { lockedLevel = level; return false; }
    progress_.current = level;
    answer.clear();
    feedback = progress_.solved(level) ? "Level completed. Here is the pattern." : "Take your time. Look for the connection.";
    feedbackError = false;
    hintOpen_=false;
    lockedLevel.reset();
    persist();
    return true;
}
void Game::input(char c) {
    if (progress_.solved(progress_.current) || answer.size() >= 12) return;
    if ((c >= '0' && c <= '9') || (c == '-' && answer.empty())) answer.push_back(c);
}
void Game::backspace() { if (!progress_.solved(progress_.current) && !answer.empty()) answer.pop_back(); }
bool Game::submit() {
    if (progress_.solved(progress_.current) || lockedLevel) return false;
    Number entered{};
    const auto parsed = std::from_chars(answer.data(), answer.data() + answer.size(), entered);
    if (answer.empty() || parsed.ec != std::errc() || parsed.ptr != answer.data() + answer.size()) {
        feedback = "Enter a whole number, then submit.";
        feedbackError = true; return false;
    }
    if (entered != puzzle().answer()) {
        feedback = "Not quite. Look again and try another number.";
        feedbackError = true; return false;
    }
    // The model, not the UI, enforces sequential completion.
    if (progress_.current != progress_.completed + 1) return false;
    ++progress_.completed;
    hintOpen_=false;
    feedbackError = false;
    feedback = progress_.completed == 30 ? "You did it! All 30 levels are complete." :
        "Correct! Level " + std::to_string(progress_.completed + 1) + " is now unlocked.";
    persist();
    return true;
}
bool Game::hint() {
    if (lockedLevel || progress_.solved(progress_.current) || hintsLeft() == 0) return false;
    ++progress_.hints[progress_.current - 1];
    hintOpen_=true;
    // Save newly unlocked clues immediately. Reopening never grants extra clues.
    persist();
    return true;
}
bool Game::reviewHint() {
    if(lockedLevel||progress_.solved(progress_.current)||hintsUsed()==0)return false;
    hintOpen_=true;
    return true;
}
}
