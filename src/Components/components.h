#pragma once
#include "entt/entt.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "classes.h"
#include "box2d/box2d.h"


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

  struct Interaction_Rect {
    SDL_FRect rect;
  };

  struct Unit {

  };

  struct Body {
    b2Body* body;
    b2Body* rigidBody;
  };

  struct Name {
    std::string first;
    std::string last;
    std::string nickname;
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

  struct Radius {
    float fRadius = 0.0f;
  };

  struct Velocity {
    float dX, dY;
    f2 magnitude;
    float speed;
    float angle;
  };

  struct Building_Collider {
    std::vector<i2>walls;
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
    W
  };

  struct Drop_Equipment {

  };

  //    struct t_Stored_Frame_Data {
  //        uint64_t frameTime = 0;
  //        int frameIndex = 0;
  //        int reversing = 0;
  //        int currentFrame = 0;
  //    };
  //
  //    struct t_Sheet_Data {
  //        Frame_Data_Packer actionFrameData[17];
  //        int sheetWidth = 0;
  //        int spriteWidth = 0;
  //        int spriteHeight = 0;
  //    };

  //    struct asd { ;
  //        t_Sheet_Data t_SheetDataArray[1000];
  //    };

  //    struct t_Rendering_Data {
  //        SDL_Rect clipRect = {0,0,0,0};
  //        SDL_FRect renderRect = {0,0,0,0};
  //    };
  //
  //    struct t_Texture {
  //        SDL_Texture *texture = NULL;
  //    };

  struct Line_Segment {
    Position p[2];

    //		Position p1 = {0.0f, 0.0f};
    //		Position p2 = {0.0f, 0.0f};
  };

  struct Renderable {
    float y = 0.0f; //point
    int alpha = 0;
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
    SDL_Texture* texture;
    SDL_Rect clip;
    int numFrames;
    int currentFrame;
    int frameLength;
    int currentFrameTime;
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
    unit,
    foliage,
    spell,    
    item,
    object,
    prop,
    building,
    SIZE
  };

  enum class Object_Subtype{
    interior,
    exteior,
    background,
    foreground,
    SIZE    
  };
  
  enum class Units {
    SIZE
  };

  enum class Foliage {
    SIZE
  };
  
  enum class Spells {
    SIZE
  };

  enum class Items {
    SIZE
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
    house,
    wall,
    tower,
    SIZE
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
    int castTime = 2000;
    int counter = 2000;
    float x = 0.0f;
    float y = 0.0f;
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

  struct Attack_Speed {
    int period = 0;
    int counter = 0;
  };

  struct In_Combat {
    bool combat;
  };

  struct Damage {
    int minDamage;
    int maxDamage;
  };

  struct Melee_Damage {
    int minDamage = 0;
    int maxDamage = 0;
    bool critical = false;
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
    int struck = 0;
    bool critical = false;
  };

  //AI

  struct Sight_Range {
    float range;
    SDL_FRect sightBox;
  };

  struct Target_Range {
    float range;
    SDL_FRect rangeBox;
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
    f2 renderRectSize;
    f2 renderPositionOffset;
  };

  struct On_Mouse {
    entt::entity item_ID;
  };

  struct Inventory {

  };

  struct Pickup_Item {
    entt::entity item_ID;
    float x;
    float y;
    float radius;
  };

  struct Scrolling_Damage_Text {
    SDL_FPoint position;
    std::string damageText;
    int lingerTime;
    bool special;
  };


  struct Caster_ID {
    entt::entity caster_ID;
  };

  struct Srite_Direction {
    Direction direction;
    int numFrames;
  };

  struct Followers {
    std::vector<entt::entity> followers;
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

