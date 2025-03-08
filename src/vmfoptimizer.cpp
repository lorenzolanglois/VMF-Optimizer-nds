//Program deletes VMF parameters with default values, since missing parameters are automatically replaced with defaults, hence lossless optimization
//Made with TF2 in mind, but supports .vmf of any game
//Uses default Hammer/Hammer++ default values as an example. Custom FGDs not accounted.
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//TO DO:
//options to remove the old file
//options to remove vertices_plus (hammer++) and editor (entity data)
//option "is this a prefab?"
int main() {
    cout<<"VMF optimizer by dabmasterars."<<endl<<"Type file name: "<<endl;
    string line; cin>>line;//reused later as a buffer
    ifstream in;
    if (line.find(".vmf")==string::npos) in.open(line.append(".vmf"));
    else in.open(line);
    //ifstream in("file.txt");//test input
    if (!in.is_open()) {
        cerr << "Error. File \""<< line <<"\" doesn't exist."<<endl;
        ofstream log("log.txt");//log
        log<<"Error. File \""<< line <<"\" doesn't exist.";
        log.close();
        system("pause");
        return 1;
    }
    ofstream out(line.append("OPTIMIZED.vmf"));//output
    //ofstream out("file2.txt");//test output
    if (!out.is_open()) {
        cerr << "Output error." << endl;
        system("pause");
        return 1;
    }
    char type;
    //0 - none, 1 - prop_static, 2 - prop_dynamic, 3 - prop_physics, 4 - func_detail, 5 - func_brush, 6 - light, 7 - light_spot, 8 - light_dynamic, 9 - func_door/func_door_rotating
    //10 - info_decal, 11 - info overlay, 12 - trigger_once/remove, 13 - trigger_multiple, 14 - trigger_hurt, 15 - func_areaportal, 16 - func_areaportalwindow, 17 - ambient_generic
    //string line;//line that is written down to the second file
    unsigned int count_t=0,count_r=0;//count total amount of lines/count how many lines were removed
    while (getline(in, line)) {//yandere dev moment
        if (line.length()>4||line.find("classname") == string::npos){//originally there was find("name") to not corrupt the vmf if some dumbass decides to name their prop "angles 0 0 0", but you can't use quotes without corruption anyway.
            switch(type){
                case 0:
                    out << line << endl;
                    break;
                case 1://prop_static
                    if (line.find("angles\" \"0 0 0")==string::npos&&
                    line.find("fademindist\" \"-1")==string::npos&&
                    line.find("fadescale\" \"1")==string::npos&&
                    line.find("lightmapresolutionx\" \"32")==string::npos&&//i could probably
                    line.find("lightmapresolutiony\" \"32")==string::npos&&//merge these two
                    line.find("skin\" \"0")==string::npos&&
                    line.find("solid\" \"6")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 2://prop_dynamic
                    out << line << endl;
                    break;
                case 5://func_brush
                    if (line.find("InputFilter\" \"0")==string::npos&&
                    line.find("invert_exclusion\" \"0")==string::npos&&
                    line.find("renderamt\" \"255")==string::npos&&
                    line.find("rendercolor\" \"255 255 255")==string::npos&&//i could probably
                    line.find("renderfx\" \"0")==string::npos&&//merge these two
                    line.find("rendermode\" \"0")==string::npos&&
                    line.find("solidbsp\" \"0")==string::npos&&
                    line.find("Solidity\" \"0")==string::npos&&
                    line.find("StartDisabled")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 6://light: since light and light_spot have some identical parameter names, i wanted to merge them for efficiency (case 7: case 6: break;), but my code shat itself, so yeah
                    if (line.find("_constant_attn\" \"0")==string::npos&&
                    line.find("_distance\" \"0")==string::npos&&
                    line.find("_fifty_percent_distance\" \"0")==string::npos&&
                    line.find("_hardfalloff\" \"0")==string::npos&&
                    line.find("_light\" \"255 255 255 200")==string::npos&&
                    line.find("_lightHDR\" \"-1 -1 -1 1")==string::npos&&
                    line.find("_lightscaleHDR\" \"1")==string::npos&&
                    line.find("_linear_attn\" \"0")==string::npos&&
                    line.find("_quadratic_attn\" \"1")==string::npos&&
                    line.find("_zero_percent_distance\" \"0")==string::npos&&
                    line.find("style\" \"0")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 7://light_spot
                    if (line.find("_constant_attn\" \"0")==string::npos&&
                    line.find("_distance\" \"0")==string::npos&&
                    line.find("_fifty_percent_distance\" \"0")==string::npos&&
                    line.find("_hardfalloff\" \"0")==string::npos&&
                    line.find("_light\" \"255 255 255 200")==string::npos&&
                    line.find("_lightHDR\" \"-1 -1 -1 1")==string::npos&&
                    line.find("_lightscaleHDR\" \"1")==string::npos&&
                    line.find("_linear_attn\" \"0")==string::npos&&
                    line.find("_quadratic_attn\" \"1")==string::npos&&
                    line.find("_zero_percent_distance\" \"0")==string::npos&&
                    line.find("style\" \"0")==string::npos&&
                    line.find("angles\" \"0 0 0")==string::npos&&
                    line.find("_cone\" \"45")==string::npos&&
                    line.find("_exponent\" \"1")==string::npos&&
                    line.find("_inner_cone\" \"30")==string::npos&&
                    line.find("pitch\" \"-90")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 8://light_dynamic
                    if (line.find("style\" \"0")==string::npos&&
                    line.find("_cone\" \"45")==string::npos&&
                    line.find("_inner_cone\" \"30")==string::npos&&
                    line.find("angles\" \"0 0 0")==string::npos&&
                    line.find("brightness\" \"0")==string::npos&&
                    line.find("distance\" \"120")==string::npos&&
                    line.find("spotlight_radius\" \"80")==string::npos&&
                    line.find("pitch\" \"-90")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 9://func_door
                    if (line.find("shadows\" \"0")==string::npos&&//removed both disableshadows and disablereceiveshadows
                    line.find("dmg\" \"0")==string::npos&&
                    line.find("forceclosed\" \"0")==string::npos&&
                    line.find("health\" \"0")==string::npos&&//i could probably
                    line.find("ignoredebris\" \"0")==string::npos&&//merge these two
                    line.find("lip\" \"0")==string::npos&&
                    line.find("locked_sentence\" \"0")==string::npos&&
                    line.find("loopmovesound\" \"0")==string::npos&&
                    line.find("movedir\" \"0 0 0")==string::npos&&
                    line.find("renderamt\" \"255")==string::npos&&
                    line.find("rendercolor\" \"255 255 255")==string::npos&&//i could probably
                    line.find("renderfx\" \"0")==string::npos&&//merge these two
                    line.find("rendermode\" \"0")==string::npos&&
                    line.find("speed\" \"100")==string::npos&&
                    line.find("unlocked_sentence\" \"0")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 10://info_decal
                    if (line.find("angles\" \"0 0 0")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 11://info_overlay
                    if (line.find("fademindist\" \"-1")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 12://trigger_once, trigger_remove, other triggers
                    if (line.find("StartDisabled\" \"0")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 13://trigger_multiple
                    if (line.find("StartDisabled\" \"0")==string::npos&&
                    line.find("wait\" \"1")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 14://trigger_hurt
                    if (line.find("StartDisabled\" \"0")==string::npos&&
                    line.find("damagemodel\" \"0")&&
                    line.find("damagetype\" \"0")&&
                    line.find("nodmgforce\" \"0")==string::npos)
                    out << line << endl; else count_r++;
                    break;
                case 15://func_areaportal
                    if (line.find("PortalVersion\" \"1")==string::npos&&
                    line.find("StartOpen\" \"1")==string::npos)
                        out << line << endl; else count_r++;
                    break;
                case 16://func_areaportal_window
                    if (line.find("PortalVersion\" \"1")==string::npos&&
                    line.find("TranslucencyLimit\" \"0.2")==string::npos)
                        out << line << endl; else count_r++;
                    break;
                case 17://ambient_generic
                    if (line.find("cspinup\" \"0")==string::npos&&
                    line.find("fadeinsecs\" \"0")&&
                    line.find("fadeoutsecs\" \"0")&&
                    line.find("health\" \"10")&&
                    line.find("lfomodpitch\" \"0")&&
                    line.find("lfomodvol\" \"0")&&
                    line.find("lforate\" \"0")&&
                    line.find("lfotype\" \"0")&&
                    line.find("pitch\" \"100")&&
                    line.find("pitchstart\" \"100")&&
                    line.find("preset\" \"0")&&
                    line.find("radius\" \"1250")&&
                    line.find("spindown\" \"0")&&
                    line.find("spinup\" \"0")&&
                    line.find("volstart\" \"0")==string::npos)
                        out << line << endl; else count_r++;
                    break;
                default: out << line << endl; break;
            }
            if (line.find("classname") != string::npos){//for some reason, moving this out of the loop (turning it to else if) breaks the script
                if (line.find("\"prop_")!= string::npos){
                    if(line.find("static")!= string::npos) type=1;
                    else if(line.find("dynamic")!= string::npos) type=2;//also affects prop_dynamic_override
                    else type=3;//physics, also affects prop_physics_override, detail and other misc prop objects
                }
                else if (line.find("\"func_brush")!= string::npos) type=5;
                else if (line.find("\"light")!= string::npos){
                    if(line.find("spot")!= string::npos) type=7;//light_spot
                    else if(line.find("dynamic")!= string::npos) type=8;//light_dynamic
                    else type=6;//light
                }
                else if (line.find("\"func_door")!= string::npos) type=9;//both normal and rotating
                else if (line.find("\"info_decal")!= string::npos) type=10;
                else if (line.find("\"info_overlay")!= string::npos) type=11;
                else if(line.find("\"trigger")!= string::npos){
                    if(line.find("multiple")!= string::npos) type=13;//multiple goes first because it's the most frequent
                    else if(line.find("hurt")!= string::npos) type=14;
                    else type=12;
                }
                else if (line.find("\"areaportal")!= string::npos){
                    if(line.find("window")!= string::npos) type=16;//areaportal_window
                    else type=15;//areaportal
                }
                else if (line.find("\"ambient_generic")!= string::npos) type=17;
            }
        }
        else out << line << endl;
        count_t++;
        //if (line=="entity") cout << "cock";
    }
    in.close();
    out.close();
    cout <<endl<<endl<< "Execution successful." << endl << "Removed " << count_r << " out of " <<count_t<< " lines. ("<<(float)count_r/(float)count_t*100<<"%)"<<endl<<endl;
    ofstream log("log.txt");//log
    log<<"Execution successful." << endl << "Removed " << count_r << " out of " <<count_t<< " lines. ("<<(float)count_r/(float)count_t*100<<"%)";
    log.close();
    system("pause");
    return 0;
}