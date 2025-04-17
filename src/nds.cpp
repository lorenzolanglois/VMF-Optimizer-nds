#include "vmfOptimizer.hpp"

#include <string>

using namespace std;

void optimizeWorldNds(Stats *stats, Data *data) {
    if (data->line.find("musicpostfix\" \"") != string::npos ||
        data->line.find("maxpropscreenwidth\" \"") != string::npos ||
        data->line.find("maxblobcount\" \"") != string::npos ||
        data->line.find("detailvbsp\" \"") != string::npos ||
        data->line.find("detailmaterial\" \"") != string::npos ||
        data->line.find("comment\" \"") != string::npos ||
        data->line.find("mapversion\" \"") != string::npos ||
        data->line.find("lightmapscale\" \"") != string::npos) {
            data->line = "";
            stats->currentRemovedLines++;
    }
}

void optimizeEntitiesNds(Stats *stats, Data *data) {
    ;
}
