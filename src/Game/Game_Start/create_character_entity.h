#include "Game/Game_Start/character_options.h"
#include "Items/items.h"
#include "Objects/Stats/character_stats.h"
#include "create_entities.h"
#include "entt/entt.hpp"
#include "item_components.h"
#include "load_zone.h"

namespace Create_Character_Entity {
    void Equip_Units(entt::registry &zone, int &state, Character_Options::Customization &options) {
        std::vector<std::string> gear;
        Item_Component::Item hair;
        Item_Component::Item beard;
        Item_Component::Item horns;

        if (options.species == Character_Options::Species::dwarf) {
            gear = {
                    "F_Dwarf_Sword003",
                    "F_Dwarf_Bra",
                    "F_Dwarf_01_Legs"};
            if ((int) options.hairStyle < SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::dwarf_female][Item_Component::Item_Type::hair].size()) {
                hair = SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::dwarf_female][Item_Component::Item_Type::hair][options.hairStyle];
            }
            beard = {"none", "none", "none"};
            horns = {"none", "none", "none"};
        } else if (options.species == Character_Options::Species::elf) {
            gear = {
                    "F_Elf_Hammer001a",
                    "F_Elf_01_TorsoArmour",
                    "F_Elf_12_LegArmour"};
            if ((int) options.hairStyle < SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::elf_female][Item_Component::Item_Type::hair].size()) {
                hair = SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::elf_female][Item_Component::Item_Type::hair][options.hairStyle];
            }
            beard = {"none", "none", "none"};
            horns = {"none", "none", "none"};
        } else if (options.species == Character_Options::Species::human) {
            gear = {
                    "F_Human_Axe001",
                    "F_Human_StarterOutfitTorso",
                    "F_Human_StarterOutfitLegs"};
            if ((int) options.hairStyle < SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::human_female][Item_Component::Item_Type::hair].size()) {
                hair = SQLite_Item_Data::Items[Item_Component::Unit_Equip_Type::human_female][Item_Component::Item_Type::hair][options.hairStyle];
            }
            beard = {"none", "none", "none"};
            horns = {"none", "none", "none"};
        } else {
            gear = Get_Sex(options.sex);
            hair = Get_Hair_Name(options);
            beard = Get_Beard_Name(options);
            horns = Get_Horn_Name(options);
        }

        auto view = zone.view<Item_Component::Equipment, Component::Position, Rendering_Components::Unit_Frame_Portrait, Rendering_Components::Body_Frame, Action_Component::Action>();
        for (auto unit: view) {
            auto &equipment = view.get<Item_Component::Equipment>(unit);
            auto &position = view.get<Component::Position>(unit);
            auto &unitPortraitFrame = view.get<Rendering_Components::Unit_Frame_Portrait>(unit);
            auto &bodyFrame = view.get<Rendering_Components::Body_Frame>(unit);
            auto &action = view.get<Action_Component::Action>(unit);

            entt::entity item = Items::Create_And_Equip_Weapon(zone, state, position, equipment.type, SQLite_Item_Data::Load_Specific_Item(gear[0].c_str()), Character_Options::Color[0]);
            equipment.equippedItems[Item_Type::mainhand] = item;
            action.weaponType = zone.get<Item_Component::Weapon_Type>(item);
            Load::Get_Bust_Textures(zone, state, item, Item_Type::mainhand, bodyFrame, unitPortraitFrame);

            item = Items::Create_And_Equip_Armor(zone, state, position, Item_Type::chest, equipment.type, SQLite_Item_Data::Load_Specific_Item(gear[1].c_str()), Character_Options::Color[0]);
            equipment.equippedItems[Item_Type::chest] = item;
            Load::Get_Bust_Textures(zone, state, item, Item_Type::chest, bodyFrame, unitPortraitFrame);

            item = Items::Create_And_Equip_Armor(zone, state, position, Item_Type::legs, equipment.type, SQLite_Item_Data::Load_Specific_Item(gear[2].c_str()), Character_Options::Color[0]);
            equipment.equippedItems[Item_Type::legs] = item;
            Load::Get_Bust_Textures(zone, state, item, Item_Type::legs, bodyFrame, unitPortraitFrame);

            item = Items::Create_And_Equip_Cosmetic(zone, state, position, Item_Type::hair, equipment.type, hair, Character_Options::Color[options.hairColor]);
            equipment.equippedItems[Item_Type::hair] = item;
            Load::Get_Bust_Textures(zone, state, item, Item_Type::hair, bodyFrame, unitPortraitFrame);

            item = Items::Create_And_Equip_Cosmetic(zone, state, position, Item_Type::facialHair, equipment.type, beard, Character_Options::Color[options.hairColor]);
            equipment.equippedItems[Item_Type::facialHair] = item;
            Load::Get_Bust_Textures(zone, state, item, Item_Type::facialHair, bodyFrame, unitPortraitFrame);

            if (horns.hasTexture) {
                item = Items::Create_And_Equip_Cosmetic(zone, state, position, Item_Type::horns, equipment.type, horns, Character_Options::Color[0]);
                equipment.equippedItems[Item_Type::horns] = item;
                Load::Get_Bust_Textures(zone, state, item, Item_Type::horns, bodyFrame, unitPortraitFrame);
            }

            zone.emplace_or_replace<Item_Component::Item_Equip>(unit);
        }
    }

    void Update_Items(entt::registry &zone, int &state) {
        Character_Stats::Update_Equip_slots(zone, state);
        Character_Stats::Update_Unit_Stats(zone, state);
    }

    void Init_Items() {
        Items::Init_Item_Data();
    }

    void Init_Player(entt::registry &zone, int &state, Character_Options::Customization &options) {
        db::Unit_Data data = Entity_Loader::Get_Character_Create(Character_Options::Get_Character(options));
        Component::Unit_Index unitIndex = {"demons", 0};

        Social_Component::Summon summon;
        //    Create_Entities::Create_Entity(zone, state, 0, 0, data, true, summon, unitIndex);
        Create_Entities::Create_Entity(zone, state, 1660, 8924, data, true, summon, unitIndex);
        Equip_Units(zone, state, options);
        Character_Stats::Init_Player_Stats(zone, state);
    }

    void Recreate_Player(entt::registry &zone, int &state, Character_Options::Customization &options) {
        db::Unit_Data data = Entity_Loader::Get_Character_Create(Character_Options::Get_Character(options));
        Social_Component::Summon summon;
        Component::Unit_Index unitIndex = {"demons", 0};

        Create_Entities::Create_Entity(zone, state, 1660, 8924, data, true, summon, unitIndex);
        Character_Stats::Init_Player_Stats(zone, state);
    }
}// namespace Create_Character_Entity