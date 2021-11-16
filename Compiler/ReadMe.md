# Projects in Class CS480 (Language Processors and Programming Environments)

### Class Purpose
Survey of basic programming language processors and software development tools such as assemblers, interpreters, and compilers. 
Topics include design and construction of language processors, formal syntactic definition methods, parsing techniques, and code-generation techniques.
### How to compile and run
For C files:
```
gcc [programName].c -o programName

./programName
```
For lex and yacc files:
```
 lex example.l
 yacc example.y
 gcc -o example y.tab.c
 ./example
```

### Features
The code in this class demonstrates:
- Compiler design
- Lexical analysis
- Syntax analysis - grammars, LL(1) parsers, LR(1) parsers
- Semantic processing
- Code generation and optimization



