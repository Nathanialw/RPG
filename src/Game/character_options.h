#include "SDL2/SDL.h"
#include "string"
#include "unordered_map"
#include "vector"
#include "SQLite_item_data.h"

namespace Character_Options {

  std::vector<std::string> Gear_Male = {"Male_Rogue_DaggerMain", "Male_Druid_Top",
                                        "Male_Footman_Bottom"};

  std::vector<std::string> Gear_Female = {"Female_Rogue_DaggerMain",
                                          "Female_Druid_Top", "Female_Scout_Bottom"};

  enum class Sex {
    male,
    female
  };

  enum class Species {
    orc,
    zombie,
    skeleton,
    demon,
    fleshbeast,
    elves,
    euro,
    asian,
    indian,
    african
  };

  std::vector<std::string> Species_Male = {
      {"Male_Orc_1"},
      {"Male_Walker"},
      {"Male_Skeleton"},
      {"Male_DaemonKnight"},
      {"Male_Meat_Zombie"},
      {"Male_ArcherElven"},
      {"Male_Archer"},
      {"Male_Assassin"},
      {"Male_Commoner_1"},
      {"Male_WitchHunter"},
  };

  std::vector<std::string> Species_Female = {
      {"Female_Orc_1"},
      {"Female_Zombie"},
      {"none"},
      {"none"},
      {"none"},
      {"Female_ArcherElven"},
      {"Female_Archer"},
      {"Female_Assassin"},
      {"Female_Bard"},
      {"Female_WitchHunter"},
  };

  std::vector<SDL_Color> Color = {
      {255, 255, 255},  //white
      {117, 37, 10},    //red
      {179, 139, 103},  //blonde
      {61, 35, 20},     //brown
      {49, 49, 49},     //black
      {116, 116, 116}}; //grey

  enum Cast {
    RTP,
    Classes,
    Medieval
  };

  //body image
  struct Images {
    std::vector<SDL_Texture *> species;
    std::vector<SDL_Texture *> hairStyles;
    SDL_Texture * chest = NULL;
    SDL_Texture * legs = NULL;
    SDL_Texture * weapon = NULL;
  };
  Images genderImages[2];

  void Load_Start_Character_Images() {

    for (int i = 0; i < Species_Female.size(); ++i) {
      genderImages[(int) Sex::female].species.emplace_back(Graphics::createTexture(("assets/" + Entity_Loader::Get_Character_Create(Species_Female[i]).bodyPath).c_str()));
    }
    for (int i = 0; i < SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_female][Item_Component::Item_Type::hair].size(); ++i) {
      genderImages[(int)Sex::female].hairStyles.emplace_back(Graphics::createTexture(("assets/" + SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_female][Item_Component::Item_Type::hair][i].body_pngPath).c_str()));
    }
    genderImages[(int)Sex::female].weapon = Graphics::createTexture(("assets/" + SQLite_Item_Data::Load_Specific_Item(Gear_Female[0]).body_pngPath).c_str());
    genderImages[(int)Sex::female].chest = Graphics::createTexture(("assets/" + SQLite_Item_Data::Load_Specific_Item(Gear_Female[1]).body_pngPath).c_str());
    genderImages[(int)Sex::female].legs = Graphics::createTexture(("assets/" + SQLite_Item_Data::Load_Specific_Item(Gear_Female[2]).body_pngPath).c_str());


    for (int i = 0; i < Species_Male.size(); ++i) {
      genderImages[(int) Sex::male].species.emplace_back(Graphics::createTexture(Entity_Loader::Get_Character_Create(Species_Male[i]).bodyPath.c_str()));
    }
    for (int i = 0; i < SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_male][Item_Component::Item_Type::hair].size(); ++i) {
      genderImages[(int)Sex::male].hairStyles.emplace_back(Graphics::createTexture(("assets/" + SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_male][Item_Component::Item_Type::hair][i].body_pngPath).c_str()));
    }
    genderImages[(int) Sex::male].weapon = Graphics::createTexture(("assets/" + SQLite_Item_Data::Load_Specific_Item(Gear_Male[0]).body_pngPath).c_str());
    genderImages[(int) Sex::male].chest = Graphics::createTexture(("assets/" + SQLite_Item_Data::Load_Specific_Item(Gear_Male[1]).body_pngPath).c_str());
    genderImages[(int) Sex::male].legs = Graphics::createTexture(("assets/" + SQLite_Item_Data::Load_Specific_Item(Gear_Male[2]).body_pngPath).c_str());



    //female gear

    //male gear
  }


  struct Customization {
    bool success = true;
    Sex sex = Sex::male;
    Species species = Species::euro;
    int hairStyle = 0;
    int hairColor = 1;
  };

  std::vector<std::string> Get_Sex(Sex sex) {
    if (sex == Sex::male) {
      return Gear_Male;
    }
    return Gear_Female;
  }

  std::vector<Item_Component::Item> Get_Hair(Sex sex) {
    if (sex == Sex::male) {
      std::cout << SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_male][Item_Component::Item_Type::hair].size() << std::endl;
      return SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_male][Item_Component::Item_Type::hair];
    }
    std::cout << SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_female][Item_Component::Item_Type::hair].size() << std::endl;
    return SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::classes_female][Item_Component::Item_Type::hair];
  }

  std::vector<std::string> Get_Race(Sex sex) {
    if (sex == Sex::male) {
      return Species_Male;
    }
    return Species_Female;
  }

  std::string Get_Character(Character_Options::Customization &options) {
    if (options.sex == Character_Options::Sex::male) {
      return Species_Male[(int)options.species];
    }
    return Species_Female[(int)options.species];
  }
}// namespace Character_Options