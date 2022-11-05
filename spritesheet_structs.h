#pragma once
#include "db.h"

namespace Spritesheet_Structs {


    struct Frame_Data {
        //number of frames/end if sheet
        int numFrames = 0;
        //index of beginning frame
        int firstFrame = 0;
        //reversability
        bool reverses = false;
        //time between frames
        int64_t frameLength = 16;
    };

    struct Frame {
        int startFrame;
        int numFrames;
    };



    struct spriteSheet {
        //size of sprites
        int w;
        int h;
        std::map<Component::Action_State, Frame_Data>* frameData;
        float offset_x;
        float offset_y;
        SDL_Texture *texture;
    };

    spriteSheet* demon;
    spriteSheet* cow;
    spriteSheet* skeleton_00;

    std::map<Component::Action_State, Frame_Data> frameData;


    struct Data_On_Unit {
        spriteSheet frameData;
        float timeOnFrame;
        int currentFrame;
    };



    struct sheetData {
        int w = 0;
        int h = 0;
        float x_offset = 0;
        float y_offset = 0;
        int sheet_width = 0;
        int64_t time_between_frames = 0;
        std::string sheet_type = "flare";
        Frame_Data isStatic = {0, 0, 0, 0};
        Frame_Data idle = {0, 0, 0, 0};
        Frame_Data walk = {0, 0, 0, 0};
        Frame_Data run = {0, 0, 0, 0};
        Frame_Data attack = {0, 0, 0, 0};
        Frame_Data attack2 = {0, 0, 0, 0};
        Frame_Data cast = {0, 0, 0, 0};
        Frame_Data struck = {0, 0, 0, 0};
        Frame_Data block = {0, 0, 0, 0};
        Frame_Data evade = {0, 0, 0, 0};
        Frame_Data stunned = {0, 0, 0, 0};
        Frame_Data dying = {0, 0, 0, 0};
        Frame_Data corpse = {0, 0, 0, 0};
        Frame_Data low_hp = {0, 0, 0, 0};
        Frame_Data resting = {0, 0, 0, 0};
        Frame_Data ranged = {0, 0, 0, 0};
        Frame_Data cheer = {0, 0, 0, 0};
        Frame_Data behavior = {0, 0, 0, 0};
        Frame_Data summoned = {0, 0, 0, 0};
    };

    std::string Get_Sprite_Sheet(std::string &name) {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
        std::string unit_name = db::Append_Quotes(name);
        const unsigned char* sheet;
        sqlite3_stmt* stmt;
        char buf[300];
        const char* jj = "SELECT sprite_sheet FROM unit_data WHERE name = ";
        strcpy(buf, jj);
        strcat(buf, unit_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            sheet = sqlite3_column_text(stmt, 0);
            const char * s = (const char *)sheet;
            unit_name = std::string(reinterpret_cast< const char *> (s));
        }

        return unit_name;
    }



}