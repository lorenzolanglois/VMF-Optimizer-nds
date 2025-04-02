#pragma once

#include <string>

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
        Data() {
            log.open("log.txt");
        }
        // String containing the last input entered by the user
        std::string userInput = "";
        // String containing the current line
        std::string line = "";
        // The file that is being read
        std::ifstream file;
        // The log file
        std::ofstream log;
        // The file that is being written
        std::ofstream out;
        // If boolean is true, apply nds optimization
        bool isNds = true;
        // If boolean is false, remove useless characters
        bool isLow = false;
};

