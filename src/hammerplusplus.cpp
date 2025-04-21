#include "vmfOptimizer.hpp"

#include <string>

using namespace std;

void optimizeHammerPlusPlus(Stats *stats, Data *data) {
    if (data->line.find("_plus\r") != string::npos) {
        while (getline(data->file, data->line) && data->line.find("}") == string::npos) {
            stats->currentRemovedLines++;
            data->line = "";
        }
        data->line = "";
        stats->currentRemovedLines++;
    }
}
