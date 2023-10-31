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
//#include "ProcessExif.h"
#include "EXIFInfo_02.h"



#define DEBUG_PrintFilePath 0

namespace fs = std::filesystem;
//ProcessExif pe;


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
    directoryPath = "F:\\git_repositories\\ProcessJPGFiles_02\\oliver";
    //directoryPath = "C:\\Users\\lletham\\source\\repos\\ProcessJPGFiles_02\\oliver";


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
    //ifstream infile;
    string jpgIFileName = "01_nat_hist_london_0033.jpg";
    //string jpgIFileName = "01_nat_hist_london_0001.jpg";
    //string jpgIFileName = "0C875ED2-oliver-18664-000.JPEG";
    //string jpgIFileName = "guatemala_125222-SL-002.jpg";
    //string jpgIFileName = "guatemala_125222-SL-003.jpg";
    //string jpgIFileName = "tanya_20230523_142345.jpg";
    //string jpgIFileName = "tanya_20230523_142359.jpg";
    //string jpgIFileName = "tanya_20230523_142415.jpg";
    //string jpgIFileName = "date_test_data_01.txt";

    
    basic_ifstream<unsigned char> infile;
    unsigned char* jpgBuffer = NULL;

    streamoff length;

    infile.open(jpgIFileName, ios::binary);

    if (infile.is_open()) {
        cout << "Read file: " << jpgIFileName << "\tfound" << endl;

        // Get file size
        infile.seekg(0, ios::end);
        length = infile.tellg();
        infile.seekg(0, ios::beg);

        //allocate memory
       jpgBuffer = new unsigned char[length];

        // move file contents into the buffer
        infile.read(jpgBuffer, length);

        // close the file
        infile.close();
    }
    else {
        cout << "Read file: " << jpgIFileName << "\t NOT found" << endl;
    }

    //// Open output file
    //// Name is the same as the input file with "_fixed" apended onto the end
    //basic_ofstream<unsigned char> outfile;
    //string jpgOFileName = jpgIFileName;
    //std::string key(".JP");
    //std::size_t found = jpgOFileName.rfind(key);
    //if (found != std::string::npos)
    //    jpgOFileName.replace(found, key.length(), "_fixed.jp");

    //bool outFileOpen = false;
    //outfile.open(jpgOFileName, ios::binary);
    //if (outfile.is_open()) {
    //    cout << jpgOFileName << "\texists" << endl;
    //    outFileOpen = true;
    //}
    //else {
    //    cout << jpgOFileName << "\tdoes NOT exist" << endl;
    //    outFileOpen = false;
    //}

    //pe.printXBytes(jpgBuffer, 0, 0x4f);
    //if (uint8_t tmp = pe.verifyJPGFile(jpgBuffer, length) != 0) {
    //    printf("File is not valid jpg file. Error code: %X", tmp);
    //};



    // Parse EXIF
    easyexif::EXIFInfo result;
    //unsigned char writeData[20] = "12:34:5678 9a:bc:de\0";
    result.writeData("12:34:5678 9a:bc:de\0");
    int code = result.parseFrom(jpgBuffer, length);
    //delete[] jpgBuffer;
    if (code) {
        printf("Error parsing EXIF: code %d\n", code);
        return -3;
    }

    // Dump EXIF information
    printf("Camera make          : %s\n", result.Make.c_str());
    printf("Camera model         : %s\n", result.Model.c_str());
    printf("Software             : %s\n", result.Software.c_str());
    printf("Bits per sample      : %d\n", result.BitsPerSample);
    printf("Image width          : %d\n", result.ImageWidth);
    printf("Image height         : %d\n", result.ImageHeight);
    printf("Image description    : %s\n", result.ImageDescription.c_str());
    printf("Image orientation    : %d\n", result.Orientation);
    printf("Image copyright      : %s\n", result.Copyright.c_str());
    printf("Image date/time      : %s\n", result.DateTime.c_str());
    printf("Original date/time   : %s\n", result.DateTimeOriginal.c_str());
    printf("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
    printf("Subsecond time       : %s\n", result.SubSecTimeOriginal.c_str());
    printf("Exposure time        : 1/%d s\n",
        (unsigned)(1.0 / result.ExposureTime));
    printf("F-stop               : f/%.1f\n", result.FNumber);
    printf("Exposure program     : %d\n", result.ExposureProgram);
    printf("ISO speed            : %d\n", result.ISOSpeedRatings);
    printf("Subject distance     : %f m\n", result.SubjectDistance);
    printf("Exposure bias        : %f EV\n", result.ExposureBiasValue);
    printf("Flash used?          : %d\n", result.Flash);
    printf("Flash returned light : %d\n", result.FlashReturnedLight);
    printf("Flash mode           : %d\n", result.FlashMode);
    printf("Metering mode        : %d\n", result.MeteringMode);
    printf("Lens focal length    : %f mm\n", result.FocalLength);
    printf("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
    printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
        result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
        result.GeoLocation.LatComponents.minutes,
        result.GeoLocation.LatComponents.seconds,
        result.GeoLocation.LatComponents.direction);
    printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
        result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
        result.GeoLocation.LonComponents.minutes,
        result.GeoLocation.LonComponents.seconds,
        result.GeoLocation.LonComponents.direction);
    printf("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
    printf("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
    printf("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
    printf("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
    printf("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
    printf("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
    printf("Lens make            : %s\n", result.LensInfo.Make.c_str());
    printf("Lens model           : %s\n", result.LensInfo.Model.c_str());
    printf("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
    printf("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);


    delete[] jpgBuffer;


    return 0;
}
