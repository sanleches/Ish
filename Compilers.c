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
* File name: compilers.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: May 01 2024
* Professor: Paulo Sousa
* Purpose: This file is the main program of Compilers Project
* Function list: main().
************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

BufferPointer stringLiteralTable = NULL;
ish_intg errorNumber = 0;

ish_intg main(int argc, char** argv) {
    ish_intg i;
    printLogo();
    if (DEBUG) {
        for (i = 0; i < argc; ++i)
            printf("argv[%d] = %s\n", i, argv[i]);
    }
    if (argc < 2) {
        printf("%s%c%s%c%s%c%s", "OPTIONS:\n* [",
            PGM_READER, "] - Reader\n* [",
            PGM_SCANNER, "] - Scanner\n* [",
            PGM_PARSER, "] - Parser\n");
        return EXIT_FAILURE;
    }
    ish_cha option = argv[1][0];
    switch (option) {
    case PGM_READER:
        printf("%s%c%s", "\n[Option '", PGM_READER, "': Starting READER .....]\n\n");
        mainReader(argc, argv);
        break;
    
    case PGM_SCANNER:
        printf("%s%c%s", "\n[Option '", PGM_SCANNER, "': Starting SCANNER .....]\n\n");
        mainScanner(argc, argv);
        break;
    case PGM_PARSER:
        printf("%s%c%s", "\n[Option '", PGM_PARSER, "': Starting PARSER .....]\n\n");
        mainParser(argc, argv);
        break;
    
    default:
        printf("%s%c%s%c%s%c%s", "* OPTIONS:\n- [",
            PGM_READER, "] - Reader\n- [",
            PGM_SCANNER, "] - Scanner\n- [",
            PGM_PARSER, "] - Parser\n");
        break;
    }
    return EXIT_SUCCESS;
}

ish_void  printLogo() {
    printf(
        "\t=------------------------------------------------------=\n"
        "\t|  ISH LANGUAGE COMPILER                              |\n"
        "\t=------------------------------------------------------=\n"
        );
}
