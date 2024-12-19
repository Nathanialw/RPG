//
// Created by nathanial on 12/18/24.
//
#pragma once
#include "warrior.h"
#include "mage.h"
#include "rogue.h"
#include "general.h"
#include "skills.h"

namespace  Skill {


    void Update(entt::registry &zone, f2 scale) {
        Warrior::Update(zone, scale);
        Mage::Update(zone, scale);
        Rogue::Update(zone, scale);
        General::Update(zone, scale);
    }

    void Render() {
        General::Render();
        Mage::Render();
        Rogue::Render();
        Warrior::Render();
    }

    void HighLight() {
        General::Hover_Highlight();
        Mage::Hover_Highlight();
        Rogue::Hover_Highlight();
        Warrior::Hover_Highlight();
    }

    void Click(entt::registry &zone, entt::entity &entity) {
        if (General::Click(zone, entity) || Mage::Click(zone, entity) || Rogue::Click(zone, entity) || Warrior::Click(zone, entity))
            Update(zone, zone.get<Component::Camera>(entity).scale);
    }

    bool Mouse_Inside() {
        if (General::Mouse_Inside() || Mage::Mouse_Inside() || Rogue::Mouse_Inside() || Warrior::Mouse_Inside())
            return true;
    }
}
