#include "debug.h"
#include "constants.h"

void initDebug()
{
    #ifdef DEBUG
    HCMAX7219.Clear();
    #endif
    return;
}

void debug(char TextString[], unsigned int Offset)
{
    #ifdef DEBUG
    HCMAX7219.Clear();
    HCMAX7219.print7Seg(TextString, Offset);
    HCMAX7219.Refresh();
    #endif
    return;
}

void debug(long number, unsigned int Offset)
{
    #ifdef DEBUG
    HCMAX7219.Clear();
    HCMAX7219.print7Seg(number, Offset);
    HCMAX7219.Refresh();
    #endif
    return;
}
