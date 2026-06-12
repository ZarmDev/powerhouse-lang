# powerhouse-lang
The goal is to create an interpreted programming language that:
1. Fast
2. Takes less time to create a program with
3. And has a garbage collector

Copyright (c) 2025 ZarmDev. All rights reserved.

## Why would it be faster than other languages?
1. There will be **NO abstract syntax tree!**
2. It will instead use **one for loop** that uses a state variable (enum) that decides what the compiler should be "looking for"
Basically, **it will read each character only one time** and skip the process of converting it to the AST format
3. It will also have a system where all variables will be in one map that will **get deleted immediately** after finding a "}" or some kind of end statement.
4. Hopefully there will also be a different compiler that will generate optimizations like sorting all the functions names into an unordered_map or it could also parse all parentheses and different type concatenation without modifying the actual file
5. Rather than deal with the nuisances of converting each variable to a type (in the compiler), instead, a string could be used for built-in types which could make it very fast to declare a variable (example: the first letter of the actual variable value could be ! or & or ^ and each character will represent a different type, the rest of the string is the value)
6. Concatenation/parsing parentheses with different types or multiple parentheses will not work by default unless a line is added on the top of the program or you run the optimization compiler first
