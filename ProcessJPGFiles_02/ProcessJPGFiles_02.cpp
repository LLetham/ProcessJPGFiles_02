// read_file_names_console_app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <filesystem> // C++17 standard header file name
#include <sys/stat.h>
#include "EXIFInfo_02.h"
#include "ProcessFilesInDir.h"



#define DEBUG_PrintFilePath 0

namespace fs = std::filesystem;
//ProcessExif pe;


using namespace std;


int main()
{
    //string directoryPath;
    string extension;
    string stem;
    string outfilename_str;
    string filename;
    bool foundJPG = false;
    bool foundJSON = false;
    int numFiles = 0;

    // Object of ProcessFileInDir class
    ProcessFilesInDir pfid;

    // This class will distinguish a file from a directory
    struct stat sb;

    /*************************************************************/
    // Looping until all the items of the directory are exhausted
    numFiles = 0;

    std::filesystem::path directoryPath = fs::current_path();

    // add subdirectory during testing only
    // remove this subdirector for the release version.
    directoryPath += "\\oliver";      // alternate way: directoryPath = directoryPath / "oliver";
    std::cout << "directoryPath " << directoryPath << std::endl;

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
#if DEBUG_PrintFilePath
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

    pfid.outputDateTakenInfo();

    /*************************************************************************************/
    // This is where the jpg files need to be modified to include the json information

    pfid.xferDateTimeToJPG();
    cout << "JPG file processing is complete" << endl;


     // Open input file
     // 
     // 
    string jpgIFileName = "test_hour_01_fixed.jpg";
    //string jpgIFileName = "test_hour_12.jpg";
    //string jpgIFileName = "test_hour_11_fixed.jpg";
    //string jpgIFileName = "test_hour_10_fixed.jpg";
    //string jpgIFileName = "test_hour_09_fixed.jpg";
    //string jpgIFileName = "test_hour_08_fixed.jpg";
    //string jpgIFileName = "test_hour_07_fixed.jpg";
    //string jpgIFileName = "test_hour_06_fixed.jpg";
    //string jpgIFileName = "test_hour_05_fixed.jpg";
    //string jpgIFileName = "test_hour_04_fixed.jpg";
    //string jpgIFileName = "5BD95E56-046C-4770-B917-16D1AB24D51B-38175-000_fixed.jpg";
    //string jpgIFileName = "4F20D0F2-C937-4BE3-BAF3-B94F0D76B43E-2333-0000_fixed.jpg";
    //string jpgIFileName = "2019-02-22(4)_fixed.jpg";
    //string jpgIFileName = "IMG_2889_fixed.jpg";
    //string jpgIFileName = "0C875ED2-3BE3-4312-9930-208718EA62B3-18664-000_fixed.jpg";

    
    basic_ifstream<unsigned char> testInfile;
    unsigned char* jpgTestBuffer = NULL;

    streamoff lengthTest;

    cout << "directory path :\t" << directoryPath << endl;
    testInfile.open(jpgIFileName, ios::binary);

    if (testInfile.is_open()) {
        cout << "Read file: " << jpgIFileName << "\tfound" << endl;

        // Get file size
        testInfile.seekg(0, ios::end);
        lengthTest = testInfile.tellg();
        testInfile.seekg(0, ios::beg);

        //allocate memory
        jpgTestBuffer = new unsigned char[lengthTest];

        // move file contents into the buffer
        testInfile.read(jpgTestBuffer, lengthTest);

        // close the file
        testInfile.close();
    }
    else {
        cout << "Read file: " << jpgIFileName << "\t NOT found" << endl;
    }


    // Parse EXIF to see if the date was successfully changed
    // Disable writing the date before reading the file.
    easyexif::EXIFInfo resultTest;
    resultTest.writeData("\0", 0);

    int code = resultTest.parseFrom(jpgTestBuffer, lengthTest);
    if (code) {
        printf("Error parsing EXIF: code %d\n", code);
        return -3;
    }


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
    printf("Image date/time      : %s\n", resultTest.DateTime.c_str());
    printf("Original date/time   : %s\n", resultTest.DateTimeOriginal.c_str());
    printf("Digitize date/time   : %s\n", resultTest.DateTimeDigitized.c_str());
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


    delete[] jpgTestBuffer;


    return 0;
}
