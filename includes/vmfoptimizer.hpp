#pragma once

#include <string>
#include <fstream>
#include <list>

using namespace std;

class Stats {
    public:
        // Total number of lines in all files
        unsigned long long int totalLines = 0;
        // Total number of removed lines in all files
        unsigned long long int totalRemovedLines = 0;
        // Total number of files optimized
        unsigned short int totalFiles = 0;
        // Current number of lines in the file
        unsigned int currentLines = 0;
        // Current number of removed lines in the file
        unsigned int currentRemovedLines = 0;
};

class Data {
    public:
        // List of string containing the potential files added in parameter
        list<std::string> fileList;
        // String containing the last input entered by the user
        string userInput = "";
        // String containing the current line
        string line = "";
        // String containing the filename of the log file
        string logfilename = "log.txt";
        // The file that is being read
        ifstream file;
        // The log file
        ofstream log;
        // The file that is being written
        ofstream out;
        // If boolean is true, apply nds optimization
        bool isNds = true;
        // If boolean is false, remove useless characters
        bool isLow = false;
        // If boolean is true, write logs in a file
        bool isLog = true;
        // If boolean is false, do not use filelist given in argument
        bool isFileList = false;
};

// nds

void optimizeWorldNds(Stats *stats, Data *data);
void optimizeEntitiesNds(Stats *stats, Data *data);
