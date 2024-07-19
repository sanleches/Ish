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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */

#define FLAG_EMP 0x01  // Bit 0: Buffer empty flag (0000.0001)_2
#define FLAG_FUL 0x02  // Bit 1: Buffer full flag (0000.0010)_2
#define FLAG_REL 0x04  // Bit 2: Reallocation memory flag (0000.0100)_2
#define FLAG_END 0x08  // Bit 3: End of buffer flag (0000.1000)_2

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* DONE: Adjust datatypes - DONE by SAntiago */

/* Offset declaration */
typedef struct position {
	ish_intg mark;			/* the offset to the mark position (in chars) */
	ish_intg read;			/* the offset to the get a char position (in chars) */
	ish_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	ish_thread	content;			/* pointer to the beginning of character array (character buffer) */
	ish_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	ish_intg		increment;			/* character array increment factor */
	ish_intg		mode;				/* operational mode indicator */
	ish_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;				/* Offset / position field */
	ish_intg		histogram[NCHAR];	/* Statistics of chars */
	ish_intg		numReaderErrors;	/* Number of errors from Reader */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate(ish_intg, ish_intg, ish_intg);
BufferPointer	readerAddChar(BufferPointer const, ish_cha);
ish_bool		readerClear(BufferPointer const);
ish_bool		readerFree(BufferPointer const);
ish_bool		readerIsFull(BufferPointer const);
ish_bool		readerIsEmpty(BufferPointer const);
ish_bool		readerSetMark(BufferPointer const, ish_intg);
ish_intg		readerPrint(BufferPointer const);
ish_intg		readerLoad(BufferPointer const, FILE* const);
ish_bool		readerRecover(BufferPointer const);
ish_bool		readerRetract(BufferPointer const);
ish_bool		readerRestore(BufferPointer const);
ish_void		readerChecksum(BufferPointer const);
/* Getters */
ish_cha		    readerGetChar(BufferPointer const);
ish_thread	    readerGetContent(BufferPointer const, ish_intg);
ish_intg		readerGetPosRead(BufferPointer const);
ish_intg		readerGetPosWrte(BufferPointer const);
ish_intg		readerGetPosMark(BufferPointer const);
ish_intg		readerGetSize(BufferPointer const);
ish_intg		readerGetInc(BufferPointer const);
ish_intg		readerGetMode(BufferPointer const);
ish_byte		readerGetFlags(BufferPointer const);
ish_bool		readerPrintStat(BufferPointer const);
ish_intg		readerNumErrors(BufferPointer const);

#endif