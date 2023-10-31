#pragma once

#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>


#define FILE_TOO_SHORT              0x0001      // problem with jpg file
#define JPG_SOI_NOT_FOUND           0x0002      //0xFFD8 not found
#define JPG_EOI_NOT_FOUND           0x0003      //0xFFD9 not found
#define JPG_APP1_NOT_FOUND          0x0004      //0xFFE1 not found
#define JPG_EXIF_NOT_FOUND          0x0005      // string "Exif\0\0" not found
#define TIFF_ENDIANNESS_NOT_FOUND   0x0006      //II or MM not found
#define TIFF_0x002a_NOT_FOUND       0x0007      //0x002a not found
#define GOOD                        0x0008      //0x002a not found


using namespace std;


class ProcessExif
{
private:
    streamoff currentPtr = 0;
    streamoff SOIPtr = 0;
    streamoff EOIPtr;
    streamoff SOF0Ptr;
    streamoff SOF2Ptr;
    streamoff DHTPtr;
    streamoff DQTPtr;
    streamoff DRIPtr;
    streamoff SOSPtr;
    streamoff APP1Ptr;
    streamoff TIFFPtr;
    streamoff COMPtr;

    unsigned short exifSize;
    unsigned long offsetIFD0;
    bool alignIntel;


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
    int verifyJPGFile(unsigned char* buf, streamoff length) {
        currentPtr = 0;
        SOIPtr = 0;
        EOIPtr = 0;
        SOF0Ptr = 0;
        SOF2Ptr = 0;
        DHTPtr = 0;
        DQTPtr = 0;
        DRIPtr = 0;
        SOSPtr = 0;
        APP1Ptr = 0;
        COMPtr = 0;
        uint16_t failureFlags = 0;


        // All jpg files start with 0xFFD8. Verify file not too small to include 0xFFD8.
        if (!buf || length < 4) {
            cout << "File too small to be valid jpg file" << endl;
            return FILE_TOO_SHORT;
        }

        // Verify 0xFFD8 is at the beginning.
        if ((buf[currentPtr] != 0xFF) || (buf[currentPtr + 1] != 0xD8)) {
            cout << "jpg SOI marker 0xFFD8 not found" << endl;
            return JPG_SOI_NOT_FOUND;
        }
        else {
            printf("JPG SOI found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
        }

        // Search for 0xFFD9 at or near the end of the file. If needed search up to 
        // within 2 bytes of the start of the file.
        currentPtr = length - 2;
        while ((currentPtr > 2) && (failureFlags == 0x00)) {
            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD9)) {
                printf("JPG EOI found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
                break;
            }

            currentPtr--;
        }

        if (currentPtr == 2) {
            cout << "jpg EOI marker 0xFFD9 not found" << endl;
            return JPG_EOI_NOT_FOUND;
        }
    
        // Search for different headers in the jpg file
        currentPtr = 0;
        while ((currentPtr != (length - 2)) && failureFlags == 0x00) {
            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xC0)) {
                SOF0Ptr = currentPtr;
                printf("JPG SOF0 found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xC2)) {
                SOF2Ptr = currentPtr;
                printf("JPG SOF2 found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xC4)) {
                DHTPtr = currentPtr;
                printf("JPG DHT found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xDB)) {
                DQTPtr = currentPtr;
                printf("JPG DQT found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xDD)) {
                DRIPtr = currentPtr;
                printf("JPG DRI found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xDA)) {
                SOSPtr = currentPtr;
                printf("JPG SOS found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD0) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD1) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD2) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD3) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD4) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD5) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD6) ||
                (buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xD7)) 
            {
                printf("JPG RSTx found: %X%X\t but not marked\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            /*****************************************************/
            // App1 header:
            //  2 bytes:    0xFFE1
            //  2 bytes:    APP1 size
            //  6 bytes:    "Exif\0\0" string
            //  2 bytes:    TIFF header (Endianess: little: II or big: MM)
            //  2 bytes:    TIFF magic (0x2a00 big endian order)
            //  4 bytes:    Offset to first IFD0
            // ----------
            // 18 bytes
            /******************************************************/
            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xE1)) {
                if (!std::equal(buf[4], buf[9], "Exif\0\0")) {
                    return JPG_EXIF_NOT_FOUND;
                }
                else {
                    exifSize = parse_value<uint16_t>(buf + 2, false);
                }

                if ((buf[currentPtr + 10] == 'I') && (buf[currentPtr + 11] == 'I')) {
                    alignIntel = true;
                }
                else if ((buf[currentPtr + 10] == 'M') && (buf[currentPtr + 11] == 'M')) {
                    alignIntel = false;
                }
                else {
                    return TIFF_ENDIANNESS_NOT_FOUND;
                }

                if (0x2a != parse_value<uint16_t>(buf + 12, alignIntel))
                    return TIFF_0x002a_NOT_FOUND;

                offsetIFD0 = parse_value<uint32_t>(buf + 16, alignIntel);


                // The above verifies that the APP1 is valid, so mark the start of APP1.
                // Also mark the start of the TIFF header.
                APP1Ptr = currentPtr;
                TIFFPtr = currentPtr + 10;

                printf("JPG APP1 found: %X%X\t at%08X\n", buf[currentPtr], buf[currentPtr + 1], unsigned int(APP1Ptr));
            }

            //if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xE2)) {
            //    APP2Ptr = currentPtr;
            //    printf("JPG APP2 found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            //}

            if ((buf[currentPtr] == 0xFF) && (buf[currentPtr + 1] == 0xFE)) {
                COMPtr = currentPtr;
                printf("JPG COM found: %X%X\n", buf[currentPtr], buf[currentPtr + 1]);
            }

            currentPtr++;
        }

        return(GOOD);    
    }

    /****************************************************************/
    // Functions for parsing data from the buffer into a variable.
    // Accounts for endianness and number of bits.
    template <typename T>
    T parse_value(unsigned char* buf, bool alignIntel) {
        if (alignIntel) {
            return parse<T, true>(buf);
        }
        else {
            return parse<T, false>(buf);
        }
    }

    template <typename T, bool alignIntel>
    T parse(const unsigned char* buf);

    template <>
    uint8_t parse<uint8_t, false>(const unsigned char* buf) {
        return *buf;
    }

    template <>
    uint8_t parse<uint8_t, true>(const unsigned char* buf) {
        return *buf;
    }

    template <>
    uint16_t parse<uint16_t, false>(const unsigned char* buf) {
        return (static_cast<uint16_t>(buf[0]) << 8) | buf[1];
    }

    template <>
    uint16_t parse<uint16_t, true>(const unsigned char* buf) {
        return (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
    }

    template <>
    uint32_t parse<uint32_t, false>(const unsigned char* buf) {
        return (static_cast<uint32_t>(buf[0]) << 24) |
            (static_cast<uint32_t>(buf[1]) << 16) |
            (static_cast<uint32_t>(buf[2]) << 8) | buf[3];
    }

    template <>
    uint32_t parse<uint32_t, true>(const unsigned char* buf) {
        return (static_cast<uint32_t>(buf[3]) << 24) |
            (static_cast<uint32_t>(buf[2]) << 16) |
            (static_cast<uint32_t>(buf[1]) << 8) | buf[0];
    }



};

