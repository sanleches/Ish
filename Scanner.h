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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Project: Ish Compiler
* Purpose: This file declares lexer tokens, token attributes, and scanner APIs.
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

#define NUM_TOKENS 23

enum TOKENS {
	ERR_T,        /*  0: Error token */
	ID_T,         /*  1: Identifier token */
	INL_T,        /*  2: Integer literal token */
	STR_T,        /*  3: String literal token */
	LPR_T,        /*  4: Left parenthesis token */
	RPR_T,        /*  5: Right parenthesis token */
	LBR_T,        /*  6: Left brace token */
	RBR_T,        /*  7: Right brace token */
	KW_T,         /*  8: Keyword token */
	EOS_T,        /*  9: End of statement (semicolon) */
	RTE_T,        /* 10: Run-time error token */
	SEOF_T,       /* 11: Source end-of-file token */
	CMT_T,        /* 12: Comment token */
	FL_T,         /* 13: Float token */
	AO_T,         /* 14: Arithmetic Operator Token */
	EQ_T,         /* 15: Equal Sign Token */
	FUNC_T,       /* 16: Function keyword (e.g., `funk`) */
	VAR_T,        /* 17: Variable keyword (e.g., `v`) */
	THREAD_T,     /* 18: Thread keyword (e.g., `ish_thread`) */
	TYPE_T,       /* 19: Type keyword (e.g., `ish_intg`, `ish_flop`) */
	CALL_T,       /* 20: Function call token */
	COM_T,        /* 21: Comma token */
	REL_T         /* 22: Relational operator token */
};


extern ish_thread tokenStrTable[NUM_TOKENS];

typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

typedef union TokenAttribute {
	ish_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	ish_intg intValue;				/* integer literal attribute (value) */
	ish_intg keywordIndex;			/* keyword index in the keyword table */
	ish_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	ish_real floatValue;				/* floating-point literal attribute (value) */
	ish_cha idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	ish_cha errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

typedef struct idAttibutes {
	ish_byte flags;			/* Flags information */
	union {
		ish_intg intValue;				/* Integer value */
		ish_real floatValue;			/* Float value */
		ish_thread stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	ish_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	ish_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

ish_intg			startScanner(BufferPointer psc_buf);
ish_void			printScannerData(ScannerData scData);
Token				tokenizer(ish_void);

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

#define KWT_SIZE 17

extern ish_thread keywordTable[KWT_SIZE];

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

typedef struct languageAttributes {
	ish_cha indentationCharType;
	ish_thread indentationCurrentPos;
} LanguageAttributes;

/* Number of errors */
extern ish_thread numScannerErrors;

/* Scanner data */
extern ScannerData scData;

#endif
