/**************************************************************************
  exif.h  -- A simple ISO C++ library to parse basic EXIF
             information from a JPEG file.

  Based on the description of the EXIF file format at:
  -- http://park2.wakwak.com/~tsuruzoh/Computer/Digicams/exif-e.html
  -- http://www.media.mit.edu/pia/Research/deepview/exif.html
  -- http://www.exif.org/Exif2-2.PDF

  Copyright (c) 2010-2016 Mayank Lahiri
  mlahiri@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  -- Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  -- Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
   NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
   OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __EXIF_H
#define __EXIF_H

#include <string>

namespace easyexif {

    //
    // Class responsible for storing and parsing EXIF information from a JPEG blob
    //
    class EXIFInfo {
    public:
        // Parsing function for an entire JPEG image buffer.
        //
        // PARAM 'data': A pointer to a JPEG image.
        // PARAM 'length': The length of the JPEG image.
        // RETURN:  PARSE_EXIF_SUCCESS (0) on succes with 'result' filled out
        //          error code otherwise, as defined by the PARSE_EXIF_ERROR_* macros
        int parseFrom(unsigned char* data, unsigned length);
        int parseFrom(std::string& data);

        // Parsing function for an EXIF segment. This is used internally by parseFrom()
        // but can be called for special cases where only the EXIF section is
        // available (i.e., a blob starting with the bytes "Exif\0\0").
        int parseFromEXIFSegment(unsigned char* buf, unsigned len);

        // Set all data members to default values.
        void clear();

        // Print out bytes from buffer
        void printXBytes(unsigned char* buf, unsigned int offs, int numBytes);


        // Data fields filled out by parseFrom()
        char ByteAlign;                   // 0 = Motorola byte alignment, 1 = Intel
        std::string ImageDescription;     // Image description
        std::string Make;                 // Camera manufacturer's name
        std::string Model;                // Camera model
        unsigned short Orientation;       // Image orientation, start of data corresponds to
        // 0: unspecified in EXIF data
        // 1: upper left of image
        // 3: lower right of image
        // 6: upper right of image
        // 8: lower left of image
        // 9: undefined
        unsigned short BitsPerSample;     // Number of bits per component
        std::string Software;             // Software used
        std::string DateTime;             // File change date and time
        std::string DateTimeOriginal;     // Original file date and time (may not exist)
        std::string DateTimeDigitized;    // Digitization date and time (may not exist)
        std::string SubSecTimeOriginal;   // Sub-second time that original picture was taken
        std::string Copyright;            // File copyright information
        double ExposureTime;              // Exposure time in seconds
        double FNumber;                   // F/stop
        unsigned short ExposureProgram;   // Exposure program
        // 0: Not defined
        // 1: Manual
        // 2: Normal program
        // 3: Aperture priority
        // 4: Shutter priority
        // 5: Creative program
        // 6: Action program
        // 7: Portrait mode
        // 8: Landscape mode
        unsigned short ISOSpeedRatings;   // ISO speed
        double ShutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
        double ExposureBiasValue;         // Exposure bias value in EV
        double SubjectDistance;           // Distance to focus point in meters
        double FocalLength;               // Focal length of lens in millimeters
        unsigned short FocalLengthIn35mm; // Focal length in 35mm film
        char Flash;                       // 0 = no flash, 1 = flash used
        unsigned short FlashReturnedLight;// Flash returned light status
        // 0: No strobe return detection function
        // 1: Reserved
        // 2: Strobe return light not detected
        // 3: Strobe return light detected
        unsigned short FlashMode;         // Flash mode
        // 0: Unknown
        // 1: Compulsory flash firing
        // 2: Compulsory flash suppression
        // 3: Automatic mode
        unsigned short MeteringMode;      // Metering mode
        // 1: average
        // 2: center weighted average
        // 3: spot
        // 4: multi-spot
        // 5: multi-segment
        unsigned ImageWidth;              // Image width reported in EXIF data
        unsigned ImageHeight;             // Image height reported in EXIF data
        struct Geolocation_t {            // GPS information embedded in file
            double Latitude;                  // Image latitude expressed as decimal
            double Longitude;                 // Image longitude expressed as decimal
            double Altitude;                  // Altitude in meters, relative to sea level
            char AltitudeRef;                 // 0 = above sea level, -1 = below sea level
            double DOP;                       // GPS degree of precision (DOP)
            struct Coord_t {
                double degrees;
                double minutes;
                double seconds;
                char direction;
            } LatComponents, LonComponents;   // Latitude, Longitude expressed in deg/min/sec
        } GeoLocation;
        struct LensInfo_t {               // Lens information
            double FStopMin;                // Min aperture (f-stop)
            double FStopMax;                // Max aperture (f-stop)
            double FocalLengthMin;          // Min focal length (mm)
            double FocalLengthMax;          // Max focal length (mm)
            double FocalPlaneXResolution;   // Focal plane X-resolution
            double FocalPlaneYResolution;   // Focal plane Y-resolution
            unsigned short FocalPlaneResolutionUnit; // Focal plane resolution unit
            // 1: No absolute unit of measurement.
            // 2: Inch.
            // 3: Centimeter.
            // 4: Millimeter.
            // 5: Micrometer.
            std::string Make;               // Lens manufacturer
            std::string Model;              // Lens model
        } LensInfo;


        EXIFInfo() {
            clear();
        }
    };

}

// Parse was successful
#define PARSE_EXIF_SUCCESS                    0
// No JPEG markers found in buffer, possibly invalid JPEG file
#define PARSE_EXIF_ERROR_NO_JPEG              1982
// No EXIF header found in JPEG file.
#define PARSE_EXIF_ERROR_NO_EXIF              1983
// Byte alignment specified in EXIF file was unknown (not Motorola or Intel).
#define PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN    1984
// EXIF header was found, but data was corrupted.
#define PARSE_EXIF_ERROR_CORRUPT              1985

#endif


/**************************************************************************
  exif.cpp  -- A simple ISO C++ library to parse basic EXIF
               information from a JPEG file.

  Copyright (c) 2010-2015 Mayank Lahiri
  mlahiri@gmail.com
  All rights reserved (BSD License).

  See exif.h for version history.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  -- Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  -- Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
  NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//#include "exif.h"

#include <algorithm>
#include <cstdint>
#include <stdio.h>
#include <vector>

using std::string;

namespace {

    struct Rational {
        uint32_t numerator, denominator;
        operator double() const {
            if (denominator < 1e-20) {
                return 0;
            }
            return static_cast<double>(numerator) / static_cast<double>(denominator);
        }
    };

    // IF Entry
    class IFEntry {
    public:
        using byte_vector = std::vector<uint8_t>;
        using ascii_vector = std::string;
        using short_vector = std::vector<uint16_t>;
        using long_vector = std::vector<uint32_t>;
        using rational_vector = std::vector<Rational>;

        IFEntry()
            : tag_(0xFF), format_(0xFF), data_(0), length_(0), val_byte_(nullptr) {}
        IFEntry(const IFEntry&) = delete;
        IFEntry& operator=(const IFEntry&) = delete;
        IFEntry(IFEntry&& other)
            : tag_(other.tag_),
            format_(other.format_),
            data_(other.data_),
            length_(other.length_),
            val_byte_(other.val_byte_) {
            other.tag_ = 0xFF;
            other.format_ = 0xFF;
            other.data_ = 0;
            other.length_ = 0;
            other.val_byte_ = nullptr;
        }
        ~IFEntry() { delete_union(); }
        unsigned short tag() const { return tag_; }
        void tag(unsigned short tag) { tag_ = tag; }
        unsigned short format() const { return format_; }
        bool format(unsigned short format) {
            switch (format) {
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x07:
            case 0x09:
            case 0x0a:
            case 0xff:
                break;
            default:
                return false;
            }
            delete_union();
            format_ = format;
            new_union();
            return true;
        }
        unsigned data() const { return data_; }
        void data(unsigned data) { data_ = data; }
        unsigned length() const { return length_; }
        void length(unsigned length) { length_ = length; }

        /***************************************************/
        // functions to access the data
        //
        // !! it's CALLER responsibility to check that format !!
        // !! is correct before accessing it's field          !!
        //
        // - getters are use here to allow future addition
        //   of checks if format is correct
        byte_vector& val_byte() {
            return *val_byte_;
        }

        ascii_vector& val_string() {
            return *val_string_;
        }

        short_vector& val_short() {
            return *val_short_;
        }

        long_vector& val_long() {
            return *val_long_;
        }

        rational_vector& val_rational() {
            return *val_rational_;
        }

    private:
        // Raw fields
        unsigned short tag_;
        unsigned short format_;
        unsigned data_;
        unsigned length_;

        // Parsed fields
        union {
            byte_vector* val_byte_;
            ascii_vector* val_string_;
            short_vector* val_short_;
            long_vector* val_long_;
            rational_vector* val_rational_;
        };

        void delete_union() {
            switch (format_) {
            case 0x1:
                delete val_byte_;
                val_byte_ = nullptr;
                break;
            case 0x2:
                delete val_string_;
                val_string_ = nullptr;
                break;
            case 0x3:
                delete val_short_;
                val_short_ = nullptr;
                break;
            case 0x4:
                delete val_long_;
                val_long_ = nullptr;
                break;
            case 0x5:
                delete val_rational_;
                val_rational_ = nullptr;
                break;
            case 0xff:
                break;
            default:
                // should not get here
                // should I throw an exception or ...?
                break;
            }
        }
        void new_union() {
            switch (format_) {
            case 0x1:
                val_byte_ = new byte_vector();
                break;
            case 0x2:
                val_string_ = new ascii_vector();
                break;
            case 0x3:
                val_short_ = new short_vector();
                break;
            case 0x4:
                val_long_ = new long_vector();
                break;
            case 0x5:
                val_rational_ = new rational_vector();
                break;
            case 0xff:
                break;
            default:
                // should not get here
                // should I throw an exception or ...?
                break;
            }
        }
    };

    /*****************************************************/
    // Helper functions
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

    template <>
    Rational parse<Rational, true>(const unsigned char* buf) {
        Rational r;
        r.numerator = parse<uint32_t, true>(buf);
        r.denominator = parse<uint32_t, true>(buf + 4);
        return r;
    }

    template <>
    Rational parse<Rational, false>(const unsigned char* buf) {
        Rational r;
        r.numerator = parse<uint32_t, false>(buf);
        r.denominator = parse<uint32_t, false>(buf + 4);
        return r;
    }

    /**********************************************************/
     //* Try to read entry.length() values for this entry.
     //*
     //* Returns:
     //*  true  - entry.length() values were read
     //*  false - something went wrong, vec's content was not touched
     /***********************************************************************/
    template <typename T, bool alignIntel, typename C>
    bool extract_values(C& container, const unsigned char* buf, const unsigned base, const unsigned len, const IFEntry& entry) {
        const unsigned char* data;
        uint32_t reversed_data;
        // if data fits into 4 bytes, they are stored directly in
        // the data field in IFEntry
        if (sizeof(T) * entry.length() <= 4) {
            if (alignIntel) {
                reversed_data = entry.data();
            }
            else {
                reversed_data = entry.data();
                // this reversing works, but is ugly
                unsigned char* rdata = reinterpret_cast<unsigned char*>(&reversed_data);
                unsigned char tmp;
                tmp = rdata[0];
                rdata[0] = rdata[3];
                rdata[3] = tmp;
                tmp = rdata[1];
                rdata[1] = rdata[2];
                rdata[2] = tmp;
            }
            data = reinterpret_cast<const unsigned char*>(&(reversed_data));
        }
        else {
            data = buf + base + entry.data();
            if (data + sizeof(T) * entry.length() > buf + len) {
                return false;
            }
        }
        container.resize(entry.length());
        for (size_t i = 0; i < entry.length(); ++i) {
            container[i] = parse<T, alignIntel>(data + sizeof(T) * i);
        }
        return true;
    }

    template <bool alignIntel>
    void parseIFEntryHeader(const unsigned char* buf, unsigned short& tag,
        unsigned short& format, unsigned& length,
        unsigned& data) {
        // Each directory entry is composed of:
        // 2 bytes: tag number (data field)
        // 2 bytes: data format
        // 4 bytes: number of components
        // 4 bytes: data value or offset to data value
        tag = parse<uint16_t, alignIntel>(buf);
        format = parse<uint16_t, alignIntel>(buf + 2);
        length = parse<uint32_t, alignIntel>(buf + 4);
        data = parse<uint32_t, alignIntel>(buf + 8);
    }

    template <bool alignIntel>
    void parseIFEntryHeader(const unsigned char* buf, IFEntry& result) {
        unsigned short tag;
        unsigned short format;
        unsigned length;
        unsigned data;

        parseIFEntryHeader<alignIntel>(buf, tag, format, length, data);

        result.tag(tag);
        result.format(format);
        result.length(length);
        result.data(data);
    }

    template <bool alignIntel>
    IFEntry parseIFEntry_temp(const unsigned char* buf, const unsigned offs,
        const unsigned base, const unsigned len) {
        IFEntry result;

        // check if there even is enough data for IFEntry in the buffer
        if (buf + offs + 12 > buf + len) {
            result.tag(0xFF);
            return result;
        }

        parseIFEntryHeader<alignIntel>(buf + offs, result);

        // Parse value in specified format
        switch (result.format()) {
        case 1:
            if (!extract_values<uint8_t, alignIntel>(result.val_byte(), buf, base,
                len, result)) {
                result.tag(0xFF);
            }
            break;
        case 2:
            // string is basically sequence of uint8_t (well, according to EXIF even
            // uint7_t, but
            // we don't have that), so just read it as bytes
            if (!extract_values<uint8_t, alignIntel>(result.val_string(), buf, base, len, result)) {
                result.tag(0xFF);
            }
            // and cut zero byte at the end, since we don't want that in the
            // std::string
            if (result.val_string()[result.val_string().length() - 1] == '\0') {
                result.val_string().resize(result.val_string().length() - 1);
            }
            break;
        case 3:
            if (!extract_values<uint16_t, alignIntel>(result.val_short(), buf, base, len, result)) {
                result.tag(0xFF);
            }
            break;
        case 4:
            if (!extract_values<uint32_t, alignIntel>(result.val_long(), buf, base, len, result)) {
                result.tag(0xFF);
            }
            break;
        case 5:
            if (!extract_values<Rational, alignIntel>(result.val_rational(), buf, base, len, result)) {
                result.tag(0xFF);
            }
            break;
        case 7:
        case 9:
        case 10:
            break;
        default:
            result.tag(0xFF);
        }
        return result;
    }

    /*************************************************************/
    // helper functions for convinience
    template <typename T>
    T parse_value(const unsigned char* buf, bool alignIntel) {
        if (alignIntel) {
            return parse<T, true>(buf);
        }
        else {
            return parse<T, false>(buf);
        }
    }

    void parseIFEntryHeader(const unsigned char* buf, bool alignIntel,
        unsigned short& tag, unsigned short& format,
        unsigned& length, unsigned& data) {
        if (alignIntel) {
            parseIFEntryHeader<true>(buf, tag, format, length, data);
        }
        else {
            parseIFEntryHeader<false>(buf, tag, format, length, data);
        }
    }

    IFEntry parseIFEntry(const unsigned char* buf, const unsigned offs,
        const bool alignIntel, const unsigned base,
        const unsigned len) {
        if (alignIntel) {
            return parseIFEntry_temp<true>(buf, offs, base, len);
        }
        else {
            return parseIFEntry_temp<false>(buf, offs, base, len);
        }
    }
}

/***********************************************************/
//
// Locates the EXIF segment and parses it using parseFromEXIFSegment
//
int easyexif::EXIFInfo::parseFrom(unsigned char* buf, unsigned len) {
    // Sanity check: all JPEG files start with 0xFFD8.
    if (!buf || len < 4) return PARSE_EXIF_ERROR_NO_JPEG;

    printXBytes(buf, 0, 0xf);

    if (buf[0] != 0xFF || buf[1] != 0xD8)
        return PARSE_EXIF_ERROR_NO_JPEG;
    else
        //std::cout << "SOI 0xFFD8 found" << std::endl;

    // Sanity check: some cameras pad the JPEG image with some bytes at the end.
    // Normally, we should be able to find the JPEG end marker 0xFFD9 at the end
    // of the image buffer, but not always. As long as there are some bytes
    // except 0xD9 at the end of the image buffer, keep decrementing len until
    // an 0xFFD9 is found. If JPEG end marker 0xFFD9 is not found,
    // then we can be reasonably sure that the buffer is not a JPEG.

    printXBytes(buf, len - 0xf, 0xf);


    while (len > 2) {
        if (buf[len - 1] == 0xD9 && buf[len - 2] == 0xFF)
            break;
        //else
        //    std::cout << "EOI 0xFFD9 found" << std::endl;

        len--;
    }
    if (len <= 2)
        return PARSE_EXIF_ERROR_NO_JPEG;

    clear();

    // Scan for EXIF header (bytes 0xFF 0xE1) and do a sanity check by
    // looking for bytes "Exif\0\0". The marker length data is in Motorola
    // byte order, which results in the 'false' parameter to parse16().
    // The marker has to contain at least the TIFF header, otherwise the
    // EXIF data is corrupt. So the minimum length specified here has to be:
    //   2 bytes: section size
    //   6 bytes: "Exif\0\0" string
    //   2 bytes: TIFF header (either "II" or "MM" string)
    //   2 bytes: TIFF magic (short 0x2a00 in Motorola byte order)
    //   4 bytes: Offset to first IFD
    // =========
    //  16 bytes
    unsigned int offs = 0;  // current offset into buffer

    // Search in the buffer until 0xFFE1 start of APP1 is found
    for (offs = 0; offs < len - 1; offs++) {
        printXBytes(buf, offs, 0xf);

        if (buf[offs] == 0xFF && buf[offs + 1] == 0xE1)
            break;
    }

    printf("APP1 found: buf[%X]: %X\tbuf[%X]: %X\n", offs, buf[offs], offs + 1, buf[offs + 1]);

    if (offs + 4 > len)
        return PARSE_EXIF_ERROR_NO_EXIF;
    offs += 2;              // move past 0xFFE1 in buffer
    printXBytes(buf, offs, 0xf);

    // here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unsigned short section_length = parse_value<uint16_t>(buf + offs, false);
    printf("\nsection_length =        %08X\n", section_length);
    printf("section_length + offs = %08X\n", section_length + offs);

    if (offs + section_length > len || section_length < 16)
        return PARSE_EXIF_ERROR_CORRUPT;
    offs += 2;

    printXBytes(buf, offs, 0xf);

    int tmp = parseFromEXIFSegment(buf + offs, len - offs);

    return (tmp);
}

int easyexif::EXIFInfo::parseFrom(string& data) {
    return parseFrom(reinterpret_cast<unsigned char*>(data.data()), static_cast<unsigned>(data.length()));
}

//
// Main parsing function for an EXIF segment.
//
// PARAM: 'buf' start of the EXIF TIFF, which must be the bytes "Exif\0\0".
// PARAM: 'len' length of buffer
//
int easyexif::EXIFInfo::parseFromEXIFSegment(unsigned char* buf, unsigned len) {
    bool alignIntel = true;  // byte alignment (defined in EXIF header)
    unsigned offs = 0;       // current offset into buffer. At start of EXIF block
    if (!buf || len < 6)
        return PARSE_EXIF_ERROR_NO_EXIF;

    if (!std::equal(buf, buf + 6, "Exif\0\0"))
        return PARSE_EXIF_ERROR_NO_EXIF;
    offs += 6;              // move past "Exif\0\0" in buffer

    printXBytes(buf, offs, 0xf);


    // Now parsing the TIFF header. The first two bytes are either "II" or
    // "MM" for Intel or Motorola byte alignment. Sanity check by parsing
    // the unsigned short that follows, making sure it equals 0x2a. The
    // last 4 bytes are an offset into the first IFD, which are added to
    // the global offset counter. For this block, we expect the following
    // minimum size:
    //  2 bytes: 'II' or 'MM'
    //  2 bytes: 0x002a
    //  4 bytes: offset to first IDF
    // -----------------------------
    //  8 bytes
    if (offs + 8 > len) return PARSE_EXIF_ERROR_CORRUPT;
    unsigned tiff_header_start = offs;
    printf("tiff_header_start = %08X\n", tiff_header_start);
    printXBytes(buf, offs, 0xf);


    if (buf[offs] == 'I' && buf[offs + 1] == 'I')
        alignIntel = true;
    else {
        if (buf[offs] == 'M' && buf[offs + 1] == 'M')
            alignIntel = false;
        else
            return PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN;
    }
    this->ByteAlign = alignIntel;
    offs += 2;              // move past the MM or II in the buffer
    printXBytes(buf, offs, 0xf);

    if (0x2a != parse_value<uint16_t>(buf + offs, alignIntel))
        return PARSE_EXIF_ERROR_CORRUPT;
    offs += 2;
    printXBytes(buf, offs, 0xf);

    unsigned first_ifd_offset = parse_value<uint32_t>(buf + offs, alignIntel);
    printf("first_ifd_offset = %08X\n", first_ifd_offset);

    offs += first_ifd_offset - 4;
    printXBytes(buf, offs, 0xf);

    if (offs >= len) return PARSE_EXIF_ERROR_CORRUPT;



    // Now parsing the first Image File Directory (IFD0, for the main image).
    // An IFD consists of a variable number of 12-byte directory entries. The
    // first two bytes of the IFD section contain the number of directory
    // entries in the section. The last 4 bytes of the IFD contain an offset
    // to the next IFD, which means this IFD must contain exactly 6 + 12 * num
    // bytes of data.
    if (offs + 2 > len) return PARSE_EXIF_ERROR_CORRUPT;
    int num_entries = parse_value<uint16_t>(buf + offs, alignIntel);
    printf("num_entries = %08X\n", num_entries);

    if (offs + 6 + 12 * num_entries > len) return PARSE_EXIF_ERROR_CORRUPT;
    offs += 2;
    printXBytes(buf, offs, 0xf);

    unsigned exif_sub_ifd_offset = len;
    printf("exif_sub_ifd_offset = %08X\n", exif_sub_ifd_offset);

    unsigned gps_sub_ifd_offset = len;
    printf("gps_sub_ifd_offset = %08X\n", gps_sub_ifd_offset);

    while (--num_entries >= 0) {
        IFEntry result = parseIFEntry(buf, offs, alignIntel, tiff_header_start, len);
        offs += 12;
        printXBytes(buf, offs, 0xf);

        switch (result.tag()) {
        case 0x102:
            // Bits per sample
            if (result.format() == 3 && result.val_short().size())
                this->BitsPerSample = result.val_short().front();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x10E:
            // Image description
            if (result.format() == 2)
                this->ImageDescription = result.val_string();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x10F:
            // Digicam make
            if (result.format() == 2)
                this->Make = result.val_string();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x110:
            // Digicam model
            if (result.format() == 2)
                this->Model = result.val_string();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x112:
            // Orientation of image
            if (result.format() == 3 && result.val_short().size())
                this->Orientation = result.val_short().front();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x131:
            // Software used for image
            if (result.format() == 2)
                this->Software = result.val_string();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x132:
            // EXIF/TIFF date/time of image modification
            if (result.format() == 2)
                this->DateTime = result.val_string();
            break;

        case 0x8298:
            // Copyright information
            if (result.format() == 2)
                this->Copyright = result.val_string();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x8825:
            // GPS IFS offset
            gps_sub_ifd_offset = tiff_header_start + result.data();

            printXBytes(buf, offs, 0xf);

            break;

        case 0x8769:
            // EXIF SubIFD offset
            exif_sub_ifd_offset = tiff_header_start + result.data();
            printf("exif_sub_ifd_offset = %08X\n", exif_sub_ifd_offset);

            printXBytes(buf, offs, 0xf);

            break;
        }
    }

    // Jump to the EXIF SubIFD if it exists and parse all the information
    // there. Note that it's possible that the EXIF SubIFD doesn't exist.
    // The EXIF SubIFD contains most of the interesting information that a
    // typical user might want.
    if (exif_sub_ifd_offset + 4 <= len) {
        offs = exif_sub_ifd_offset;
        printXBytes(buf, offs, 0xf);

        int num_sub_entries = parse_value<uint16_t>(buf + offs, alignIntel);
        printf("num_sub_entries = %08X\n", num_sub_entries);

        if (offs + 6 + 12 * num_sub_entries > len) return PARSE_EXIF_ERROR_CORRUPT;
        offs += 2;
        printXBytes(buf, offs, 0xf);

        while (--num_sub_entries >= 0) {
            IFEntry result =
                parseIFEntry(buf, offs, alignIntel, tiff_header_start, len);
            switch (result.tag()) {

                // IFD attribute info 2
            case 0x829a:
                // Exposure time in seconds
                if (result.format() == 5 && result.val_rational().size())
                    this->ExposureTime = result.val_rational().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x829d:
                // FNumber
                if (result.format() == 5 && result.val_rational().size())
                    this->FNumber = result.val_rational().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x8822:
                // Exposure Program
                if (result.format() == 3 && result.val_short().size())
                    this->ExposureProgram = result.val_short().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x8827:
                // ISO Speed Rating
                if (result.format() == 3 && result.val_short().size())
                    this->ISOSpeedRatings = result.val_short().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 1
            case 0x9003:
                // Original date and time
                if (result.format() == 2)
                    this->DateTimeOriginal = result.val_string();

                // here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                printXBytes(buf, offs, 0xf);
                //buf[offs] = 0x03;

                break;

                // IFD attribute info 1
            case 0x9004:
                // Digitization date and time
                if (result.format() == 2)
                    this->DateTimeDigitized = result.val_string();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x9201:
                // Shutter speed value
                if (result.format() == 5 && result.val_rational().size())
                    this->ShutterSpeedValue = result.val_rational().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x9204:
                // Exposure bias value
                if (result.format() == 5 && result.val_rational().size())
                    this->ExposureBiasValue = result.val_rational().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x9206:
                // Subject distance
                if (result.format() == 5 && result.val_rational().size())
                    this->SubjectDistance = result.val_rational().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x9207:
                // Metering mode
                if (result.format() == 3 && result.val_short().size())
                    this->MeteringMode = result.val_short().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0x9209:
                // Flash used
                if (result.format() == 3 && result.val_short().size()) {
                    uint16_t data = result.val_short().front();

                    this->Flash = data & 1;
                    this->FlashReturnedLight = (data & 6) >> 1;
                    this->FlashMode = (data & 24) >> 3;

                    printXBytes(buf, offs, 0xf);

                }
                break;

                // IFD attribute info 2
            case 0x920a:
                // Focal length
                if (result.format() == 5 && result.val_rational().size())
                    this->FocalLength = result.val_rational().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 1
            case 0x9291:
                // Subsecond original time
                if (result.format() == 2)
                    this->SubSecTimeOriginal = result.val_string();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 1
            case 0xa002:
                // EXIF Image width
                if (result.format() == 4 && result.val_long().size())
                    this->ImageWidth = result.val_long().front();

                printXBytes(buf, offs, 0xf);

                if (result.format() == 3 && result.val_short().size())
                    this->ImageWidth = result.val_short().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 1
            case 0xa003:
                // EXIF Image height
                if (result.format() == 4 && result.val_long().size())
                    this->ImageHeight = result.val_long().front();
                if (result.format() == 3 && result.val_short().size())
                    this->ImageHeight = result.val_short().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info 2
            case 0xa20e:
                // EXIF Focal plane X-resolution
                if (result.format() == 5) {
                    this->LensInfo.FocalPlaneXResolution = result.val_rational()[0];

                    printXBytes(buf, offs, 0xf);

                }
                break;

                // IFD attribute info 2
            case 0xa20f:
                // EXIF Focal plane Y-resolution
                if (result.format() == 5) {
                    this->LensInfo.FocalPlaneYResolution = result.val_rational()[0];

                    printXBytes(buf, offs, 0xf);

                }
                break;

                // IFD attribute info 2
            case 0xa210:
                // EXIF Focal plane resolution unit
                if (result.format() == 3 && result.val_short().size()) {
                    this->LensInfo.FocalPlaneResolutionUnit = result.val_short().front();

                    printXBytes(buf, offs, 0xf);

                }
                break;

                // IFD attribute info 2
            case 0xa405:
                // Focal length in 35mm film
                if (result.format() == 3 && result.val_short().size())
                    this->FocalLengthIn35mm = result.val_short().front();

                printXBytes(buf, offs, 0xf);

                break;

                // IFD attribute info ?????
            case 0xa432:
                // Focal length and FStop.
                if (result.format() == 5) {
                    int sz = static_cast<unsigned>(result.val_rational().size());
                    if (sz)
                        this->LensInfo.FocalLengthMin = result.val_rational()[0];
                    if (sz > 1)
                        this->LensInfo.FocalLengthMax = result.val_rational()[1];
                    if (sz > 2)
                        this->LensInfo.FStopMin = result.val_rational()[2];
                    if (sz > 3)
                        this->LensInfo.FStopMax = result.val_rational()[3];

                    printXBytes(buf, offs, 0xf);

                }
                break;

                // IFD attribute info ?????
            case 0xa433:
                // Lens make.
                if (result.format() == 2) {
                    this->LensInfo.Make = result.val_string();

                    printXBytes(buf, offs, 0xf);

                }
                break;

                // IFD attribute info ?????
            case 0xa434:
                // Lens model.
                if (result.format() == 2) {
                    this->LensInfo.Model = result.val_string();

                    printXBytes(buf, offs, 0xf);

                }
                break;
            }
            offs += 12;
            printXBytes(buf, offs, 0xf);

        }
    }

    // Jump to the GPS SubIFD if it exists and parse all the information
    // there. Note that it's possible that the GPS SubIFD doesn't exist.
    if (gps_sub_ifd_offset + 4 <= len) {
        printf("gps_sub_ifd_offset = %08X\n", gps_sub_ifd_offset);

        offs = gps_sub_ifd_offset;
        int num_sub_entries = parse_value<uint16_t>(buf + offs, alignIntel);
        printf("num_sub_entries = %08X\n", num_sub_entries);

        if (offs + 6 + 12 * num_sub_entries > len) return PARSE_EXIF_ERROR_CORRUPT;
        offs += 2;
        printXBytes(buf, offs, 0xf);

        while (--num_sub_entries >= 0) {
            unsigned short tag, format;
            unsigned length, data;
            parseIFEntryHeader(buf + offs, alignIntel, tag, format, length, data);
            printf("tag = %08X\n", tag);
            printf("format = %08X\n", format);
            printf("length = %08X\n", length);

            switch (tag) {
            case 1:
                // GPS north or south
                this->GeoLocation.LatComponents.direction = *(buf + offs + 8);
                if (this->GeoLocation.LatComponents.direction == 0) {
                    this->GeoLocation.LatComponents.direction = '?';
                }
                if ('S' == this->GeoLocation.LatComponents.direction) {
                    this->GeoLocation.Latitude = -this->GeoLocation.Latitude;
                }

                printXBytes(buf, offs, 0xf);

                break;

            case 2:
                // GPS latitude
                if ((format == 5 || format == 10) && length == 3) {
                    this->GeoLocation.LatComponents.degrees = parse_value<Rational>(
                        buf + data + tiff_header_start, alignIntel);
                    this->GeoLocation.LatComponents.minutes = parse_value<Rational>(
                        buf + data + tiff_header_start + 8, alignIntel);
                    this->GeoLocation.LatComponents.seconds = parse_value<Rational>(
                        buf + data + tiff_header_start + 16, alignIntel);
                    this->GeoLocation.Latitude =
                        this->GeoLocation.LatComponents.degrees +
                        this->GeoLocation.LatComponents.minutes / 60 +
                        this->GeoLocation.LatComponents.seconds / 3600;
                    if ('S' == this->GeoLocation.LatComponents.direction) {
                        this->GeoLocation.Latitude = -this->GeoLocation.Latitude;

                        printXBytes(buf, offs, 0xf);

                    }
                }
                break;

            case 3:
                // GPS east or west
                this->GeoLocation.LonComponents.direction = *(buf + offs + 8);
                if (this->GeoLocation.LonComponents.direction == 0) {
                    this->GeoLocation.LonComponents.direction = '?';
                }
                if ('W' == this->GeoLocation.LonComponents.direction) {
                    this->GeoLocation.Longitude = -this->GeoLocation.Longitude;

                    printXBytes(buf, offs, 0xf);

                }
                break;

            case 4:
                // GPS longitude
                if ((format == 5 || format == 10) && length == 3) {
                    this->GeoLocation.LonComponents.degrees = parse_value<Rational>(
                        buf + data + tiff_header_start, alignIntel);
                    this->GeoLocation.LonComponents.minutes = parse_value<Rational>(
                        buf + data + tiff_header_start + 8, alignIntel);
                    this->GeoLocation.LonComponents.seconds = parse_value<Rational>(
                        buf + data + tiff_header_start + 16, alignIntel);
                    this->GeoLocation.Longitude =
                        this->GeoLocation.LonComponents.degrees +
                        this->GeoLocation.LonComponents.minutes / 60 +
                        this->GeoLocation.LonComponents.seconds / 3600;
                    if ('W' == this->GeoLocation.LonComponents.direction)
                        this->GeoLocation.Longitude = -this->GeoLocation.Longitude;

                    printXBytes(buf, offs, 0xf);

                }
                break;

            case 5:
                // GPS altitude reference (below or above sea level)
                this->GeoLocation.AltitudeRef = *(buf + offs + 8);
                if (1 == this->GeoLocation.AltitudeRef) {
                    this->GeoLocation.Altitude = -this->GeoLocation.Altitude;

                    printXBytes(buf, offs, 0xf);

                }
                break;

            case 6:
                // GPS altitude
                if ((format == 5 || format == 10)) {
                    this->GeoLocation.Altitude = parse_value<Rational>(
                        buf + data + tiff_header_start, alignIntel);
                    if (1 == this->GeoLocation.AltitudeRef) {
                        this->GeoLocation.Altitude = -this->GeoLocation.Altitude;
                    }

                    printXBytes(buf, offs, 0xf);

                }
                break;

            case 11:
                // GPS degree of precision (DOP)
                if ((format == 5 || format == 10)) {
                    this->GeoLocation.DOP = parse_value<Rational>(
                        buf + data + tiff_header_start, alignIntel);
                }

                printXBytes(buf, offs, 0xf);

                break;
            }
            offs += 12;
            printXBytes(buf, offs, 0xf);

        }
    }

    return PARSE_EXIF_SUCCESS;
}

void easyexif::EXIFInfo::printXBytes(unsigned char* buf, unsigned int offs, int numBytes) {
    // here!!!!!!!!!!!!!!!!!!!!!!!!
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

void easyexif::EXIFInfo::clear() {
    // Strings
    ImageDescription = "";
    Make = "";
    Model = "";
    Software = "";
    DateTime = "";
    DateTimeOriginal = "";
    DateTimeDigitized = "";
    SubSecTimeOriginal = "";
    Copyright = "";

    // Shorts / unsigned / double
    ByteAlign = 0;
    Orientation = 0;

    BitsPerSample = 0;
    ExposureTime = 0;
    FNumber = 0;
    ExposureProgram = 0;
    ISOSpeedRatings = 0;
    ShutterSpeedValue = 0;
    ExposureBiasValue = 0;
    SubjectDistance = 0;
    FocalLength = 0;
    FocalLengthIn35mm = 0;
    Flash = 0;
    FlashReturnedLight = 0;
    FlashMode = 0;
    MeteringMode = 0;
    ImageWidth = 0;
    ImageHeight = 0;

    // Geolocation
    GeoLocation.Latitude = 0;
    GeoLocation.Longitude = 0;
    GeoLocation.Altitude = 0;
    GeoLocation.AltitudeRef = 0;
    GeoLocation.DOP = 0;
    GeoLocation.LatComponents.degrees = 0;
    GeoLocation.LatComponents.minutes = 0;
    GeoLocation.LatComponents.seconds = 0;
    GeoLocation.LatComponents.direction = '?';
    GeoLocation.LonComponents.degrees = 0;
    GeoLocation.LonComponents.minutes = 0;
    GeoLocation.LonComponents.seconds = 0;
    GeoLocation.LonComponents.direction = '?';

    // LensInfo
    LensInfo.FocalLengthMax = 0;
    LensInfo.FocalLengthMin = 0;
    LensInfo.FStopMax = 0;
    LensInfo.FStopMin = 0;
    LensInfo.FocalPlaneYResolution = 0;
    LensInfo.FocalPlaneXResolution = 0;
    LensInfo.FocalPlaneResolutionUnit = 0;
    LensInfo.Make = "";
    LensInfo.Model = "";
}