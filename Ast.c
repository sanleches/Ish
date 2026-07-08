#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#ifndef AST_H_
#include "Ast.h"
#endif

static ish_thread astKindName(AstNodeKind kind);
static ish_void astPrintNode(const AstTree* tree, ish_intg nodeIndex, ish_intg depth);

ish_void astInit(AstTree* tree) {
    if (!tree)
        return;
    tree->count = 0;
    tree->root = AST_NO_NODE;
}

ish_intg astAddNode(AstTree* tree, ish_intg parent, AstNodeKind kind, ish_thread label, ish_intg line) {
    ish_intg index = astCreateNode(tree, kind, label, line);
    if (index == AST_NO_NODE)
        return AST_NO_NODE;

    if (parent == AST_NO_NODE) {
        tree->root = index;
        return index;
    }

    astAppendChild(tree, parent, index);
    return index;
}

ish_intg astCreateNode(AstTree* tree, AstNodeKind kind, ish_thread label, ish_intg line) {
    if (!tree || tree->count >= AST_MAX_NODES)
        return AST_NO_NODE;

    ish_intg index = tree->count++;
    AstNode* node = &tree->nodes[index];
    node->kind = kind;
    node->line = line;
    node->firstChild = AST_NO_NODE;
    node->nextSibling = AST_NO_NODE;
    node->label[0] = '\0';
    if (label) {
        strncpy(node->label, label, AST_LABEL_LEN - 1);
        node->label[AST_LABEL_LEN - 1] = '\0';
    }

    return index;
}

ish_bool astAppendChild(AstTree* tree, ish_intg parent, ish_intg child) {
    if (!tree || parent == AST_NO_NODE || child == AST_NO_NODE)
        return ISH_FALSE;

    AstNode* parentNode = &tree->nodes[parent];
    if (parentNode->firstChild == AST_NO_NODE) {
        parentNode->firstChild = child;
    }
    else {
        ish_intg sibling = parentNode->firstChild;
        while (tree->nodes[sibling].nextSibling != AST_NO_NODE)
            sibling = tree->nodes[sibling].nextSibling;
        tree->nodes[sibling].nextSibling = child;
    }

    return ISH_TRUE;
}

ish_void astPrint(const AstTree* tree) {
    if (!tree || tree->root == AST_NO_NODE)
        return;
    printf("\nAST:\n");
    astPrintNode(tree, tree->root, 0);
}

static ish_void astPrintNode(const AstTree* tree, ish_intg nodeIndex, ish_intg depth) {
    const AstNode* node = &tree->nodes[nodeIndex];
    for (ish_intg i = 0; i < depth; i++)
        printf("  ");
    printf("%s", astKindName(node->kind));
    if (node->label[0] != '\0')
        printf(": %s", node->label);
    printf("\n");

    for (ish_intg child = node->firstChild; child != AST_NO_NODE; child = tree->nodes[child].nextSibling)
        astPrintNode(tree, child, depth + 1);
}

static ish_thread astKindName(AstNodeKind kind) {
    switch (kind) {
    case AST_PROGRAM: return "Program";
    case AST_BLOCK: return "Block";
    case AST_FUNCTION_DECL: return "FunctionDecl";
    case AST_FUNCTION_CALL: return "FunctionCall";
    case AST_VAR_DECL: return "VarDecl";
    case AST_ASSIGNMENT: return "Assignment";
    case AST_INPUT: return "Input";
    case AST_PRINT: return "Print";
    case AST_WHILE: return "While";
    case AST_RETURN: return "Return";
    case AST_EXPRESSION: return "Expression";
    case AST_BINARY_EXPR: return "BinaryExpr";
    case AST_IDENTIFIER: return "Identifier";
    case AST_LITERAL: return "Literal";
    case AST_COMMENT: return "Comment";
    default: return "Unknown";
    }
}
