#pragma once

#include "SDL2/SDL.h"
#include "components.h"
#include "item_components.h"
#include "vector"
#include <map>
#include "colors.h"

namespace Rendering_Components {

    struct Background {
    };

    struct Foreground {
    };

    struct Sprite_Offset {
	float x;
	float y;
    };

    struct Offsets {
	Rendering_Components::Sprite_Offset offset;
	Rendering_Components::Sprite_Offset colliderOffset;
	Rendering_Components::Sprite_Offset placementOffset;
    };

    struct Sprite_Sheet_Data {
	SDL_Rect clip;
	float x_offset;
	float y_offset;
    };

    struct Frame_Data_Packer {
	int startFrame = 9999;
	int NumFrames = 0;
	int reverses = 0;
	int frameSpeed = 75;
    };

    struct Portrait {
	SDL_Texture *texture = nullptr;
	SDL_Color color = Color::white;
    };

    struct Body {
	SDL_Texture *texture = nullptr;
	SDL_Color color = Color::white;
    };

    struct Unit_Frame_Portrait {
	SDL_Texture *texture = nullptr;
	std::unordered_map<int, Portrait> gear;
    };

    struct Body_Frame {
	SDL_Texture *texture = nullptr;
	std::unordered_map<int, Portrait> gear;
    };

    /// one per item, but they can point to the same texture

    struct Sheet_Data {
	SDL_Texture *texture = nullptr;
	///data for each frame in the sprite sheet
	std::vector<Sprite_Sheet_Data> frameList;
	/// start frame by state, number of frames per state.
	std::unordered_map<uint8_t, Frame_Data_Packer> actionFrameData;
	///store the frame duration for each frame of each state, probably not worth it
	//        std::unordered_map<Component::Action_State, std::vector<int>> Frame_Speed_By_Action;
    };

    struct Image_Data {
	SDL_Texture *texture = nullptr;
	int w = 0;
	int h = 0;
    };

    struct Sheet_Data_Flare {
	SDL_Texture *texture = nullptr;//texture
	SDL_Color color = Color::white;
	int sheetWidth = 0;
	int spriteWidth = 0;
	int spriteHeight = 0;
	//        Frame_Data_Packer actionFrameDataArray[17];
	//        std::vector<Frame_Data_Packer> actionFrameDataVector[20];

	// find the frame data from the action
	std::unordered_map<uint8_t, Frame_Data_Packer> actionFrameData;

	// find the frame data from the index of the building
    };

    struct Mount_Sprite {
	std::unordered_map<std::string, Sheet_Data> *mount = nullptr;
	std::string front_name = "default";
    };

    struct Static_Sprite_Animation_Data {
	std::unordered_map<std::string, Sheet_Data> *ItemSheetData = nullptr;
	//name is the index into the map that stores the data
	std::string name = "empty";
	SDL_Color color = Color::white;
	uint16_t FrameIndex = 0;
	entt::entity itemID;
	float frameTime = 0.0f;
	Sprite_Offset offset = {0.0f, 0.0f};
    };

    struct Static_Sprite_Animation {
	std::unordered_map<std::string, Static_Sprite_Animation_Data> buffs;
	std::vector<Static_Sprite_Animation_Data> sheet;
    };

    struct Equip_Slot_Data {
	std::unordered_map<std::string, Sheet_Data> *ItemSheetData = nullptr;
	//name is the index into the map that stores the data
	std::string name = "empty";
	SDL_Color color = Color::white;
	int FrameIndex = 0;
	entt::entity itemID;
    };

    struct Equipment_Sprites {
	Equip_Slot_Data sheet[(int) Item_Component::Item_Type::SIZE];
    };

    //import the data to sheath
    //set it to equip slot back
    //remove it from equip slot back
    struct Sheathed {
	Equip_Slot_Data sheatheItemData;
    };

    enum Blend_Type {
	normal,
	reanimated,
	ghost
    };

    ///component for the unit
    struct Sprite_Sheet_Info {
	std::string type = "default";

	//        a union can how different types of data in the same memory address
	//        union {
	std::unordered_map<std::string, Sheet_Data_Flare> *flareSpritesheet = nullptr;
	std::unordered_map<std::string, Sheet_Data> *sheetData = nullptr;
	std::unordered_map<std::string, Image_Data> *imageData = nullptr;
	//        };

	//key used to get correct sprite  clip rect
	std::string sheet_name = "default";

	std::unordered_map<std::string, Sheet_Data> *mount = nullptr;
	std::string mount_name = "unmounted";
	SDL_Color color = Color::gray;
	//        Equip_Slot_Data equipmentSheets[30];
	///possible replacement for a string map is to store the name of the sprite sheet in a map and match it to a unique index, save that index and make the sheetData and vector, access the vector with the index
	uint16_t frameIndex = 0;
	uint8_t reversing = 0;
	Blend_Type blendType = normal;
	bool interior = false;
	bool unity = false;
	bool hexDir = false;
    };

    struct Interior_Sheet_Info {
	Sprite_Sheet_Info interior;
	Sprite_Offset offset;
	std::string collisionBocArrayIndex;
    };

    struct Showing_Interior {
    };

    struct Used_Graphics {
	int usedIndex = 0;
	std::string usedIcon;
    };

    struct Metamorphosis {
	std::unordered_map<std::string, Sheet_Data> *sheetData = nullptr;
	std::string sheet_name = "default";
	Equipment_Sprites equipment;
	Sprite_Offset offsets = {0.0f, 0.0f};
	Component::Interaction_Rect rect = {0.0f, 0.0f, 0.0f, 0.0f};
	Component::Radius radius = {0.0f};
	int duration = 1000;
	Component::Unit_Index unit;
	const char *spellEffect;
    };
}// namespace Rendering_Components
