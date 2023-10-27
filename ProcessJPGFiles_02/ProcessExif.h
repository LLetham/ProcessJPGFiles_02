#pragma once

#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>



using namespace std;


class ProcessExif
{
private:
    //vector<char>::iterator it;


public:




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


};

