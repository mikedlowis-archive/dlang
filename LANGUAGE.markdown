DLang 0.3 Language Specification
==============================================

Lanugage Overview
----------------------------------------------

### Basic Syntax

Datatypes
----------------------------------------------

### Prototype Objects

A Work In Progress

### Basic Types
#### Numbers

Numbers are represented internally as double precision floating point numbers.
The syntax is very similar to lanugages like C, C++, Java, and Javascript.
Here are some examples for defining numbers:

    # Integer representations
    4
    -4

    # Floating point representations
    0.4
    -0.4

    # Floating point representations with exponents
    4.0e3
    4.0e-3
    -4.0e3
    -4.0e-3

#### Characters

    'A'

#### Symbols

    $some_symbol

#### Boolean Values

    True
    False
    Nil

### Collections
#### Lists

    ()          # An empty list
    (1)         # This is NOT a list. This evaluates to 1
    (1, 2)      # A list with two numbers
    ('a', 1)    # A list with two elements of different types

#### Vectors

    []          # An empty vector
    [1]         # A vector with on element
    [1, 2]      # A vector with two number elements
    ['a', 1]    # A vector with two elements of different types

#### Maps
#### Strings

### Blocks
#### Definition and Usage
#### Lexical Scoping
#### Performance and Behavior

### Streams

A Work In Progress

Operations and Operators
----------------------------------------------

### Definition and Assignment
#### Definition
#### Assignment

### Arithmetic Operators
#### Addition
#### Subtraction
#### Multiplication
#### Division

### Logical Operators

### Comparison Operators
### Indexing and Grouping Operators
### Member Access
### Operator Precedence
### Operator Definition and Overloading

A Work In Progress

Modules
----------------------------------------------

A Work In Progress

Macros
----------------------------------------------

### Defining Macros
### Using Macros

Builtin Functions
----------------------------------------------

Builtin Macros
----------------------------------------------

Formal Syntax and Semantics
----------------------------------------------

### DLang EBNF Grammar

    Program    = Expression*

    Expression = MacroDefinition
               | AssignExpr

    AssignExpr = MacroExpansion
               | LogicalExpr ':=' LogicalExpr
               | LogicalExpr '=' LogicalExpr

    LogicalExpr = CompExpr (('&&' | '||') CompExpr)*

    CompExpr = AddSubExpr (('==' | '!=' | '<' | '>' | '<=' | '>=') AddSubExpr)*

    AddSubExpr = MulDivExpr (('+' | '-') MulDivExpr)*

    MulDivExpr = UnaryExpr (('*' | '/') UnaryExpr)*

    UnaryExpr = '!' GroupExpr
              | GroupExpr

    GroupExpr = '(' LogicalExpr ')'
              | '(' LogicalExpr ')' '(' ExpList ')'
              | '(' LogicalExpr ')' '[' LogicalExpr ']'
              | Literal
              | Literal '[' LogicalExpr ']'

    MemberExpr = Literal '.' LogicalExpr

    Literal = MapLiteral
            | VectorLiteral
            | ListLiteral
            | FuncLiteral
            | ID
            | NUM
            | CHAR
            | STRING
            | SYMBOL

    MapLiteral = '{' (Literal ':' AssignExpr)* '}'

    VectorLiteral = '[' ExpList ']'

    ListLiteral = '(' ExpList ')'

    FuncLiteral = '{' ExpBlock '}'
                |  '{' '|' ExpList '|' ExpBlock '}'

    MacroDefinition = '%' ID '[' MacroParamList ']'

    MacroPatternList = MacroPattern (',' MacroPattern)*

    MacroPattern = '(' ID+ ')' ':' LogicalExpr

    ExpList = (GroupExpr (',' GroupExpr)*)?

    ExpBlock = Expression*

