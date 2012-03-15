DLang 0.3 Language Specification
==============================================

Lanugage Overview
----------------------------------------------

Datatypes
----------------------------------------------

### Prototype Objects

### Basic Types
#### Numbers
#### Characters
#### Symbols

### Collections
#### Lists
#### Vectors
#### Maps
#### Strings

### Blocks
#### Definition and Usage
#### Lexical Scoping
#### Performance and Behavior

### Streams

Operations and Operators
----------------------------------------------

### Arithmetic Operators
### Logical Operators
### Comparison Operators
### Operator Precedence

Modules
----------------------------------------------

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

    AssignExpr = LogicalExpr ':=' LogicalExpr
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
              | Literal '(' ExpList ')'
              | Literal '[' LogicalExpr ']'

    MemberExpr = Literal '.' LogicalExpr

    Literal = VectorLiteral
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

