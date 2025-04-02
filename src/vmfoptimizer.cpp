// TO DO:
// multiple files support
// options to remove the old file
// options to remove vertices_plus (hammer++) and editor (entity data)
// option "is this a prefab?" (removes more stuff if yes)
// option to save log (default on)
// option to remove lightmap values (default off cause the default lightmap value can be changed; if on, removes parameter if the value is 16 [default if unconfigured])

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string removeExtraCharacters(string line) {
    string search[] = {"\r", "\n", "\t", "\" \""};
    string replace[] = {"", "", "", "\"\""};

    for (size_t i = 0, pos = 0; i < size(search); i += 1, pos = 0) {
        while ((pos = line.find(search[i], pos)) != string::npos) {
            line.replace(pos, search[i].length(), replace[i]);
            pos += replace[i].length();
        }
    }
    return line;
}

void loadFile(string &userInput, ifstream &file, ofstream &log, ofstream &out) {
    getline(cin, userInput);
    if (userInput.length() == 0) {
        exit(0);
    }
    if (userInput.at(0) == '\"') {
        userInput.erase(userInput.begin());
        userInput.erase(userInput.length() - 1);
    }
    if (userInput.find(".vmf") == string::npos) {
        file.open(userInput.append(".vmf"), ios::binary);
    } else {
        file.open(userInput, ios::binary);
    }
    if (!file.is_open()) {
        cerr << "Error. File \"" << userInput << "\" doesn't exist." << endl;
        log << "Error. File \"" << userInput << "\" doesn't exist.";
        log.close();
        exit(1);
    }
    out.open(userInput.erase(userInput.length() - 4).append("_opti.vmf"));
    if (!out.is_open()) {
        cerr << "Output error." << endl;
        exit(1);
    }
    size_t pos = userInput.find_last_of('\\');
    if (pos != string::npos) {
        userInput.erase(0, pos + 1);
    }
    userInput.erase(userInput.length() - 9);
    cout << "\nCompressing " << userInput << " . . .\n";
}

void optimizer() {
    string userInput = "";
    ifstream file(userInput, ios::binary);
    ofstream log("log.txt");
    ofstream out;
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

    cout << "VMF Optimizer nds, originally by dabmasterars, fork by lorenzolanglois.\nMultiple files have to be inserted consecutively.\nDrop your .vmf file here or type file name: " << endl;
    while (1) {
        loadFile(userInput, file, log, out);
        unsigned char type;
        //0 - none, 1 - prop_static, 2 - prop_dynamic, 3 - prop_physics, 4 - func_detail, 5 - func_brush, 6 - light, 7 - light_spot, 8 - light_dynamic, 9 - func_door/func_door_rotating
        //10 - info_decal, 11 - info overlay, 12 - trigger_once/remove, 13 - trigger_multiple, 14 - trigger_hurt, 15 - func_areaportal, 16 - func_areaportalwindow, 17 - ambient_generic, 18 - brush entities
        //19 - light_environment, 20 - item_ammopack/item_healthkit, 21 - move/keyframe_rope, 22 - info_particle_system, 23 - point_spotlight
        string line;

        while (getline(file, line) && line !=
                                    "entity") {//scans the world brushes (which are all before entities), should be faster than the next cycle
            if (line.find("vertices_plus") != string::npos) {//if "vertices_plus" is found
                while (line.find("material") == string::npos && getline(file, line)) {
                    currentLines++;
                    currentRemovedLines++;
                }
                out << removeExtraCharacters(line);
            }//lightmaps are not being erased, since the default lightmap value can be changed
            else if (line.find("rotation\" \"0") == string::npos &&
                     line.find("smoothing_groups\" \"0") == string::npos &&
                     line.find("elevation\" \"0") == string::npos &&
                     line.find("subdiv\" \"0") == string::npos)
                out << removeExtraCharacters(line);
            else currentRemovedLines++;
            currentLines++;
        }
        out << removeExtraCharacters(line);
        currentLines++;
        //cout<<"\nWorld brushes until line "<<currentLines;
        while (getline(file, line)) {//yandere dev moment
            if (line.length() > 4) {
                if (line.find("classname") == string::npos &&
                    line.find("vertices_plus") == string::npos) {//originally there was find("name") to not corrupt the vmf if some dumbass decides to name their prop "angles 0 0 0", but you can't use quotes without corruption anyway.
                    switch (type) {
                        case 0:
                            out << removeExtraCharacters(line);
                            break;
                        case 1://prop_static
                            if (line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("fademaxdist\" \"0") == string::npos &&
                                line.find("fademindist\" \"-1") == string::npos &&
                                line.find("fadescale\" \"1") == string::npos &&
                                line.find("lightmapresolutionx\" \"32") == string::npos &&//i could probably
                                line.find("lightmapresolutiony\" \"32") == string::npos &&//merge these two
                                line.find("skin\" \"0") == string::npos &&
                                line.find("solid\" \"6") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 2://prop_dynamic
                            if (line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("DisableBoneFollowers\" \"0") == string::npos &&
                                line.find("shadows\" \"0") == string::npos &&
                                //removes both disableshadows and disablereceiveshadows
                                line.find("ExplodeDamage\" \"0") == string::npos &&
                                line.find("ExplodeRadius\" \"0") == string::npos &&
                                line.find("fademaxdist\" \"0") == string::npos &&
                                line.find("fademindist\" \"-1") == string::npos &&
                                line.find("fadescale\" \"1") == string::npos &&
                                line.find("MaxAnimTime\" \"10") == string::npos &&
                                line.find("MinAnimTime\" \"5") == string::npos &&
                                line.find("modelscale\" \"1.0") == string::npos &&
                                line.find("PerformanceMode\" \"0") == string::npos &&
                                line.find("pressuredelay\" \"0") == string::npos &&
                                line.find("RandomAnimation\" \"0") == string::npos &&
                                line.find("renderamt\" \"255") == string::npos &&
                                line.find("rendercolor\" \"255 255 255") == string::npos &&
                                line.find("renderfx\" \"0") == string::npos &&
                                line.find("rendermode\" \"0") == string::npos &&
                                line.find("SetBodyGroup\" \"0") == string::npos &&
                                line.find("skin\" \"0") == string::npos &&
                                line.find("solid\" \"6") == string::npos &&
                                line.find("StartDisabled\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 3://prop_physics
                            if (line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("damagetoenablemotion\" \"0") == string::npos &&
                                line.find("Damagetype\" \"0") == string::npos &&
                                line.find("shadows\" \"0") == string::npos &&
                                line.find("ExplodeDamage\" \"0") == string::npos &&
                                line.find("ExplodeRadius\" \"0") == string::npos &&
                                line.find("fademaxdist\" \"0") == string::npos &&
                                line.find("fademindist\" \"-1") == string::npos &&
                                line.find("fadescale\" \"1") == string::npos &&
                                line.find("forcetoenablemotion\" \"0") == string::npos &&
                                line.find("inertiaScale\" \"1.0") == string::npos &&
                                line.find("massScale\" \"0") == string::npos &&
                                line.find("minhealthdmg\" \"0") == string::npos &&
                                line.find("modelscale\" \"1.0") == string::npos &&
                                line.find("nodamageforces\" \"0") == string::npos &&
                                line.find("PerformanceMode\" \"0") == string::npos &&
                                line.find("physdamagescale\" \"0.1") == string::npos &&
                                line.find("pressuredelay\" \"0") == string::npos &&
                                line.find("renderamt\" \"255") == string::npos &&
                                line.find("rendercolor\" \"255 255 255") == string::npos &&
                                line.find("renderfx\" \"0") == string::npos &&
                                line.find("rendermode\" \"0") == string::npos &&
                                line.find("shadowcastdist\" \"0") == string::npos &&
                                line.find("skin\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 4://func_detail
                            if (line.find("dxlevel\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 5://func_brush
                            if (line.find("InputFilter\" \"0") == string::npos &&
                                line.find("invert_exclusion\" \"0") == string::npos &&
                                line.find("renderamt\" \"255") == string::npos &&
                                line.find("rendercolor\" \"255 255 255") == string::npos &&
                                line.find("renderfx\" \"0") == string::npos &&
                                line.find("rendermode\" \"0") == string::npos &&
                                line.find("solidbsp\" \"0") == string::npos &&
                                line.find("Solidity\" \"0") == string::npos &&
                                line.find("StartDisabled") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 6://light: since light and light_spot have some identical parameter names, i wanted to merge them for efficiency (case 7: case 6: break;), but my code shat itself, so yeah
                            if (line.find("_constant_attn\" \"0") == string::npos &&
                                line.find("_distance\" \"0") == string::npos &&
                                line.find("_fifty_percent_distance\" \"0") == string::npos &&
                                line.find("_hardfalloff\" \"0") == string::npos &&
                                line.find("_light\" \"255 255 255 200") == string::npos &&
                                line.find("_lightHDR\" \"-1 -1 -1 1") == string::npos &&
                                line.find("_lightscaleHDR\" \"1") == string::npos &&
                                line.find("_linear_attn\" \"0") == string::npos &&
                                line.find("_quadratic_attn\" \"1") == string::npos &&
                                line.find("_zero_percent_distance\" \"0") == string::npos &&
                                line.find("style\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 7://light_spot
                            if (line.find("_constant_attn\" \"0") == string::npos &&
                                line.find("_distance\" \"0") == string::npos &&
                                line.find("_fifty_percent_distance\" \"0") == string::npos &&
                                line.find("_hardfalloff\" \"0") == string::npos &&
                                line.find("_light\" \"255 255 255 200") == string::npos &&
                                line.find("_lightHDR\" \"-1 -1 -1 1") == string::npos &&
                                line.find("_lightscaleHDR\" \"1") == string::npos &&
                                line.find("_linear_attn\" \"0") == string::npos &&
                                line.find("_quadratic_attn\" \"1") == string::npos &&
                                line.find("_zero_percent_distance\" \"0") == string::npos &&
                                line.find("style\" \"0") == string::npos &&
                                line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("_cone\" \"45") == string::npos &&
                                line.find("_exponent\" \"1") == string::npos &&
                                line.find("_inner_cone\" \"30") == string::npos &&
                                line.find("pitch\" \"-90") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 8://light_dynamic
                            if (line.find("style\" \"0") == string::npos &&
                                line.find("_cone\" \"45") == string::npos &&
                                line.find("_inner_cone\" \"30") == string::npos &&
                                line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("brightness\" \"0") == string::npos &&
                                line.find("distance\" \"120") == string::npos &&
                                line.find("spotlight_radius\" \"80") == string::npos &&
                                line.find("pitch\" \"-90") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 9://func_door
                            if (line.find("shadows\" \"0") == string::npos &&
                                line.find("dmg\" \"0") == string::npos &&
                                line.find("forceclosed\" \"0") == string::npos &&
                                line.find("health\" \"0") == string::npos &&
                                line.find("ignoredebris\" \"0") == string::npos &&
                                line.find("lip\" \"0") == string::npos &&
                                line.find("locked_sentence\" \"0") == string::npos &&
                                line.find("loopmovesound\" \"0") == string::npos &&
                                line.find("movedir\" \"0 0 0") == string::npos &&
                                line.find("renderamt\" \"255") == string::npos &&
                                line.find("rendercolor\" \"255 255 255") == string::npos &&
                                line.find("renderfx\" \"0") == string::npos &&
                                line.find("rendermode\" \"0") == string::npos &&
                                line.find("speed\" \"100") == string::npos &&
                                line.find("unlocked_sentence\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 10://info_decal
                            if (line.find("angles\" \"0 0 0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 11://info_overlay
                            if (line.find("fademaxdist\" \"0") == string::npos &&
                                line.find("fademindist\" \"-1") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 12://trigger_once, trigger_remove, other triggers
                            if (line.find("StartDisabled\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 13://trigger_multiple
                            if (line.find("StartDisabled\" \"0") == string::npos &&
                                line.find("wait\" \"1") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 14://trigger_hurt
                            if (line.find("StartDisabled\" \"0") == string::npos &&
                                line.find("damagemodel\" \"0") == string::npos &&
                                line.find("damagetype\" \"0") == string::npos &&
                                line.find("nodmgforce\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 15://func_areaportal
                            if (line.find("PortalVersion\" \"1") == string::npos &&
                                line.find("StartOpen\" \"1") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 16://func_areaportal_window
                            if (line.find("PortalVersion\" \"1") == string::npos &&
                                line.find("TranslucencyLimit\" \"0.2") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 17://ambient_generic
                            if (line.find("cspinup\" \"0") == string::npos &&
                                line.find("secs\" \"0") == string::npos &&//both fadeinsecs and fadeoutsecs
                                line.find("health\" \"10") == string::npos &&
                                line.find("lfomodpitch\" \"0") == string::npos &&
                                line.find("lfomodvol\" \"0") == string::npos &&
                                line.find("lforate\" \"0") == string::npos &&
                                line.find("lfotype\" \"0") == string::npos &&
                                line.find("pitch\" \"100") == string::npos &&
                                line.find("pitchstart\" \"100") == string::npos &&
                                line.find("preset\" \"0") == string::npos &&
                                line.find("radius\" \"1250") == string::npos &&
                                line.find("spindown\" \"0") == string::npos &&
                                line.find("spinup\" \"0") == string::npos &&
                                line.find("volstart\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 18://brush entities
                            if (line.find("rotation\" \"0") == string::npos &&
                                line.find("smoothing_groups\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 19://light_environment
                            if (line.find("_ambient\" \"255 255 255 20") == string::npos &&
                                line.find("_light\" \"255 255 255 200") == string::npos &&
                                line.find("HDR\" \"-1 -1 -1 1") == string::npos &&//both normal and ambient
                                line.find("caleHDR\" \"1") == string::npos &&//both
                                line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("_inner_cone\" \"30") == string::npos &&
                                line.find("pitch\" \"0") == string::npos &&
                                line.find("SunSpreadAngle\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 20://item_
                            if (line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("AutoMaterialize\" \"1") == string::npos &&
                                line.find("fademaxdist\" \"0") == string::npos &&
                                line.find("fademindist\" \"-1") == string::npos &&
                                line.find("StartDisabled\" \"0") == string::npos &&
                                line.find("TeamNum\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 21://ropes
                            if (line.find("Barbed\" \"0") == string::npos &&
                                line.find("Breakable\" \"0") == string::npos &&
                                line.find("Collide\" \"0") == string::npos &&
                                line.find("Dangling\" \"-1") == string::npos &&
                                line.find("dxlevel\" \"0") == string::npos &&
                                line.find("NoWind\" \"0") == string::npos &&
                                line.find(R"(RopeMaterial" "cable/cable.vmt")") == string::npos &&
                                line.find("Slack\" \"25") == string::npos &&
                                line.find("Subdiv\" \"2") == string::npos &&
                                line.find("TextureScale\" \"1") == string::npos &&
                                line.find("Type\" \"0") == string::npos &&
                                line.find("Width\" \"2") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 22://info_particle_system
                            if (line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("parent\" \"0") == string::npos &&
                                line.find("flag_as_weather\" \"0") == string::npos &&
                                line.find("start_active\" \"0") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 23://point_spotlight
                            if (line.find("angles\" \"0 0 0") == string::npos &&
                                line.find("disablereceiveshadows\" \"0") == string::npos &&
                                line.find("HDRColorScale\" \"1.0") == string::npos &&
                                line.find("IgnoreSolid\" \"0") == string::npos &&
                                line.find("dxlevel\" \"0") == string::npos &&
                                line.find("renderamt\" \"255") == string::npos &&
                                line.find("rendercolor\" \"255 255 255") == string::npos &&
                                line.find("renderfx\" \"0") == string::npos &&
                                line.find("rendermode\" \"0") == string::npos &&
                                line.find("spotlightlength\" \"500") == string::npos &&
                                line.find("spotlightwidth\" \"50") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        case 24://editor_text
                            if (line.find("angles\" \"-0 0 0") == string::npos &&
                                line.find("color\" \"255 255 255") == string::npos &&
                                line.find("textsize\" \"10") == string::npos)
                                out << removeExtraCharacters(line);
                            else currentRemovedLines++;
                            break;
                        default:
                            out << removeExtraCharacters(line);
                            break;
                    }
                } else if (line.find("classname") != string::npos) {//if "classname" is found
                    if (line.find("\"prop_") != string::npos) {
                        if (line.find("static") != string::npos) type = 1;
                        else if (line.find("dynamic") != string::npos) type = 2;//also affects prop_dynamic_override
                        else type = 3;//physics, also affects prop_physics_override, detail and other misc prop objects
                    } else if (line.find("\"func_") != string::npos) {
                        if (line.find("detail") != string::npos) type = 4;
                        else if (line.find("brush") != string::npos) type = 5;
                        else if (line.find("door") != string::npos) type = 9;//both normal and rotating
                        else if (line.find("areaportal") != string::npos) {
                            if (line.find("window\"") != string::npos) type = 16;//areaportal_window
                            else type = 15;//areaportal
                        } else type = 0;
                    } else if (line.find("\"light") != string::npos) {
                        if (line.find("spot") != string::npos) type = 7;//light_spot
                        else if (line.find("dynamic") != string::npos) type = 8;//light_dynamic
                        else if (line.find("environment") != string::npos) type = 19;//light_environment
                        else type = 6;//light
                    } else if (line.find("\"info_") != string::npos) {
                        if (line.find("decal") != string::npos) type = 10;
                        else if (line.find("overlay") != string::npos) type = 11;
                        else if (line.find("particle") != string::npos) type = 22;
                        else type = 0;
                    } else if (line.find("\"trigger") != string::npos) {
                        if (line.find("multiple") != string::npos)
                            type = 13;
                        else if (line.find("hurt") != string::npos) type = 14;
                        else type = 12;
                    } else if (line.find("\"ambient_generic") != string::npos) type = 17;
                    else if (line.find("\"item_") != string::npos) type = 20;
                    else if (line.find("rope\"") != string::npos) type = 21;
                    else if (line.find("\"point_spotlight") != string::npos) type = 23;
                    else if (line.find("\"editor") != string::npos) type = 24;
                    else type = 0;
                    out << removeExtraCharacters(line);
                } else if (line.find("vertices_plus") != string::npos) {
                    while (line.find("material") == string::npos && getline(file, line)) {
                        currentLines++;
                        currentRemovedLines++;
                    }
                    type = 18;
                    out << removeExtraCharacters(line);
                }
            }
            else out << removeExtraCharacters(line);
            currentLines++;
        }
        file.close();
        out.close();
        cout << endl << endl << "Finished file " << userInput << ".\nRemoved " << currentRemovedLines << " out of " << currentLines << " lines. ("
             << (float) currentRemovedLines / (float) currentLines * 100 << "%)\n\nPress ENTER to finish program. If you wish to continue, drop another file here or type file name:\n";
        log << "Finished file " << userInput <<".\nRemoved " << currentRemovedLines << " out of " << currentLines << " lines. ("
            << (float) currentRemovedLines / (float) currentLines * 100 << "%)\n\n";
        totalFiles++;
        totalRemovedLines = totalRemovedLines + currentRemovedLines;
        totalLines = totalLines + currentLines;
    }
    cout << "\nSuccessful compression of " << totalFiles << " files.\nRemoved " << totalRemovedLines << " out of " << totalLines << " total lines. (" << (float) totalRemovedLines / (float) totalLines * 100 << "%)\n\n";
    log << "\nSuccessful compression of " << totalFiles << " files.\nRemoved " << totalRemovedLines << " out of " << totalLines << " total lines. (" << (float) totalRemovedLines / (float) totalLines * 100 << "%)";
    log.close();
}

int main() {
    optimizer();
    return 0;
}
