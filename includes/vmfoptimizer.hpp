#pragma once

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
