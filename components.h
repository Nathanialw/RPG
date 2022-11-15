#pragma once
#include "entt/entt.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "classes.h"
#include "include/box2d/box2d.h"




namespace Component {

	struct ObjectID {
		int id;
	};

	struct In_Object_Tree {
		//QuadTreeItemLocation<typename std::list<QuadTreeItem<someObjectWithArea>>::iterator> pItem;
		//std::_List_iterator<DynamicQuadTreeContainer<someObjectWithArea>> item_it;
		bool inTree;
	};

	struct Remove_From_Object_Tree {
		SDL_FRect rect;
	};

	struct Destroyed {
		SDL_FRect rect;
	};


	struct Body {
		b2Body* body;
		b2Body* rigidBody;
	};

	struct handle {
		std::string sName;
	};

	struct Cell_Assigned {
		int iIndex;
		entt::entity iCell_Assigned_To;
	};

	struct Assigned_To_Formation {
		int iIndex;
		entt::entity iUnit_Assigned_To;
	};

	struct Soldier {

	};

	struct Health {
		int  currentHealth;
		int maxHealth;
	};

	struct Alive {
		bool bIsAlive;
	};

	//Position
	struct Position {
		float x;
		float y;
	};

	struct Radius {
		float fRadius;
	};

	struct Velocity {
		float dX, dY;
		DataTypes::f2d magnitude;
		float speed;
		float angle;
	};

	struct Building_Collider {
		std::vector<DataTypes::i2d>walls;
	};

    /// place 2-h and bow animations in a separate array
    /// if I have a bow equipped it links a different array but the state value remains the same

    /// need to have a check somewhere to see whether I am in 1h, 2h or bow mode and pick the correct array
    /// maybe save a piece of state within another component already used that denotes the current weapon state, use that to index which array to read from

    enum Weapon_State {
        unarmed,
        _1hand,
        _2hand,
        bow,
        xbow
    };

//    enum t_Action_State {
//        isStatic,
//        idle,
//        idle,
//        idle1,
//        idle2,
//        idle3,
//        /// Idle_4 is unarmed
//        idle4,
//        ///these two align with th first 2 idles
//        fidget,
//        fidget2,
//        ///this fidget is unarmed and aligns with idle4
//        fidget3,
//        ///unarmed
//        talking1,
//        talking2,
//        walk,
//        run,
//        attack,
//        attack2,
//        attack3,
//        attack4,
//        idle2H,
//        idleCritical1,
//        idleCritical2,
//        ///weapon only visible with a staff - can cast with a staff shield but cannot attack
//        cast,
//        struck,
//        block,
//        evade,
//        stunned,
//        idle_low_hp,
//        idle_low_hp2,
//        dead,
//        dead2,
//        dead3,
//        dead4,
//        dead5,
//        dead6,
//        resting,
//        cheer,
//        behavior,
//        summoned,
//        kneel,
//        pray_kneeled,
//        pray_standing,
//        in_combat,
//        in_combat2,
//        climb,
//        use_item,
//        crouch
//    };

	enum Action_State {
		isStatic,
		idle,
		idle2,
		idle6,
		walk,
        walkBow,
        run,
		attack,
        attack2,
        attack2H_1,
        attack2H_2,
        idle2H,
        idleCritical1,
        idleCritical2,
        run2H,
        walk2H,
		cast,
		struck,
        block,
        evade,
        stunned,
        idle_low_hp,
        idle_low_hp2,
		dead,
		dead2,
		dead3,
		dead4,
		dead5,
		dead6,
        resting,
		ranged,
        ranged_bow1,
        idleBow,
		cheer,
        behavior,
		summoned,
        kneel,
        pray_kneeled,
        pray_standing,
        in_combat,
        in_combat2,
        climb,
        use_item,
        crouch

	};

	enum class Direction {
		E,
		N,
		NE,
		NW,
		S,
		SE,
		SW,
		W
	};

	struct Sprite_Offset {
        float x;
        float y;
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

    ////////////////////////////////////////////////////////////////////////////////////
    /// These are not components but components have pointers to them


    struct Weapon_Sheet_Data {
        SDL_Texture* texture = NULL;
        std::vector<Sprite_Sheet_Data> frameList;
        /// start frame by state, number of frames per state.
        std::unordered_map<Component::Weapon_State, Frame_Data_Packer> actionFrameData;
        ///store the frame duration for each frame of each state, probably not worth it
//        std::unordered_map<Component::Action_State, std::vector<int>> Frame_Speed_By_Action;
    };
    ////////////////////////////////////////////////////////////////////////////////////

        /// one per item, but they can point to the same texture

    struct Sheet_Data {
        SDL_Texture* texture = NULL;
        std::vector<Sprite_Sheet_Data> frameList;
        /// start frame by state, number of frames per state.
        std::unordered_map<Component::Action_State, Frame_Data_Packer> actionFrameData;
        ///store the frame duration for each frame of each state, probably not worth it
//        std::unordered_map<Component::Action_State, std::vector<int>> Frame_Speed_By_Action;
    };

    struct Sheet_Data_Flare {
        SDL_Texture* texture = NULL;		//texture
        int sheetWidth = 0;
        int spriteWidth = 0;
        int spriteHeight = 0;
//        Frame_Data_Packer actionFrameDataArray[17];
//        std::vector<Frame_Data_Packer> actionFrameDataVector[20];
        std::unordered_map<Component::Action_State, Frame_Data_Packer> actionFrameData;
    };

    ////////////////////////////////////////////////////////////////////////////////////

        ///only exists once and I look it up
    struct Item_Render_Data {
        SDL_Texture* texture  = NULL;
            /// actual clip data of each sprite
        std::vector<Sprite_Sheet_Data> frameList;
            /// frame start and num frames calced from XML
        std::unordered_map<Component::Action_State, Frame_Data_Packer> actionFrameData[2500];
    };

    //////////////////////////////////////////////////////////////////////////////////

    struct Equipped_Items {
            ///get the string from the item name and look up the texture from the data
        std::unordered_map<std::string, Item_Render_Data> equippedItems;
    };

    struct Action {
        Action_State state;
    };

    enum Frame {
        firstFrame,
        normalFrame,
        finalFrame
    };

    ///component for the unit
    struct Sprite_Sheet_Info {
        std::string type = "default";

        std::unordered_map<std::string, Sheet_Data_Flare>* flareSpritesheet = NULL;
        std::unordered_map<std::string, Sheet_Data>* sheetData = NULL;
        std::string sheet_name = "default";

        std::unordered_map<std::string, Sheet_Data>* sheetDataWeapon = NULL;
        std::string weapon_name = "unarmed";
        int weaponFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataChestpiece = NULL;
        std::string chest_name = "unarmed";
        int chestpieceFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataLegs = NULL;
        std::string legs_name = "unarmed";
        int legplateFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataHelm = NULL;
        std::string helm_name = "unarmed";
        int helmFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataHair = NULL;
        std::string hair_name = "unarmed";
        int hairFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataBack = NULL;
        std::string back_name = "unarmed";
        int backFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataBeard = NULL;
        std::string beard_name = "unarmed";
        int beardFrameIndex = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataBodysuit = NULL;
        std::string bodysuit_name = "unarmed";
        int bodysuit = 0;

        std::unordered_map<std::string, Sheet_Data>* sheetDataOffhand = NULL;
        std::string offhand_name = "unarmed";
        int offhand = 0;
            ///possible replacement for a string map is to store the name of the sprite sheet in a map and match it to a unique index, save that index and make the sheetData and vector, access the vector with the index
//        std::string* sheetData;
//        int index = 0;
        uint64_t frameTime = 0;
        int frameIndex = 0;
        int reversing = 0;
        int currentFrame = 0;
        Frame finalFrame = normalFrame;
    };

    struct t_Stored_Frame_Data {
        uint64_t frameTime = 0;
        int frameIndex = 0;
        int reversing = 0;
        int currentFrame = 0;
    };

    struct t_Sheet_Data {
        Frame_Data_Packer actionFrameData[17];
        int sheetWidth = 0;
        int spriteWidth = 0;
        int spriteHeight = 0;
    };

    struct asd { ;
        t_Sheet_Data t_SheetDataArray[1000];
    };

    struct t_Rendering_Data {
        SDL_Rect clipRect = {0,0,0,0};
        SDL_FRect renderRect = {0,0,0,0};
    };

    struct t_Texture {
        SDL_Texture *texture = NULL;
    };

	struct Renderable {
		float y;
		int alpha;
	};

	struct Scale {
		float scale;
	};

	struct Terrain_Renderable {
		float y;
	};

	struct Input {
		//std::vector<bool>keysPressed;
	};

	struct Camera {
		SDL_FRect screen;
		DataTypes::f2d scale;
	};

	struct Mass {
		float fKilos;
	};

	struct Commanded_Move {
		float fX_Destination;
		float fY_Destination;
	};

	struct Commandable {

	};

	struct Commanding {
	};

	struct Selected {

	};

	enum class Entity_Type {
		player,
		unit,
		foliage,
		spell,
		melee,
		item
	};

	struct Entity_Interaction{
		entt::entity entity;
		Entity_Type type;
	};

	struct Collided {
		float x;
		float y;
		std::vector<Entity_Interaction>InteractionList;
	};

	struct Moving {

	};

	struct Environment {

	};

	struct Terrain {

	};

	struct Mouse_Move {
		float fX_Destination;
		float fY_Destination;
	};

	struct Linear_Move {
		float fX_Direction;
		float fY_Direction;
	};

	struct Spell_Range {
		float fSourceX;
		float fSourceY;
		float fRange;
	};

	struct Casting {
	};

	struct Cast {
		float targetX;
		float targetY;
	};

	struct Spell {

	};

    struct Caster {
        entt::entity caster;
    };

	struct Spellbook {

	};

	struct Spell_Name {
		const char* spell;
	};

	struct Casted {

	};

	struct Sprite_Frames {
		int maxFrames;
		int currentFrame;
		int frameX;
		int frameY;
	};

	struct Texture {
		SDL_Texture* pTexture;		//texture
		SDL_Rect clippedSpriteFrame;
	};

	struct Frame_Delay {
		float timeBetweenFrames;
		float currentFrameTime;
	};

	struct Explosion {
		DataTypes::i2d positionOffset;
		SDL_FRect renderPosition;
		DataTypes::f2d offsetToAlignWithFireball;

	};

	struct Melee {

	};

	struct Attacking {
		entt::entity target_ID;
	};

	struct Attack {
		entt::entity target_ID;
		float targetX;
		float targetY;
	};

	struct Attack_Speed {
		int period;
		int counter;
	};

	struct In_Combat {
		bool combat;
	};

	struct Damage {
		int minDamage;
		int maxDamage;
	};

	struct Melee_Damage {
		int minDamage;
		int maxDamage;
	};

	struct Melee_Range {
		float meleeRange;
	};

	struct Weapon_Size {
		SDL_FRect attackArea;
	};

	struct Attack_Box_Duration {
		int64_t lifeTime;
		int64_t count;
	};

	struct Struck {
		int struck;
	};

	//AI

	struct Sight_Range {
        float range;
		SDL_FRect sightBox;
	};

    struct Alert {
        float sightRange;
    };

	struct Bag {
		std::vector<entt::entity> bag;
	};

	struct Icon {
		SDL_Texture* pBackground;
		SDL_Texture* pTexture;
		SDL_Texture* pIconRarityBorder;
		SDL_Texture* pIconBorder;
		SDL_Rect clipSprite;
		SDL_Rect clipIcon;
		DataTypes::f2d renderRectSize;
		DataTypes::f2d renderPositionOffset;
	};

	struct On_Mouse {
		entt::entity item_ID;
	};

	struct Inventory {

	};

	struct Item_Pickup {
		entt::entity item_ID;
		float x;
		float y;
		float radius;
	};

	struct Scrolling_Damage_Text {
		SDL_FPoint position;
		std::string damageText;
		int lingerTime;
	};


	struct Caster_ID {
		entt::entity caster_ID;
	};

	struct Srite_Direction {
		Direction direction;
		int numFrames;
	};
}


namespace Component_Camera { //unused yet

	struct Viewport {
		SDL_FRect viewport;
	};

	struct Resolution {
		SDL_FRect resolution;
	};

	struct Screen {
		SDL_FRect screen;
	};
}

