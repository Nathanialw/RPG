#pragma once
#include "movement.h"
#include "collision.h"

namespace Spells {

	void Spell_Move_Target(entt::entity entity, float& x, float& y) { //sends spell to where the mouse is
		World::zone.emplace<Moving>(entity);
		World::zone.emplace<Mouse_Move>(entity, x, y);
		World::zone.remove<Casted>(entity);
	}

	void Spell_Linear_Target(entt::entity& entity, float& x, float& y, float& sourceX, float& sourceY) { //sends spell to where the mouse is
		World::zone.emplace<Moving>(entity);
		World::zone.emplace<Linear_Move>(entity, x, y);
		World::zone.emplace<Spell_Range>(entity, sourceX, sourceY, 0.0f);
		World::zone.remove<Casted>(entity);		
	}

	void Spell_Stack_Spells(float& x, float& y) { //sends spell to where the mouse is
		auto view = World::zone.view<Spell>();
		for (auto entity : view) {
			World::zone.emplace_or_replace<Moving>(entity);
			World::zone.emplace_or_replace<Mouse_Move>(entity, x, y);			
		}
	}

	DataTypes::f2d Spell_Direction(DataTypes::f2d& pos, Component::Direction& direction, float& scale) {
		switch (direction) {
		case Direction::N: return { pos.x, pos.y - (20.0f * scale) };
		case Direction::S: return { pos.x, pos.y + (20.0f * scale) };
		case Direction::E: return { pos.x + (20.0f * scale) , pos.y };
		case Direction::W: return { pos.x - (20.0f * scale) , pos.y };
		case Direction::NW: return { pos.x - (20.0f * scale) , pos.y - (20.0f * scale)  };
		case Direction::NE: return { pos.x + (20.0f * scale) , pos.y - (20.0f * scale)  };
		case Direction::SW: return { pos.x - (20.0f * scale) , pos.y + (20.0f * scale)  };
		case Direction::SE: return { pos.x + (20.0f * scale) , pos.y + (20.0f * scale)  };
		}
		Utilities::Log("Spell_Direction() function fallthrough error");
		return { 1.0f, 1.0f };
	}

	void create_spell(entt::entity caster_ID, entt::entity& entity, DataTypes::f2d& pos, Direction& direction, const char* spellname, float& targetX, float& targetY) {
		
		float scale = 1.0f;
		
		Entity_Loader::Data data = Entity_Loader::parse_data(spellname);
		DataTypes::f2d spelldir = Spell_Direction(pos, direction, scale);
 
		//rendering data
		World::zone.emplace<animation>(entity, Graphics::fireball_0); /// need to load the texture /only /once and pass the pointer into this function
		World::zone.get<animation>(entity).sheet = { //populate the vector
			{ NULL },
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, 16.0f }, //idle
			{ {0, 0, 64, 64 }, 0, 512, 0, 0, 16.0f } //walk
		};
		World::zone.emplace<Sprite_Offset>(entity, 32.0f * scale, 32.0f * scale);
		World::zone.emplace<Scale>(entity, scale);

		World::zone.emplace<Actions>(entity, walk);
		World::zone.get<Actions>(entity).frameCount = { {0, 0}, {0, 0}, {8, 0} };
		
		//positon data
		World::zone.emplace<Position>(entity, spelldir.x, spelldir.y); 

		//spell data
		World::zone.emplace<Radius>(entity, data.radius * scale);
		World::zone.emplace<Velocity>(entity, 0.f, 0.0f, 0.0f, 0.0f, data.speed);
		World::zone.emplace<Mass>(entity, data.mass * scale);
		World::zone.emplace<Entity_Type>(entity, Entity_Type::spell);
		World::zone.emplace<Damage>(entity, 1, 100);

		//Scenes::scene.emplace<Spell_Range>(spell, 1000.0f);
		World::zone.emplace<Caster_ID>(entity, caster_ID);

		//default data
		World::zone.emplace<Spell>(entity);
		World::zone.emplace<Casted>(entity);
		World::zone.emplace<Renderable>(entity);
		World::zone.emplace<Direction>(entity, direction); //match Direction of the caster
		World::zone.emplace<Alive>(entity, true);		

		bool yes = true;
		Collision::Create_Dynamic_Body(World::zone, entity, spelldir.x, spelldir.y, data.radius, data.mass, yes);
		
		
		Spell_Linear_Target(entity, targetX, targetY, spelldir.x, spelldir.y);
		//Spell_Move_Target(spell, targetX, targetY);
		//std::cout << "casted " << scene.get<handle>(spell).sName << std::endl;
	}


	void create_fireball(entt::entity & caster_ID, float& x, float& y, Direction& direction, const char* spellname, float& targetX, float& targetY) {
		auto fireball = World::zone.create();
		DataTypes::f2d pos = { x, y };
		create_spell(caster_ID, fireball, pos, direction, spellname, targetX, targetY);
	}



	void cast_fireball() {
		auto view = World::zone.view<Direction, Actions, Position, Cast, Spell_Name, Velocity>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			act.action = cast;
			act.frameCount[act.action].currentFrame = 0;
			auto& target = view.get<Cast>(entity);

			auto& direction = view.get<Direction>(entity);
			auto& x = view.get<Position>(entity).x;
			auto& y = view.get<Position>(entity).y;
			auto& angle = view.get<Velocity>(entity).angle;
	
			auto& name = view.get<Spell_Name>(entity).spell;
			//look at target
			
			direction = Movement::Look_At_Target(x, y, target.targetX, target.targetY, angle);
			//cast Fireball
			create_fireball(entity, x, y, direction, name, target.targetX, target.targetY);		
			//set into casting mode
			World::zone.emplace_or_replace<Casting>(entity);
			World::zone.remove<Cast>(entity);			
		}
	}


	void add_spells_to_scene() {
		cast_fireball();
	}

	void Create_Explosion(float& x, float y) { //creates the explosion for fireballs
		auto explosion = World::zone.create();

		World::zone.emplace<Position>(explosion, x, y);
		World::zone.emplace<Sprite_Frames>(explosion, 63, 0, 0, 0);
		World::zone.emplace<Texture>(explosion, Graphics::fireball_explosion_0, 0, 0, 128, 128);
		World::zone.emplace<Frame_Delay>(explosion, 16.0f, 0.0f);
		World::zone.emplace<Explosion>(explosion, 0, 0, 0.0f, 0.0f, 128.0f, 128.0f, 64.0f, 100.0f);
	}


	void Destroy_NonMoving_Spells() {
		auto view = World::zone.view<Spell, Body, Position, Radius, In_Object_Tree>(entt::exclude<Mouse_Move, Linear_Move, Explosion>);
		for (auto entity : view) {
			auto& x = view.get<Position>(entity).x;
			auto& y = view.get<Position>(entity).y;
			auto& radius = view.get<Radius>(entity).fRadius;
			auto& body = view.get<Body>(entity);
			auto& inTree = view.get<In_Object_Tree>(entity).inTree;
			
			//create explosion
			Create_Explosion(x, y);
			//destroy box2d body
			Collision::world->DestroyBody(body.body);
			World::zone.remove<Body>(entity);
			//Collision::world->DestroyBody(body.rigidBody);
			// 
			//set to remove from quad tree on update
			SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
			World::zone.emplace<Remove_From_Object_Tree>(entity, rect); //goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
			World::zone.emplace<Destroyed>(entity, rect); //goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
		}
	}

	void Clear_Collided_Spells() {
		auto view = World::zone.view<Spell, Position, Alive, Body, Radius, In_Object_Tree>(entt::exclude<Mouse_Move, Linear_Move, Explosion>);

		for (auto entity : view) {
			if (view.get<Alive>(entity).bIsAlive == false) {
				auto& x = view.get<Position>(entity).x;
				auto& y = view.get<Position>(entity).y;
				auto& radius = view.get<Radius>(entity).fRadius;
				auto& body = view.get<Body>(entity);
				auto& inTree = view.get<In_Object_Tree>(entity).inTree;
				//create explosion
				Create_Explosion(x, y);
				//destroy box2d body
				Collision::world->DestroyBody(body.body);
				World::zone.remove<Body>(entity);
				
				SDL_FRect rect = Utilities::Get_FRect_From_Point_Radius(radius, x, y);
				//set to remove from quad tree on update
				World::zone.emplace<Remove_From_Object_Tree>(entity, rect); //goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
				World::zone.emplace<Destroyed>(entity, rect); //goto: Dynamic_Quad_Tree::Remove_From_Tree_And_Registry()
			}
		}
	}
	
	void Casting_Updater() {
		auto view = World::zone.view<Casting, Actions>();
		for (auto entity : view) {
			auto& act = view.get<Actions>(entity);
			//std::cout << act.frameCount[act.action].currentFrame << "/" << act.frameCount[act.action].NumFrames <<std::endl;
			if (act.action != dead) {
				if (act.frameCount[act.action].currentFrame == act.frameCount[act.action].NumFrames) {
					act.action = idle;
					World::zone.remove<Casting>(entity);
				}
			}
		}
	}

	void Update_Spells() {
		
		Destroy_NonMoving_Spells();
		Clear_Collided_Spells();
		Casting_Updater();
		add_spells_to_scene();
	}
}