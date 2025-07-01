#pragma once
#include <memory>
#include <string>
#include <vector>
#include <optional>

// Base AST node
struct ASTNode {
    virtual ~ASTNode() = default;
};

// Forward declarations
struct Statement;
struct Expression;
struct Block;

// Program root: list of blocks (functions or statements)
struct Program : ASTNode {
    std::vector<std::unique_ptr<ASTNode>> blocks;
};

// Function definition
struct Function : ASTNode {
    std::string name;
    std::unique_ptr<Block> body;
};

// Block: a sequence of statements or controls
struct Block : ASTNode {
    std::vector<std::unique_ptr<Statement>> statements;
};

// Statement base
struct Statement : ASTNode {
};

// Instruction statement (e.g., definir_pin, salida, etc.)
struct Instruction : Statement {
    enum class Kind {
        DefinirPin, Salida, Entrada, Prender, Apagar, Esperar,
        Declaracion, Asignacion
    } kind;

    // For all instructions
    std::vector<std::string> ids;
    std::vector<double> numbers;
    std::optional<std::string> str_literal;
    std::unique_ptr<Expression> expr1;
    std::unique_ptr<Expression> expr2;
    std::string tipo; // For declaracion
};

// Declaration statement
struct Declaracion : Statement {
    std::string tipo;
    std::string id;
    std::unique_ptr<Expression> expr;
};

// Assignment statement
struct Asignacion : Statement {
    std::string id;
    std::unique_ptr<Expression> expr;
};

// Control statement base
struct Control : Statement {
};

// Mientras (while) control
struct Mientras : Control {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> body;
};

// Si (if) control
struct Si : Control {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Block> thenBlock;
    std::vector<std::pair<std::unique_ptr<Expression>, std::unique_ptr<Block>>> sinoSiBlocks;
    std::unique_ptr<Block> sinoBlock;
};

// Por (for) control
struct Por : Control {
    std::string id;
    std::unique_ptr<Expression> initExpr;
    std::unique_ptr<Expression> condExpr;
    std::unique_ptr<Asignacion> update;
    std::unique_ptr<Block> body;
};

// Expression base
struct Expression : ASTNode {
};

// Binary operation
struct BinaryExpr : Expression {
    std::unique_ptr<Expression> left;
    std::string op;
    std::unique_ptr<Expression> right;
};

// Literal (number, string, bool)
struct LiteralExpr : Expression {
    enum class Kind { Number, String, Bool } kind;
    double numberValue;
    std::string stringValue;
    bool boolValue;
};

// Variable reference
struct VariableExpr : Expression {
    std::string name;
};

// Leer (read) expression
struct LeerExpr : Expression {
    std::string id;
};
