#ifndef AST_H_
#define AST_H_

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#define AST_MAX_NODES 512
#define AST_LABEL_LEN 96
#define AST_NO_NODE (-1)

typedef enum AstNodeKind {
    AST_PROGRAM,
    AST_BLOCK,
    AST_FUNCTION_DECL,
    AST_FUNCTION_CALL,
    AST_VAR_DECL,
    AST_ASSIGNMENT,
    AST_INPUT,
    AST_PRINT,
    AST_WHILE,
    AST_RETURN,
    AST_EXPRESSION,
    AST_BINARY_EXPR,
    AST_IDENTIFIER,
    AST_LITERAL,
    AST_COMMENT
} AstNodeKind;

typedef struct AstNode {
    AstNodeKind kind;
    ish_cha label[AST_LABEL_LEN];
    ish_intg line;
    ish_intg firstChild;
    ish_intg nextSibling;
} AstNode;

typedef struct AstTree {
    AstNode nodes[AST_MAX_NODES];
    ish_intg count;
    ish_intg root;
} AstTree;

ish_void astInit(AstTree* tree);
ish_intg astAddNode(AstTree* tree, ish_intg parent, AstNodeKind kind, ish_thread label, ish_intg line);
ish_intg astCreateNode(AstTree* tree, AstNodeKind kind, ish_thread label, ish_intg line);
ish_bool astAppendChild(AstTree* tree, ish_intg parent, ish_intg child);
ish_void astPrint(const AstTree* tree);

#endif
