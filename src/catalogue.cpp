#include "thinkgame/puzzle.hpp"
#include <iostream>
int main() {
    thinkgame::validateCatalogue();
    std::cout << "# ThinkGame: all 30 authored levels\n\nEvery level is one puzzle with two completed examples and one incomplete example. Only the final output is hidden. The rules are different; the progression is authored and still benefits from playtesting. Shape reuse never counts as rule variety.\n\n";
    for(const auto& p:thinkgame::puzzles()) {
        std::cout << "## Level " << p.level << " - " << p.title << " (" << thinkgame::shapeName(p.shape) << ")\n\n"
                  << "- Rule: `" << p.formula << "`\n- Positions: " << p.variables << "\n- Explanation: " << p.explanation
                  << "\n- Hint allowance: " << thinkgame::hintLimit(p.level) << "\n- Hint in words: " << p.hint;
        for(std::size_t i=0;i<p.extraHints.size();++i)std::cout<<"\n- Hint "<<i+2<<" in words: "<<p.extraHints[i];
        std::cout << "\n- Difficulty rationale: " << p.difficulty << "\n- Distinct relationship: " << p.distinction << "\n- Examples:\n";
        for(std::size_t i=0;i<p.examples.size();++i)std::cout<<"  - `"<<thinkgame::substitution(p,p.examples[i])<<"`"<<(i==2?" (output hidden in game)":"")<<"\n";
        std::cout << "- Missing position: the result of the third example.\n- Answer: **" << p.answer() << "**\n\n";
    }
}
