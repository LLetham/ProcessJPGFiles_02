#pragma once

//#include <iostream>
//#include <fstream>
//#include <string>
//#include <cstring>
//#include <cmath>
//#include <stdio.h>
//#include <ctime>
//#include <stdlib.h>
//#include <list>
//
//#include <filesystem> // C++17 standard header file name
//#include <sys/stat.h>

#define DEBUG_processJSON 0
#define DEBUG_processDATE 0
#define DEBUG_processJPGFiles 1


using namespace std;


// Data structure to hold information regarding the file names in the
// dirctory. If a jpg file is found with a matching json file, the data
// from the json file needs to be extracted for later insertion into the
// matching jpg file.
// Extract the data from the json file regardless then use if a matching
// jpg file is found.
// If matching jpg file and json files are not found, then record what is found.
// If a non-jpg and/or non-json file is found, record its full path with stem.
// The inforamation on non-jpg and/or non-json files may not be needed, but
// it is recorded here for possible future use.
// Dates are for when the photo was taken.
// Date format in a jpg file in number acsii codes: YYYY:MM:DD HH:MM:SS'\0'
// Date format in json file in ascii codes: 
//		Jan 10, 2018, 9:21:49 PM UTC
//		Jan 31, 2018, 10:40:57 PM UTC
//		Mar 4, 2018, 12:10:13 AM UTC
//		Mar 2, 2018, 1:10:08 AM UTC
// Store json dates and times as ascii characters followed by NULL.
struct FileInfoRecord {
	char* fullPathFile = NULL;		// Name of file with full path name and extension.
	char* fullPathNoExt = NULL;		// Full path name of file without extension.
	char* stemName = NULL;			// File name before the extension
	int typeJPGFound = 0;			// A file with a .jpg extension found for the stem.
	int typeJSONFound = 0;			// A file with a .json extension found for the stem.
	int typePNGFound = 0;			// File is .png file
	int typeOtherFound = 0;			// File is non-jpg or non-json file
	char* fullPathOther = NULL;	// full path of non-jpg and/or non-json file with extension
	char takenMonth[4] = { NULL };
	char takenDay[3] = { NULL };
	char takenYear[5] = { NULL };
	char takenHour[3] = { NULL };
	char takenMin[3] = { NULL };
	char takenSec[3] = { NULL };
	char dateTaken[20] = {NULL};
};

/************************************************************************/
class ProcessFilesInDir
{

private:
	// variable to receive the full path file name with extension
	std::string fullFilePathName;
	std::string fullFilePathNameNoExt;
	std::string fileExtension;
	std::string fileStem;
	bool matchFound = false;
	std::string tmpStr;
	std::string tmpMonth;
	std::string tmpHour;

	// structure object to receive data for adding to a std::list
	struct FileInfoRecord fInfoNode;

	// make linked list objects for STL list
	std::list<FileInfoRecord> fileRecordList;
	std::list<FileInfoRecord>::iterator fileRecordListIterator;
	std::list<FileInfoRecord>::iterator nextItr;

	// count the number of files read from the directory
	int numFilesFound = 0;
	int numMatchedJPG = 0;
	int numMatchedPNG = 0;
	int numUnMatchedJPG = 0;
	int numUnMatchedPNG = 0;
	int numUnMatchedJSON = 0;
	int numOther = 0;
	int numTrio = 0;

	// JSON file handling
	std::ifstream infile;
	bool fileFound = false;
	std::string fLine;
	bool photoTakenTimeFound = false;

	int i;
	int j;
	int k;
	int charCount = 0;
	int strMatch = 0;
	char c;

	// map string month name (e.g., "Jan") to string number (e.g., "01")
	map<string, string> months{
		{"Jan", "01"},
		{"Feb", "02"},
		{"Mar", "03"},
		{"Apr", "04"},
		{"May", "05"},
		{"Jun", "06"},
		{"Jul", "07"},
		{"Aug", "08"},
		{"Sep", "09"},
		{"Oct", "10"},
		{"Nov", "11"},
		{"Dec", "12"}
	};

	// Map hour with AM/PM to characters in 24-hour time.
	// Conversion for takenHour[3] in struct
	map<char, string> hour12PMto9PM {
		{'1', "13"},
		{'2', "14"},
		{'3', "15"},
		{'4', "16"},
		{'5', "17"},
		{'6', "18"},
		{'7', "19"},
		{'8', "20"},
		{'9', "21"}
	};

	map<char, string> hour10PMto12AM{
		{'0', "22"},
		{'1', "23"},
		{'2', "12"}
	};

	// variables for processing jpg file
	basic_ifstream<unsigned char> infileJPG;
	unsigned char* jpgBuffer = NULL;
	string jpgIFileName;
	streamoff length;

	basic_ofstream<unsigned char> outfileJPG;
	string jpgOFileName;

public:

	/*****************************************************************/
	// compare characters until the '\0' character is reached in str1
	// str1 = std::string
	// str2 = char*
	// This enables comparison of a string to characters in a structure.
	// The string (i.e., str1) always has a '\0' added to the end.
	int cmpStrings(std::string str1, char* str2) {
		charCount = 0;
		strMatch = 1;
		while (!((str1[charCount] == '\0') || (str2[charCount] == '\0'))) {
			if (str1[charCount] != str2[charCount])
				strMatch = 0;
			// increment the pointer then see if either string is '\0' 
			// to not go past the end of the string.			
			charCount++;
			//if ((str1[charCount] == '\0') || (str2[charCount] == '\0')) 
				//contLoop = 0;
		}

		// This final check is for strings whose beginninng characters are the same.
		if (str1[charCount] != str2[charCount])
			strMatch = 0;

		return (strMatch);
	}

	/***************************************************************/
	// Traverse the list and print out the date the photograph was taken
	// as retreived from the json file for all jpg-json matching files.
	void outputDateTakenInfo() {

		fileRecordListIterator = fileRecordList.begin();
		numMatchedJPG = 0;

		std::cout << std::endl << "Matching .jpg and .json files" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 1) &&
				(fileRecordListIterator->typeJSONFound == 1) &&
				(fileRecordListIterator->typePNGFound == 0) &&
				(fileRecordListIterator->typeOtherFound == 0)) {

				std::cout << "dateTaken: " << fileRecordListIterator->dateTaken << "\t" << fileRecordListIterator->stemName << std::endl;

				numMatchedJPG++;
			}

			fileRecordListIterator++;
		}
		numMatchedJPG = numMatchedJPG;
		std::cout << "Number of records = " << numMatchedJPG << std::endl;

	}

	/*************************************************************/
	// Report the files that were found in the directory. Report:
	//		.jpg files with matching .json file
	//		.png files with matching .json file
	//		.json files with no matching .jpg or .png file
	//		.jpg files with no matcing .json file
	//		.png files with no matching .json file

	void outputFileInfo() {
		/******************************************/
		// 11111111111111111111111111111
		// Find matched .jpg & .json files
		fileRecordListIterator = fileRecordList.begin();
		numMatchedJPG = 0;

		std::cout << std::endl << "Matching .jpg and .json files" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 1) &&
				(fileRecordListIterator->typeJSONFound == 1) &&
				(fileRecordListIterator->typePNGFound == 0) &&
				(fileRecordListIterator->typeOtherFound == 0)) {
				std::cout << "\t" << fileRecordListIterator->stemName << ".jpg" << std::endl;
				numMatchedJPG++;
			}

			fileRecordListIterator++;
		}
		numMatchedJPG = numMatchedJPG * 2;
		std::cout << "Matching files = " << numMatchedJPG << std::endl;


		/******************************************/
		// Find matched .png & .json files
		// 222222222222222222222222222222222222
		fileRecordListIterator = fileRecordList.begin();
		numMatchedPNG = 0;

		std::cout << std::endl << "Matching .png and .json files" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 0) &&
				(fileRecordListIterator->typeJSONFound == 1) &&
				(fileRecordListIterator->typePNGFound == 1) &&
				(fileRecordListIterator->typeOtherFound == 0)) {
				std::cout << "\t" << fileRecordListIterator->stemName << ".png" << std::endl;
				numMatchedPNG++;
			}

			fileRecordListIterator++;
		}
		numMatchedPNG = numMatchedPNG * 2;
		std::cout << "Matching files = " << numMatchedPNG << std::endl;

		/******************************************/
		// 3333333333333333333333333333333333
		// Find .jpg files with no matching .json or .png
		fileRecordListIterator = fileRecordList.begin();
		numUnMatchedJPG = 0;

		std::cout << std::endl << ".jpg files with no matching .json file" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 1) &&
				(fileRecordListIterator->typeJSONFound == 0) &&
				(fileRecordListIterator->typePNGFound == 0) &&
				(fileRecordListIterator->typeOtherFound == 0)) {
				std::cout << "\t" << fileRecordListIterator->stemName << ".jpg" << std::endl;
				numUnMatchedJPG++;
			}

			fileRecordListIterator++;
		}

		std::cout << "Unmatched files = " << numUnMatchedJPG << std::endl;

		/******************************************/
		// 4444444444444444444444444444444444444
		// Find .png files with no matching .json
		fileRecordListIterator = fileRecordList.begin();
		numUnMatchedPNG = 0;

		std::cout << std::endl << ".png files with no matcing .json  or .jpg file" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 0) &&
				(fileRecordListIterator->typeJSONFound == 0) &&
				(fileRecordListIterator->typePNGFound == 1) &&
				(fileRecordListIterator->typeOtherFound == 0)) {
				std::cout << "\t" << fileRecordListIterator->stemName << ".png" << std::endl;
				numUnMatchedPNG++;
			}

			fileRecordListIterator++;
		}

		std::cout << "Unmatched files = " << numUnMatchedPNG << std::endl;

		/**************************************************/
		// 5555555555555555555555555555555555555
		// Find .json files with no matching .jpg or .png
		fileRecordListIterator = fileRecordList.begin();
		numUnMatchedJSON = 0;

		std::cout << std::endl << ".json files with no matching .jpg or .png file" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 0) &&
				(fileRecordListIterator->typeJSONFound == 1) &&
				(fileRecordListIterator->typePNGFound == 0) &&
				(fileRecordListIterator->typeOtherFound == 0)) {
				std::cout << "\t" << fileRecordListIterator->stemName << ".json" << std::endl;
				numUnMatchedJSON++;
			}

			fileRecordListIterator++;
		}

		std::cout << "Unmatched files = " << numUnMatchedJSON << std::endl;

		/**************************************************/
		// 66666666666666666666666666666666666666666666666
		// Number of other files: non-.jpg, non-.json, non-.png
		fileRecordListIterator = fileRecordList.begin();
		numOther = 0;

		std::cout << std::endl << "Number of other files (non-.jpg, non-.json, non-.png)" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 0) &&
				(fileRecordListIterator->typeJSONFound == 0) &&
				(fileRecordListIterator->typePNGFound == 0) &&
				(fileRecordListIterator->typeOtherFound == 1)) {
				std::cout << "\t" << fileRecordListIterator->stemName << std::endl;
				numOther++;
			}

			fileRecordListIterator++;
		}

		std::cout << "Other files = " << numOther << std::endl;

		/**************************************************/
		// 777777777777777777777777777777777777777777777
		// Number of .jpg, .json and .png files with same stem
		fileRecordListIterator = fileRecordList.begin();
		numTrio = 0;

		std::cout << std::endl << ".jpg, .png and.json files with same stem" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			if ((fileRecordListIterator->typeJPGFound == 1) &&
				(fileRecordListIterator->typeJSONFound == 1) &&
				(fileRecordListIterator->typePNGFound == 1) &&
				(fileRecordListIterator->typeOtherFound == 0)) {
				std::cout << "\t" << fileRecordListIterator->stemName << std::endl;
				numTrio++;
			}

			fileRecordListIterator++;
		}
		numTrio = numTrio * 3;
		std::cout << "Same Stem files = " << numTrio << std::endl;




		/*********************************************/
		// summary
		std::cout << std::endl;
		std::cout << "SUMMARY" << std::endl;
		std::cout << "\tMatching .jpg and .json files = " << numMatchedJPG << std::endl;
		std::cout << "\tMatching .png and .json files = " << numMatchedPNG << std::endl;
		std::cout << "\tUnmatched .jpg files = " << numUnMatchedJPG << std::endl;
		std::cout << "\tUnmatched .png files = " << numUnMatchedPNG << std::endl;
		std::cout << "\tUnmatched .json files = " << numUnMatchedJSON << std::endl;
		std::cout << "\tOther files = " << numOther << std::endl;
		std::cout << "\tSame stem files = " << numTrio << std::endl;

		numFilesFound = numMatchedJPG +
			numMatchedPNG +
			numUnMatchedJPG +
			numUnMatchedPNG +
			numUnMatchedJSON +
			numOther +
			numTrio;

		std::cout << "Total number of files processed: " << numFilesFound << std::endl;
	}


	/******************************************************************************/
	// get date information from json file and put into the fileRecordListIterator->
	// node of the linked list.
	// Get data out of json and into node of list.
	// Format in json:
	//			"formatted": "Mar 2, 2018, 1:10:08 AM UTC"
	//			"formatted": "Feb 27, 2018, 6:04:25 PM UTC"
	//			"formatted": "Jan 29, 2018, 11:27:42 PM UTC"
	// Format in jpg. The time is 24-hour time stored as a string:
	//			YYYY:MM:DD HH:MM:SS
	void processJSONFile(std::ifstream& infileC) {
		// get a line from the file, see if it has the "photoTakenTime" phrase.
		photoTakenTimeFound = false;
		while (!infileC.eof()) {
			std::getline(infileC, fLine);		// transfer the line from file to a string.
			i = int(fLine.find("photoTakenTime"));
			if ((fLine.find("photoTakenTime") != std::string::npos) && (photoTakenTimeFound == false)) {
				photoTakenTimeFound = true;
#if DEBUG_processJSON
				std::cout << "line photoTakenTime found" << std::endl;
#endif

				std::getline(infile, fLine);	// get timestamp line
				std::getline(infile, fLine);	// get date line
				fLine.push_back('\0');

				// extract date and time from the date lline extracted above.
				i = 0;
				while ((fLine[i] != ':') && fLine[i] != '\0') i++;

				while ((fLine[i] != '"') && fLine[i] != '\0') i++;
				i++;	// now pointing to start of month.

				// Transfer Month to list node
				tmpStr = fLine.substr(i, fLine.length());

				fileRecordListIterator->takenMonth[0] = tmpStr[0];
				fileRecordListIterator->takenMonth[1] = tmpStr[1];
				fileRecordListIterator->takenMonth[2] = tmpStr[2];
				fileRecordListIterator->takenMonth[3] = '\0';
				i = i + 4;		// i is now pointing at day of date

				tmpStr = fLine.substr(i, fLine.length());	// move pointer tmpStr to towards end of string
				//std::cout << tmpStr << std::endl;
				//std::cout << "tmpStr[" << i << "]: " << char(tmpStr[i]) << std::endl;
				//std::cout << "tmpStr[" << (i + 1) << "]: " << char(tmpStr[i + 1]) << std::endl;

				// see if Day is one or two characters
				if (tmpStr[1] == ',') {
					// date is one character
					fileRecordListIterator->takenDay[0] = '0';	// leading zero
					fileRecordListIterator->takenDay[1] = tmpStr[0];
					fileRecordListIterator->takenDay[2] = '\0';
					i = i + 3;		// i now pointing to year.
				}
				else {
					// date is two characters
					fileRecordListIterator->takenDay[0] = tmpStr[0];	// leading zero
					fileRecordListIterator->takenDay[1] = tmpStr[1];
					fileRecordListIterator->takenDay[2] = '\0';
					i = i + 4;		// i now pointing to year.
				}

				tmpStr = fLine.substr(i, fLine.length());

				// Get year
				fileRecordListIterator->takenYear[0] = tmpStr[0];
				fileRecordListIterator->takenYear[1] = tmpStr[1];
				fileRecordListIterator->takenYear[2] = tmpStr[2];
				fileRecordListIterator->takenYear[3] = tmpStr[3];
				fileRecordListIterator->takenYear[4] = '\0';
				i = i + 6;		// i now points to hour

				tmpStr = fLine.substr(i, fLine.length());

				// determine if the hour is one or two characters
				if (tmpStr[1] == ':') {
					// hour is one character
					fileRecordListIterator->takenHour[0] = '0';			// leading zero
					fileRecordListIterator->takenHour[1] = tmpStr[0];
					fileRecordListIterator->takenHour[2] = '\0';
					i = i + 2;		// i now points to minutes
				}
				else {
					// hour is two characters
					fileRecordListIterator->takenHour[0] = tmpStr[0];
					fileRecordListIterator->takenHour[1] = tmpStr[1];
					fileRecordListIterator->takenHour[2] = '\0';
					i = i + 3;		// i now points to minutes
				}

				tmpStr = fLine.substr(i, fLine.length());

				// Minutes is two characters
				fileRecordListIterator->takenMin[0] = tmpStr[0];
				fileRecordListIterator->takenMin[1] = tmpStr[1];
				fileRecordListIterator->takenMin[2] = '\0';
				i = i + 3;		// i now points to seconds

				tmpStr = fLine.substr(i, fLine.length());

				// Seconds is two characters
				fileRecordListIterator->takenSec[0] = tmpStr[0];
				fileRecordListIterator->takenSec[1] = tmpStr[1];
				fileRecordListIterator->takenSec[2] = '\0';
				i = i + 3;		// i now points to AM/PM

				tmpStr = fLine.substr(i, fLine.length());

				// Convert time to 24-hour time
				// See if AM or PM
				tmpHour = '\0';

				if ((tmpStr[0] == 'P') || (tmpStr[0] == 'p')) {

					// Convert 1 pm to 9 pm to 24-hour
					if ((fileRecordListIterator->takenHour[0] == '0')) {
						// Use map to convert taken hour to 24 hour
						tmpHour = hour12PMto9PM.find(fileRecordListIterator->takenHour[1])->second;
					}

					// Convert 10 pm to 11 pm to 24-hour
					if (fileRecordListIterator->takenHour[0] == '1')
					{
						// Use map to convert taken hour to 24 hour
						tmpHour = hour10PMto12AM.find(fileRecordListIterator->takenHour[1])->second;
					}

					//fileRecordListIterator->takenHour = tmpHour;
					tmpHour.copy(fileRecordListIterator->takenHour, tmpHour.length(), 0);
				}

				// Convert 12 am to 24-hour
				// All other AM times remain unchanged.
				else if ((fileRecordListIterator->takenHour[0] == '1') && (fileRecordListIterator->takenHour[0] == '2')) {
					// 12 AM must be converted to 00 hour
					tmpHour = "00";
					tmpHour.copy(fileRecordListIterator->takenHour, tmpHour.length(), 0);
				}

			}

		}

		/********************************************************************/
		// Convert data taken for json file into a string in the format:
		//		YYYY:MM:DD HH:MM:SS\0
		// 
		// Store the date time string in dateTaken of the FileInfoRecord structure. 
		if (photoTakenTimeFound == true) {
			// Convert month in FileInfoRecord to an ascii number
			tmpMonth.clear();
			tmpStr.clear();

			// Use map to convert month string (e.g., "Jan", "Feb") to string number (e.g., "01", "02")
			tmpMonth = months.find(fileRecordListIterator->takenMonth)->second;

			tmpStr = fileRecordListIterator->takenYear;
			tmpStr.push_back(':');
			tmpStr.append(tmpMonth);
			tmpStr.push_back(':');
			tmpStr.append(fileRecordListIterator->takenDay, 2);
			tmpStr.push_back(' ');
			tmpStr.append(fileRecordListIterator->takenHour, 2);
			tmpStr.push_back(':');
			tmpStr.append(fileRecordListIterator->takenMin, 2);
			tmpStr.push_back(':');
			tmpStr.append(fileRecordListIterator->takenSec, 2);
			tmpStr.push_back('\0');
			tmpStr.copy(fileRecordListIterator->dateTaken, tmpStr.length());
#if DEBUG_processDATE
			cout << "tmpStr: " << tmpStr << endl;
#endif
		}

		infile.close();
	}


	//***********************************************************************************//
	// outfilename: is the .path() portion of the stat structure for accessing file names
	// A file has been found in the directory. Save the information regarding the file in the
	// linked list.
	void storeFileFound(std::filesystem::path outfilename) {
		// get the file name with the extension and the full path
		fullFilePathName = outfilename.string();
		fullFilePathName.push_back('\0');

		// remove extension from full file name
		// start at the end of the full path file name and move backwards to find the period.
		fullFilePathNameNoExt = fullFilePathName;
		for (i = (int(fullFilePathNameNoExt.size() - 1)); i > (fullFilePathNameNoExt.size() - 7); i--) {
			c = fullFilePathName[i];
			if (fullFilePathNameNoExt[i] == '.')
				fullFilePathNameNoExt[i] = '\0';
		}

		fullFilePathNameNoExt = fullFilePathNameNoExt.c_str(); // adjust size of string
		fullFilePathNameNoExt.push_back('\0');

		// Get the stem and the extension from outfilename
		// get stem and extension of file name
		fileExtension = outfilename.extension().string();
		fileExtension.push_back('\0');
		// make file extension lower case
		for (int i = 0; i < fileExtension.length(); i++) {
			fileExtension[i] = tolower(fileExtension[i]);
		}

		fileStem = outfilename.stem().string();
		fileStem.push_back('\0');

		// See of the stem has already been stored in the list.
		// Start at the beginning of the list and search for a match in the fullPathNoExt variable
		// of the struct.
		matchFound = false;
		fileRecordListIterator = fileRecordList.begin();
		while (fileRecordListIterator != fileRecordList.end() && (matchFound == false)) {
			if (cmpStrings(fileStem, fileRecordListIterator->stemName)) {
				// match found. fileRecordListIterator points to the node in the list.
				matchFound = true;
			}
			else fileRecordListIterator++;
		}

		if (matchFound == false) {
			// match not found. Add a new node to the list and make fileRecordListIterator point
			// to it. initialize the values of node in the list.
			// Put the stem into the node to be written to the linked list
			// Put the stem in to the node regardless of value of extension.
			// Since fInfoNode has been initialized, putting it into the list puts
			// the initialized values into the node of the list.
			fileRecordList.push_front(fInfoNode);
			fileRecordListIterator = fileRecordList.begin();

			fileRecordListIterator->fullPathFile = (char*)malloc(fullFilePathName.length());
			fullFilePathName.copy(fileRecordListIterator->fullPathFile, fullFilePathName.length());

			fileRecordListIterator->fullPathNoExt = (char*)malloc(fullFilePathNameNoExt.length());
			fullFilePathNameNoExt.copy(fileRecordListIterator->fullPathNoExt, fullFilePathNameNoExt.length());

			fileRecordListIterator->stemName = (char*)malloc(fileStem.length());
			fileStem.copy(fileRecordListIterator->stemName, fileStem.length());
		}

		// If the file is a jpg, set the jpg bit in the node
		// fileRecordListIterator is already pointing to where the data must be stored.
		if ((fileExtension.compare(0, 4, ".jpg")) == 0 ||
			(fileExtension.compare(0, 5, ".jpeg")) == 0) {

			fileRecordListIterator->typeJPGFound = 1;
		}

		// If the file is a png, set the png bit in the node
		// fileRecordListIterator is already pointing to where the data must be stored.
		else if ((fileExtension.compare(0, 4, ".png")) == 0) {

			fileRecordListIterator->typePNGFound = 1;
		}

		// If the file is a json file, set the json bit in the node
		// fileRecordListIterator is already pointing to where the data must be stored.
		else if ((fileExtension.compare(0, 5, ".json")) == 0) {

			fileRecordListIterator->typeJSONFound = 1;

			// Extract date information from json file
			// the variable fullFilePathName should be the full path to the json file
			fileFound = false;
			infile.open(fullFilePathName);
			if (infile.is_open()) {
				fileFound = true;
			}
			else {
				std::cout << fullFilePathName << " NOT found" << std::endl;
			}

			// Extract date line from json file and put date in node of list
			if (fileFound) {
				// get date information from json file and put into the fileRecordListIterator->
				// node of the linked list.
				// Get data out of json and into node of list.
				// Format in json:
				//			"formatted": "Mar 2, 2018, 1:10:08 AM UTC"
				//			"formatted": "Feb 27, 2018, 6:04:25 PM UTC"
				//			"formatted": "Jan 29, 2018, 11:27:42 PM UTC"
				// Format in jpg. The time is 24-hour time:
				//			&&&&:MM:DD HH:MM:SS

				processJSONFile(infile);
			}
		}

		// the file is neither jpg nor json, so set other bit and save path with extension
		else {
			fileRecordListIterator->typeOtherFound = 1;
			fileRecordListIterator->fullPathOther = (char*)malloc(fullFilePathName.length() + 1);
			fullFilePathName.copy(fileRecordListIterator->fullPathOther, fullFilePathName.length());
		}
	}


	//***********************************************************************************//
	// Process jpg files that have a matching json file. Put the date from the json file
	// into the jpg file as the DateTimeOriginal.
	void xferDateTimeToJPG() {

		fileRecordListIterator = fileRecordList.begin();
		numMatchedJPG = 0;

		//std::cout << std::endl << "Matching .jpg and .json files" << std::endl;
		while (fileRecordListIterator != fileRecordList.end()) {
			// Search for records that have matching jpg and json files and that have
			// the date and time extracted from the json, formed into a string and stored
			// in dateTaken.
			if ((fileRecordListIterator->typeJPGFound == 1) &&
				(fileRecordListIterator->typeJSONFound == 1) &&
				(fileRecordListIterator->typePNGFound == 0) &&
				(fileRecordListIterator->typeOtherFound == 0) &&
				(fileRecordListIterator->dateTaken != NULL))
			{
				// Open jpg file, put the jpg file into a buffer and insert the json date into the buffer.
				// Then create an output file that has the same stem as the jpg file but that appends "_fixed.jpg".
				// Writ the modified buffer into the output file. 
				jpgIFileName = fileRecordListIterator->fullPathFile;
				cout << "dateTaken: " << fileRecordListIterator->dateTaken << "\t" << fileRecordListIterator->stemName << ".jpg" << endl;
				infileJPG.open(jpgIFileName, ios::binary);

				if (infileJPG.is_open()) {
#if DEBUG_processJPGFiles
					cout << "Read file: " << jpgIFileName << "\tfound" << endl;
#endif

					// Get file size
					infileJPG.seekg(0, ios::end);
					length = infileJPG.tellg();
					infileJPG.seekg(0, ios::beg);

					//allocate memory
					jpgBuffer = new unsigned char[length];

					// move file contents into the buffer
					infileJPG.read(jpgBuffer, length);

					// close the file
					infileJPG.close();
				}
				else {
					cout << "Read file: " << jpgIFileName << "\t NOT found" << endl;
				}

				// Parse the jpg file to access the EXIF metadata
				// parsing from the jpgBuffer also changes the date in the Buffer at TagID 0x9003 to the
				// dateTaken provided from the json file. When this section of code is executed, the buffer 
				// holds the entire jpg file with the new dataTaken.
				easyexif::EXIFInfo result;
				result.writeData(fileRecordListIterator->dateTaken, 1);
				int code = result.parseFrom(jpgBuffer, length);
				if (code) {
					printf("Error parsing EXIF: code %d\n", code);
				}

				// Create the output jpg file name using the stem for the input jpg file.
				// Append "_fixed.jpg" to the stem then open an output file with that name.
				// Write the buffer into the output file then close the output file and delete the buffer.
				jpgOFileName = fileRecordListIterator->stemName;
				jpgOFileName.append("_fixed.jpg");

				bool outFileOpen = false;
				outfileJPG.open(jpgOFileName, ios::binary);
				if (outfileJPG.is_open()) {
#if DEBUG_processJPGFiles
					cout << "Write file: " << jpgOFileName << "\tfound" << endl;
#endif

					outfileJPG.write(jpgBuffer, length);
					
					// close the file
					outfileJPG.close();

					// delete the buffer
					delete[] jpgBuffer;
				}
				else {
				    cout << jpgOFileName << "\tdoes NOT exist" << endl;
				}

#if DEBUG_processJPGFiles
				std::cout << "\t" << fileRecordListIterator->stemName << ".jpg processed" << std::endl;
#endif
				numMatchedJPG++;
			}

			fileRecordListIterator++;
		}

		std::cout << "Number of jpg files processed = " << numMatchedJPG << std::endl;

	}



};

