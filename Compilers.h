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
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

// Debug mode flag
#define DEBUG 0

// Language name
#define STR_LANGNAME    "Ish"

// Logical constants
#define ISH_TRUE  1
#define ISH_FALSE 0

// Data type typedefs for Ish language

//BASE DATATYPES
typedef int             ish_intg;       // Integer data type
typedef float           ish_flop;       // Floating point data type
typedef char*           ish_thread;     // String data type
typedef char            ish_cha;        // Character data type
typedef unsigned char   ish_bool;       // Boolean data type
//ADDITIONAL DATATYPES
typedef float           ish_real;       // Floating point data type
typedef void            ish_void;       // Void data type
typedef long            ish_long;       // Long integer data type
typedef double          ish_doub;       // Double precision floating point data type
typedef unsigned char   ish_byte;       // Byte data type
//SPECIFIC DATATYPES
// PENDING Isha WAS HEREEEEEEE

// Size and range definitions for Ish data types
#define ISH_NUMI_SIZE   4
#define ISH_NUMI_MIN    (-2147483648)
#define ISH_NUMI_MAX    2147483647

#define ISH_FLOP_SIZE   4
// Floating point range: Approximately ±3.4E-38 to ±3.4E+38

// String size is variable and limited by memory
// Char size is variable (usually 1 byte) and limited to ASCII range

#define ISH_LONINT_SIZE 8
#define ISH_LONINT_MIN  (-9223372036854775808LL)
#define ISH_LONINT_MAX  9223372036854775807LL

// Programs
enum PROGRAMS {
    PGM_READER = 'r',  // Reader program
    PGM_SCANNER = 's',  // Scanner program
    PGM_PARSER = 'p'   // Parser program
};

// Function signatures
ish_intg mainReader(ish_intg argc, ish_thread* argv);   // Main function for the Reader program
ish_intg mainScanner(ish_intg argc, ish_thread* argv);  // Main function for the Scanner program
ish_intg mainParser(ish_intg argc, ish_thread* argv);   // Main function for the Parser program

// Function to print logo
ish_void printLogo();

#endif
