#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace thinkgame {
using Number = std::int64_t;
enum class Shape { Square, Hexagon, Rectangle, Star, Circle };
struct Example { std::vector<Number> inputs; Number output; };
struct Puzzle {
    int level;
    Shape shape;
    std::string title, formula, variables, explanation, difficulty, distinction;
    std::vector<Example> examples;
    std::string hint;
    std::vector<std::string> extraHints;
    Number answer() const { return examples.back().output; }
};
const std::vector<Puzzle>& puzzles();
Number evaluate(int level, const std::vector<Number>& inputs);
std::string shapeName(Shape shape);
std::string substitution(const Puzzle& puzzle, const Example& example);
void validateCatalogue();
int hintLimit(int level);
}
