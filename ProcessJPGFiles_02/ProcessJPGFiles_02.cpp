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


#define DEBUG_directory 0

#define DEBUG_PrintFilePath 0
#define DEBUG_outputDateTakenInfo 0
#define DEBUG_outputFileInfo 0


namespace fs = std::filesystem;
//ProcessExif pe;


using namespace std;


int main()
{
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

#if DEBUG_directory == 1

    // add subdirectory during testing only
    // remove this subdirector for the release version.
    directoryPath += "\\oliver";      // alternate way: directoryPath = directoryPath / "oliver";
#endif

    std::cout << "Searching for jpg and json files in directoryPath " << directoryPath << std::endl;

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

    cout << "Files in directory processed to find matching jpg and json files" << endl;
    std::cout << std::endl << "Total number of files in directory: " << numFiles << std::endl << endl;

    /*************************************************************/
    // Print out classification results of files found and classified
#if DEBUG_outputFileInfo == 1
    cout << "Summary of files found" << endl;
    pfid.outputFileInfo();
    std::cout << std::endl << "Total number of files in directory: " << numFiles << std::endl << endl;
#endif


#if DEBUG_outputDateTakenInfo == 1
    cout << "DateTaken information retreived from json files" << endl;
    pfid.outputDateTakenInfo();
#endif

    /*************************************************************************************/
    // This is where the jpg files need to be modified to include the json information
    cout << "Transfer json dates to matching jpg files" << endl;
    pfid.xferDateTimeToJPG();
    cout << "Trasfer complete" << endl << endl;

    // Verify that the json dates were actually written to the jpg files
    cout << "Verify json dates written into the jpg files" << endl;
    pfid.verify9003DateTimeInJPG();
    cout << "Verification complete" << endl << endl;

    return 0;
}
