/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: Santiago Ugarte
* Professors: Paulo Sousa
************************************************************
                        "\t=------------------------------------------------------=\n"
                        "\t|  ISH LANGUAGE COMPILER                              |\n"
                        "\t=------------------------------------------------------=\n"

        "::::::::::::::::::::::::::::::'####:'######:'##::::'##:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::. ##:'##... ##:##:::: ##:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::: ##::##:::..::##:::: ##:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::: ##:. ######::#########:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::: ##::..... ##:##.... ##:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::: ##:'##::: ##:##:::: ##:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::'####. ######::##:::: ##:::::::::::::::::::::::::::::::::::::\n"
        "::::::::::::::::::::::::::::::....::......::..:::::..::::::::::::::::::::::::::::::::::::::\n"
        "'########'##::::'##'########::'########:'########:::'##:::::::::'###:::'##::: ##:'######:::\n"
        "... ##..::##:::: ##:##.....::::##.... ##:##.... ##:::##::::::::'## ##:::###:: ##'##... ##::\n"
        "::: ##::::##:::: ##:##:::::::::##:::: ##:##:::: ##:::##:::::::'##:. ##::####: ##:##:::..:::\n"
        "::: ##::::#########:######:::::##:::: ##:########::::##::::::'##:::. ##:## ## ##:##::'####:\n"
        "::: ##::::##.... ##:##...::::::##:::: ##:##.... ##:::##:::::::#########:##. ####:##::: ##::\n"
        "::: ##::::##:::: ##:##:::::::::##:::: ##:##:::: ##:::##:::::::##.... ##:##:. ###:##::: ##::\n"
        "::: ##::::##:::: ##:########:::########::########::::########:##:::: ##:##::. ##. ######:::\n"
        ":::..::::..:::::..:........:::........::........::::........:..:::::..:..::::..::......::::\n"
*/

/*
************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/* Parser data */
Token lookahead;
ish_intg syntaxErrorNumber = 0;
ish_intg numParserErrors = 0;
ish_intg numSemanticErrors = 0;
static SymbolTable symbolTable;
ParserData psData; /* BNF statistics */
ish_thread BNFStrTable[NUM_BNF_RULES] = {
    "BNF_error",
    "BNF_codeSession",
    "BNF_comment",
    "BNF_dataSession",
    "BNF_optVarListDeclarations",
    "BNF_optionalStatements",
    "BNF_outputStatement",
    "BNF_outputVariableList",
    "BNF_program",
    "BNF_statement",
    "BNF_statements",
    "BNF_statementsPrime",
    "BNF_functionDefinition",
    "BNF_functionCall",
    "BNF_variableDeclaration",
    "BNF_inputStatement",
    "BNF_assignmentStatement",
    "BNF_whileStatement",
    "BNF_expression"
};

/* Function to start the parser */
ish_void startParser() {
    /* Initialize Parser data */
    ish_intg i = 0;
    for (i = 0; i < NUM_BNF_RULES; i++) {
        psData.parsHistogram[i] = 0;
    }
    numSemanticErrors = 0;
    symbolTableInit(&symbolTable);
    /* Proceed parser */
    lookahead = tokenizer();
    if (lookahead.code != SEOF_T) {
        program();
    }
    matchToken(SEOF_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
    printf("%s: Semantic errors: %d\n", STR_LANGNAME, numSemanticErrors);
}

/* Function to match tokens */
ish_void matchToken(ish_intg tokenCode, ish_intg tokenAttribute) {
    ish_intg matchFlag = 1;

    // Check if the current token matches the expected token
    switch (lookahead.code) {
    case KW_T:
        if (lookahead.attribute.codeType != tokenAttribute) {
            matchFlag = 0;  // Token type does not match
        }
        break;
    default:
        if (lookahead.code != tokenCode) {
            matchFlag = 0;  // Token code does not match
        }
        break;
    }

    // If there's a match or if it's the end of the file
    if (matchFlag || lookahead.code == SEOF_T) {
        // If the match flag is set, move to the next token
        if (matchFlag) {
            lookahead = tokenizer();
            if (lookahead.code == ERR_T) {
                printError();
                lookahead = tokenizer();  // Move to the next token after an error
                syntaxErrorNumber++;
            }
        }
    }
    else {
        // If there was no match, handle synchronization errors
        syncErrorHandler(tokenCode);
    }
}


/* Function to handle synchronization errors */
ish_void syncErrorHandler(ish_intg syncTokenCode) {
    printError();
    syntaxErrorNumber++;
    while (lookahead.code != syncTokenCode) {
        if (lookahead.code == SEOF_T)
            exit(syntaxErrorNumber);
        lookahead = tokenizer();
    }
    if (lookahead.code != SEOF_T)
        lookahead = tokenizer();
}

/* Function to print errors */
ish_void printError() {
    Token t = lookahead;
    numParserErrors++;
    printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error: Line:", line);
    printf("*****  Token code:%3d Attribute: ", t.code);
    switch (t.code) {
    case ERR_T:
        printf("*ERROR*: %s\n", t.attribute.errLexeme);
        break;
    case SEOF_T:
        printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
        break;
    case ID_T:
        printf("ID_T:\t\t%s\t\n", t.attribute.idLexeme);
        break;
    case STR_T:
        printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
        break;
    case KW_T:
        printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
        break;
    case LPR_T:
        printf("LPR_T\n");
        break;
    case RPR_T:
        printf("RPR_T\n");
        break;
    case LBR_T:
        printf("LBR_T\n");
        break;
    case RBR_T:
        printf("RBR_T\n");
        break;
    case EOS_T:
        printf("NA\n");
        break;
    default:
        printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
        break;
    }
}

ish_void program() {
    /* Update program statistics */
    psData.parsHistogram[BNF_program]++;
    while (lookahead.code != SEOF_T) {
        if (lookahead.code == CMT_T) {
            comment();
        }
        else if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_funk) {
            functionDefinition();
        }
        else {
            statement();
        }
    }
    printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/* Function for the comment non-terminal */

ish_void comment() {
    psData.parsHistogram[BNF_comment]++;
    matchToken(CMT_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

/* Function for the data session non-terminal */
ish_void dataSession() {
    psData.parsHistogram[BNF_dataSession]++;
    matchToken(KW_T, KW_data);
    matchToken(LBR_T, NO_ATTR);
    optVarListDeclarations();
    matchToken(RBR_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Data session parsed");
}

/* Function for the optional variable list declarations non-terminal */
ish_void optVarListDeclarations() {
    psData.parsHistogram[BNF_optVarListDeclarations]++;
    switch (lookahead.code) {
    default:
        break;
    }
    printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/* Function for the code session non-terminal */
ish_void codeSession() {
    psData.parsHistogram[BNF_codeSession]++;
    matchToken(KW_T, KW_code);
    matchToken(LBR_T, NO_ATTR);
    optionalStatements();
    matchToken(RBR_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Code session parsed");
}

/* Function for the optional statements non-terminal */
ish_void optionalStatements() {
    psData.parsHistogram[BNF_optionalStatements]++;
    while (lookahead.code != RBR_T && lookahead.code != SEOF_T) {
        if (lookahead.code == CMT_T)
            comment();
        else
            statement();
    }
    printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/* Function for the statements non-terminal */
ish_void statements() {
    psData.parsHistogram[BNF_statements]++;
    statement();
    statementsPrime();
    printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/* Function for the statements prime non-terminal */
ish_void statementsPrime() {
    psData.parsHistogram[BNF_statementsPrime]++;
    while (lookahead.code != RBR_T && lookahead.code != SEOF_T)
        statement();
}

/* Function for the statement non-terminal */
ish_void statement() {
    psData.parsHistogram[BNF_statement]++;
    switch (lookahead.code) {
    case KW_T:
        switch (lookahead.attribute.codeType) {
        case KW_v:
            variableDeclaration();
            break;
        case KW_print:
            outputStatement();
            break;
        case KW_input:
            inputStatement();
            break;
        case KW_while:
            whileStatement();
            break;
        case KW_return:
            matchToken(KW_T, KW_return);
            expressionUntil(EOS_T);
            matchToken(EOS_T, NO_ATTR);
            break;
        default:
            printError();
            lookahead = tokenizer();
        }
        break;
    case ID_T:
        assignmentStatement();
        break;
    case CMT_T:
        comment();
        break;
    default:
        printError();
        lookahead = tokenizer();
    }
    printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}


/* Function for the output statement non-terminal */
ish_void outputStatement() {
    psData.parsHistogram[BNF_outputStatement]++;
    matchToken(KW_T, KW_print);
    matchToken(LPR_T, NO_ATTR);
    outputVariableList();
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/* Function for the output variable list non-terminal */
ish_void outputVariableList() {
    psData.parsHistogram[BNF_outputVariableList]++;
    expressionUntil(RPR_T);
    printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/* Function to print BNF data */
ish_void printBNFData(ParserData psData) {
    ish_intg i;
    printf("\nBNF Statements Histogram:\n");
    for (i = 0; i < NUM_BNF_RULES; i++) {
        printf("%s = %2d\n", BNFStrTable[i], psData.parsHistogram[i]);
    }
}



ish_void functionDefinition() {
    psData.parsHistogram[BNF_functionDefinition]++;
    matchToken(KW_T, KW_funk);  // Match the function keyword
    if (lookahead.code == ID_T && !symbolTableDeclare(&symbolTable, lookahead.attribute.idLexeme, SYMBOL_FUNCTION, SYMBOL_TYPE_VOID, line))
        semanticError("duplicate function declaration", lookahead.attribute.idLexeme);
    matchToken(ID_T, NO_ATTR);  // Match the function name
    matchToken(LPR_T, NO_ATTR); // Match opening parenthesis
    matchToken(RPR_T, NO_ATTR); // Match closing parenthesis
    matchToken(LBR_T, NO_ATTR); // Match opening curly brace
    symbolTableEnterScope(&symbolTable);
    optionalStatements(); // Parse function body
    symbolTableLeaveScope(&symbolTable);
    matchToken(RBR_T, NO_ATTR); // Match closing curly brace
    printf("%s%s\n", STR_LANGNAME, ": Function definition parsed");
}

ish_void functionCall() {
    psData.parsHistogram[BNF_functionCall]++;
    if (lookahead.code == ID_T)
        validateFunctionUse(lookahead.attribute.idLexeme);
    matchToken(ID_T, NO_ATTR);  // Match function call name
    matchToken(LPR_T, NO_ATTR); // Match opening parenthesis
    expressionUntil(RPR_T);
    matchToken(RPR_T, NO_ATTR); // Match closing parenthesis
    matchToken(EOS_T, NO_ATTR); // Match end of statement
    printf("%s%s\n", STR_LANGNAME, ": Function call parsed");
}

ish_void variableDeclaration() {
    psData.parsHistogram[BNF_variableDeclaration]++;
    matchToken(KW_T, KW_v);

    SymbolType type = SYMBOL_TYPE_UNKNOWN;
    if (lookahead.code == KW_T && isTypeKeyword(lookahead.attribute.codeType)) {
        type = symbolTypeFromKeyword(lookahead.attribute.codeType);
        matchToken(KW_T, lookahead.attribute.codeType);
    }
    else
        printError();

    while (lookahead.code != EOS_T && lookahead.code != SEOF_T) {
        if (lookahead.code == ID_T && !symbolTableDeclare(&symbolTable, lookahead.attribute.idLexeme, SYMBOL_VARIABLE, type, line))
            semanticError("duplicate variable declaration", lookahead.attribute.idLexeme);
        matchToken(ID_T, NO_ATTR);
        if (lookahead.code == EQ_T) {
            matchToken(EQ_T, NO_ATTR);
            while (lookahead.code != COM_T && lookahead.code != EOS_T && lookahead.code != SEOF_T) {
                if (lookahead.code == ID_T)
                    validateVariableUse(lookahead.attribute.idLexeme);
                lookahead = tokenizer();
            }
        }
        if (lookahead.code == COM_T)
            matchToken(COM_T, NO_ATTR);
        else
            break;
    }

    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Variable declaration parsed");
}

ish_void inputStatement() {
    psData.parsHistogram[BNF_inputStatement]++;
    matchToken(KW_T, KW_input);
    matchToken(LPR_T, NO_ATTR);
    if (lookahead.code != RPR_T) {
        if (lookahead.code == ID_T)
            validateVariableUse(lookahead.attribute.idLexeme);
        matchToken(ID_T, NO_ATTR);
        while (lookahead.code == COM_T) {
            matchToken(COM_T, NO_ATTR);
            if (lookahead.code == ID_T)
                validateVariableUse(lookahead.attribute.idLexeme);
            matchToken(ID_T, NO_ATTR);
        }
    }
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

ish_void assignmentStatement() {
    ish_cha name[VID_LEN + 1] = { 0 };
    if (lookahead.code == ID_T)
        strncpy(name, lookahead.attribute.idLexeme, VID_LEN);
    matchToken(ID_T, NO_ATTR);
    if (lookahead.code == LPR_T) {
        psData.parsHistogram[BNF_functionCall]++;
        validateFunctionUse(name);
        matchToken(LPR_T, NO_ATTR);
        expressionUntil(RPR_T);
        matchToken(RPR_T, NO_ATTR);
        matchToken(EOS_T, NO_ATTR);
        printf("%s%s\n", STR_LANGNAME, ": Function call parsed");
        return;
    }

    psData.parsHistogram[BNF_assignmentStatement]++;
    validateVariableUse(name);
    matchToken(EQ_T, NO_ATTR);
    expressionUntil(EOS_T);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

ish_void whileStatement() {
    psData.parsHistogram[BNF_whileStatement]++;
    matchToken(KW_T, KW_while);
    matchToken(LPR_T, NO_ATTR);
    expressionUntil(RPR_T);
    matchToken(RPR_T, NO_ATTR);
    block();
    if (lookahead.code == EOS_T)
        matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": While statement parsed");
}

ish_void block() {
    matchToken(LBR_T, NO_ATTR);
    optionalStatements();
    matchToken(RBR_T, NO_ATTR);
}

ish_void expressionUntil(ish_intg terminatorToken) {
    if (lookahead.code == terminatorToken)
        return;

    psData.parsHistogram[BNF_expression]++;
    while (lookahead.code != terminatorToken && lookahead.code != SEOF_T) {
        if (lookahead.code == ID_T)
            validateVariableUse(lookahead.attribute.idLexeme);
        lookahead = tokenizer();
    }
}

ish_bool isTypeKeyword(ish_intg keyword) {
    return keyword == KW_numi || keyword == KW_flop || keyword == KW_thread || keyword == KW_longint;
}

SymbolType symbolTypeFromKeyword(ish_intg keyword) {
    switch (keyword) {
    case KW_numi:
        return SYMBOL_TYPE_INT;
    case KW_flop:
        return SYMBOL_TYPE_FLOAT;
    case KW_thread:
        return SYMBOL_TYPE_STRING;
    case KW_longint:
        return SYMBOL_TYPE_LONG;
    default:
        return SYMBOL_TYPE_UNKNOWN;
    }
}

ish_void semanticError(ish_thread message, ish_thread name) {
    numSemanticErrors++;
    printf("%s: Semantic error: Line:%3d: %s '%s'\n", STR_LANGNAME, line, message, name ? name : "");
}

ish_void validateVariableUse(ish_thread name) {
    if (!symbolTableLookup(&symbolTable, name, SYMBOL_VARIABLE))
        semanticError("undefined variable", name);
}

ish_void validateFunctionUse(ish_thread name) {
    if (!symbolTableLookup(&symbolTable, name, SYMBOL_FUNCTION))
        semanticError("undefined function", name);
}
