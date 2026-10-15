#include "thinkgame/puzzle.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <regex>
#include <set>
#include <stdexcept>

namespace thinkgame {
int hintLimit(int level){if(level<1||level>30)throw std::out_of_range("Unknown level");return (level-1)/10+1;}
namespace {
Number divide(Number a, Number b) { if (!b || a % b) throw std::domain_error("Division must be exact and nonzero"); return a / b; }
Number power(Number a, Number b) { if (b < 0 || b > 8) throw std::domain_error("Exponent outside supported range"); Number result=1; while(b--) result*=a; return result; }
Number factorial(Number a) { if (a<0 || a>8) throw std::domain_error("Factorial outside supported range"); Number result=1; for(Number i=2;i<=a;++i)result*=i; return result; }
Number tens(Number a) { if(a<10||a>99)throw std::domain_error("Expected a two-digit number");return a/10; }
Number ones(Number a) { tens(a); return a%10; }
}
Number evaluate(int level, const std::vector<Number>& v) {
    const auto a=v.at(0), b=v.size()>1?v[1]:0, c=v.size()>2?v[2]:0, d=v.size()>3?v[3]:0;
    for(Number n:v)if(n<0||n>100)throw std::domain_error("Puzzle input outside safe range");
    switch(level) {
    case 1:return 2*a+b;
    case 2:return a*a;
    case 3:return a+b-c;
    case 4:return 3*a-1;
    case 5:return a*b;
    case 6:return divide(a+b,2);
    case 7:return a*(a+1);
    case 8:return (a+b)*c;
    case 9:return a%3;
    case 10:return (a-b)*(a-b);
    case 11:return a*a-b;
    case 12:return tens(a)+ones(a);
    case 13:return a*b-c;
    case 14:return factorial(a);
    case 15:return a*a+b*b;
    case 16:return (a+b)*(a+b);
    case 17:return 10*ones(a)+tens(a);
    case 18:return divide(a,b)+c;
    case 19:return a*(a-1)*(a-2);
    case 20:return divide(a*a,b);
    case 21:return a*b+a+b;
    case 22:return std::abs(tens(a)-ones(a));
    case 23:return a*b+c*c;
    case 24:return tens(a)*ones(a);
    case 25:return divide(a,b)+c*d;
    case 26:if(!a||!b)throw std::domain_error("LCM expects positive inputs");return std::lcm(a,b);
    case 27:return ones(a)*ones(a)+tens(a);
    case 28:return (a-b)*c+a;
    case 29:return power(a,b)-a-b;
    case 30:return divide(a*b-c*d,a-c);
    default:throw std::out_of_range("Unknown level");
    }
}
std::string shapeName(Shape shape) {
    switch(shape){case Shape::Square:return "Square";case Shape::Rectangle:return "Rectangle";case Shape::Hexagon:return "Hexagon";case Shape::Star:return "Star";case Shape::Circle:return "Circle";}
    return "";
}
const std::vector<Puzzle>& puzzles() {
    static const std::vector<Puzzle> data={
      {1,Shape::Square,"Double, then add","c = 2*a + b","a = left; b = middle; c = right, in each row.","Double the left number and add the middle number.","Two small operations, demonstrated across rows.","A weighted sum of two separate inputs.",{{{3,2},8},{{5,4},14},{{7,3},17}},"Try using the left number twice, then bring in the middle number.",{}},
      {2,Shape::Hexagon,"Square pairs","b = a^2","Each clockwise a-b pair follows the same rule.","Square each a to obtain its paired b.","Recognize paired positions and a power.","A single input squared, without another operation.",{{{2},4},{{4},16},{{5},25}},"Look at neighbouring pairs. What happens when a number is multiplied by itself?",{}},
      {3,Shape::Rectangle,"Balance the row","d = a + b - c","Read each row: a, b, c, d from left to right.","Add the first two numbers, then subtract the third.","Track three inputs instead of two.","An additive balance involving three independent inputs.",{{{8,5,3},10},{{9,7,4},12},{{6,8,5},9}},"The first two numbers work together; the third takes something away.",{}},
      {4,Shape::Star,"Threefold pairs","b = 3*a - 1","Clockwise pairs around the star are a, then b.","Triple each a, then subtract one.","Find adjacent pairs around a non-grid shape.","A single-input affine relationship, unlike the two-input weighted sum.",{{{2},5},{{4},11},{{7},20}},"Look at neighbouring pairs. Try three copies of the first number, then a small adjustment.",{}},
      {5,Shape::Circle,"Sector products","c = a*b","In each circle, a and b are inputs; c is the result.","Multiply a by b in each circle.","Infer multiplication from sector positions.","A pure product of two independent inputs.",{{{3,4},12},{{5,2},10},{{6,3},18}},"Think about equal groups. How could the two inputs combine by multiplication?",{}},
      {6,Shape::Square,"Meet in the middle","c = (a + b)/2","a = left; b = middle; c = right, in each row.","Find the average of the first two numbers.","Infer averaging, including an exact division.","A mean, distinct from both a product and a weighted sum.",{{{4,8},6},{{6,10},8},{{8,14},11}},"The result sits halfway between the first two numbers.",{}},
      {7,Shape::Hexagon,"Neighbour products","b = a*(a + 1)","Each clockwise a-b pair follows the same rule.","Multiply a by the next whole number.","Create an intermediate value from the input.","A product of consecutive integers.",{{{2},6},{{4},20},{{6},42}},"Try multiplying the first number by its next-door neighbour on the number line.",{}},
      {8,Shape::Rectangle,"Group before multiplying","d = (a + b)*c","Read each row: a, b, c, d from left to right.","Add a and b first, then multiply their sum by c.","Parentheses change which operation comes first.","A sum multiplied by a third independent input.",{{{2,3,4},20},{{4,2,3},18},{{3,5,2},16}},"Combine the first two numbers before letting the third make the result bigger.",{}},
      {9,Shape::Star,"What remains","b = a % 3","% means remainder after division; each a-b pair is independent.","Divide a by three and take the remainder as b.","Recognize a cyclic remainder relationship rather than growth.","Modular arithmetic, distinct from powers, products, and digit operations.",{{{7},1},{{11},2},{{8},2}},"Think about groups of three. What is left over after making complete groups?",{}},
      {10,Shape::Circle,"Distance squared","c = (a - b)^2","In each circle, a and b are inputs; c is the result.","Subtract b from a, then square the difference.","A difference creates an intermediate value before a power.","The square of a difference, including its cross term.",{{{7,4},9},{{9,5},16},{{8,2},36}},"Find the gap between the two inputs, then multiply that gap by itself.",{}},
      {11,Shape::Square,"Square and subtract","c = a^2 - b","a = left; b = middle; c = right, in each row.","Square the left number, then subtract the middle number.","Apply a power to only one of two inputs.","One squared operand followed by subtraction.",{{{4,3},13},{{5,7},18},{{6,8},28}},"Follow each row from left to right. The left number does more work than the middle one.",{"Multiply the left number by itself, then subtract the middle number from that result."}},
      {12,Shape::Hexagon,"Inside the digits","b = tens(a) + ones(a)","a is a two-digit number; b is its paired result.","Add the tens digit and the ones digit of a.","Change from whole-number operations to digit decomposition.","A digit sum, not an operation on the complete number.",{{{24},6},{{53},8},{{67},13}},"Try treating each digit as a separate number rather than using the whole number.",{"Split the first number into its tens and ones digits, then add those digits together."}},
      {13,Shape::Rectangle,"Product with a deduction","d = a*b - c","Read each row: a, b, c, d from left to right.","Multiply the first two numbers, then subtract the third.","Choose the correct operation order across three inputs.","A two-input product reduced by a third input.",{{{4,5,3},17},{{6,3,4},14},{{7,4,6},22}},"Look for two operations in each row. The third number reduces the result.",{"Multiply the first and second numbers, then subtract the third number."}},
      {14,Shape::Star,"Factorial pairs","b = a!","a! means 1*2*...*a; each a-b pair is independent.","Multiply all whole numbers from one through a.","Recognize a variable-length multiplication chain.","Factorial, whose number of factors depends on a.",{{{3},6},{{4},24},{{5},120}},"Large jumps between paired numbers can come from a growing chain of multiplication.",{"Multiply every whole number from one up to the input. The chain grows with the input."}},
      {15,Shape::Circle,"Two squares together","c = a^2 + b^2","In each circle, a and b are inputs; c is the result.","Square a and b separately, then add the squares.","Two independent powers must be combined.","A sum of squares, with no cross term.",{{{3,4},25},{{2,5},29},{{4,6},52}},"Work on each input separately before combining what you get.",{"Multiply each input by itself first. Add those two results together."}},
      {16,Shape::Square,"Square the whole sum","c = (a + b)^2","a = left; b = middle; c = right, in each row.","Add the first two numbers, then square the entire sum.","Distinguish grouping from separately squaring inputs.","Square of a sum; different from the sum of squares in Level 15.",{{{2,3},25},{{3,4},49},{{4,5},81}},"Combine the two inputs before repeating that total in a multiplication.",{"Add the two inputs together, then multiply that total by itself."}},
      {17,Shape::Hexagon,"Turn the digits around","b = 10*ones(a) + tens(a)","a is a two-digit number; b is its paired result.","Reverse the two digits of a.","Recognize a positional digit transformation.","Digit reversal, rather than digit arithmetic.",{{{24},42},{{36},63},{{57},75}},"The digits stay the same, but their places change.",{"Swap the tens and ones digits. The last digit becomes the first."}},
      {18,Shape::Rectangle,"Divide before adding","d = a/b + c","Read each row: a, b, c, d from left to right.","Divide a exactly by b, then add c.","Recover an exact quotient before combining another input.","A quotient followed by addition.",{{{12,3,2},6},{{20,4,3},8},{{24,6,5},9}},"Work from left to right. Think about equal groups first, then an extra amount.",{"Divide the first number by the second, then add the third number."}},
      {19,Shape::Star,"Three descending factors","b = a*(a - 1)*(a - 2)","Clockwise pairs around the star are a, then b.","Multiply a and the two whole numbers immediately below it.","Build and combine two intermediate factors.","Exactly three descending factors, unlike a factorial's variable-length chain.",{{{3},6},{{5},60},{{7},210}},"A fixed number of neighbouring values can produce a large result when multiplied.",{"Multiply the input by the number just below it and the number below that."}},
      {20,Shape::Circle,"Square, then share","c = a^2/b","In each circle, a and b are inputs; c is the result.","Square a, then divide that square exactly by b.","Combine a power and exact division.","A squared numerator divided by an independent input.",{{{6,3},12},{{8,4},16},{{9,3},27}},"One input makes the result grow; the other is used to share that result.",{"Multiply the first input by itself, then divide the result by the second input."}},
      {21,Shape::Square,"Product plus both inputs","c = a*b + a + b","a = left; b = middle; c = right, in each row.","Multiply a and b, then add both original inputs.","Use the same inputs in several parts of the relationship.","A bilinear product with two independent additive terms.",{{{2,3},11},{{3,4},19},{{4,5},29}},"Use both inputs more than once. Multiplication is part of the pattern.",{"Multiply the left and middle numbers, but keep both original numbers for another step.", "Add the two original numbers to their product. Check that this works in both completed rows."}},
      {22,Shape::Hexagon,"Digit distance","b = abs(tens(a) - ones(a))","a is two-digit; abs means the nonnegative distance.","Find the positive difference between the two digits.","Combine digit decomposition with a nonnegative difference.","An absolute digit difference, distinct from digit sum or reversal.",{{{29},7},{{63},3},{{94},5}},"Focus on the individual digits instead of the whole number.",{"Compare the tens digit with the ones digit. The order of the digits should not matter.", "Subtract the smaller digit from the larger digit to find their distance."}},
      {23,Shape::Rectangle,"A product and a square","d = a*b + c^2","Read each row: a, b, c, d from left to right.","Multiply a by b, then add the square of c.","Two different operation branches combine into one result.","A product of two inputs plus the square of a third.",{{{3,4,2},16},{{5,2,3},19},{{4,6,5},49}},"Different inputs may need different operations before they are combined.",{"Use multiplication on the first two inputs and a separate operation on the third.", "Multiply the first two inputs. Multiply the third by itself, then add both results."}},
      {24,Shape::Star,"Multiply the digits","b = tens(a)*ones(a)","a is a two-digit number; b is its paired result.","Multiply the tens digit by the ones digit.","Use digit positions as operands of multiplication.","A digit product, distinct from operating on a or summing its digits.",{{{24},8},{{53},15},{{76},42}},"Look inside the two-digit number. Both digits contribute to its partner.",{"Separate the tens and ones digits before doing any arithmetic.", "Multiply the two digits together. Check the completed pairs using the same rule."}},
      {25,Shape::Circle,"Two branches, one result","e = a/b + c*d","In each circle, a,b,c,d are inputs; e is the result.","Divide a by b and multiply c by d, then add the two results.","Coordinate four inputs and two independent branches.","A quotient plus an independent product.",{{{12,3,2,4},12},{{20,4,3,2},11},{{24,6,4,3},16}},"Split the four inputs into two pairs and work on each pair separately.",{"The first pair uses division. The second pair uses multiplication.", "Divide the first input by the second, multiply the third by the fourth, then add the results."}},
      {26,Shape::Square,"Meet at a multiple","c = lcm(a,b)","lcm = smallest positive number divisible by both a and b.","Find the smallest positive common multiple of a and b.","Infer a divisibility relationship rather than fixed arithmetic.","Least common multiple, depending on shared prime factors.",{{{4,6},12},{{5,8},40},{{6,10},30}},"Think about numbers that both inputs can divide into evenly.",{"List the multiples of each input and look for numbers that appear in both lists.", "Choose the smallest positive number that can be divided evenly by both inputs."}},
      {27,Shape::Hexagon,"Different jobs for the digits","b = ones(a)^2 + tens(a)","a is two-digit; square its ones digit and add its tens digit.","Square the ones digit, then add the tens digit.","Assign asymmetric operations to different digit positions.","A mixed digit power and addition.",{{{23},11},{{45},29},{{62},10}},"Treat the two digits separately; they do not have equal roles.",{"Multiply the ones digit by itself. The tens digit still has a part to play.", "Add the tens digit to the result you got from multiplying the ones digit by itself."}},
      {28,Shape::Rectangle,"Return to the first input","d = (a - b)*c + a","Read each row: a, b, c, d from left to right.","Subtract b from a, multiply by c, then add a again.","Follow a grouped operation chain and reuse an original input.","A scaled difference with a separate return to the first operand.",{{{8,3,2},18},{{9,5,3},21},{{7,2,4},27}},"Some inputs are used more than once. Begin by comparing the first two.",{"Subtract the second input from the first, then multiply the gap by the third.", "Add the first input again after multiplying the gap by the third input."}},
      {29,Shape::Star,"Power with a correction","c = a^b - (a + b)","Each star has inputs a,b and result c; ^ means exponent.","Raise a to the power b, then subtract their sum.","Distinguish a variable exponent from multiplication and apply a correction.","An input-controlled exponent combined with a subtraction branch.",{{{2,3},3},{{3,2},4},{{3,4},74}},"Ordinary multiplication may not explain the size of the results. Try repeated multiplication.",{"Use the second input as the number of repeated factors, then consider the sum of the inputs.", "Multiply that many copies of the first input together, then subtract the sum of both inputs."}},
      {30,Shape::Circle,"The final balance","e = (a*b - c*d)/(a - c)","In each circle, a,b,c,d are inputs; e is the result.","Subtract the two products, then divide by the difference between a and c.","Combine two products, a subtraction, and a separate denominator.","A rational relationship whose numerator and denominator both depend on shared inputs.",{{{6,3,2,5},2},{{10,6,4,3},8},{{9,5,3,7},4}},"Compare the products of the two input pairs. Then compare the first input in each pair.",{"Subtract the second pair's product from the first pair's product. You will need one more step.", "Divide that result by the difference between the first input of each pair."}}
    };
    return data;
}
std::string substitution(const Puzzle& p,const Example& e) {
    auto result=p.formula;
    for(std::size_t i=0;i<e.inputs.size();++i)result=std::regex_replace(result,std::regex("\\b"+std::string(1,static_cast<char>('a'+i))+"\\b"),std::to_string(e.inputs[i]));
    return result+" = "+std::to_string(e.output);
}
void validateCatalogue() {
    if(puzzles().size()!=30)throw std::logic_error("Expected 30 levels");
    std::array<int,5> shapes{};
    std::set<std::string> formulas;
    for(std::size_t i=0;i<puzzles().size();++i) {
        const auto& p=puzzles()[i];
        if(p.level!=static_cast<int>(i+1)||p.examples.size()!=3||p.formula.empty()||p.explanation.empty()||p.hint.empty()||p.extraHints.size()!=static_cast<std::size_t>(hintLimit(p.level)-1))throw std::logic_error("Invalid level metadata");
        if(!formulas.insert(p.formula).second)throw std::logic_error("Duplicate formula");
        ++shapes[static_cast<int>(p.shape)];
        for(const auto& e:p.examples)if(evaluate(p.level,e.inputs)!=e.output)throw std::logic_error("Wrong answer at level "+std::to_string(p.level));
    }
    for(int count:shapes)if(count!=6)throw std::logic_error("Expected six levels per shape");
}
}
