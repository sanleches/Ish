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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.	DONE
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/
BufferPointer readerCreate(ish_intg size, ish_intg increment, ish_intg mode) {
	BufferPointer readerPointer;
	// Checking all parameters
	if (size <= 0)
		size = READER_DEFAULT_SIZE;
	if (increment <= 0)
		increment = READER_DEFAULT_INCREMENT;
	if (mode != MODE_FIXED && mode != MODE_ADDIT && mode != MODE_MULTI)
		mode = MODE_FIXED;

	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));
	if (!readerPointer) return NULL;

	readerPointer->content = (ish_thread)malloc(size);
	if (!readerPointer->content) {
		free(readerPointer);
		return NULL;
	}

	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	readerPointer->flags = FLAG_EMP;

	readerPointer->content[0] = READER_TERMINATOR;
	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, ish_cha ch) {
	if (!readerPointer || readerIsFull(readerPointer)) {
		return NULL; // Defensive programming
	}

	if (readerPointer->position.wrte * sizeof(ish_cha) >= readerPointer->size) {
		ish_intg newSize = readerPointer->size + readerPointer->increment;
		ish_thread tempReader = (ish_thread)realloc(readerPointer->content, newSize);
		if (!tempReader) {
			return NULL; // Defensive programming
		}
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}

	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->flags &= ~FLAG_EMP; // Clear the empty flag
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerClear(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.wrte = 0;
	readerPointer->position.mark = 0;
	readerPointer->position.read = 0;
	readerPointer->flags = FLAG_EMP; // Set the empty flag
	return ISH_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerFree(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	if (readerPointer->content) {
		free(readerPointer->content);
	}
	free(readerPointer);
	return ISH_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerIsFull(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	return (readerPointer->position.wrte * sizeof(ish_cha) >= readerPointer->size);
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerIsEmpty(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	return (readerPointer->position.wrte == 0);
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerSetMark(BufferPointer const readerPointer, ish_intg mark) {
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.mark = mark;
	return ISH_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ish_intg readerPrint(BufferPointer const readerPointer) {
	if (!readerPointer || readerIsEmpty(readerPointer)) {
		return 0; // Defensive programming
	}

	ish_intg cont = 0;
	ish_cha c = readerGetChar(readerPointer);

	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerLoad(BufferPointer const readerPointer, FILE* const fileDescriptor) {
	if (!readerPointer || !fileDescriptor) {
		return READER_ERROR; // Defensive programming
	}

	ish_intg size = 0;
	ish_cha c = (ish_cha)fgetc(fileDescriptor);

	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (ish_cha)fgetc(fileDescriptor);
		size++;
	}
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerRecover(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.read = 0;
	return ISH_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerRetract(BufferPointer const readerPointer) {
	if (!readerPointer || readerPointer->position.read <= 0) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.read--;
	return ISH_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerRestore(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.read = readerPointer->position.mark;
	return ISH_TRUE;
}


/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_cha readerGetChar(BufferPointer const readerPointer) {
	if (!readerPointer || readerPointer->position.read >= readerPointer->position.wrte) {
		return READER_TERMINATOR; // Defensive programming
	}

	return readerPointer->content[readerPointer->position.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_cha* readerGetContent(BufferPointer const readerPointer, ish_intg pos) {
	if (!readerPointer || pos < 0 || pos >= readerPointer->position.wrte) {
		return NULL; // Defensive programming
	}

	return readerPointer->content + pos;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerGetPosRead(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerGetPosWrte(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerGetPosMark(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerGetSize(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerGetInc(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerGetMode(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->mode;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_byte readerGetFlags(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return 0; // Defensive programming
	}

	return readerPointer->flags;
}



/*
***********************************************************
* Function name: readerPrintStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_bool readerPrintStat(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	printf("Printing buffer statistics:\n");
	printf("Write position: %d\n", readerPointer->position.wrte);
	printf("Read position: %d\n", readerPointer->position.read);
	printf("Mark position: %d\n", readerPointer->position.mark);
	printf("Size: %d\n", readerPointer->size);
	printf("Increment: %d\n", readerPointer->increment);
	printf("Mode: %d\n", readerPointer->mode);
	return ISH_TRUE;
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ish_intg readerNumErrors(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return READER_ERROR; // Defensive programming
	}

	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ish_void readerChecksum(BufferPointer readerPointer) {
	if (!readerPointer) {
		return; // Defensive programming
	}

	// Example checksum calculation logic
	ish_byte checksum = 0;

	for (ish_intg i = 0; i < readerPointer->position.wrte; i++) {
		checksum ^= readerPointer->content[i]; // XOR all characters
	}

	checksum &= 0x0F; // Only keep the lowest 4 bits
	readerPointer->flags = (readerPointer->flags & 0xF0) | checksum; // Update the lowest 4 bits in flags

	return;
}

/*
***********************************************************
* Function name: readerRewind
* Purpose: Rewinds the buffer. ADDITIONAL TO ORIGINAL DOCUMENT
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
*************************************************************
*/

ish_bool readerRewind(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.read = 0;
	return ISH_TRUE;
}

/*
***********************************************************
* Function name: readerReset
* Purpose: Resets the buffer. ADDITIONAL
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
ish_bool readerReset(BufferPointer const readerPointer) {
	if (!readerPointer) {
		return ISH_FALSE; // Defensive programming
	}

	readerPointer->position.read = readerPointer->position.mark;
	return ISH_TRUE;
}