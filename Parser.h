/*
************************************************************
* Ish Compiler Project
* Version: 0.1.0
* Author: Santiago Ugarte
* Reviewer: Paulo Sousa
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
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Project: Ish Compiler
* Purpose: This file declares parser, AST, and semantic-analysis front-end APIs.
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif
#ifndef SYMBOL_TABLE_H_
#include "SymbolTable.h"
#endif
#ifndef AST_H_
#include "Ast.h"
#endif

/* Global vars */
extern Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern ish_intg			line;
extern Token			tokenizer(ish_void);
extern ish_thread		keywordTable[KWT_SIZE];
extern ish_intg			syntaxErrorNumber;
static const ish_cha	NEWLINE_CHARACTER = '\n';
extern ish_bool parserDumpAst;

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_code,
	KW_data,
	KW_if,
	KW_else,
	KW_while,
	KW_for,
	KW_return,
	KW_funk,
	KW_v,
	KW_numi,
	KW_flop,
	KW_thread,
	KW_print,
	KW_input,
	KW_longint,
	KW_TRUE,
	KW_FALSE
};

#define NUM_BNF_RULES 19

/* Parser */
typedef struct parserData {
	ish_intg parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
extern ish_intg numParserErrors;
extern ish_intg numSemanticErrors;

/* Scanner data */
extern ParserData psData;

/* Function definitions */
ish_void startParser();
ish_void matchToken(ish_intg, ish_intg);
ish_void syncErrorHandler(ish_intg);
ish_void printError();
ish_void printBNFData(ParserData psData);

/* List of BNF statements */
enum BNF_RULES {
	BNF_error,										/*  0: Error token */
	BNF_codeSession,								/*  1 */
	BNF_comment,									/*  2 */
	BNF_dataSession,								/*  3 */
	BNF_optVarListDeclarations,						/*  4 */
	BNF_optionalStatements,							/*  5 */
	BNF_outputStatement,							/*  6 */
	BNF_outputVariableList,							/*  7 */
	BNF_program,									/*  8 */
	BNF_statement,									/*  9 */
	BNF_statements,									/* 10 */
	BNF_statementsPrime,							/* 11 */
	BNF_functionDefinition,							/* 12 */
	BNF_functionCall,								/* 13 */
	BNF_variableDeclaration,						/* 14 */
	BNF_inputStatement,							/* 15 */
	BNF_assignmentStatement,						/* 16 */
	BNF_whileStatement,							/* 17 */
	BNF_expression								/* 18 */
};

extern ish_thread BNFStrTable[NUM_BNF_RULES];

/* Place ALL non-terminal function declarations */
ish_void codeSession();
ish_void comment();
ish_void dataSession();
ish_void optVarListDeclarations();
ish_void optionalStatements();
ish_void outputStatement();
ish_void outputVariableList();
ish_void program();
ish_void statement();
ish_void statements();
ish_void statementsPrime();
ish_void functionDefinition();
ish_void functionCall();
ish_void variableDeclaration();
ish_void inputStatement();
ish_void assignmentStatement();
ish_void whileStatement();
ish_void block();
ish_void expressionUntil(ish_intg terminatorToken);
ish_bool isTypeKeyword(ish_intg keyword);
SymbolType symbolTypeFromKeyword(ish_intg keyword);
ish_void semanticError(ish_thread message, ish_thread name);
ish_void validateVariableUse(ish_thread name);
ish_void validateFunctionUse(ish_thread name);

#endif
