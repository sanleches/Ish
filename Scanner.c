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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Project: Ish Compiler
* Purpose: This file implements lexical analysis for Ish source files.
* Function list: (...).
************************************************************
*/

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
ish_intg line;								/* Current line number of the source code */
extern ish_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern ish_thread keywordTable[KWT_SIZE];

ish_thread tokenStrTable[NUM_TOKENS] = {
	"ERR_T", "ID_T", "INL_T", "STR_T", "LPR_T", "RPR_T", "LBR_T",
	"RBR_T", "KW_T", "EOS_T", "RTE_T", "SEOF_T", "CMT_T", "FL_T",
	"AO_T", "EQ_T", "FUNC_T", "VAR_T", "THREAD_T", "TYPE_T", "CALL_T",
	"COM_T", "REL_T"
};

ish_thread keywordTable[KWT_SIZE] = {
	"code", "data", "if", "else", "while", "for", "return", "funk", "v",
	"ish_intg", "ish_flop", "ish_thread", "print", "input", "longint", "TRUE",
	"FALSE"
};

/* Local(file) global objects - variables */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */
ish_thread numScannerErrors = NULL;
ScannerData scData;

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
ish_intg startScanner(BufferPointer psc_buf) {
	for (ish_intg i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(ish_void) {
	Token currentToken = { 0 };
	ish_cha c;

	while (1) {
		c = readerGetChar(sourceBuffer);
		if (c == ' ' || c == '\t' || c == '\f' || c == '\r')
			continue;
		if (c == '\n') {
			line++;
			continue;
		}
		break;
	}

	if (c == CHARSEOF0) {
		currentToken.code = SEOF_T;
		currentToken.attribute.seofType = SEOF_0;
		scData.scanHistogram[currentToken.code]++;
		return currentToken;
	}

	if (isalpha((unsigned char)c) || c == '_') {
		ish_cha lexeme[VID_LEN + 1] = { 0 };
		ish_intg length = 0;
		while (isalnum((unsigned char)c) || c == '_') {
			if (length < VID_LEN)
				lexeme[length++] = c;
			c = readerGetChar(sourceBuffer);
		}
		if (c != CHARSEOF0)
			readerRetract(sourceBuffer);

		for (ish_intg i = 0; i < KWT_SIZE; i++) {
			if (strcmp(lexeme, keywordTable[i]) == 0) {
				currentToken.code = KW_T;
				currentToken.attribute.codeType = i;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
		}

		currentToken.code = ID_T;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
		scData.scanHistogram[currentToken.code]++;
		return currentToken;
	}

	if (isdigit((unsigned char)c)) {
		ish_cha lexeme[ERR_LEN + 1] = { 0 };
		ish_intg length = 0;
		ish_bool isFloat = ISH_FALSE;

		while (isdigit((unsigned char)c) || c == '.') {
			if (c == '.') {
				if (isFloat)
					break;
				isFloat = ISH_TRUE;
			}
			if (length < ERR_LEN)
				lexeme[length++] = c;
			c = readerGetChar(sourceBuffer);
		}
		if (c != CHARSEOF0)
			readerRetract(sourceBuffer);

		if (isFloat) {
			currentToken.code = FL_T;
			currentToken.attribute.floatValue = strtof(lexeme, NULL);
		}
		else {
			currentToken.code = INL_T;
			currentToken.attribute.intValue = atoi(lexeme);
		}
		scData.scanHistogram[currentToken.code]++;
		return currentToken;
	}

	if (c == '"' || c == '\'') {
		ish_cha quote = c;
		currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
		c = readerGetChar(sourceBuffer);
		while (c != quote && c != CHARSEOF0) {
			if (c == '\n')
				line++;
			if (!readerAddChar(stringLiteralTable, c)) {
				currentToken.code = RTE_T;
				strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
				errorNumber = RTE_CODE;
				scData.scanHistogram[currentToken.code]++;
				return currentToken;
			}
			c = readerGetChar(sourceBuffer);
		}
		if (c != quote) {
			currentToken.code = ERR_T;
			strcpy(currentToken.attribute.errLexeme, "unterminated string literal");
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		}
		readerAddChar(stringLiteralTable, CHARSEOF0);
		currentToken.code = STR_T;
		scData.scanHistogram[currentToken.code]++;
		return currentToken;
	}

	if (c == '#') {
		ish_cha next = readerGetChar(sourceBuffer);
		if (next == '#') {
			while ((c = readerGetChar(sourceBuffer)) != '\n' && c != CHARSEOF0) {
				;
			}
			if (c == '\n')
				line++;
		}
		else {
			c = next;
			while (c != '#' && c != CHARSEOF0) {
				if (c == '\n')
					line++;
				c = readerGetChar(sourceBuffer);
			}
		}
		currentToken.code = CMT_T;
		scData.scanHistogram[currentToken.code]++;
		return currentToken;
	}

	switch (c) {
	case ';': currentToken.code = EOS_T; break;
	case ',': currentToken.code = COM_T; break;
	case '(': currentToken.code = LPR_T; break;
	case ')': currentToken.code = RPR_T; break;
	case '{': currentToken.code = LBR_T; break;
	case '}': currentToken.code = RBR_T; break;
	case '+': currentToken.code = AO_T; currentToken.attribute.arithmeticOperator = OP_ADD; break;
	case '-': currentToken.code = AO_T; currentToken.attribute.arithmeticOperator = OP_SUB; break;
	case '*': currentToken.code = AO_T; currentToken.attribute.arithmeticOperator = OP_MUL; break;
	case '/': currentToken.code = AO_T; currentToken.attribute.arithmeticOperator = OP_DIV; break;
	case '%': currentToken.code = AO_T; currentToken.attribute.arithmeticOperator = OP_MOD; break;
	case '=':
		if (readerGetChar(sourceBuffer) == '=') {
			currentToken.code = REL_T;
			currentToken.attribute.relationalOperator = OP_EQ;
		}
		else {
			readerRetract(sourceBuffer);
			currentToken.code = EQ_T;
		}
		break;
	case '<':
		if (readerGetChar(sourceBuffer) == '>') {
			currentToken.code = REL_T;
			currentToken.attribute.relationalOperator = OP_NE;
		}
		else {
			readerRetract(sourceBuffer);
			currentToken.code = REL_T;
			currentToken.attribute.relationalOperator = OP_LT;
		}
		break;
	case '>': currentToken.code = REL_T; currentToken.attribute.relationalOperator = OP_GT; break;
	default:
		currentToken.code = ERR_T;
		currentToken.attribute.errLexeme[0] = c;
		currentToken.attribute.errLexeme[1] = CHARSEOF0;
		break;
	}

	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

ish_void printToken(Token t) {
	extern ish_thread keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case ID_T:
		printf("ID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (ish_intg)t.attribute.contentString);
		printf("%s\n", readerGetContent(stringLiteralTable, (ish_intg)t.attribute.contentString));
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
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case FL_T:
		printf("FL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case AO_T:
		printf("AO_T\n");
		break;
	case EQ_T:
		printf("EQ_T\n");
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case REL_T:
		printf("REL_T\n");
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
ish_void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
