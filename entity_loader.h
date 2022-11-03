#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "db.h"
#include <string.h>
#include "stdio.h"

namespace Entity_Loader {
    struct Data {
        float radius = 0.0f;
        float speed = 0.0f;
        float mass = 0.0f;
        int health = 0;
        int damage_min = 0;
        int damage_max = 0;
        float melee_range = 0;
        int attack_speed = 0;
        float sight_radius = 0.0f;
        float scale = 0.0f;
        int body_type = 2;
        std::string entity_class = "monster";
        std::string sheet = "none";
    };

	void init_db() {
        const char * db_filepath = "db/data.db";
		int error = sqlite3_open(db_filepath, &db::db);
        if(error){
            //if error then display error and close connection
            std::cout << "DB Open Error: " << sqlite3_errmsg(db::db) << std::endl;
            sqlite3_close(db::db);
        }
        else {
            std::cout << "Opened Database Successfully!" << std::endl;
        }
	}

	Data parse_data(std::string name) {// needs to search for  a specific row that I can input in the arguments
		//check if the name exists??
        std::string unit_name = db::Append_Quotes(name);
		Data values;
		sqlite3_stmt* stmt;
		char buf[400];
		const char* jj = "SELECT radius, speed, mass, health, damage_min, damage_max, melee_range, attack_speed, sight_radius, scale, body_type FROM unit_data WHERE name = ";
		strcpy(buf, jj);
		strcat(buf, unit_name.c_str());
		sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
		while (sqlite3_step(stmt) != SQLITE_DONE) {
			values.radius = (float)sqlite3_column_double(stmt, 0); //0 only increments up when calling more than one column
			values.speed = (float)sqlite3_column_double(stmt, 1); //0 only increments up when calling more than one column
			values.mass= (float)sqlite3_column_double(stmt, 2); //0 only increments up when calling more than one column
			values.health = sqlite3_column_int(stmt, 3);
			values.damage_min = sqlite3_column_int(stmt, 4);
			values.damage_max = sqlite3_column_int(stmt, 5);
			values.melee_range = (float)sqlite3_column_double(stmt, 6);
			values.attack_speed = sqlite3_column_int(stmt, 7);
			values.sight_radius = (float)sqlite3_column_double(stmt, 8);
			values.scale = (float)sqlite3_column_double(stmt, 9);
			values.body_type = sqlite3_column_int(stmt, 10);
            //std::cout << "data: " << name << std::endl;
		}
		return values;
	}

    ////
    /// std::string entity_class MUST BE SINGLE QUOTES WRAPPED IN DOUBLE QUOTES OR ELSE IT THROWS A MEMORY READ EXCEPTION
    ///
    ///
    std::string Get_All_Of_Class(std::string entity_class) {// needs to search for  a specific row that I can input in the arguments
		//check if the name exists??
        std::string class_name = db::Append_Quotes(entity_class);
		std::vector<std::string> db_name;
        const unsigned char* name;
		sqlite3_stmt* stmt;
		char buf[300];
		const char* jj = "SELECT name FROM unit_data WHERE entity_class = ";
		strcpy(buf, jj);
		strcat(buf, class_name.c_str());
		sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
		while (sqlite3_step(stmt) != SQLITE_DONE) {
            name = sqlite3_column_text(stmt, 0);
            const char * s = (const char *)name;
            std::string retname = std::string(reinterpret_cast< const char *> (s));
            db_name.push_back(retname);
            //std::cout << "class: " << name << std::endl;
		}
        //get random index for name
        int i = rand() % ( db_name.size() ) + 1;
        std::string output_name = db_name.at(i-1);
    	return output_name;
	}

    ////
    /// std::string entity_class MUST BE SINGLE QUOTES WRAPPED IN DOUBLE QUOTES OR ELSE IT THROWS A MEMORY READ EXCEPTION
    ///I should probably just fix that
    std::string Get_Sprite_Sheet(std::string &name) {// needs to search for  a specific row that I can input in the arguments
        //check if the name exists??
        std::string unit_name = db::Append_Quotes(name);
        std::vector<std::string> db_name;
        const unsigned char* sheet;
        sqlite3_stmt* stmt;
        char buf[300];
        const char* jj = "SELECT sprite_sheet FROM unit_data WHERE name = ";
        strcpy(buf, jj);
        strcat(buf, unit_name.c_str());
        sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
        std::string retname;
        while (sqlite3_step(stmt) != SQLITE_DONE) {
            sheet = sqlite3_column_text(stmt, 0);
            const char * s = (const char *)sheet;
            unit_name = std::string(reinterpret_cast< const char *> (s));
        }
        //append the ' ' onto the teh first and last character of the string

        return unit_name;
    }
}
