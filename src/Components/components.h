#pragma once

#include "box2d/box2d.h"
#include "classes.h"
#include "entt/entt.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <sys/types.h>
#include <vector>

namespace Component {

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

    struct Interaction_Rect {
	SDL_FRect rect{};
	bool tall = false;
    };

    struct Unit {
    };

    struct Religion {
	std::string religion = "Pantosian";
	int faith = 0;
    };

    struct Skills {
	int unspent = 0;
    };

    struct Stats {
	int unspent = 0;
	int strength = 30;
	int strengthBase = 30;
	int dexterity = 30;
	int dexterityBase = 30;
	int intelligence = 30;
	int intelligenceBase = 30;
    };

    struct Level {
	int level = 1;
	int xp = 0;
    };

    struct Body {
	b2Body *body;
    };

    struct Unit_Index {
	std::string type;
	int index;
    };

    struct Object_Name {
	std::string objectName;
    };

    struct Name {
	std::string first;
	std::string last;
	std::string nickname;
	std::string race = "Human";
	std::string sex = "Male";
	std::string Class = "Scholar";
    };

    struct Tile_Index {
	int i;
	int j;
    };

    struct Reset {
    };

    struct Info {
	std::string cityBorn;
	std::string dateBorn;
	std::string father;
	std::string mother;
	std::vector<std::string> children;
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
	int currentHealth = 0;
	int maxHealth = 0;
	int base = 0;
	int growth = 2;
    };

    struct Mana {
	int current = 0;
	int max = 0;
    };

    struct Gold {
	int gold;
	int gems;
    };

    struct Dungeon_Level {
	int level = 1;
    };

    struct Time_On_Map {
	int current = 0;
	int max = 0;
    };

    struct Alive {
	bool bIsAlive = true;
    };

    struct Dead {
	bool dead;
    };

    //Position
    struct Position {
	float x;
	float y;
    };

    struct Location {
	Position spawn;
	Position current;
    };

    struct Radius {
	float fRadius = 0.0f;
    };

    struct Velocity {
	float dX{}, dY{};
	f2 magnitude{};
	float speed{};
	float angle{};
	bool hexDir = false;
    };

    /// place 2-h and bow animations in a separate array
    /// if I have a bow equipped it links a different array but the state value remains the same

    /// need to have a check somewhere to see whether I am in 1h, 2h or bow mode and pick the correct array
    /// maybe save a piece of state within another component already used that denotes the current weapon state, use that to index which array to read from

    enum class Direction {
	E,
	N,
	NE,
	NW,
	S,
	SE,
	SW,
	W,
	SSE,
	SSW,
	NNE,
	NNW,
	ESE,
	ENE,
	WSW,
	WNW
    };

    struct Drop_Equipment {
    };

    struct Loot {
	std::vector<entt::entity> items;
    };

    struct Line_Segment {
	Position start;
	Position end;
    };

    //  typedef std::vector<Line_Segment> Line_Segments;

    struct Renderable {
	Position point = {0.0f, 0.0f};//point
	int alpha = 0;
	std::vector<Line_Segment> lineSegment;
	std::string name;
	bool inside = false;
    };

    struct Is_Inside {
	bool inside = false;
    };

    struct Saved_Line_Segments {
	std::vector<Line_Segment> lineSegment;
	std::string name;
    };

    struct Scale {
	float scale;
    };

    struct keyData {
	f2 velocity = {0, 0};
	bool pressed = false;
    };

    struct Input {
	std::unordered_map<SDL_Keycode, keyData> keyboardControl;
    };

    void Add_Input_Component(entt::registry &zone, Component::Velocity &velocity, entt::entity &entity) {

	std::unordered_map<SDL_Keycode, keyData> keyboardControl;
	keyboardControl[SDLK_w].velocity.x = 0.0f;
	keyboardControl[SDLK_w].velocity.y -= velocity.speed;
	keyboardControl[SDLK_s].velocity.x = 0.0f;
	keyboardControl[SDLK_s].velocity.y += velocity.speed;
	keyboardControl[SDLK_a].velocity.x -= velocity.speed;
	keyboardControl[SDLK_a].velocity.y = 0.0f;
	keyboardControl[SDLK_d].velocity.x += velocity.speed;
	keyboardControl[SDLK_d].velocity.y = 0.0f;
//        keyboardControl[SDLK_q].velocity.x -= velocity.speed;
//        keyboardControl[SDLK_q].velocity.y -= velocity.speed;
//        keyboardControl[SDLK_e].velocity.x += velocity.speed;
//        keyboardControl[SDLK_e].velocity.y -= velocity.speed;
//        keyboardControl[SDLK_c].velocity.x += velocity.speed;
//        keyboardControl[SDLK_c].velocity.y += velocity.speed;
//        keyboardControl[SDLK_z].velocity.x -= velocity.speed;
//        keyboardControl[SDLK_z].velocity.y += velocity.speed;

	auto &input = zone.emplace_or_replace<Input>(entity);
	input.keyboardControl = keyboardControl;
    }

    struct Camera {
	SDL_FRect screen;
	f2 scale;
    };

    struct Mass {
	float fKilos = 0.0f;
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
	SDL_Texture *texture{};
	SDL_Rect clip{};
	int numFrames{};
	int currentFrame{};
	float frameLength{};
	float currentFrameTime{};
	int targetIndex{};
	bool selected = false;
    };

    struct Retinue {
	entt::entity list[10];
	//        std::vector<entt::entity> list;
    };

    struct Household {
	entt::entity list[10];
    };

    struct Workers {
	entt::entity list[10];
    };

    struct Subordinates {
	entt::entity list[10];
    };

    struct Relationships {
	entt::entity list[10];
    };

    enum class Entity_Type {
	portal,
	foliage,
	unit,
	item,
	object,
	building,
	spell,
	prop,
	tile,
	SIZE
    };

    enum class Object_Subtype {
	interior,
	exteior,
	background,
	foreground,
	SIZE
    };

    enum class Units {
	SIZE = 1
    };

    enum class Foliage {
	SIZE = 1
    };

    enum class Spells {
	SIZE = 1
    };

    enum class Items {
	SIZE = 1
    };

    enum class Objects {
	food_table,
	firepit,
	SIZE
    };

    enum class Props {
	rug,
	blood,
	SIZE
    };

    enum class Buildings {
	tent,
	house,
	wall,
	tower,
	SIZE
    };

    struct Entity_Interaction {
	entt::entity entity;
	Entity_Type type;
    };

    struct Collided {
	float x;
	float y;
	std::vector<Entity_Interaction> InteractionList;
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
	float castTime = 2000.0f;
	float counter = 2000.0f;
	float x = 0.0f;
	float y = 0.0f;
	const char *name = nullptr;
	const char *effect = nullptr;
	const char *hit = nullptr;
	bool linear = true;
	entt::entity target_ID;
    };

    struct Cast {
	float targetX;
	float targetY;
    };

    struct Spell {
	const char *hit;
    };

    struct Particle {
    };

    struct Caster {
	entt::entity caster;
    };

    struct Spellbook {
    };

    struct Spell_Name {
	const char *spell;
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
	SDL_Texture *pTexture;//texture
	SDL_Rect clippedSpriteFrame;
    };

    struct Frame_Delay {
	float timeBetweenFrames;
	float currentFrameTime;
    };

    struct Explosion {
	i2 positionOffset;
	SDL_FRect renderPosition;
	f2 offsetToAlignWithFireball;
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

    struct Interacting {
	entt::entity target_ID;
	float targetX;
	float targetY;
    };

    struct Interact {
	entt::entity target_ID;
	float targetX;
	float targetY;
    };

    struct Attack_Speed {
	float period = 0.0f;
	float counter = 0.0f;
    };

    enum Attack_Type {
	normal,
	skill,
	fire,
	frost
    };

    enum Bonus_Damage_Type {
	add,
	multiply
    };

    struct In_Combat {
	bool combat;
    };

    struct Damage {
	int minDamage;
	int maxDamage;
	int critChance;
	bool critical;
    };

    struct Melee_Damage {
	int minDamage = 0;
	int maxDamage = 0;
	int critChance = 5;
	int weaponType = 0;
    };

    struct Melee_Range {
	float meleeRange;
    };

    struct Aura_Damage {
	const char *effect = "teleport";
	float duration = 2000.0f;
	float count = 2000.0f;
    };

    struct Might {
	const char *effect = "teleport";
	float duration = 4000.0f;
	float count = 4000.0f;
    };

    struct DOT {
	const char *effect = "teleport";
	int ticks = 4;
	float duration = 2000.0f;
	float count = 2000.0f;
    };

    struct Damage_Over_Time {
	std::vector<DOT> debuffs;
    };

    struct Weapon_Size {
	SDL_FRect attackArea;
    };

    struct Attack_Box_Duration {
	float lifeTime;
	float count;
    };

    struct Struck {
	int struck = 0;
	bool critical = false;
    };

    //AI

    struct Sight_Range {
	float range;
	SDL_FRect sightBox;
    };

    struct Pathing {
	std::vector<f2> path;
	int index = 0;
    };

    struct Target_Range {
	float range;
	SDL_FRect rangeBox;
    };

    struct Alert {
	float sightRange;
    };

    struct Bag {
	//    std::vector<entt::entity> bag;
	f2 numOfSlots = {8.0f, 4.0f};
	std::array<entt::entity, 32> bag{};
    };

    struct Sprite_Icon {
	std::string name;
	SDL_Texture *texture = nullptr;
	int maxFrames = 0;
	int frame = 0;
	float timer = 0.0f;
	SDL_Rect clipRect = {};

	void Dead(const std::string &deadIconStr) {
	    name = deadIconStr;
	    frame = 1;
	    maxFrames = 1;
	}
    };

    struct Interior_Sheet_Info {

    };

    struct Icon {
	SDL_Texture *pBackground = nullptr;
	SDL_Texture *pTexture = nullptr;
	SDL_Texture *pIconRarityBorder = nullptr;
	SDL_Texture *pIconBorder = nullptr;
	SDL_Rect clipSprite = {};
	SDL_Rect clipIcon = {};
	f2 renderRectSize = {};
	f2 renderPositionOffset = {};
    };

    enum class Icon_Type {
	none,
	building,
	spell,
	item,
	SIZE
    };

    struct On_Mouse {
    };

    struct Dungeon {
	int instance;
	std::string tilesetName = "cave";
    };

    struct Inventory {
    };

    struct Pickup_Item {
	entt::entity item_ID;
	float x;
	float y;
	float radius;
    };

    struct Light_Radius {
	int lightRadius;
    };

    struct Scrolling_Damage_Text {
	SDL_FPoint position;
	std::string damageText;
	float lingerTime;
	Attack_Type type;
	bool critical;
    };

    struct Caster_ID {
	entt::entity caster_ID;
    };

    struct Followers {
	std::vector<entt::entity> followers;
    };

    struct Interactable {
    };
}// namespace Component


namespace Component_Camera {//unused yet

    struct Viewport {
	SDL_FRect viewport;
    };

    struct Resolution {
	SDL_FRect resolution;
    };

    struct Screen {
	SDL_FRect screen;
    };
}// namespace Component_Camera
