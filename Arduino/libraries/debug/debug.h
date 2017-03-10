#ifndef DEBUG_H
#define DEBUG_H

#include "constants.h"

#ifdef DEBUG
    /* Include the HCMAX7219 and SPI library */
    #include <HCMAX7219.h>
    #include "SPI.h"

    /* Set the LOAD (CS) digital pin number*/
    #define LOAD 10
    
    /* Create an instance of the library */
    HCMAX7219 HCMAX7219(LOAD);
#endif

void initDebug();
void debug(char TextString[], unsigned int Offset);
void debug(long number, unsigned int Offset);

#endif
