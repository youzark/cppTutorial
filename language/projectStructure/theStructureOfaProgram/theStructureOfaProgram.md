# The Structure of a Program

## The process of building a program

Input Files  
Development Tool Chain  
Executable Files  
O/S Loader  
Running Program  

### Input Files

1.User-defined code
    1. Header files: .h .hpp .hh
    2. Source files: .c .cpp .cxx .C
    3. Resource files .res .qrc .rcc
2.Dependencies(libraries)
    1. Header files: 
    2. Precompiled files:
    3. Source files:
    4. Resources

### C++ Programming Ecosystem

1. Source Code Tool
    1. Editor
    2. Version Control
    3. Dependency Management(Conan Build2 Buckarro, Cmake...)
2. Build Tools: make CMake Ninja boost.build ...
    1. Compiler
    2. Linker
3. DLL/Archive:

Diagram:

![buildExecutable](/home/youzark/projects/cppTutorial/language/projectStructure/theStructureOfaProgram/img/buildExecutable.png)

Compilation: Source Code Files --> Object Files
1. Lexical analysis         : char stream  --> token sequence,symbol table
2. Syntax analysis(parsing) : token sequence --> parse tree
3. Semantic analysis        : augment syntax tree & symbol table --> intermedia representation
4. Code generation          : intermedia representation --> machine code

Linking: Combine binary to executable

## What a Translation unit is?

compilation  <==> translation

translation is performed on translation unit

A translation unit is defined as: A source file with preprocessing
1. A source file
2. all header via #include
3. Minus any source lines skipped by preprocessing directive
4. All macros expanded

## The phases of translation
9 phases in total to deal with : preprocessing, tokenize ,parse ,Semantic analysis ,instantiation, linking ...

source code  --> phases 1~6(preprocessing)  --> translation unit

translation unit --> phases 7~8(syntax,Semantic analysis + codegen)  --> object code

object code  --> linking  --> executable

## Declarations, Definitions and Linkage

### Entities:

value object reference structured_binding function enumerator type class_member bit-field template template_specialization
namespace pack


## The One-dedinition rule

## Storage Duration

## ABIs and name-mangling

## Linking and Loading
