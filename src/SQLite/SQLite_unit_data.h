#pragma once

#include "db.h"
#include <SDL2/SDL.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace Entity_Loader {
    struct Data {
	float radius = 0.0f;
	float speed = 0.0f;
	float mass = 0.0f;
	int health = 0;
	int damage_min = 0;
	int damage_max = 0;
	float melee_range = 0.0f;
	float attack_speed = 0.0f;
	float sight_radius = 0.0f;
	float scale = 0.0f;
	int body_type = 2;
	float interact_r = 10.0f;
	float interact_h = 10.0f;
	float x_offset = 0.0f;
	float y_offset = 0.0f;
	std::string temp_type_name = "NPC";
	std::string equip_type = "none";
	std::string entity_class = "monster";
	std::string sheet = "none";
	std::string race = "rogue";
	int whole_sprite = 1;
	bool hexDir;
	int interactable = 0;
	int light_radius = 0;
	std::string icon_sprite = "cryptkeeper";
	std::string icon_dead = "cryptkeeper";
	int icon_num_frames = 4;
    };

    int Get_Unit_Data_By_Column(std::string name, std::string column) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	std::string unit_column = db::Append_Quotes(column);
	int value;

	sqlite3_stmt *stmt;
	char buf[100];
	const char *jj = "SELECT ";
	const char *dd = " FROM unit_data WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, column.c_str());
	strcat(buf, dd);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);

	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    value = sqlite3_column_int(stmt, 0);
	}
	return value;
    }

    Data parse_data(std::string name) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	Data values;
	sqlite3_stmt *stmt;
	char buf[400];
	const char *jj = "SELECT radius, speed, mass, health, damage_min, damage_max, melee_range, attack_speed, sight_radius, scale, body_type, interact_r, interact_h, x_offset, y_offset, equip_type, race, temp_type_name, whole_sprite, hexa_directional, interactable, light_radius, icon_sprite, num_icon_frames, icon_dead FROM unit_data WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    values.radius = (float) sqlite3_column_double(stmt, 0);//0 only increments up when calling more than one column
	    values.speed = (float) sqlite3_column_double(stmt, 1); //0 only increments up when calling more than one column
	    values.mass = (float) sqlite3_column_double(stmt, 2);  //0 only increments up when calling more than one column
	    values.health = sqlite3_column_int(stmt, 3);
	    values.damage_min = sqlite3_column_int(stmt, 4);
	    values.damage_max = sqlite3_column_int(stmt, 5);
	    values.melee_range = (float)sqlite3_column_double(stmt, 6);
	    values.attack_speed = (float)sqlite3_column_double(stmt, 7);
	    values.sight_radius = (float) sqlite3_column_double(stmt, 8);
	    values.scale = (float) sqlite3_column_double(stmt, 9);
	    values.body_type = sqlite3_column_int(stmt, 10);
	    values.interact_r = (float)sqlite3_column_double(stmt, 11);
	    values.interact_h = (float)sqlite3_column_double(stmt, 12);
	    values.x_offset = (float)sqlite3_column_double(stmt, 13);
	    values.y_offset = (float)sqlite3_column_double(stmt, 14);
	    values.equip_type = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 15)));
	    values.race = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 16)));
	    values.temp_type_name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 17)));
	    values.whole_sprite = sqlite3_column_int(stmt, 18);
	    values.hexDir = sqlite3_column_int(stmt, 19);
	    values.interactable = sqlite3_column_int(stmt, 20);
	    values.light_radius = sqlite3_column_int(stmt, 21);
	    values.icon_sprite = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 22)));
	    values.icon_num_frames = sqlite3_column_int(stmt, 23);
	    values.icon_dead = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 24)));
	    //std::cout << "data: " << name << std::endl;
	}
	return values;
    }

    std::vector<db::Unit_Data> Get_Unit_Subtypes(std::string race, std::string entity_class) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string race_name = db::Append_Quotes(race);
	std::string type_name = db::Append_Quotes(entity_class);
	std::string text = race_name + " AND entity_class = " + type_name;// + " AND sprite_sheet = texture_packer";
	std::vector<db::Unit_Data> db_name;

	db::Unit_Data data;
	const unsigned char *name;
	const unsigned char *imgPath;
	const unsigned char *facePath;
	const unsigned char *bodyPath;
	sqlite3_stmt *stmt;
	char buf[300];

	const char *jj = "SELECT name, image_path, face_path, body_path FROM unit_data WHERE race = ";
	strcpy(buf, jj);
	strcat(buf, text.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    std::string retname = db::Convert_Char("name", name);

	    imgPath = sqlite3_column_text(stmt, 1);
	    std::string img_path = db::Convert_Char("image_path", imgPath);

	    facePath = sqlite3_column_text(stmt, 2);
	    std::string face_path = db::Convert_Char("face_path", facePath);

	    bodyPath = sqlite3_column_text(stmt, 3);
	    std::string body_path = db::Convert_Char("body_path", bodyPath);

	    data = {retname, img_path, face_path, body_path};
	    db_name.push_back(data);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    db::Unit_Data Get_Character_Create(std::string name) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string text = db::Append_Quotes(name);
	db::Unit_Data data;

	const unsigned char *imgPath;
	const unsigned char *facePath;
	const unsigned char *bodyPath;
	sqlite3_stmt *stmt;
	char buf[300];

	const char *jj = "SELECT image_path, face_path, body_path FROM unit_data WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, text.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    imgPath = sqlite3_column_text(stmt, 0);
	    std::string img_path = db::Convert_Char("image_path", imgPath);

	    facePath = sqlite3_column_text(stmt, 1);
	    std::string face_path = db::Convert_Char("face_path", facePath);

	    bodyPath = sqlite3_column_text(stmt, 2);
	    std::string body_path = db::Convert_Char("body_path", bodyPath);

	    return {name, img_path, face_path, body_path};
	}
	Utilities::Log("db::Get_Character_Create() " + name + " from DB NULL value, passthrough error");
	return {"", "", "", ""};
    }

    std::vector<std::string> Get_Tileset_Objects(std::string race, std::string type) {
	std::string race_name = db::Append_Quotes(race);
	std::string type_name = db::Append_Quotes(type);
	std::string text = race_name + " AND type = " + type_name + " AND tileset = 'forest'";
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];

	const char *jj = "SELECT name FROM building_exteriors WHERE race = ";
	strcpy(buf, jj);
	strcat(buf, text.c_str());

	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	//sort
	std::sort(db_name.begin(), db_name.end(), [](const std::string &lhs, const std::string &rhs) {
	    return lhs < rhs;
	});
	return db_name;
    }

    std::vector<std::string> Get_Names_Of_SubType(std::string race, std::string type, std::string subtype, std::string collider_type) {
	std::string race_name = db::Append_Quotes(race);
	std::string type_name = db::Append_Quotes(type);
	std::string class_name = db::Append_Quotes(subtype);
	std::string subtype_name = db::Append_Quotes(collider_type);
	std::string text = race_name + " AND type = " + type_name + " AND subtype = " + class_name + " AND collider_type = " + subtype_name;
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];

	const char *jj = "SELECT name FROM building_exteriors WHERE race = ";
	strcpy(buf, jj);
	strcat(buf, text.c_str());

	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	//    sort
	std::sort(db_name.begin(), db_name.end(), [](const std::string &lhs, const std::string &rhs) {
	    return lhs < rhs;
	});
	return db_name;
    }

    std::vector<std::string> Get_Names_Of_SubType(std::string race, std::string type, std::string subtype) {
	std::string race_name = db::Append_Quotes(race);
	std::string type_name = db::Append_Quotes(type);
	std::string class_name = db::Append_Quotes(subtype);
	//    std::string subtype_name = db::Append_Quotes(collider_type);
	std::string text = race_name + " AND type = " + type_name + " AND subtype = " + class_name;
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];

	const char *jj = "SELECT name FROM building_exteriors WHERE race = ";
	strcpy(buf, jj);
	strcat(buf, text.c_str());

	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    std::string retname = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	//    sort
	std::sort(db_name.begin(), db_name.end(), [](const std::string &lhs, const std::string &rhs) {
	    return lhs < rhs;
	});
	return db_name;
    }

    std::vector<std::string> Get_All_Names_Of_SubType(std::string &type) {
	std::string class_name = db::Append_Quotes(type);
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT name FROM building_exteriors WHERE type = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    std::string retname = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::string Get_Tileset_Path(std::string tilesetName) {
	std::string tileset = db::Append_Quotes(tilesetName);
	std::string tilesetPath;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT path FROM tilesets WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, tileset.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    tilesetPath = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	}
	return tilesetPath;
    }

    struct TileSet_Data {
	std::string name;
	std::string music;
	std::string tileset;
	std::string tileUnits;
	std::string tilesetXML;
    };

    TileSet_Data Get_Tileset_Data(std::string tilesetName) {
	std::string tileset = db::Append_Quotes(tilesetName);
	std::string tilesetMusic;
	std::string tilesetTileset;
	std::string tilesetUnits;
	std::string tilesetXML;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT music, tileset, units, tileset_XML FROM tilesets WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, tileset.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    tilesetMusic = db::Convert_Char("", sqlite3_column_text(stmt, 0));
	    tilesetTileset = db::Convert_Char("", sqlite3_column_text(stmt, 1));
	    tilesetUnits = db::Convert_Char("", sqlite3_column_text(stmt, 2));
	    tilesetXML = db::Convert_Char("", sqlite3_column_text(stmt, 3));
	}
	return {tilesetName, tilesetMusic, tilesetTileset, tilesetUnits, tilesetXML};
    }

    std::vector<std::string> Get_All_Subtype_Of_Type(std::string &type) {
	std::string class_name = db::Append_Quotes(type);
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT subtype FROM building WHERE type = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    std::string retname = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::vector<std::string> Get_All_Types_Of_Race(std::string &type) {
	std::string class_name = db::Append_Quotes(type);
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT type FROM building_exteriors WHERE race = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    std::string retname = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::vector<std::string> Get_All_Of_Races() {// needs to search for  a specific row that I can input in the arguments
	std::vector<std::string> db_name;
	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT race FROM race_relationships";
	strcpy(buf, jj);
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::vector<std::string> Get_All_Items_Of_Type(std::string &type) {
	std::string class_name = db::Append_Quotes(type);
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT type FROM weapon_types WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::vector<std::string> Get_All_Objects_Of_Type(std::string &type) {
	std::string class_name = db::Append_Quotes(type);
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT name FROM building_exteriors WHERE type = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::vector<std::string> Get_All_Units_Of_Type(std::string &type) {
	std::string class_name = db::Append_Quotes(type);
	std::vector<std::string> db_name;

	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT name FROM unit_data WHERE type = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }

    std::vector<std::string> Get_All_Of_Type() {// needs to search for  a specific row that I can input in the arguments
	std::vector<std::string> db_name;
	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT object_type FROM unit_types";
	strcpy(buf, jj);
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	db_name.shrink_to_fit();
	return db_name;
    }


    ////
    /// std::string entity_class MUST BE SINGLE QUOTES WRAPPED IN DOUBLE QUOTES OR ELSE IT THROWS A MEMORY READ EXCEPTION
    ///
    ///
    std::string Get_All_Of_Class(std::string entity_class) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string class_name = db::Append_Quotes(entity_class);
	std::vector<std::string> db_name;
	const unsigned char *name;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT name FROM unit_data WHERE entity_class = ";
	strcpy(buf, jj);
	strcat(buf, class_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    name = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) name;
	    std::string retname = std::string(reinterpret_cast<const char *>(s));
	    db_name.push_back(retname);
	}
	//get random index for name
	int i = rand() % (db_name.size()) + 1;
	std::string output_name = db_name.at(i - 1);
	return output_name;
    }

    ////
    /// std::string entity_class MUST BE SINGLE QUOTES WRAPPED IN DOUBLE QUOTES OR ELSE IT THROWS A MEMORY READ EXCEPTION
    ///I should probably just fix that
    std::string Get_Sprite_Sheet(std::string &name) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	const unsigned char *sheet;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT sprite_sheet FROM unit_data WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    sheet = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) sheet;
	    unit_name = std::string(reinterpret_cast<const char *>(s));
	}

	return unit_name;
    }

    std::string Get_Building_Sprite_layout(std::string &name) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	std::string sprite_layout;
	const unsigned char *sheet;
	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT sprite_layout FROM building_exteriors WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    sheet = sqlite3_column_text(stmt, 0);
	    const char *s = (const char *) sheet;
	    sprite_layout = std::string(reinterpret_cast<const char *>(s));
	}
	return sprite_layout;
    }

    struct Building_Data {
	std::string collider_type = "none";
	float x_offset = 0.0f;
	float y_offset = 0.0f;
	float radius = 0.0f;
	std::string sprite_layout;
	std::string xml;
	std::string img;
	std::string race = "neutral";
	int whole_sprite = 1;
	std::string interior;
	int direction;
	std::string icon_sprite;
	std::string icon_dead;
	int icon_num_frames;
    };

    struct Interior_Building_Data {
	float x_offset = 0.0f;
	float y_offset = 0.0f;
	float x_collision_offset = 0.0f;
	float y_collision_offset = 0.0f;
	std::string xml;
	std::string img;
    };

    Building_Data Get_Building_Data(std::string &name) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	Building_Data data;
	const unsigned char *text;

	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT collider_type, radius, x_offset, y_offset, sprite_layout, xml, img, race, whole_sprite, interior, direction, icon_sprite,  icon_num_frames, icon_dead FROM building_exteriors WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    text = sqlite3_column_text(stmt, 0);
	    data.collider_type = db::Get_String(text);

	    data.radius = (float) sqlite3_column_double(stmt, 1);
	    data.x_offset = (float) sqlite3_column_double(stmt, 2);
	    data.y_offset = (float) sqlite3_column_double(stmt, 3);

	    text = sqlite3_column_text(stmt, 4);
	    data.sprite_layout = db::Get_String(text);

	    text = sqlite3_column_text(stmt, 5);
	    data.xml = db::Get_String(text);

	    text = sqlite3_column_text(stmt, 6);
	    data.img = db::Get_String(text);

	    text = sqlite3_column_text(stmt, 7);
	    data.race = db::Get_String(text);

	    data.whole_sprite = sqlite3_column_int(stmt, 8);

	    text = sqlite3_column_text(stmt, 9);
	    data.interior = db::Get_String(text);

	    data.direction = sqlite3_column_int(stmt, 10);

	    text = sqlite3_column_text(stmt, 11);
	    data.icon_sprite = db::Get_String(text);

	    data.icon_num_frames = sqlite3_column_int(stmt, 12);

	    text = sqlite3_column_text(stmt, 13);
	    data.icon_dead = db::Get_String(text);
	}
	return data;
    }

    Interior_Building_Data Get_Interior_Building_Data(std::string &name) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	Interior_Building_Data data;
	const unsigned char *text;

	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT x_offset, y_offset, collision_offset_x, collision_offset_y, xml, png FROM building_interiors WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    data.x_offset = (float) sqlite3_column_double(stmt, 0);
	    data.y_offset = (float) sqlite3_column_double(stmt, 1);
	    data.x_collision_offset = (float) sqlite3_column_double(stmt, 2);
	    data.y_collision_offset = (float) sqlite3_column_double(stmt, 3);

	    text = sqlite3_column_text(stmt, 4);
	    data.xml = db::Get_String(text);

	    text = sqlite3_column_text(stmt, 5);
	    data.img = db::Get_String(text);
	}
	return data;
    }

    std::string Get_Building_Icon(std::string name) {
	//check if the name exists??
	std::string unit_name = db::Append_Quotes(name);
	std::string buildingName;

	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT icon FROM building_exteriors WHERE name = ";
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);

	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    const unsigned char *text = sqlite3_column_text(stmt, 0);
	    buildingName = db::Get_String(text);
	}
	return buildingName;
    }

    std::string Increment_Direction(std::string name, int &direction) {// needs to search for  a specific row that I can input in the arguments
	//check if the name exists??
	name = Get_Building_Icon(name);
	direction++;
	std::string unit_name = db::Append_Quotes(name);
	const unsigned char *text;

	std::string buildingName;
	std::vector<std::string> directions;

	sqlite3_stmt *stmt;
	char buf[300];
	const char *jj = "SELECT name FROM building_exteriors WHERE icon = ";
	// AND direction == direction
	strcpy(buf, jj);
	strcat(buf, unit_name.c_str());
	sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);

	while (sqlite3_step(stmt) != SQLITE_DONE) {
	    text = sqlite3_column_text(stmt, 0);
	    buildingName = db::Get_String(text);

	    directions.emplace_back(buildingName);
	}

	if (directions.empty()) return name;

	if (direction >= directions.size()) {
	    direction = 0;
	    return directions[0];
	} else {
	    return directions[direction];
	}
    }
}// namespace Entity_Loader
