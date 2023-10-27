#pragma once

#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>


using namespace std;


class ProcessExif
{
private:
    uint16_t startPtr = 0;
    uint16_t endPtr;
    uint16_t exifStart;


public:

    /********************************************************************/
    // Print out values from the buffer in hex. This is used as a debug
    // tool to see what is being found in the input file.
    /********************************************************************/
    void printXBytes(unsigned char* buf, unsigned int offs, int numBytes) {
        int x = 0;
        for (int i = 0; i < numBytes + 1; i++) {
            if (i % 0x10 == 0) printf("%08x: ", offs);
            printf("%02X ", buf[offs + i]);
            x++;
            if (x == 0x10) {
                printf("\n");
                x = 0;
            }
        }
    }

    /*********************************************************************/
    // Verify that the jpg file is properly formed and that it has an Exif
    // header. Mark the location of the various portions of the jpg file
    // in variables that are maintained in the class.
    /*********************************************************************/

};

