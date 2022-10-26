//#pragma once
//#include "timer.h"
//#include "ai_control.h"
//#include "components.h"
//#include "utilities.h"
//
//namespace Movement {
//	namespace {
//		int64_t Player_Move_Poll = 0;
//		int64_t Update_Position_Poll = 0;
//		int64_t linearMovePoll = 0;
//	}
//
//
//
//	void Mouse_Moving(entt::registry& zone) { // maybe change to move and attack?
//		if (zone.empty<Component::Selected>()) {
//			auto view = zone.view<Component::Input>();
//			for (auto player : view) {
//				if (Mouse::bRight_Mouse_Pressed) {
//					AI::Move_Order(zone, player, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
//				}
//			}
//		}
//	}
//
//
//
//
//
//	void Update_Position() {
//		auto view = World::zone.view<Component::Position, Component::Velocity, Component::Moving>();
//		Update_Position_Poll += Timer::timeStep;
//		float angleY = 0.0f;
//		//std::cout << Update_Position_Poll << std::endl;
//		if (Update_Position_Poll >= 0) {
//			for (auto entity : view) {
//				auto& vel = view.get<Component::Velocity>(entity);
//				auto& pX = view.get<Component::Position>(entity);
//				auto& pY = view.get<Component::Position>(entity);
//				if (vel.magnitude.x != 0 || vel.magnitude.y != 0) {
//					if (fabs(vel.magnitude.x) < 0.01) { vel.magnitude.x = 0; }; //clamp rounding errors
//					if (fabs(vel.magnitude.y) < 0.01) { vel.magnitude.y = 0; };
//					vel.angle = atan2f(vel.magnitude.x, vel.magnitude.y);
//					angleY = atan2f(vel.magnitude.y, vel.magnitude.x);
//					float velocityX = sinf(vel.angle) * vel.speed;
//					float velocityY = sinf(angleY) * vel.speed;
//					vel.dX = velocityX;
//					vel.dY = velocityY;
//					pX.x += velocityX * Update_Position_Poll;
//					pY.y += velocityY * Update_Position_Poll;
//				}
//			}
//			Update_Position_Poll = 0;
//		}
//	};
//
//	Direction Set_Direction(float angleInRadians) {
//
//		if (angleInRadians > 2.74889 || angleInRadians <= (-2.74889)) { return N; }
//		else if (angleInRadians > 1.96349 && angleInRadians <= (2.74889)) { return NE; }
//		else if (angleInRadians > 1.17809 && angleInRadians <= (1.96349)) { return E; }
//		else if (angleInRadians > 0.39269 && angleInRadians <= (1.17809)) { return SE; }
//		else if (angleInRadians > (-0.39269) && angleInRadians <= (0.39269)) { return S; }
//		else if (angleInRadians > (-1.17811) && angleInRadians <= (-0.39269)) { return SW; }
//		else if (angleInRadians > (-1.96351) && angleInRadians <= (-1.17811)) { return W; }
//		else if (angleInRadians > (-2.74889) && angleInRadians <= (-1.96351)) { return NW; }
//
//		Utilities::Log("Set_Direction() fallthrough error");
//		return N;
//	}
//
//	Direction Look_At_Target(float& positionX, float& positionY, float& targetX, float& targetY, float& angleInRadians) {
//		angleInRadians = Utilities::Get_Direction_Point(positionX, positionY, targetX, targetY);
//		return Set_Direction(angleInRadians);
//	}
//
//	void Update_Direction() {
//		auto view = World::zone.view<Direction, Actions, Velocity, Moving>();
//		for (auto entity : view) {
//			auto& vel = view.get<Velocity>(entity);
//			auto& b = view.get<Direction>(entity);
//			auto& c = view.get<Actions>(entity);
//
//			b = Set_Direction(vel.angle);
//
//			if (c.action == walk) {
//				if (vel.magnitude.x == 0 && vel.magnitude.y == 0) {
//					c.action = idle;
//				};
//			}
//		}
//	}
//
//
//	//takes in the x,y of the unit and checks if it is within the destination x,y within an accuracy of the set variable
//	bool Check_If_Arrived(const float& unitX, const float& unitY, const float& destinationX, const float& destinationY) {
//		float accuracy = 5.0f;
//		if (unitX + accuracy > destinationX &&
//			unitX - accuracy < destinationX &&
//			unitY - accuracy < destinationY &&
//			unitY + accuracy > destinationY) {
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//
//	void Mouse_Move_Arrived() {
//		auto view = World::zone.view<Position, Velocity, Actions, Mouse_Move>();
//		for (auto entity : view) {
//			auto& act = view.get<Actions>(entity);
//			auto& v = view.get<Velocity>(entity);
//			const auto& x = view.get<Position>(entity);
//			const auto& y = view.get<Position>(entity);
//			const auto& mov = view.get<Mouse_Move>(entity);
//			if (Check_If_Arrived(x.x, y.y, mov.fX_Destination, mov.fY_Destination)) {
//				if (act.action == walk) {
//					v.magnitude.x = 0.0f;
//					v.magnitude.y = 0.0f;
//					act.action = idle;
//
//					World::zone.remove<Mouse_Move>(entity);
//					World::zone.remove<Moving>(entity);
//				}
//			}
//		}
//	}
//
//
//	void Mouse_Move_To() { //calculates unit direction after you give them a "Mouse_Move" component with destination coordinates
//		Player_Move_Poll += Timer::timeStep;
//		if (Player_Move_Poll >= 200) {
//			Player_Move_Poll = 0;
//			auto view = World::zone.view<Position, Velocity, Mouse_Move, Actions, Moving>();
//			for (auto entity : view) {
//				const auto& x = view.get<Position>(entity);
//				const auto& y = view.get<Position>(entity);
//				auto& act = view.get<Actions>(entity);
//				auto& v = view.get<Velocity>(entity);
//				auto& mov = view.get<Mouse_Move>(entity);
//				act.action = walk;
//				v.magnitude.x = v.speed * (mov.fX_Destination - x.x);
//				v.magnitude.y = v.speed * (mov.fY_Destination - y.y);
//			}
//		}
//	}
//
//
//
//	void Linear_Move_To() {
//		linearMovePoll += Timer::timeStep;
//		if (linearMovePoll >= 50) {
//			auto view = World::zone.view<Velocity, Actions, Moving, Linear_Move, Spell_Range>();
//			for (auto entity : view) {
//				auto& act = view.get<Actions>(entity);
//				auto& v = view.get<Velocity>(entity);
//				auto& mov = view.get<Linear_Move>(entity);
//				auto& range = view.get<Spell_Range>(entity);
//
//				if (range.fRange <= 1500) {
//					act.action = walk;
//					v.magnitude.x = v.speed * (mov.fX_Direction - range.fSourceX);
//					v.magnitude.y = v.speed * (mov.fY_Direction - range.fSourceY);
//					range.fRange += linearMovePoll;
//				}
//				else {
//					World::zone.remove<Linear_Move>(entity);
//				}
//			}
//			linearMovePoll = 0;
//		}
//	}
//
//
//	void Update_Entity_Positions(entt::registry& zone) {
//		//Mouse_Attack_Move(); //runs every frame to see if mouse is down, if it is it moves you to the new location
//		Mouse_Moving(zone);
//		Linear_Move_To();
//		Mouse_Move_To();
//		Mouse_Move_Arrived();
//		Update_Direction();
//		Update_Position();
//	}
//
//
//}