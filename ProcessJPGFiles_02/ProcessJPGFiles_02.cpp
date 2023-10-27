// read_file_names_console_app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <filesystem> // C++17 standard header file name
#include <sys/stat.h>
#include "ProcessFilesInDir.h"
//#include "EXIFInfo.h"



#define DEBUG_PrintFilePath 0

namespace fs = std::filesystem;

using namespace std;


int main()
{
    string directoryPath;
    string extension;
    string stem;
    string outfilename_str;
    string filename;
    bool foundJPG = false;
    bool foundJSON = false;
    int numFiles = 0;

    // Object of ProcessFileInDir class
    ProcessFilesInDir pfid;


    // Path to the directory
    //directoryPath = "F:\\git_repositories\\ProcessJPGFiles_02\\oliver";
    directoryPath = "C:\\Users\\lletham\\source\\repos\\ProcessJPGFiles_02\\oliver";


    // This class will distinguish a file from a directory
    struct stat sb;

    /*************************************************************/
    // Looping until all the items of the directory are exhausted
    numFiles = 0;
    for (const auto& entry : fs::directory_iterator(directoryPath)) {

        // Converting the path to const char * in the subsequent lines
        std::filesystem::path outfilename = entry.path();
        outfilename_str = outfilename.string();
        const char* path = outfilename_str.c_str();
        filename = outfilename_str.c_str();

        // Testing whether the path points to a
        // non-directory or not If it does, displays path
        if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {
            // path is pointing to a file
#if DEBUG_PrintFilePath == 1
            std::cout << path << std::endl;
#endif
            numFiles++; // count number of files found in the directory
        }

        // process, classify and store file names in a linked list
        pfid.storeFileFound(filename);
    }

    /*************************************************************/
    // Print out classification results of files found and classified
    pfid.outputFileInfo();

    std::cout << std::endl << "Total number of files in directory: " << numFiles << std::endl;

    /*************************************************************************************/
    // This is where the jpg and png files need to be modified to include the json information
    // here!!!!!!!!!!!!!!!!!!!!!!!!!!

     // Open input file
    ifstream infile;
    //const char jpgIFileName[31] = "0C875ED2-oliver-18664-000.JPEG";
    //const char jpgIFileName[28] = "01_nat_hist_london_0001.jpg";
    string jpgIFileName = "0C875ED2-oliver-18664-000.JPEG";
    bool inFileOpen = false;

    infile.open(jpgIFileName, ios::binary);
    if (infile.is_open()) {
        cout << jpgIFileName << "\texists" << endl;
        inFileOpen = true;
    }
    else {
        cout << jpgIFileName << "\tdoes NOT exist" << endl;
        inFileOpen = false;
    }

    // Open output file
    // Name is the same as the input file with "_fixed" apended onto the end
    ofstream outfile;
    string jpgOFileName = jpgIFileName;
    std::string key(".JP");
    std::size_t found = jpgOFileName.rfind(key);
    if (found != std::string::npos)
        jpgOFileName.replace(found, key.length(), "_fixed.jp");

    bool outFileOpen = false;
    outfile.open(jpgOFileName, ios::binary);
    if (outfile.is_open()) {
        cout << jpgOFileName << "\texists" << endl;
        outFileOpen = true;
    }
    else {
        cout << jpgOFileName << "\tdoes NOT exist" << endl;
        outFileOpen = false;
    }

    // get length of input file into length. Return to beginning of file.
    infile.seekg(0, ios::end);
    streamoff length = infile.tellg();
    infile.seekg(0, ios::beg);

    // Put the file  contents into a vector
    //template < class T, class Alloc = allocator<T> > class vector; // generic template
    vector<char> jpgVector;
    vector<char>::iterator it;

    //allocate memory
    char* jpgBuffer = new char[length + 1];

    ////read the data as a block into the buffer then close the file
    infile.read(jpgBuffer, length);

    //jpgVector.assign(&jpgBuffer, &jpgBuffer + length + 1);
    //infile.close();

    // print out jpgVector contents



    ////#define _CRT_SECURE_NO_WARNINGS

    //// here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ////// copy all input file into a vector
    ////vector<unsigned char> jpgIBuffer(istreambuf_interator<char>(input), {});
    ////const char jpgIFileName[28] = "01_nat_hist_london_0001.jpg";
    //string jpgOFileName = "01_nat_hist_london_0001_fix.jpg";

    //const char jpgIFileName[31] = "0C875ED2-oliver-18664-000.JPEG";
    ////const char jpgIFileName[28] = "01_nat_hist_london_0001.jpg";

    //cout << endl << endl;

    //// Read the JPEG file into a buffer
    //FILE* fp = fopen(jpgIFileName, "rb");
    //if (!fp) {
    //    printf("Can't open file.\n");
    //    return -1;
    //}
    //fseek(fp, 0, SEEK_END);
    //unsigned long fsize = ftell(fp);
    //rewind(fp);
    //unsigned char* buf = new unsigned char[fsize];
    //if (fread(buf, 1, fsize, fp) != fsize) {
    //    printf("Can't read file.\n");
    //    delete[] buf;
    //    return -2;
    //}
    //fclose(fp);

    //// Parse EXIF
    //easyexif::EXIFInfo result;
    //int code = result.parseFrom(buf, fsize);
    //delete[] buf;
    //if (code) {
    //    printf("Error parsing EXIF: code %d\n", code);
    //    return -3;
    //}

    //// Dump EXIF information
    //printf("Camera make          : %s\n", result.Make.c_str());
    //printf("Camera model         : %s\n", result.Model.c_str());
    //printf("Software             : %s\n", result.Software.c_str());
    //printf("Bits per sample      : %d\n", result.BitsPerSample);
    //printf("Image width          : %d\n", result.ImageWidth);
    //printf("Image height         : %d\n", result.ImageHeight);
    //printf("Image description    : %s\n", result.ImageDescription.c_str());
    //printf("Image orientation    : %d\n", result.Orientation);
    //printf("Image copyright      : %s\n", result.Copyright.c_str());
    //printf("Image date/time      : %s\n", result.DateTime.c_str());
    //printf("Original date/time   : %s\n", result.DateTimeOriginal.c_str());
    //printf("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
    //printf("Subsecond time       : %s\n", result.SubSecTimeOriginal.c_str());
    //printf("Exposure time        : 1/%d s\n",
    //    (unsigned)(1.0 / result.ExposureTime));
    //printf("F-stop               : f/%.1f\n", result.FNumber);
    //printf("Exposure program     : %d\n", result.ExposureProgram);
    //printf("ISO speed            : %d\n", result.ISOSpeedRatings);
    //printf("Subject distance     : %f m\n", result.SubjectDistance);
    //printf("Exposure bias        : %f EV\n", result.ExposureBiasValue);
    //printf("Flash used?          : %d\n", result.Flash);
    //printf("Flash returned light : %d\n", result.FlashReturnedLight);
    //printf("Flash mode           : %d\n", result.FlashMode);
    //printf("Metering mode        : %d\n", result.MeteringMode);
    //printf("Lens focal length    : %f mm\n", result.FocalLength);
    //printf("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
    //printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
    //    result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
    //    result.GeoLocation.LatComponents.minutes,
    //    result.GeoLocation.LatComponents.seconds,
    //    result.GeoLocation.LatComponents.direction);
    //printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
    //    result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
    //    result.GeoLocation.LonComponents.minutes,
    //    result.GeoLocation.LonComponents.seconds,
    //    result.GeoLocation.LonComponents.direction);
    //printf("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
    //printf("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
    //printf("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
    //printf("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
    //printf("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
    //printf("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
    //printf("Lens make            : %s\n", result.LensInfo.Make.c_str());
    //printf("Lens model           : %s\n", result.LensInfo.Model.c_str());
    //printf("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
    //printf("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);


    return 0;
}
