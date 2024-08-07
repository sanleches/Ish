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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
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

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 21

/* TO_DO: Define Token codes - Create your token classes */
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
	CALL_T        /* 20: Function call token */
};


/* Define the list of tokens for the .ish language */
static ish_thread tokenStrTable[NUM_TOKENS] = {
	"ERR_T",        /*  0: Error token */
	"ID_T",         /*  1: Identifier token */
	"INL_T",        /*  2: Integer literal token */
	"STR_T",        /*  3: String literal token */
	"LPR_T",        /*  4: Left parenthesis token */
	"RPR_T",        /*  5: Right parenthesis token */
	"LBR_T",        /*  6: Left brace token */
	"RBR_T",        /*  7: Right brace token */
	"KW_T",         /*  8: Keyword token */
	"EOS_T",        /*  9: End of statement (semicolon) */
	"RTE_T",        /* 10: Run-time error token */
	"SEOF_T",       /* 11: Source end-of-file token */
	"CMT_T",        /* 12: Comment token */
	"FL_T",         /* 13: Float token */
	"AO_T",         /* 14: Arithmetic Operator Token */
	"EQ_T",         /* 15: Equal Sign Token */
	"FUNC_T",       /* 16: Function keyword (e.g., `funk`) */
	"VAR_T",        /* 17: Variable keyword (e.g., `v`) */
	"THREAD_T",     /* 18: Thread keyword (e.g., `ish_thread`) */
	"TYPE_T",       /* 19: Type keyword (e.g., `ish_intg`, `ish_flop`) */
	"CALL_T"        /* 20: Function call token */
};


/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
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

/* TO_DO: Should be used if no symbol table is implemented */
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

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '#'
#define CHRCOL3 '_'
#define CHRCOL4 '"'
#define CHRCOL5 '.'
#define CHRCOL6 '\n'


/* TO_DO: Error states and illegal state */
#define ESNR	14		/* Error state with no retract */
#define ESWR	15		/* Error state with retract */
#define FS		16		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		16
#define CHAR_CLASSES	8

/* TO_DO: Transition table - type of states defined in separate table */
static ish_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    #,    _,    ",    .,   \n,    0
	   L(0), D(1), RTS2    U4,   Q5,   P6,   N7,   O8 */
	{     6,    8,    1, ESNR,   12,   ESNR, ESNR, ESNR},		// S0: NOAS
	{     4,    4,    2,    4,    4,      4,    4,    4},		// S1: NOAS
	{     2,    2,    2,    2,    2,	  2,    3,    2},		// S2: NOAS
	{    FS,   FS,   FS,   FS,   FS,	 FS,   FS,   FS},		// S3: ASNR (SLC)
	{     4,    4,    5,    4,    4,	  4,    4,    4},		// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,	 FS,   FS,   FS},		// S5: ASNR (MLC)
	{     6,    6,    7,    6,    7,	  7,    7,    7},		// S6: NOAS
	{    FS,   FS,   FS,   FS,   FS,	 FS,   FS,   FS},		// S7: ASWR (VID|MID|KEY)
	{     9,    8,    9,    9,    9,	 10,    9,    9},		// S8: NOAS
	{    FS,   FS,   FS,   FS,   FS,	 FS,   FS,   FS},		// S9: ASWR (IL)
	{    11,   10,   11,   11,   11,	 11,   11,   11},		// S10: NOAS
	{    FS,   FS,   FS,   FS,   FS,	 FS,   FS,   FS},		// S11: ASWR
	{    12,   12,   12,   12,   13,	 12,   12,   12},		// S12: NOAS
	{    FS,   FS,   FS,   FS,   FS,	 FS,   FS,   FS}		// S13: ASNR (SL)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static ish_intg stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	NOFS, /* 02 */
	FSNR, /* 03 (SLC) */
	NOFS, /* 04 */
	FSNR, /* 05 (MLC) */
	NOFS, /* 06 */
	FSWR, /* 07 (MID|VID|KEY) */
	NOFS, /* 08 */
	FSWR, /* 09 (IL) */
	NOFS, /* 10 */
	FSWR, /* 11 (FL) */
	NOFS, /* 12 */
	FSNR  /* 13 (SL) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
ish_intg			startScanner(BufferPointer psc_buf);
static ish_intg	nextClass(ish_cha c);					/* character class function */
static ish_intg	nextState(ish_intg, ish_cha);		/* state machine function */
ish_void			printScannerData(ScannerData scData);
Token				tokenizer(ish_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(ish_thread lexeme);

/* Declare accepting states functions */
Token funcSL	(ish_thread lexeme);
Token funcIL	(ish_thread lexeme);
Token funcFL	(ish_thread lexeme);
Token funcID	(ish_thread lexeme);
Token funcCMT   (ish_thread lexeme);
Token funcErr	(ish_thread lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	NULL,		/* -    [02] */
	funcCMT,	/* SLC	[03] */
	NULL,		/*      [04] */
	funcCMT,	/* -    [05] */
	NULL,		/* -    [06] */
	funcID,		/* SL   [07] */
	NULL,		/* -    [08] */
	funcIL	,	/* IL   [09] */
	NULL,		/* -    [10]*/
	funcFL,		/* FL   [11] */
	NULL,		/* -    [12]*/
	funcSL,		/* SSL  [13] */
	funcErr,	/* ERR  [14] */
	funcErr 	/* ERR  [15] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 15

/* TO_DO: Define the list of keywords */
static ish_thread keywordTable[KWT_SIZE] = {
	"if",       /* KW01 */
	"else",     /* KW02 */
	"while",    /* KW03 */
	"for",      /* KW04 */
	"return",   /* KW05 */
	"funk",     /* KW06 */
	"v",        /* KW07 (var) */
	"ish_intg", /* KW08 (integer) */
	"ish_flop", /* KW09 (float) */
	"ish_thread", /* KW10 (string) */
	"print",    /* KW11 */
	"input",    /* KW12 */
	"longint",  /* KW13 (signed integer) */
	"TRUE",     /* KW14 */
	"FALSE"     /* KW15 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	ish_cha indentationCharType;
	ish_thread indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
ish_thread numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
