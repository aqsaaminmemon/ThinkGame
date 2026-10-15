# ThinkGame: all 30 authored levels

Every level is one puzzle with two completed examples and one incomplete example. Only the final output is hidden. The rules are different; the progression is authored and still benefits from playtesting. Shape reuse never counts as rule variety.

## Level 1 - Double, then add (Square)

- Rule: `c = 2*a + b`
- Positions: a = left; b = middle; c = right, in each row.
- Explanation: Double the left number and add the middle number.
- Hint allowance: 1
- Hint in words: Try using the left number twice, then bring in the middle number.
- Difficulty rationale: Two small operations, demonstrated across rows.
- Distinct relationship: A weighted sum of two separate inputs.
- Examples:
  - `c = 2*3 + 2 = 8`
  - `c = 2*5 + 4 = 14`
  - `c = 2*7 + 3 = 17` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **17**

## Level 2 - Square pairs (Hexagon)

- Rule: `b = a^2`
- Positions: Each clockwise a-b pair follows the same rule.
- Explanation: Square each a to obtain its paired b.
- Hint allowance: 1
- Hint in words: Look at neighbouring pairs. What happens when a number is multiplied by itself?
- Difficulty rationale: Recognize paired positions and a power.
- Distinct relationship: A single input squared, without another operation.
- Examples:
  - `b = 2^2 = 4`
  - `b = 4^2 = 16`
  - `b = 5^2 = 25` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **25**

## Level 3 - Balance the row (Rectangle)

- Rule: `d = a + b - c`
- Positions: Read each row: a, b, c, d from left to right.
- Explanation: Add the first two numbers, then subtract the third.
- Hint allowance: 1
- Hint in words: The first two numbers work together; the third takes something away.
- Difficulty rationale: Track three inputs instead of two.
- Distinct relationship: An additive balance involving three independent inputs.
- Examples:
  - `d = 8 + 5 - 3 = 10`
  - `d = 9 + 7 - 4 = 12`
  - `d = 6 + 8 - 5 = 9` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **9**

## Level 4 - Threefold pairs (Star)

- Rule: `b = 3*a - 1`
- Positions: Clockwise pairs around the star are a, then b.
- Explanation: Triple each a, then subtract one.
- Hint allowance: 1
- Hint in words: Look at neighbouring pairs. Try three copies of the first number, then a small adjustment.
- Difficulty rationale: Find adjacent pairs around a non-grid shape.
- Distinct relationship: A single-input affine relationship, unlike the two-input weighted sum.
- Examples:
  - `b = 3*2 - 1 = 5`
  - `b = 3*4 - 1 = 11`
  - `b = 3*7 - 1 = 20` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **20**

## Level 5 - Sector products (Circle)

- Rule: `c = a*b`
- Positions: In each circle, a and b are inputs; c is the result.
- Explanation: Multiply a by b in each circle.
- Hint allowance: 1
- Hint in words: Think about equal groups. How could the two inputs combine by multiplication?
- Difficulty rationale: Infer multiplication from sector positions.
- Distinct relationship: A pure product of two independent inputs.
- Examples:
  - `c = 3*4 = 12`
  - `c = 5*2 = 10`
  - `c = 6*3 = 18` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **18**

## Level 6 - Meet in the middle (Square)

- Rule: `c = (a + b)/2`
- Positions: a = left; b = middle; c = right, in each row.
- Explanation: Find the average of the first two numbers.
- Hint allowance: 1
- Hint in words: The result sits halfway between the first two numbers.
- Difficulty rationale: Infer averaging, including an exact division.
- Distinct relationship: A mean, distinct from both a product and a weighted sum.
- Examples:
  - `c = (4 + 8)/2 = 6`
  - `c = (6 + 10)/2 = 8`
  - `c = (8 + 14)/2 = 11` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **11**

## Level 7 - Neighbour products (Hexagon)

- Rule: `b = a*(a + 1)`
- Positions: Each clockwise a-b pair follows the same rule.
- Explanation: Multiply a by the next whole number.
- Hint allowance: 1
- Hint in words: Try multiplying the first number by its next-door neighbour on the number line.
- Difficulty rationale: Create an intermediate value from the input.
- Distinct relationship: A product of consecutive integers.
- Examples:
  - `b = 2*(2 + 1) = 6`
  - `b = 4*(4 + 1) = 20`
  - `b = 6*(6 + 1) = 42` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **42**

## Level 8 - Group before multiplying (Rectangle)

- Rule: `d = (a + b)*c`
- Positions: Read each row: a, b, c, d from left to right.
- Explanation: Add a and b first, then multiply their sum by c.
- Hint allowance: 1
- Hint in words: Combine the first two numbers before letting the third make the result bigger.
- Difficulty rationale: Parentheses change which operation comes first.
- Distinct relationship: A sum multiplied by a third independent input.
- Examples:
  - `d = (2 + 3)*4 = 20`
  - `d = (4 + 2)*3 = 18`
  - `d = (3 + 5)*2 = 16` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **16**

## Level 9 - What remains (Star)

- Rule: `b = a % 3`
- Positions: % means remainder after division; each a-b pair is independent.
- Explanation: Divide a by three and take the remainder as b.
- Hint allowance: 1
- Hint in words: Think about groups of three. What is left over after making complete groups?
- Difficulty rationale: Recognize a cyclic remainder relationship rather than growth.
- Distinct relationship: Modular arithmetic, distinct from powers, products, and digit operations.
- Examples:
  - `b = 7 % 3 = 1`
  - `b = 11 % 3 = 2`
  - `b = 8 % 3 = 2` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **2**

## Level 10 - Distance squared (Circle)

- Rule: `c = (a - b)^2`
- Positions: In each circle, a and b are inputs; c is the result.
- Explanation: Subtract b from a, then square the difference.
- Hint allowance: 1
- Hint in words: Find the gap between the two inputs, then multiply that gap by itself.
- Difficulty rationale: A difference creates an intermediate value before a power.
- Distinct relationship: The square of a difference, including its cross term.
- Examples:
  - `c = (7 - 4)^2 = 9`
  - `c = (9 - 5)^2 = 16`
  - `c = (8 - 2)^2 = 36` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **36**

## Level 11 - Square and subtract (Square)

- Rule: `c = a^2 - b`
- Positions: a = left; b = middle; c = right, in each row.
- Explanation: Square the left number, then subtract the middle number.
- Hint allowance: 2
- Hint in words: Follow each row from left to right. The left number does more work than the middle one.
- Hint 2 in words: Multiply the left number by itself, then subtract the middle number from that result.
- Difficulty rationale: Apply a power to only one of two inputs.
- Distinct relationship: One squared operand followed by subtraction.
- Examples:
  - `c = 4^2 - 3 = 13`
  - `c = 5^2 - 7 = 18`
  - `c = 6^2 - 8 = 28` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **28**

## Level 12 - Inside the digits (Hexagon)

- Rule: `b = tens(a) + ones(a)`
- Positions: a is a two-digit number; b is its paired result.
- Explanation: Add the tens digit and the ones digit of a.
- Hint allowance: 2
- Hint in words: Try treating each digit as a separate number rather than using the whole number.
- Hint 2 in words: Split the first number into its tens and ones digits, then add those digits together.
- Difficulty rationale: Change from whole-number operations to digit decomposition.
- Distinct relationship: A digit sum, not an operation on the complete number.
- Examples:
  - `b = tens(24) + ones(24) = 6`
  - `b = tens(53) + ones(53) = 8`
  - `b = tens(67) + ones(67) = 13` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **13**

## Level 13 - Product with a deduction (Rectangle)

- Rule: `d = a*b - c`
- Positions: Read each row: a, b, c, d from left to right.
- Explanation: Multiply the first two numbers, then subtract the third.
- Hint allowance: 2
- Hint in words: Look for two operations in each row. The third number reduces the result.
- Hint 2 in words: Multiply the first and second numbers, then subtract the third number.
- Difficulty rationale: Choose the correct operation order across three inputs.
- Distinct relationship: A two-input product reduced by a third input.
- Examples:
  - `d = 4*5 - 3 = 17`
  - `d = 6*3 - 4 = 14`
  - `d = 7*4 - 6 = 22` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **22**

## Level 14 - Factorial pairs (Star)

- Rule: `b = a!`
- Positions: a! means 1*2*...*a; each a-b pair is independent.
- Explanation: Multiply all whole numbers from one through a.
- Hint allowance: 2
- Hint in words: Large jumps between paired numbers can come from a growing chain of multiplication.
- Hint 2 in words: Multiply every whole number from one up to the input. The chain grows with the input.
- Difficulty rationale: Recognize a variable-length multiplication chain.
- Distinct relationship: Factorial, whose number of factors depends on a.
- Examples:
  - `b = 3! = 6`
  - `b = 4! = 24`
  - `b = 5! = 120` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **120**

## Level 15 - Two squares together (Circle)

- Rule: `c = a^2 + b^2`
- Positions: In each circle, a and b are inputs; c is the result.
- Explanation: Square a and b separately, then add the squares.
- Hint allowance: 2
- Hint in words: Work on each input separately before combining what you get.
- Hint 2 in words: Multiply each input by itself first. Add those two results together.
- Difficulty rationale: Two independent powers must be combined.
- Distinct relationship: A sum of squares, with no cross term.
- Examples:
  - `c = 3^2 + 4^2 = 25`
  - `c = 2^2 + 5^2 = 29`
  - `c = 4^2 + 6^2 = 52` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **52**

## Level 16 - Square the whole sum (Square)

- Rule: `c = (a + b)^2`
- Positions: a = left; b = middle; c = right, in each row.
- Explanation: Add the first two numbers, then square the entire sum.
- Hint allowance: 2
- Hint in words: Combine the two inputs before repeating that total in a multiplication.
- Hint 2 in words: Add the two inputs together, then multiply that total by itself.
- Difficulty rationale: Distinguish grouping from separately squaring inputs.
- Distinct relationship: Square of a sum; different from the sum of squares in Level 15.
- Examples:
  - `c = (2 + 3)^2 = 25`
  - `c = (3 + 4)^2 = 49`
  - `c = (4 + 5)^2 = 81` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **81**

## Level 17 - Turn the digits around (Hexagon)

- Rule: `b = 10*ones(a) + tens(a)`
- Positions: a is a two-digit number; b is its paired result.
- Explanation: Reverse the two digits of a.
- Hint allowance: 2
- Hint in words: The digits stay the same, but their places change.
- Hint 2 in words: Swap the tens and ones digits. The last digit becomes the first.
- Difficulty rationale: Recognize a positional digit transformation.
- Distinct relationship: Digit reversal, rather than digit arithmetic.
- Examples:
  - `b = 10*ones(24) + tens(24) = 42`
  - `b = 10*ones(36) + tens(36) = 63`
  - `b = 10*ones(57) + tens(57) = 75` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **75**

## Level 18 - Divide before adding (Rectangle)

- Rule: `d = a/b + c`
- Positions: Read each row: a, b, c, d from left to right.
- Explanation: Divide a exactly by b, then add c.
- Hint allowance: 2
- Hint in words: Work from left to right. Think about equal groups first, then an extra amount.
- Hint 2 in words: Divide the first number by the second, then add the third number.
- Difficulty rationale: Recover an exact quotient before combining another input.
- Distinct relationship: A quotient followed by addition.
- Examples:
  - `d = 12/3 + 2 = 6`
  - `d = 20/4 + 3 = 8`
  - `d = 24/6 + 5 = 9` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **9**

## Level 19 - Three descending factors (Star)

- Rule: `b = a*(a - 1)*(a - 2)`
- Positions: Clockwise pairs around the star are a, then b.
- Explanation: Multiply a and the two whole numbers immediately below it.
- Hint allowance: 2
- Hint in words: A fixed number of neighbouring values can produce a large result when multiplied.
- Hint 2 in words: Multiply the input by the number just below it and the number below that.
- Difficulty rationale: Build and combine two intermediate factors.
- Distinct relationship: Exactly three descending factors, unlike a factorial's variable-length chain.
- Examples:
  - `b = 3*(3 - 1)*(3 - 2) = 6`
  - `b = 5*(5 - 1)*(5 - 2) = 60`
  - `b = 7*(7 - 1)*(7 - 2) = 210` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **210**

## Level 20 - Square, then share (Circle)

- Rule: `c = a^2/b`
- Positions: In each circle, a and b are inputs; c is the result.
- Explanation: Square a, then divide that square exactly by b.
- Hint allowance: 2
- Hint in words: One input makes the result grow; the other is used to share that result.
- Hint 2 in words: Multiply the first input by itself, then divide the result by the second input.
- Difficulty rationale: Combine a power and exact division.
- Distinct relationship: A squared numerator divided by an independent input.
- Examples:
  - `c = 6^2/3 = 12`
  - `c = 8^2/4 = 16`
  - `c = 9^2/3 = 27` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **27**

## Level 21 - Product plus both inputs (Square)

- Rule: `c = a*b + a + b`
- Positions: a = left; b = middle; c = right, in each row.
- Explanation: Multiply a and b, then add both original inputs.
- Hint allowance: 3
- Hint in words: Use both inputs more than once. Multiplication is part of the pattern.
- Hint 2 in words: Multiply the left and middle numbers, but keep both original numbers for another step.
- Hint 3 in words: Add the two original numbers to their product. Check that this works in both completed rows.
- Difficulty rationale: Use the same inputs in several parts of the relationship.
- Distinct relationship: A bilinear product with two independent additive terms.
- Examples:
  - `c = 2*3 + 2 + 3 = 11`
  - `c = 3*4 + 3 + 4 = 19`
  - `c = 4*5 + 4 + 5 = 29` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **29**

## Level 22 - Digit distance (Hexagon)

- Rule: `b = abs(tens(a) - ones(a))`
- Positions: a is two-digit; abs means the nonnegative distance.
- Explanation: Find the positive difference between the two digits.
- Hint allowance: 3
- Hint in words: Focus on the individual digits instead of the whole number.
- Hint 2 in words: Compare the tens digit with the ones digit. The order of the digits should not matter.
- Hint 3 in words: Subtract the smaller digit from the larger digit to find their distance.
- Difficulty rationale: Combine digit decomposition with a nonnegative difference.
- Distinct relationship: An absolute digit difference, distinct from digit sum or reversal.
- Examples:
  - `b = abs(tens(29) - ones(29)) = 7`
  - `b = abs(tens(63) - ones(63)) = 3`
  - `b = abs(tens(94) - ones(94)) = 5` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **5**

## Level 23 - A product and a square (Rectangle)

- Rule: `d = a*b + c^2`
- Positions: Read each row: a, b, c, d from left to right.
- Explanation: Multiply a by b, then add the square of c.
- Hint allowance: 3
- Hint in words: Different inputs may need different operations before they are combined.
- Hint 2 in words: Use multiplication on the first two inputs and a separate operation on the third.
- Hint 3 in words: Multiply the first two inputs. Multiply the third by itself, then add both results.
- Difficulty rationale: Two different operation branches combine into one result.
- Distinct relationship: A product of two inputs plus the square of a third.
- Examples:
  - `d = 3*4 + 2^2 = 16`
  - `d = 5*2 + 3^2 = 19`
  - `d = 4*6 + 5^2 = 49` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **49**

## Level 24 - Multiply the digits (Star)

- Rule: `b = tens(a)*ones(a)`
- Positions: a is a two-digit number; b is its paired result.
- Explanation: Multiply the tens digit by the ones digit.
- Hint allowance: 3
- Hint in words: Look inside the two-digit number. Both digits contribute to its partner.
- Hint 2 in words: Separate the tens and ones digits before doing any arithmetic.
- Hint 3 in words: Multiply the two digits together. Check the completed pairs using the same rule.
- Difficulty rationale: Use digit positions as operands of multiplication.
- Distinct relationship: A digit product, distinct from operating on a or summing its digits.
- Examples:
  - `b = tens(24)*ones(24) = 8`
  - `b = tens(53)*ones(53) = 15`
  - `b = tens(76)*ones(76) = 42` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **42**

## Level 25 - Two branches, one result (Circle)

- Rule: `e = a/b + c*d`
- Positions: In each circle, a,b,c,d are inputs; e is the result.
- Explanation: Divide a by b and multiply c by d, then add the two results.
- Hint allowance: 3
- Hint in words: Split the four inputs into two pairs and work on each pair separately.
- Hint 2 in words: The first pair uses division. The second pair uses multiplication.
- Hint 3 in words: Divide the first input by the second, multiply the third by the fourth, then add the results.
- Difficulty rationale: Coordinate four inputs and two independent branches.
- Distinct relationship: A quotient plus an independent product.
- Examples:
  - `e = 12/3 + 2*4 = 12`
  - `e = 20/4 + 3*2 = 11`
  - `e = 24/6 + 4*3 = 16` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **16**

## Level 26 - Meet at a multiple (Square)

- Rule: `c = lcm(a,b)`
- Positions: lcm = smallest positive number divisible by both a and b.
- Explanation: Find the smallest positive common multiple of a and b.
- Hint allowance: 3
- Hint in words: Think about numbers that both inputs can divide into evenly.
- Hint 2 in words: List the multiples of each input and look for numbers that appear in both lists.
- Hint 3 in words: Choose the smallest positive number that can be divided evenly by both inputs.
- Difficulty rationale: Infer a divisibility relationship rather than fixed arithmetic.
- Distinct relationship: Least common multiple, depending on shared prime factors.
- Examples:
  - `c = lcm(4,6) = 12`
  - `c = lcm(5,8) = 40`
  - `c = lcm(6,10) = 30` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **30**

## Level 27 - Different jobs for the digits (Hexagon)

- Rule: `b = ones(a)^2 + tens(a)`
- Positions: a is two-digit; square its ones digit and add its tens digit.
- Explanation: Square the ones digit, then add the tens digit.
- Hint allowance: 3
- Hint in words: Treat the two digits separately; they do not have equal roles.
- Hint 2 in words: Multiply the ones digit by itself. The tens digit still has a part to play.
- Hint 3 in words: Add the tens digit to the result you got from multiplying the ones digit by itself.
- Difficulty rationale: Assign asymmetric operations to different digit positions.
- Distinct relationship: A mixed digit power and addition.
- Examples:
  - `b = ones(23)^2 + tens(23) = 11`
  - `b = ones(45)^2 + tens(45) = 29`
  - `b = ones(62)^2 + tens(62) = 10` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **10**

## Level 28 - Return to the first input (Rectangle)

- Rule: `d = (a - b)*c + a`
- Positions: Read each row: a, b, c, d from left to right.
- Explanation: Subtract b from a, multiply by c, then add a again.
- Hint allowance: 3
- Hint in words: Some inputs are used more than once. Begin by comparing the first two.
- Hint 2 in words: Subtract the second input from the first, then multiply the gap by the third.
- Hint 3 in words: Add the first input again after multiplying the gap by the third input.
- Difficulty rationale: Follow a grouped operation chain and reuse an original input.
- Distinct relationship: A scaled difference with a separate return to the first operand.
- Examples:
  - `d = (8 - 3)*2 + 8 = 18`
  - `d = (9 - 5)*3 + 9 = 21`
  - `d = (7 - 2)*4 + 7 = 27` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **27**

## Level 29 - Power with a correction (Star)

- Rule: `c = a^b - (a + b)`
- Positions: Each star has inputs a,b and result c; ^ means exponent.
- Explanation: Raise a to the power b, then subtract their sum.
- Hint allowance: 3
- Hint in words: Ordinary multiplication may not explain the size of the results. Try repeated multiplication.
- Hint 2 in words: Use the second input as the number of repeated factors, then consider the sum of the inputs.
- Hint 3 in words: Multiply that many copies of the first input together, then subtract the sum of both inputs.
- Difficulty rationale: Distinguish a variable exponent from multiplication and apply a correction.
- Distinct relationship: An input-controlled exponent combined with a subtraction branch.
- Examples:
  - `c = 2^3 - (2 + 3) = 3`
  - `c = 3^2 - (3 + 2) = 4`
  - `c = 3^4 - (3 + 4) = 74` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **74**

## Level 30 - The final balance (Circle)

- Rule: `e = (a*b - c*d)/(a - c)`
- Positions: In each circle, a,b,c,d are inputs; e is the result.
- Explanation: Subtract the two products, then divide by the difference between a and c.
- Hint allowance: 3
- Hint in words: Compare the products of the two input pairs. Then compare the first input in each pair.
- Hint 2 in words: Subtract the second pair's product from the first pair's product. You will need one more step.
- Hint 3 in words: Divide that result by the difference between the first input of each pair.
- Difficulty rationale: Combine two products, a subtraction, and a separate denominator.
- Distinct relationship: A rational relationship whose numerator and denominator both depend on shared inputs.
- Examples:
  - `e = (6*3 - 2*5)/(6 - 2) = 2`
  - `e = (10*6 - 4*3)/(10 - 4) = 8`
  - `e = (9*5 - 3*7)/(9 - 3) = 4` (output hidden in game)
- Missing position: the result of the third example.
- Answer: **4**

