#pragma once
#include <vector>
#include "base_structs.h"
#include "utilities.h"
#include "SDL2/SDL.h"
#include "entt/entt.hpp"

namespace Map {

  struct Cell {
    SDL_FRect sCollide_Box = { 0,0,0,0 };
    std::vector<entt::entity> entities;
  };

  struct Node0 {
    SDL_FRect sCollide_Box = {0,0,0,0};
    Cell cells[4];
  };
  struct Node1 {
    SDL_FRect sCollide_Box = { 0,0,0,0 };
    Node0 nodes[4];
  };
  struct Node2 {
    SDL_FRect sCollide_Box = { 0,0,0,0 };
    Node1 nodes[4];
  };
  struct Node3 {
    SDL_FRect sCollide_Box = { 0.0f,0.0f,0.0f,0.0f };
    Node2 nodes[4];
  };

  int quad = 2;

  Node3 map;
  Node3 terrain;

  void Create_Cell(Node0 &node) {
    float w = node.sCollide_Box.w / quad;
    float h = node.sCollide_Box.h / quad;
    int k = 0;
    for (int i = 0; i < quad; i++) {
      for (int j = 0; j < quad; j++) {
	node.cells[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
	k++;
	//	std::cout << h << std::endl;
      }
    }
  }

  void Create_Node0(Node1 &node) {
    float w = node.sCollide_Box.w / quad;
    float h = node.sCollide_Box.h / quad;
    int k = 0;
    for (int i = 0; i < quad; i++) {
      for (int j = 0; j < quad; j++) {
	node.nodes[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
	Create_Cell(node.nodes[k]);
	k++;
      }
    }
  }

  void Create_Node1(Node2 &node) {
    float w = node.sCollide_Box.w / quad;
    float h = node.sCollide_Box.h / quad;
    int k = 0;
    for (int i = 0; i < quad; i++) {
      for (int j = 0; j < quad; j++) {
	node.nodes[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
	Create_Node0(node.nodes[k]);
	k++;
      }
    }
  }

  void Create_Node2(Node3 &node) {
    float w = node.sCollide_Box.w / quad;
    float h = node.sCollide_Box.h / quad;
    int k = 0;
    for (int i = 0; i < quad; i++) {
      for (int j = 0; j < quad; j++) {
	node.nodes[k].sCollide_Box = { node.sCollide_Box.x + (w * i), node.sCollide_Box.y + (h * j), w, h };
	Create_Node1(node.nodes[k]);
	k++;
      }
    }
  }
  // h = 235, w = eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

  void Build_Map(Node3& zone) {
    zone.sCollide_Box = { 0.0f, 0.0f, 1600.0f, 1600.0f };
    Create_Node2(zone);
  }

  int size = 4;

  void Place_Point_on_Grid(SDL_FPoint& point, Map::Node3& map, entt::entity& entity) { //inserts unit into a sigle node
    if (Utilities::bFPoint_FRectIntersect(point, map.sCollide_Box)) {
      for (int i = 0; i < size; i++) {
	if (Utilities::bFPoint_FRectIntersect(point, map.nodes[i].sCollide_Box)) {
	  for (int j = 0; j < size; j++) {
	    if (Utilities::bFPoint_FRectIntersect(point, map.nodes[i].nodes[j].sCollide_Box)) {
	      for (int k = 0; k < size; k++) {
		if (Utilities::bFPoint_FRectIntersect(point, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
		  for (int l = 0; l < size; l++) {
		    if (Utilities::bFPoint_FRectIntersect(point, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
		      map.nodes[i].nodes[j].nodes[k].cells[l].entities.push_back(entity);
		      return;
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  void Place_Rect_On_Grid_Once(SDL_FRect& point, Map::Node3& map, entt::entity& entity) { //inserts unit into every node withing its collider
    if (Utilities::bFRect_Intersect(point, map.sCollide_Box)) {
      for (int i = 0; i < size; i++) {
	if (Utilities::bFRect_Intersect(point, map.nodes[i].sCollide_Box)) {
	  for (int j = 0; j < size; j++) {
	    if (Utilities::bFRect_Intersect(point, map.nodes[i].nodes[j].sCollide_Box)) {
	      for (int k = 0; k < size; k++) {
		if (Utilities::bFRect_Intersect(point, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
		  for (int l = 0; l < size; l++) {
		    if (Utilities::bFRect_Intersect(point, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
		      map.nodes[i].nodes[j].nodes[k].cells[l].entities.push_back(entity);
		      return; //ensures the entity is only placed in one square
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  void Place_Rect_On_Grid(SDL_FRect& point, Map::Node3& map, entt::entity& entity) { //inserts unit into every node withing its collider
    if (Utilities::bFRect_Intersect(point, map.sCollide_Box)) {
      for (int i = 0; i < size; i++) {
	if (Utilities::bFRect_Intersect(point, map.nodes[i].sCollide_Box)) {
	  for (int j = 0; j < size; j++) {
	    if (Utilities::bFRect_Intersect(point, map.nodes[i].nodes[j].sCollide_Box)) {
	      for (int k = 0; k < size; k++) {
		if (Utilities::bFRect_Intersect(point, map.nodes[i].nodes[j].nodes[k].sCollide_Box)) {
		  for (int l = 0; l < size; l++) {
		    if (Utilities::bFRect_Intersect(point, map.nodes[i].nodes[j].nodes[k].cells[l].sCollide_Box)) {
		      map.nodes[i].nodes[j].nodes[k].cells[l].entities.push_back(entity);
		      //return; //ensures the entity is only placed in one square
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

namespace Component {
  //wil have Regiments and Companies as administrative units

  struct Squad {
    int size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fMass;
    std::vector<float>fRadius;

    std::vector<int>iStruck;
    std::vector<bool>bAlive;

    Squad() {
      name = "Default";
      size = 8;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fMass.reserve(size);
      fRadius.reserve(size);
      iStruck.reserve(size);
      bAlive.reserve(size);
    }
  };

  struct Platoon {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fPW;
    std::vector<float>fPH;

    Platoon() {
      name = "Default";
      size = 6;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fPW.reserve(size);
      fPH.reserve(size);
    }
  };

  struct Company {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fPW;
    std::vector<float>fPH;


    Company() {
      name = "Default";
      size = 4;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fPW.reserve(size);
      fPH.reserve(size);
    }
  };

  struct Battalion {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fPW;
    std::vector<float>fPH;

    Battalion() {
      name = "Default";
      size = 6;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fPW.reserve(size);
      fPH.reserve(size);
    }
  };

  struct Regiment {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Regiment() {
      name = "Default";
      size = 3;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  struct Brigade {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Brigade() {
      name = "Default";
      size = 2;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  struct Division {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Division() {
      name = "Default";
      size = 3;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  struct Army {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>iSub_Units;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Army() {
      name = "Default";
      size = 4;
      sCollide_Box = { 0,0,0,0 };
      iSub_Units.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };
}

namespace Military {

  struct Squad {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<entt::entity>soldiers;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fMass;
    std::vector<float>fRadius;

    std::vector<f2>vPosition; //

    Squad() {
      name = "Default";
      size = 8;
      sCollide_Box = { 0,0,0,0 };
      soldiers.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fMass.reserve(size);
      fRadius.reserve(size);
      vPosition.reserve(size);
    }
  };

  struct Platoon {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Squad>squads;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fPW;
    std::vector<float>fPH;

    Platoon() {
      name = "Default";
      size = 6;
      sCollide_Box = { 0,0,0,0 };
      squads.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fPW.reserve(size);
      fPH.reserve(size);
    }
  };

  struct Company {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Platoon>platoons;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fPW;
    std::vector<float>fPH;

    Company() {
      name = "Default";
      size = 4;
      sCollide_Box = { 0,0,0,0 };
      platoons.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fPW.reserve(size);
      fPH.reserve(size);
    }
  };

  struct Battalion {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Company>companies;
    std::vector<float>fPX;
    std::vector<float>fPY;
    std::vector<float>fPW;
    std::vector<float>fPH;

    Battalion() {
      name = "Default";
      size = 6;
      sCollide_Box = { 0,0,0,0 };
      companies.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
      fPW.reserve(size);
      fPH.reserve(size);
    }
  };

  struct Regiment {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Battalion>battalions;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Regiment() {
      name = "Default";
      size = 3;
      sCollide_Box = { 0,0,0,0 };
      battalions.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  struct Brigade {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Regiment>regiments;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Brigade() {
      name = "Default";
      size = 2;
      sCollide_Box = { 0,0,0,0 };
      regiments.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  struct Division {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Brigade>brigades;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Division() {
      name = "Default";
      size = 3;
      sCollide_Box = { 0,0,0,0 };
      brigades.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  struct Army {
    size_t size;
    std::string name;
    SDL_FRect sCollide_Box;

    std::vector<Division>divisions;
    std::vector<float>fPX;
    std::vector<float>fPY;

    Army() {
      name = "Default";
      size = 4;
      sCollide_Box = { 0,0,0,0 };
      divisions.reserve(size);
      fPX.reserve(size);
      fPY.reserve(size);
    }
  };

  Army Initialize_Army() {
    Military::Army army;

    for (int i = 0; i < army.size; i++) {
      Division division;
      army.divisions.emplace_back(division);
      for (int j = 0; j < army.divisions[j].size; j++) {
	Brigade brigade;
	army.divisions[i].brigades.emplace_back(brigade);
	for (int k = 0; k < army.divisions[i].brigades[j].size; k++) {
	  Regiment regiment;
	  army.divisions[i].brigades[j].regiments.emplace_back(regiment);
	  for (int l = 0; l < army.divisions[i].brigades[j].regiments[k].size; l++) {
	    Battalion battalion;
	    army.divisions[i].brigades[j].regiments[l].battalions.emplace_back(battalion);
	    for (int m = 0; m < army.divisions[i].brigades[j].regiments[k].battalions[l].size; m++) {
	      Company company;
	      army.divisions[i].brigades[j].regiments[k].battalions[l].companies.emplace_back(company);
	      for (int n = 0; n < army.divisions[i].brigades[j].regiments[k].battalions[l].companies[m].size; n++) {
		Platoon platoon;
		army.divisions[i].brigades[j].regiments[k].battalions[l].companies[m].platoons.emplace_back(platoon);
		for (int o = 0; o < army.divisions[i].brigades[j].regiments[k].battalions[l].companies[m].size; o++) {
		  Squad squad;
		  army.divisions[i].brigades[j].regiments[k].battalions[l].companies[m].platoons[n].squads.emplace_back(squad);
		}
	      }
	    }
	  }
	}
      }
    }
    return army;
  }

  class Military_Structure {
  private:
    std::vector<Army>armies;

  public:
    void Create_Army() {
      armies.emplace_back(Military::Initialize_Army());
    }

    void Destroy_Army(int index) {
      armies.erase(armies.begin() + index); //deletes the Army at the index element of the array
    }
  };
}

namespace Test {

  enum class Formation_Type {
    squad,
    platoon,
    company,
    battalion,
    brigade,
    division,
    corps,
    army
  };

  struct Soldier {};
  struct Squad {};
  struct Platoon {};
  struct Company {};
  struct Battalion {};
  struct Brigade {};
  struct Division {};
  struct Corps {};
  struct Army {};
  struct Grouped {};

  struct Soldier_Data {
    entt::entity unit_ID;
    float x;
    float y;
    float mass;
    float radius;
    bool bAlive;
  };

  struct Collision {

  };

  struct Soldiers_Assigned_List {
    int size = 3;
    std::vector<Soldier_Data>unitData;

    Soldiers_Assigned_List() {
      unitData.reserve(size);
    }
  };

  struct Unit_Formation_Data {
    entt::entity formation_ID;
    int size;
    bool bAlive;
    Formation_Type formationType;
    SDL_FRect sCollide_Box;
    std::vector<Unit_Formation_Data>subformationData;

    Unit_Formation_Data() {
      // formation_ID;
      size = 3;
      bAlive = true;
      //formationType;
      sCollide_Box = { 0.0f, 0.0f, 0.0f, 0.0f };
      subformationData.reserve(size);
    }
  };
};




