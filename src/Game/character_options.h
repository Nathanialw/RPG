#include "SDL2/SDL.h"
#include "string"
#include "unordered_map"
#include "vector"

namespace Character_Options {

  std::vector<std::string> Gear_Male = {"Male_Rogue_DaggerMain", "Male_Druid_Top",
                                        "Male_Footman_Bottom"};

  std::vector<std::string> Gear_Female = {"Female_Rogue_DaggerMain",
                                          "Female_Druid_Top", "Female_Scout_Bottom"};

  std::vector<std::string> Hair_Male = {
      "Male_Druid_Hair",
      "Male_Footman_Hair",
      "Male_Knight_Hair",
      "Male_Mage_Hair",
      "Male_Miner_Hair",
      "Male_Monk_Hair",
      "Male_Musketeer_Hair",
      "Male_Necromancer_Hair",
      "Male_Prince_Hair",
      "Male_Rogue_Hair",
      "Male_Walker_Hair",
      "Male_WitchHunter_Hair",
      "Male_Archer_Hair",
      "Male_ArcherElven_Hair",
      "Male_Assassin_Hair",
      "Male_Battleguard_Hair",
      "Male_BeastMaster_Hair",
      "Male_Commoner_1_Hair"};

  std::vector<std::string> Hair_Female = {
      "Female_Necromancer_Hair",
      "Female_Footman_Hair",
      "Female_Mage_Hair",
      "Female_Knight_Hair",
      "Female_Archer_Hair",
      "Female_Beastmaster_Hair",
      "Female_Cleric_Hair",
      "Female_Druid_Hair",
      "Female_Musketeer_Hair",
      "Female_Necromancer_Hair",
      "Female_Battleguard_Hair",
      "Female_Bard_Hair",
      "Female_1930s_1_1_Hair",
      "Female_ArcherElven_Hair",
      "Female_Assassin_Hair",
      "Female_Jester_Hair"};

  enum Sex {
    male,
    female
  };

  enum Species {
    orc,
    zombie,
    skeleton,
    demon,
    fleshbeast,
    elves,
    euro,
    asian,
    indian,
    african,
  };

  struct Race {
    std::string templateName = "Male_DaemonKnight";
    std::string path = "sprites/units/2nd_cast_classes/races/demons/male/Male_DaemonKnight.png";
  };

  std::vector<Race> Species_Male = {
      {"Male_Orc_1", "sprites/units/2nd_cast_classes/races/orcs/male/Male_Orc_1.png"},
      {"Male_Walker", "sprites/units/2nd_cast_classes/races/zombies/male/Male_Walker.png"},
      {"Male_Skeleton", "sprites/units/2nd_cast_classes/races/skeletons/male/Male_Skeleton.png"},
      {"Male_DaemonKnight", "sprites/units/2nd_cast_classes/races/demons/male/Male_DaemonKnight.png"},
      {"Male_Meat_Zombie", "sprites/units/2nd_cast_classes/races/fleshbeast/male/Male_Meat_Zombie.png"},
      {"Male_ArcherElven", "sprites/units/2nd_cast_classes/races/elves/male/Male_ArcherElven.png"},
      {"Male_Archer", "sprites/units/2nd_cast_classes/races/humans/male/Male_Archer.png"},
      {"Male_Assassin", "sprites/units/2nd_cast_classes/races/humans/male/Male_Assassin.png"},
      {"Male_Commoner_1", "sprites/units/2nd_cast_classes/races/humans/male/Male_Commoner_1.png"},
      {"Male_WitchHunter", "sprites/units/2nd_cast_classes/races/humans/male/Male_WitchHunter.png"},
  };

  std::vector<Race> Species_Female = {
      {"Female_Orc_1", "sprites/units/2nd_cast_classes/races/orcs/female/Female_Orc_1.png"},
      {"Female_Zombie", "sprites/units/2nd_cast_classes/races/zombies/female/Female_Zombie.png"},
      {"none", ""},
      {"none", ""},
      {"none", ""},
      {"Female_ArcherElven", "sprites/units/2nd_cast_classes/races/elves/female/Female_ArcherElven.png"},
      {"Female_Archer", "sprites/units/2nd_cast_classes/races/humans/female/Female_Archer.png"},
      {"Female_Assassin", "sprites/units/2nd_cast_classes/races/humans/female/Female_Assassin.png"},
      {"Female_Bard", "sprites/units/2nd_cast_classes/races/humans/female/Female_Bard.png"},
      {"Female_WitchHunter", "sprites/units/2nd_cast_classes/races/humans/female/Female_WitchHunter.png"},
  };

//  enum Colors { red,
//                blonde,
//                brown,
//                black,
//                grey,
//                none };

//  std::unordered_map<Colors, SDL_Color> Color = {
//      {red, {117, 37, 10}},
//      {blonde, {179, 139, 103}},
//      {brown, {61, 35, 20}},
//      {black, {49, 49, 49}},
//      {grey, {116, 116, 116}},
//      {none, {255, 255, 255}}};

  std::vector<SDL_Color> Color = {
      {255, 255, 255},
      {117, 37, 10},
      {179, 139, 103},
      {61, 35, 20},
      {49, 49, 49},
      {116, 116, 116}};

  enum Cast {
    RTP,
    Classes,
    Medieval
  };

  struct Customization {
    bool success = true;
    Sex sex = male;
    Cast cast = Classes;
    Species species = euro;
    int hairStyle = 0;
    int hairColor = 1;
  };

  std::vector<std::string> Get_Sex(Sex sex) {
    if (sex == male) {
      return Gear_Male;
    }
    return Gear_Female;
  }

  std::vector<std::string> Get_Hair(Sex sex) {
    if (sex == male) {
      return Hair_Male;
    }
    return Hair_Female;
  }

  std::vector<Race> Get_Race(Sex sex) {
    if (sex == male) {
      return Species_Male;
    }
    return Species_Female;
  }

  Race Get_Gender(Character_Options::Customization &options) {
    Race race;
    if (options.sex == Character_Options::male) {
      race.path = Species_Male[options.species].path;
      race.templateName = Species_Male[options.species].templateName;
      return race;
    }
    race.path = Species_Female[options.species].path;
    race.templateName = Species_Female[options.species].templateName;
    return race;
  }
}// namespace Character_Options