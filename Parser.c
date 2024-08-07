/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2024
* Author: BY Santiago Ugarte(041090461) & Isha Gadani(041085940)
* Professors: Paulo Sousa
************************************************************
                        "\t=------------------------------------------------------=\n"
                        "\t|  ISH LANGUAGE - BY Santiago Ugarte & Isha Gadani     |\n"
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
ParserData psData; /* BNF statistics */

/* Function to start the parser */
ish_void startParser() {
    ish_intg i;
    for (i = 0; i < NUM_BNF_RULES; i++) {
        psData.parsHistogram[i] = 0;
    }
    lookahead = tokenizer();
    if (lookahead.code != SEOF_T) {
        program();
    }
    matchToken(SEOF_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}

/* Function to match tokens */
ish_void matchToken(ish_intg tokenCode, ish_intg tokenAttribute) {
    ish_intg matchFlag = 1;
    switch (lookahead.code) {
    case KW_T:
        if (lookahead.attribute.codeType != tokenAttribute)
            matchFlag = 0;
        break;
    default:
        if (lookahead.code != tokenCode)
            matchFlag = 0;
        break;
    }
    if (matchFlag && lookahead.code == SEOF_T)
        return;
    if (matchFlag) {
        lookahead = tokenizer();
        if (lookahead.code == ERR_T) {
            printError();
            lookahead = tokenizer();
            syntaxErrorNumber++;
        }
    }
    else {
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
        numParserErrors++; // Updated parser error
        break;
    }
}

/* Function for the program non-terminal */
ish_void program() {
    psData.parsHistogram[BNF_program]++;
    switch (lookahead.code) {
    case CMT_T:
        comment();
        break;
    case ID_T:
        if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
            matchToken(ID_T, NO_ATTR);
            matchToken(LBR_T, NO_ATTR);
            dataSession();
            codeSession();
            matchToken(RBR_T, NO_ATTR);
            break;
        }
        else {
            printError();
        }
        break;
    case SEOF_T:
        break;
    default:
        printError();
        break;
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
    switch (lookahead.code) {
    case CMT_T:
        comment();
        break;
    case ID_T:
        if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
            (strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
            statements();
            break;
        }
        break;
    default:
        break;
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
    switch (lookahead.code) {
    case ID_T:
        if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
            statements();
            break;
        }
        break;
    default:
        break;
    }
}

/* Function for the statement non-terminal */
ish_void statement() {
    psData.parsHistogram[BNF_statement]++;
    switch (lookahead.code) {
    case KW_T:
        switch (lookahead.attribute.codeType) {
        case KW_code:
            codeSession();
            break;
        case KW_data:
            dataSession();
            break;
        default:
            printError();
            break;
        }
        break;
    case ID_T:
        if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
            outputStatement();
            break;
        }
        break;
    default:
        printError();
        break;
    }
    printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/* Function for the output statement non-terminal */
ish_void outputStatement() {
    psData.parsHistogram[BNF_outputStatement]++;
    matchToken(ID_T, NO_ATTR);
    matchToken(LPR_T, NO_ATTR);
    outputVariableList();
    matchToken(RPR_T, NO_ATTR);
    matchToken(EOS_T, NO_ATTR);
    printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/* Function for the output variable list non-terminal */
ish_void outputVariableList() {
    psData.parsHistogram[BNF_outputVariableList]++;
    switch (lookahead.code) {
    case STR_T:
        matchToken(STR_T, NO_ATTR);
        break;
    default:
        printError();
        break;
    }
    printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/* Function to print BNF data DONE*/
ish_void printBNFData(ParserData psData) {
    ish_intg i;
    printf("\nBNF Statements Histogram:\n");
    for (i = 0; i < NUM_BNF_RULES; i++) {
        printf("BNF[%02d] = %2d\n", i, psData.parsHistogram[i]);
    }
}
