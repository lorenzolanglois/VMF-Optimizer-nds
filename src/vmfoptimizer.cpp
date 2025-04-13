// TO DO:
// options to remove vertices_plus (hammer++) and editor (entity data)
// option "is this a prefab?" (removes more stuff if yes)
// option to remove lightmap values (default off cause the default lightmap value can be changed; if on, removes parameter if the value is 16 [default if unconfigured])

#include <iostream>
#include <string>
#include <fstream>

#include "vmfoptimizer.hpp"

using namespace std;

string removeExtraCharacters(Data *data) {
    if (data->isLow) {
        return data->line;
    }
    string search[] = {"\r", "\n", "\t", "\" \""};
    string replace[] = {"", "", "", "\"\""};

    for (size_t i = 0, pos = 0; i < size(search); i += 1, pos = 0) {
        while ((pos = data->line.find(search[i], pos)) != string::npos) {
            data->line.replace(pos, search[i].length(), replace[i]);
            pos += replace[i].length();
        }
    }
    return data->line;
}

void loadFile(string &userInput, Data *data) {
    if (userInput.at(0) == '\"') {
        userInput.erase(userInput.begin());
        userInput.erase(userInput.length() - 1);
    }
    if (userInput.find(".vmf") == string::npos) {
        data->file.open(userInput.append(".vmf"), ios::binary);
    } else {
        data->file.open(userInput, ios::binary);
    }
    if (!data->file.is_open()) {
        cerr << "Error. File \"" << userInput << "\" doesn't exist." << endl;
        if (data->isLog) {
            data->log << "Error. File \"" << userInput << "\" doesn't exist.";
            data->log.close();
        }
        exit(1);
    }
    data->out.open(userInput.erase(userInput.length() - 4).append("_opti.vmf"));
    if (!data->out.is_open()) {
        cerr << "Output error." << endl;
        exit(1);
    }
    size_t pos = userInput.find_last_of('\\');
    if (pos != string::npos) {
        userInput.erase(0, pos + 1);
    }
    userInput.erase(userInput.length() - 9);
}

void optimizeWorld(Stats *stats, Data *data) {
    while (getline(data->file, data->line) && data->line != "entity") {
        if (data->isNds) {
            optimizeWorldNds(stats, data);
        }
        if (data->line.find("vertices_plus") != string::npos) {
            while (data->line.find("material") == string::npos && getline(data->file, data->line)) {
                stats->currentLines++;
                stats->currentRemovedLines++;
            }
            data->out << removeExtraCharacters(data);
        } else if (data->line.find("rotation\" \"0") == string::npos &&
                   data->line.find("smoothing_groups\" \"0") == string::npos &&
                   data->line.find("elevation\" \"0") == string::npos &&
                   data->line.find("subdiv\" \"0") == string::npos) {
            data->out << removeExtraCharacters(data);
        } else {
            stats->currentRemovedLines++;
        }
        stats->currentLines++;
    }
    data->out << removeExtraCharacters(data);
    stats->currentLines++;
}

void optimizeEntities(Stats *stats, Data *data) {
    // 0 - none, 1 - prop_static, 2 - prop_dynamic, 3 - prop_physics, 4 - func_detail, 5 - func_brush, 6 - light, 7 - light_spot, 8 - light_dynamic, 9 - func_door/func_door_rotating
    // 10 - info_decal, 11 - info overlay, 12 - trigger_once/remove, 13 - trigger_multiple, 14 - trigger_hurt, 15 - func_areaportal, 16 - func_areaportalwindow, 17 - ambient_generic, 18 - brush entities
    // 19 - light_environment, 20 - item_ammopack/item_healthkit, 21 - move/keyframe_rope, 22 - info_particle_system, 23 - point_spotlight
    unsigned char type = 0;

    while (getline(data->file, data->line)) {
        if (data->isNds) {
            optimizeEntitiesNds(stats, data);
        }
        if (data->line.length() > 4) {
            if (data->line.find("classname") == string::npos &&
                data->line.find("vertices_plus") == string::npos) {
                switch (type) {
                    case 0:
                        data->out << removeExtraCharacters(data);
                        break;
                    case 1: // prop_static
                        if (data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("fademaxdist\" \"0") == string::npos &&
                            data->line.find("fademindist\" \"-1") == string::npos &&
                            data->line.find("fadescale\" \"1") == string::npos &&
                            data->line.find("lightmapresolutionx\" \"32") == string::npos &&
                            data->line.find("lightmapresolutiony\" \"32") == string::npos &&
                            data->line.find("skin\" \"0") == string::npos &&
                            data->line.find("solid\" \"6") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 2: // prop_dynamic
                        if (data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("DisableBoneFollowers\" \"0") == string::npos &&
                            // removes both disableshadows and disablereceiveshadows
                            data->line.find("shadows\" \"0") == string::npos &&
                            data->line.find("ExplodeDamage\" \"0") == string::npos &&
                            data->line.find("ExplodeRadius\" \"0") == string::npos &&
                            data->line.find("fademaxdist\" \"0") == string::npos &&
                            data->line.find("fademindist\" \"-1") == string::npos &&
                            data->line.find("fadescale\" \"1") == string::npos &&
                            data->line.find("MaxAnimTime\" \"10") == string::npos &&
                            data->line.find("MinAnimTime\" \"5") == string::npos &&
                            data->line.find("modelscale\" \"1.0") == string::npos &&
                            data->line.find("PerformanceMode\" \"0") == string::npos &&
                            data->line.find("pressuredelay\" \"0") == string::npos &&
                            data->line.find("RandomAnimation\" \"0") == string::npos &&
                            data->line.find("renderamt\" \"255") == string::npos &&
                            data->line.find("rendercolor\" \"255 255 255") == string::npos &&
                            data->line.find("renderfx\" \"0") == string::npos &&
                            data->line.find("rendermode\" \"0") == string::npos &&
                            data->line.find("SetBodyGroup\" \"0") == string::npos &&
                            data->line.find("skin\" \"0") == string::npos &&
                            data->line.find("solid\" \"6") == string::npos &&
                            data->line.find("StartDisabled\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 3: // prop_physics
                        if (data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("damagetoenablemotion\" \"0") == string::npos &&
                            data->line.find("Damagetype\" \"0") == string::npos &&
                            data->line.find("shadows\" \"0") == string::npos &&
                            data->line.find("ExplodeDamage\" \"0") == string::npos &&
                            data->line.find("ExplodeRadius\" \"0") == string::npos &&
                            data->line.find("fademaxdist\" \"0") == string::npos &&
                            data->line.find("fademindist\" \"-1") == string::npos &&
                            data->line.find("fadescale\" \"1") == string::npos &&
                            data->line.find("forcetoenablemotion\" \"0") == string::npos &&
                            data->line.find("inertiaScale\" \"1.0") == string::npos &&
                            data->line.find("massScale\" \"0") == string::npos &&
                            data->line.find("minhealthdmg\" \"0") == string::npos &&
                            data->line.find("modelscale\" \"1.0") == string::npos &&
                            data->line.find("nodamageforces\" \"0") == string::npos &&
                            data->line.find("PerformanceMode\" \"0") == string::npos &&
                            data->line.find("physdamagescale\" \"0.1") == string::npos &&
                            data->line.find("pressuredelay\" \"0") == string::npos &&
                            data->line.find("renderamt\" \"255") == string::npos &&
                            data->line.find("rendercolor\" \"255 255 255") == string::npos &&
                            data->line.find("renderfx\" \"0") == string::npos &&
                            data->line.find("rendermode\" \"0") == string::npos &&
                            data->line.find("shadowcastdist\" \"0") == string::npos &&
                            data->line.find("skin\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 4: // func_detail
                        if (data->line.find("dxlevel\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 5: // func_brush
                        if (data->line.find("InputFilter\" \"0") == string::npos &&
                            data->line.find("invert_exclusion\" \"0") == string::npos &&
                            data->line.find("renderamt\" \"255") == string::npos &&
                            data->line.find("rendercolor\" \"255 255 255") == string::npos &&
                            data->line.find("renderfx\" \"0") == string::npos &&
                            data->line.find("rendermode\" \"0") == string::npos &&
                            data->line.find("solidbsp\" \"0") == string::npos &&
                            data->line.find("Solidity\" \"0") == string::npos &&
                            data->line.find("StartDisabled") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 6: // light: since light and light_spot have some identical parameter names, i wanted to merge them for efficiency (case 7: case 6: break;), but my code shat itself, so yeah
                        if (data->line.find("_constant_attn\" \"0") == string::npos &&
                            data->line.find("_distance\" \"0") == string::npos &&
                            data->line.find("_fifty_percent_distance\" \"0") == string::npos &&
                            data->line.find("_hardfalloff\" \"0") == string::npos &&
                            data->line.find("_light\" \"255 255 255 200") == string::npos &&
                            data->line.find("_lightHDR\" \"-1 -1 -1 1") == string::npos &&
                            data->line.find("_lightscaleHDR\" \"1") == string::npos &&
                            data->line.find("_linear_attn\" \"0") == string::npos &&
                            data->line.find("_quadratic_attn\" \"1") == string::npos &&
                            data->line.find("_zero_percent_distance\" \"0") == string::npos &&
                            data->line.find("style\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 7: // light_spot
                        if (data->line.find("_constant_attn\" \"0") == string::npos &&
                            data->line.find("_distance\" \"0") == string::npos &&
                            data->line.find("_fifty_percent_distance\" \"0") == string::npos &&
                            data->line.find("_hardfalloff\" \"0") == string::npos &&
                            data->line.find("_light\" \"255 255 255 200") == string::npos &&
                            data->line.find("_lightHDR\" \"-1 -1 -1 1") == string::npos &&
                            data->line.find("_lightscaleHDR\" \"1") == string::npos &&
                            data->line.find("_linear_attn\" \"0") == string::npos &&
                            data->line.find("_quadratic_attn\" \"1") == string::npos &&
                            data->line.find("_zero_percent_distance\" \"0") == string::npos &&
                            data->line.find("style\" \"0") == string::npos &&
                            data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("_cone\" \"45") == string::npos &&
                            data->line.find("_exponent\" \"1") == string::npos &&
                            data->line.find("_inner_cone\" \"30") == string::npos &&
                            data->line.find("pitch\" \"-90") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 8: // light_dynamic
                        if (data->line.find("style\" \"0") == string::npos &&
                            data->line.find("_cone\" \"45") == string::npos &&
                            data->line.find("_inner_cone\" \"30") == string::npos &&
                            data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("brightness\" \"0") == string::npos &&
                            data->line.find("distance\" \"120") == string::npos &&
                            data->line.find("spotlight_radius\" \"80") == string::npos &&
                            data->line.find("pitch\" \"-90") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 9: // func_door
                        if (data->line.find("shadows\" \"0") == string::npos &&
                            data->line.find("dmg\" \"0") == string::npos &&
                            data->line.find("forceclosed\" \"0") == string::npos &&
                            data->line.find("health\" \"0") == string::npos &&
                            data->line.find("ignoredebris\" \"0") == string::npos &&
                            data->line.find("lip\" \"0") == string::npos &&
                            data->line.find("locked_sentence\" \"0") == string::npos &&
                            data->line.find("loopmovesound\" \"0") == string::npos &&
                            data->line.find("movedir\" \"0 0 0") == string::npos &&
                            data->line.find("renderamt\" \"255") == string::npos &&
                            data->line.find("rendercolor\" \"255 255 255") == string::npos &&
                            data->line.find("renderfx\" \"0") == string::npos &&
                            data->line.find("rendermode\" \"0") == string::npos &&
                            data->line.find("speed\" \"100") == string::npos &&
                            data->line.find("unlocked_sentence\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 10: // info_decal
                        if (data->line.find("angles\" \"0 0 0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 11: // info_overlay
                        if (data->line.find("fademaxdist\" \"0") == string::npos &&
                            data->line.find("fademindist\" \"-1") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 12: // trigger_once, trigger_remove, other triggers
                        if (data->line.find("StartDisabled\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 13: // trigger_multiple
                        if (data->line.find("StartDisabled\" \"0") == string::npos &&
                            data->line.find("wait\" \"1") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 14: // trigger_hurt
                        if (data->line.find("StartDisabled\" \"0") == string::npos &&
                            data->line.find("damagemodel\" \"0") == string::npos &&
                            data->line.find("damagetype\" \"0") == string::npos &&
                            data->line.find("nodmgforce\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 15: // func_areaportal
                        if (data->line.find("PortalVersion\" \"1") == string::npos &&
                            data->line.find("StartOpen\" \"1") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 16: // func_areaportal_window
                        if (data->line.find("PortalVersion\" \"1") == string::npos &&
                            data->line.find("TranslucencyLimit\" \"0.2") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 17: // ambient_generic
                        if (data->line.find("cspinup\" \"0") == string::npos &&
                            data->line.find("secs\" \"0") == string::npos &&//both fadeinsecs and fadedata->outsecs
                            data->line.find("health\" \"10") == string::npos &&
                            data->line.find("lfomodpitch\" \"0") == string::npos &&
                            data->line.find("lfomodvol\" \"0") == string::npos &&
                            data->line.find("lforate\" \"0") == string::npos &&
                            data->line.find("lfotype\" \"0") == string::npos &&
                            data->line.find("pitch\" \"100") == string::npos &&
                            data->line.find("pitchstart\" \"100") == string::npos &&
                            data->line.find("preset\" \"0") == string::npos &&
                            data->line.find("radius\" \"1250") == string::npos &&
                            data->line.find("spindown\" \"0") == string::npos &&
                            data->line.find("spinup\" \"0") == string::npos &&
                            data->line.find("volstart\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 18: // brush entities
                        if (data->line.find("rotation\" \"0") == string::npos &&
                            data->line.find("smoothing_groups\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 19: // light_environment
                        if (data->line.find("_ambient\" \"255 255 255 20") == string::npos &&
                            data->line.find("_light\" \"255 255 255 200") == string::npos &&
                            data->line.find("HDR\" \"-1 -1 -1 1") == string::npos &&//both normal and ambient
                            data->line.find("caleHDR\" \"1") == string::npos &&//both
                            data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("_inner_cone\" \"30") == string::npos &&
                            data->line.find("pitch\" \"0") == string::npos &&
                            data->line.find("SunSpreadAngle\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 20: // item_
                        if (data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("AutoMaterialize\" \"1") == string::npos &&
                            data->line.find("fademaxdist\" \"0") == string::npos &&
                            data->line.find("fademindist\" \"-1") == string::npos &&
                            data->line.find("StartDisabled\" \"0") == string::npos &&
                            data->line.find("TeamNum\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 21: // ropes
                        if (data->line.find("Barbed\" \"0") == string::npos &&
                            data->line.find("Breakable\" \"0") == string::npos &&
                            data->line.find("Collide\" \"0") == string::npos &&
                            data->line.find("Dangling\" \"-1") == string::npos &&
                            data->line.find("dxlevel\" \"0") == string::npos &&
                            data->line.find("NoWind\" \"0") == string::npos &&
                            data->line.find(R"(RopeMaterial" "cable/cable.vmt")") == string::npos &&
                            data->line.find("Slack\" \"25") == string::npos &&
                            data->line.find("Subdiv\" \"2") == string::npos &&
                            data->line.find("TextureScale\" \"1") == string::npos &&
                            data->line.find("Type\" \"0") == string::npos &&
                            data->line.find("Width\" \"2") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 22: // info_particle_system
                        if (data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("parent\" \"0") == string::npos &&
                            data->line.find("flag_as_weather\" \"0") == string::npos &&
                            data->line.find("start_active\" \"0") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 23: // point_spotlight
                        if (data->line.find("angles\" \"0 0 0") == string::npos &&
                            data->line.find("disablereceiveshadows\" \"0") == string::npos &&
                            data->line.find("HDRColorScale\" \"1.0") == string::npos &&
                            data->line.find("IgnoreSolid\" \"0") == string::npos &&
                            data->line.find("dxlevel\" \"0") == string::npos &&
                            data->line.find("renderamt\" \"255") == string::npos &&
                            data->line.find("rendercolor\" \"255 255 255") == string::npos &&
                            data->line.find("renderfx\" \"0") == string::npos &&
                            data->line.find("rendermode\" \"0") == string::npos &&
                            data->line.find("spotlightlength\" \"500") == string::npos &&
                            data->line.find("spotlightwidth\" \"50") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    case 24: // editor_text
                        if (data->line.find("angles\" \"-0 0 0") == string::npos &&
                            data->line.find("color\" \"255 255 255") == string::npos &&
                            data->line.find("textsize\" \"10") == string::npos)
                            data->out << removeExtraCharacters(data);
                        else stats->currentRemovedLines++;
                        break;
                    default:
                        data->out << removeExtraCharacters(data);
                        break;
                }
            } else if (data->line.find("classname") != string::npos) {//if "classname" is found
                if (data->line.find("\"prop_") != string::npos) {
                    if (data->line.find("static") != string::npos) type = 1;
                    else if (data->line.find("dynamic") != string::npos) type = 2;//also affects prop_dynamic_override
                    else type = 3;//physics, also affects prop_physics_override, detail and other misc prop objects
                } else if (data->line.find("\"func_") != string::npos) {
                    if (data->line.find("detail") != string::npos) type = 4;
                    else if (data->line.find("brush") != string::npos) type = 5;
                    else if (data->line.find("door") != string::npos) type = 9;//both normal and rotating
                    else if (data->line.find("areaportal") != string::npos) {
                        if (data->line.find("window\"") != string::npos) type = 16;//areaportal_window
                        else type = 15;//areaportal
                    } else type = 0;
                } else if (data->line.find("\"light") != string::npos) {
                    if (data->line.find("spot") != string::npos) type = 7;//light_spot
                    else if (data->line.find("dynamic") != string::npos) type = 8;//light_dynamic
                    else if (data->line.find("environment") != string::npos) type = 19;//light_environment
                    else type = 6;//light
                } else if (data->line.find("\"info_") != string::npos) {
                    if (data->line.find("decal") != string::npos) type = 10;
                    else if (data->line.find("overlay") != string::npos) type = 11;
                    else if (data->line.find("particle") != string::npos) type = 22;
                    else type = 0;
                } else if (data->line.find("\"trigger") != string::npos) {
                    if (data->line.find("multiple") != string::npos)
                        type = 13;
                    else if (data->line.find("hurt") != string::npos) type = 14;
                    else type = 12;
                } else if (data->line.find("\"ambient_generic") != string::npos) type = 17;
                else if (data->line.find("\"item_") != string::npos) type = 20;
                else if (data->line.find("rope\"") != string::npos) type = 21;
                else if (data->line.find("\"point_spotlight") != string::npos) type = 23;
                else if (data->line.find("\"editor") != string::npos) type = 24;
                else type = 0;
                data->out << removeExtraCharacters(data);
            } else if (data->line.find("vertices_plus") != string::npos) {
                while (data->line.find("material") == string::npos && getline(data->file, data->line)) {
                    stats->currentLines++;
                    stats->currentRemovedLines++;
                }
                type = 18;
                data->out << removeExtraCharacters(data);
            }
        }
        else data->out << removeExtraCharacters(data);
        stats->currentLines++;
    }
}

void argsParser(int argc, char *argv[], Data *data) {
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-nonds") {
            data->isNds = false;
        } else if (string(argv[i]) == "-keep") {
            data->isLow = true;
        } else if (string(argv[i]) == "-nolog") {
            data->isLog = false;
        } else if (string(argv[i]) == "-nolog") {
            data->isLog = false;
        } else if (string(argv[i]).at(0) == '-') {
            cerr << "Unknown argument: " + string(argv[i]) << endl;
        } else {
            data->isFileList = true;
            data->fileList.push_back(string(argv[i]));
        }
    }
}

void optimizer(int argc, char *argv[]) {
    Stats stats;
    Data data;

    cout << "VMF Optimizer nds, originally by dabmasterars, fork by lorenzolanglois.\nMultiple files have to be inserted consecutively.\nDrop your .vmf file here or type file name: " << endl;
    argsParser(argc, argv, &data);
    if (data.isLog) {
        data.log.open("log.txt");
    }
    while (1) {
        if (data.isFileList) {
            if (data.fileList.empty()) {
                break;
            }
            loadFile(data.fileList.front(), &data);
            data.fileList.pop_front();
        } else {
            getline(cin, data.userInput);
            if (data.userInput.length() == 0 && data.line == "") {
                break;
            } else if (data.userInput.length() == 0) {
                exit(0);
            }
            loadFile(data.userInput, &data);
        }
        data.line = "";

        cout << "\nOptimizing " << data.userInput << " . . .\n";
        optimizeWorld(&stats, &data);
        optimizeEntities(&stats, &data);

        data.file.close();
        data.out.close();
        cout << endl << endl << "Finished file " << data.userInput << ".\nRemoved " << stats.currentRemovedLines << " out of " << stats.currentLines << " lines. ("
             << (float) stats.currentRemovedLines / (float) stats.currentLines * 100 << "%)\n\nPress ENTER to finish program. If you wish to continue, drop another file here or type file name:\n";
        if (data.isLog) {
            data.log << "Finished file " << data.userInput <<".\nRemoved " << stats.currentRemovedLines << " out of " << stats.currentLines << " lines. ("
                << (float) stats.currentRemovedLines / (float) stats.currentLines * 100 << "%)\n\n";
        }
        stats.totalFiles++;
        stats.totalRemovedLines = stats.totalRemovedLines + stats.currentRemovedLines;
        stats.totalLines = stats.totalLines + stats.currentLines;
    }
    cout << "\nSuccessful compression of " << stats.totalFiles << " files.\nRemoved " << stats.totalRemovedLines << " out of " << stats.totalLines << " stats.total lines. (" << (float) stats.totalRemovedLines / (float) stats.totalLines * 100 << "%)\n\n";
    if (data.isLog) {
        data.log << "\nSuccessful compression of " << stats.totalFiles << " files.\nRemoved " << stats.totalRemovedLines << " out of " << stats.totalLines << " stats.total lines. (" << (float) stats.totalRemovedLines / (float) stats.totalLines * 100 << "%)";
        data.log.close();
    }
}

int main(int argc, char *argv[]) {
    optimizer(argc, argv);
    return 0;
}
