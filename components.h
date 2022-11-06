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

	enum Action_State {
		isStatic,
		idle,
		walk,
        run,
		attack,
        attack2,
		cast,
		struck,
        block,
        evade,
        stunned,
		dead,
        low_hp,
        resting,
		ranged,
		cheer,
        behavior,
		summoned
	};

	struct Frame_Data {
		int NumFrames;
		int currentFrame;
	};


	struct Actions {
		Action_State action;
		std::vector<Frame_Data>frameCount;
        Action_State storedAction;
	};

	//enum class Direction {
	//	W,
	//	NW,
	//	N,
	//	NE,
	//	E,
	//	SE,
	//	S,
	//	SW
	//};

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

    enum class PVG_Directions {
        zero,
        S,
        W,
        E,
        N,
        SW,
        NW,
        SE,
        NE
    };



	struct Sprite_Offset {
        float x;
        float y;
	};

	struct spriteframes {
		SDL_Rect clip;
		int frameStart;
		int sheetWidth;
		bool bReversable;
		bool bReversing;
		int64_t timeBetweenFrames;
        int64_t currentFrameTime;
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
        uint64_t frameSpeed = 75;
    };

    struct Sheet_Data {
        SDL_Texture* texture = NULL;
        std::vector<Sprite_Sheet_Data> frameList;
        /// start frame by action, number of frames per action.
        std::unordered_map<Component::Action_State, Frame_Data_Packer> actionFrameData;
        ///store the frame duration for each frame of each action
        ///probably not worth it
//        std::unordered_map<Component::Action_State, std::vector<int>> Frame_Speed_By_Action;
    };

    ///component for the unit
    struct Sprite_Sheet_Info {
            ///until I fix it
		SDL_Texture* texture = NULL;		//texture
        std::string type = "default";
        SDL_Rect clipSprite = {0,0,0,0};
        SDL_FRect renderPosition = {0,0,0,0};
            ///variable for sprite type
		std::vector<spriteframes>sheet = {};
//        std::string* sheetData;
        std::unordered_map<std::string, Sheet_Data>* sheetData;
        std::string sheet_name = "default";
            ///possible replacement for a string map is to store the name of the sprite sheet in a map and match it to a unique index, save that index and make the sheetData and vector, access the vector with the index
//        int index = 0;
        uint64_t frameTime = 0;
        int frameIndex = 0;
        int reversing = 0;
        int currentFrame = 0;
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


