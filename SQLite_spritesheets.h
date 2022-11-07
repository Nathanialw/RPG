#pragma once
#include "db.h"
#include <vector>
#include <SDL2/SDL.h>
#include <string>
#include "components.h"
#include <iostream>
#include <map>
#include "spritesheet_structs.h"


namespace SQLite_Spritesheets {
//    struct Frame_Data_Packer {
//        int startFrame = 9999;
//        int NumFrames = 0;
//        int reverses = 0;
//        int frameSpeed = 75;
//    };
//            = { 9999, 0, 0, 75}

    int i = 100;

    struct Sheet_Data_Flare {
        int w = 0;
        int h = 0;
        float x_offset = 0;
        float y_offset = 0;
        int sheet_width = 0;
        int64_t time_between_frames = 0;
        std::string sheet_type = "default";
            /// initial values already set in component.h
        Component::Frame_Data_Packer isStatic;
        Component::Frame_Data_Packer idle = { 9999, 0, 0, i };
        Component::Frame_Data_Packer walk;
        Component::Frame_Data_Packer run;
        Component::Frame_Data_Packer attack = { 9999, 0, 0, i };
        Component::Frame_Data_Packer attack2 = { 9999, 0, 0, i };
        Component::Frame_Data_Packer cast = { 9999, 0, 0, i };
        Component::Frame_Data_Packer struck = { 9999, 0, 0, i };
        Component::Frame_Data_Packer block = { 9999, 0, 0, i };
        Component::Frame_Data_Packer evade = { 9999, 0, 0, i };
        Component::Frame_Data_Packer stunned;
        Component::Frame_Data_Packer dying = { 9999, 0, 0, i };
        Component::Frame_Data_Packer corpse;
        Component::Frame_Data_Packer low_hp;
        Component::Frame_Data_Packer resting;
        Component::Frame_Data_Packer ranged;
        Component::Frame_Data_Packer cheer;
        Component::Frame_Data_Packer behavior;
        Component::Frame_Data_Packer summoned;
    };
        ///will hold all the flare spritesheets
    std::unordered_map<std::string, Component::Sheet_Data_Flare> Flare_Spritesheets;

    Sheet_Data_Flare Get_Flare_Spritesheet_Data_From_db(std::string sheet) {// needs to search for  a specific row that I can input in the arguments
        std::string sheet_name = db::Append_Quotes(sheet);
        //check if the name exists??
        Sheet_Data_Flare values;
        sqlite3_stmt *stmt;
        char buf[750];
        const char *ii = "SELECT static_start, static_num_frames, static_reverses, idle_start, idle_num_frames, idle_reverses, walk_start, walk_num_frames, walk_reverses, run_start, run_num_frames, run_reverses, attack_start, attack_num_frames, attack_reverses, attack_2_start, attack_2_num_frames, attack_2_reverses, cast_start, cast_num_frames, cast_reverses, struck_start, struck_num_frames, struck_reverses, block_start, block_num_frames, block_reverses, evade_start, evade_num_frames, evade_reverses, stunned_start, stunned_num_frames, stunned_reverses, dying_start, dying_num_frames, dying_reverses, corpse_start, corpse_num_frames, corpse_reverses FROM sprite_sheet WHERE sprite_sheet = ";
        strcpy(buf, ii);
        strcat(buf, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            values.isStatic.startFrame = sqlite3_column_int(stmt, 0);
            values.isStatic.NumFrames = sqlite3_column_int(stmt, 1);
            values.isStatic.reverses = sqlite3_column_int(stmt, 2);

            values.idle.startFrame = sqlite3_column_int(stmt, 3);
            values.idle.NumFrames = sqlite3_column_int(stmt, 4);
            values.idle.reverses = sqlite3_column_int(stmt, 5);

            values.walk.startFrame = sqlite3_column_int(stmt, 6);
            values.walk.NumFrames = sqlite3_column_int(stmt, 7);
            values.walk.reverses = sqlite3_column_int(stmt, 8);

            values.run.startFrame = sqlite3_column_int(stmt, 9);
            values.run.NumFrames = sqlite3_column_int(stmt, 10);
            values.run.reverses = sqlite3_column_int(stmt, 11);

            values.attack.startFrame = sqlite3_column_int(stmt, 12);
            values.attack.NumFrames = sqlite3_column_int(stmt, 13);
            values.attack.reverses = sqlite3_column_int(stmt, 14);

            values.attack2.startFrame = sqlite3_column_int(stmt, 15);
            values.attack2.NumFrames = sqlite3_column_int(stmt, 16);
            values.attack2.reverses = sqlite3_column_int(stmt, 17);

            values.cast.startFrame = sqlite3_column_int(stmt, 18);
            values.cast.NumFrames = sqlite3_column_int(stmt, 19);
            values.cast.reverses = sqlite3_column_int(stmt, 20);

            values.struck.startFrame = sqlite3_column_int(stmt, 21);
            values.struck.NumFrames = sqlite3_column_int(stmt, 22);
            values.struck.reverses = sqlite3_column_int(stmt, 23);

            values.block.startFrame = sqlite3_column_int(stmt, 24);
            values.block.NumFrames = sqlite3_column_int(stmt, 25);
            values.block.reverses = sqlite3_column_int(stmt, 26);

            values.evade.startFrame = sqlite3_column_int(stmt, 27);
            values.evade.NumFrames = sqlite3_column_int(stmt, 28);
            values.evade.reverses = sqlite3_column_int(stmt, 29);

            values.stunned.startFrame = sqlite3_column_int(stmt, 30);
            values.stunned.NumFrames = sqlite3_column_int(stmt, 31);
            values.stunned.reverses = sqlite3_column_int(stmt, 32);

            values.dying.startFrame = sqlite3_column_int(stmt, 33);
            values.dying.NumFrames = sqlite3_column_int(stmt, 34);
            values.dying.reverses = sqlite3_column_int(stmt, 35);

            values.corpse.startFrame = sqlite3_column_int(stmt, 36);
            values.corpse.NumFrames = sqlite3_column_int(stmt, 37);
            values.corpse.reverses = sqlite3_column_int(stmt, 38);
        }
//        sqlite3_stmt* stmt2;
//        char buf2[500];
//        const char* jj = "SELECT low_hp_start, low_hp_num_frames, low_hp_reverses, resting_start, resting_num_frames, resting_reverses, ranged_start, ranged_num_frames, ranged_reverses, cheer_start, cheer_num_frames, cheer_reverses, behavior_start, behavior_num_frames, behavior_reverses, summoned_start, summoned_num_frames, summoned_reverses, FROM sprite_sheet WHERE sprite_sheet = ";
//        strcpy(buf2, jj);
//        strcat(buf2, sheet_name.c_str());
//        sqlite3_prepare_v2(db::db, buf2, -1, &stmt2, 0);
//        while (sqlite3_step(stmt2) != SQLITE_DONE) {
//            values.low_hp.firstFrame = sqlite3_column_int(stmt, 0);
//            values.low_hp.numFrames = sqlite3_column_int(stmt, 1);
//            values.low_hp.reverses = sqlite3_column_int(stmt, 39);
//
//            values.resting.firstFrame = sqlite3_column_int(stmt, 40);
//            values.resting.numFrames = sqlite3_column_int(stmt, 41);
//            values.resting.reverses = sqlite3_column_int(stmt, 42);
//
//            values.cheer.firstFrame = sqlite3_column_int(stmt, 43);
//            values.cheer.numFrames = sqlite3_column_int(stmt, 44);
//            values.cheer.reverses = sqlite3_column_int(stmt, 45);
//
//            values.ranged.firstFrame = sqlite3_column_int(stmt, 46);
//            values.ranged.numFrames = sqlite3_column_int(stmt, 47);
//            values.ranged.reverses = sqlite3_column_int(stmt, 48);
//
//            values.behavior.firstFrame = sqlite3_column_int(stmt, 49);
//            values.behavior.numFrames = sqlite3_column_int(stmt, 50);
//            values.behavior.reverses = sqlite3_column_int(stmt, 51);
//
//            values.summoned.firstFrame = sqlite3_column_int(stmt, 52);
//            values.summoned.numFrames = sqlite3_column_int(stmt, 53);
//            values.summoned.reverses = sqlite3_column_int(stmt, 54);
//            //std::cout << "use sheet: " << sheet << std::endl;
//        }
        return values;
    }

    void Get_Flare_Spritesheet_Layout_From_db(std::string &sheet, Sheet_Data_Flare &data) {// needs to search for  a specific row that I can input in the arguments
        std::string sheet_name = db::Append_Quotes(sheet);
        sqlite3_stmt *stmt;
        const unsigned char *sheetType;
        char buf[200];
        const char *ii = "SELECT sprite_width, sprite_height, x_offset, y_offset, sheet_width, sheet_type, time_between_frames FROM sprite_layout WHERE sprite_sheet = ";
        strcpy(buf, ii);
        strcat(buf, sheet_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            data.w = sqlite3_column_int(stmt, 0);
            data.h = sqlite3_column_int(stmt, 1);
            data.x_offset = (float) sqlite3_column_double(stmt, 2);
            data.y_offset = (float) sqlite3_column_double(stmt, 3);
            data.sheet_width = sqlite3_column_int(stmt, 4);
            sheetType = sqlite3_column_text(stmt, 5);
            data.time_between_frames = sqlite3_column_int(stmt, 6);
            const char *s = (const char *) sheetType;
            data.sheet_type = std::string(reinterpret_cast< const char *> (s));
        }
    }

    Sheet_Data_Flare Get_Flare_From_DB(std::string &sheet, Sheet_Data_Flare &sheetData) {
        sheetData = Get_Flare_Spritesheet_Data_From_db(sheet);
        Get_Flare_Spritesheet_Layout_From_db(sheet, sheetData);
        return sheetData;
    }

    std::unordered_map<std::string, Component::Sheet_Data_Flare>* Populate_Flare_SpriteSheet(std::string &name, Sheet_Data_Flare &data, SDL_Texture* texture) {
        if (Flare_Spritesheets[name].texture == NULL) {
            Flare_Spritesheets[name].texture = texture;
            Flare_Spritesheets[name].sheetWidth = data.sheet_width;
            Flare_Spritesheets[name].spriteWidth = data.w;
            Flare_Spritesheets[name].spriteHeight = data.h;
            Flare_Spritesheets[name].actionFrameData[Component::block] = data.block;
            Flare_Spritesheets[name].actionFrameData[Component::attack] = data.attack;
            Flare_Spritesheets[name].actionFrameData[Component::run] = data.run;
            Flare_Spritesheets[name].actionFrameData[Component::idle] = data.idle;
            Flare_Spritesheets[name].actionFrameData[Component::struck] = data.struck;
            Flare_Spritesheets[name].actionFrameData[Component::dead] = data.dying;
            Flare_Spritesheets[name].actionFrameData[Component::cast] = data.cast;
            Flare_Spritesheets[name].actionFrameData[Component::attack2] = data.attack2;
            Flare_Spritesheets[name].actionFrameData[Component::behavior] = data.behavior;
            Flare_Spritesheets[name].actionFrameData[Component::cheer] = data.cheer;
            Flare_Spritesheets[name].actionFrameData[Component::evade] = data.evade;
            Flare_Spritesheets[name].actionFrameData[Component::isStatic] = data.isStatic;
            Flare_Spritesheets[name].actionFrameData[Component::idle_low_hp] = data.low_hp;
            Flare_Spritesheets[name].actionFrameData[Component::ranged] = data.ranged;
            Flare_Spritesheets[name].actionFrameData[Component::resting] = data.resting;
            Flare_Spritesheets[name].actionFrameData[Component::stunned] = data.stunned;
            Flare_Spritesheets[name].actionFrameData[Component::summoned] = data.summoned;
            Flare_Spritesheets[name].actionFrameData[Component::walk] = data.walk;
            return &Flare_Spritesheets;
        }
        else {
            return &Flare_Spritesheets;
        }
    }
}