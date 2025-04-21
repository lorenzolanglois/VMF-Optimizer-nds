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
        data->line.find("id\" \"") != string::npos ||
        data->line.find("lightmapscale\" \"") != string::npos) {
            data->line = "";
            stats->currentRemovedLines++;
    }
    if (data->line.find("editor\r") != string::npos) {
        while (getline(data->file, data->line) && data->line.find("}") == string::npos) {
            stats->currentRemovedLines++;
        }
        data->line = "";
        stats->currentRemovedLines++;
    }
}

void optimizeEntitiesNds(Stats *stats, Data *data) {
    if (data->line.find("HDRColorScale\" \"") != string::npos ||
        data->line.find("framerate\" \"") != string::npos ||
        data->line.find("pitch\" \"") != string::npos ||
        data->line.find("pitchstart\" \"") != string::npos ||
        data->line.find("lfotype\" \"") != string::npos ||
        data->line.find("lforate\" \"") != string::npos ||
        data->line.find("lfomodvol\" \"") != string::npos ||
        data->line.find("lfomodpitch\" \"") != string::npos ||
        data->line.find("fadeoutsecs\" \"") != string::npos ||
        data->line.find("fadeinsecs\" \"") != string::npos ||
        data->line.find("smoothing_groups\" \"") != string::npos ||
        data->line.find("lightmapscale\" \"") != string::npos ||
        data->line.find("id\" \"") != string::npos ||
        data->line.find("SuppressAnimSounds\" \"") != string::npos ||
        data->line.find("SetBodyGroup\" \"") != string::npos ||
        data->line.find("DisableBoneFollowers\" \"") != string::npos ||
        data->line.find("RandomAnimation\" \"") != string::npos ||
        data->line.find("MinAnimTime\" \"") != string::npos ||
        data->line.find("MaxAnimTime\" \"") != string::npos ||
        data->line.find("_lightHDR\" \"") != string::npos ||
        data->line.find("_lightscaleHDR\" \"") != string::npos ||
        data->line.find("vrad_brush_cast_shadows\" \"") != string::npos ||
        data->line.find("ignorenormals\" \"") != string::npos ||
        data->line.find("disableselfshadowing\" \"") != string::npos ||
        data->line.find("disablevertexlighting\" \"") != string::npos ||
        data->line.find("screenspacefade\" \"") != string::npos ||
        data->line.find("maxcpulevel\" \"") != string::npos ||
        data->line.find("mincpulevel\" \"") != string::npos ||
        data->line.find("maxgpulevel\" \"") != string::npos ||
        data->line.find("mingpulevel\" \"") != string::npos ||
        data->line.find("disableshadows\" \"") != string::npos ||
        data->line.find("_shadoworiginoffset\" \"") != string::npos ||
        data->line.find("fadeOutDuration\" \"") != string::npos ||
        data->line.find("fadeInDuration\" \"") != string::npos ||
        data->line.find("onplayerdeath\" \"") != string::npos ||
        data->line.find("cspinup\" \"") != string::npos) {
            data->line = "";
            stats->currentRemovedLines++;
    }
    if (data->line.find("editor\r") != string::npos) {
        while (getline(data->file, data->line) && data->line.find("}") == string::npos) {
            stats->currentRemovedLines++;
        }
        data->line = "";
        stats->currentRemovedLines++;
    }
}
