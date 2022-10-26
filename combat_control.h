#pragma once
#include "entt.hpp"
#include "components.h"

namespace Combat_Control {

	int Calculate_Damage(Damage& damageRange) {

		if (damageRange.minDamage == 0) {
			return 0;
			std::cout << "function Calculate_Damage() min damage range 0, divide by zero error" << std::endl;
		}
		return rand() % damageRange.maxDamage + damageRange.minDamage;
	}

	//void Update_Attack_Speed(entt::registry &zone) {
	//	auto units = zone.view<Component::Attack_Speed>();
	//	
	//	for (auto unit_ID : units) {
	//		auto& attackSpeed = units.get<Component::Attack_Speed>(unit_ID);
	//		attackSpeed.counter -= Timer::timeStep;
	//	}
	//}

}
