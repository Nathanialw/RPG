#pragma once


namespace Utilities {

	void Log(const auto &string) {
		std::cout << string << std::endl;
	}

	SDL_FRect worldToScreen(SDL_FRect& Rect, SDL_FRect &camera) {
		SDL_FRect screenRect = {};

		screenRect.x = Rect.x + camera.x;
		screenRect.y = Rect.y + camera.y;
		
		return screenRect;
	}

	SDL_FRect screenToWorld(SDL_FRect &Rect, SDL_FRect &camera) {
		SDL_FRect screenRect = {};
	
		screenRect.x = Rect.x - camera.x;
		screenRect.y = Rect.y - camera.y;
	
		return screenRect;
	}

	float Get_Hypotenuse(float& length, float &width) {
		return sqrtf((length * length) + (width * width));
	}


	bool bFRect_Intersect(SDL_FRect &entity, SDL_FRect &target) {
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
	};	

	bool bRect_Intersect(SDL_Rect& entity, SDL_Rect& target) {
		if ((entity.y <= target.y + target.h) &&
			(entity.x <= target.x + target.w) &&
			(entity.y + entity.h >= target.y) &&
			(entity.x + entity.w >= target.x)) {
			return true;
		}
		return false;
	};

	bool bFPoint_FRectIntersect(SDL_FPoint &p, SDL_FRect &r) {		
		return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
	};	
	
	bool bPoint_RectIntersect(SDL_FPoint& p, SDL_FRect& r) {
		return ((p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h))) ? SDL_TRUE : SDL_FALSE;
	};

	SDL_Rect SDL_FRect_To_SDL_Rect(SDL_FRect& a) {
		SDL_Rect b = {};
		b.x = (int)a.x;
		b.y = (int)a.y;
		b.w = (int)a.w;
		b.h = (int)a.h;
		return b;
	}

	SDL_FRect SDL_Rect_To_SDL_FRect(SDL_Rect& a) {
		SDL_FRect b = {};
		b.x = (float)a.x;
		b.y = (float)a.y;
		b.w = (float)a.w;
		b.h = (float)a.h;
		return b;
	}

	SDL_Point SDL_FPoint_to_Point(SDL_FPoint &a) {
		SDL_Point b = {};
		b.x = (int)a.x;
		b.y = (int)a.y;
		return b;
	}

	float Get_Direction_Point(float &sourceX, float &sourceY, float& targetX, float& targetY) {
		float a = targetX - sourceX;
		float b = targetY - sourceY;

		return atan2f(a, b);
	}

	SDL_FRect Get_FRect_From_Point_Radius(float &radius, float &x, float &y) {
		float rectX = x - radius;
		float rectY = y - radius;
		float width = radius * 2.0f;
		float height = radius * 2.0f;

		SDL_FRect rect = { rectX, rectY, width , height };
		return rect;
	}

	

	SDL_Point Check_Collision_Rects(SDL_Rect &rect1, SDL_Rect &rect2) {
		
		if (Utilities::bRect_Intersect(rect1, rect2)) {
			SDL_Point returnRect = {};
			int xOverlap = rect1.x - rect2.x;
			int yOverlap = rect1.y - rect2.y;
			if (xOverlap == 0) {
				xOverlap = 2;
			}
			if (yOverlap == 0) {
				yOverlap = 2;
			}
			returnRect.x = (xOverlap / 2);
			returnRect.y = (yOverlap / 2);
			return returnRect;
		}
		Log("Check_Collision_Rects() fallthrough error");
		return { 0, 0 };
	}


	bool  bFrect_Contains_Frect(const SDL_FRect& rect, const SDL_FRect& rectWithin) {
		if (rectWithin.x > rect.x && (rectWithin.x + rectWithin.w) < (rect.x + rect.w) &&
			rectWithin.y > rect.y && (rectWithin.y + rectWithin.h) < (rect.y + rect.h)) {
			return true;

		};
		return false;
		
		//
		//if ((entity.y <= target.y + target.h) &&
		//	(entity.x <= target.x + target.w) &&
		//	(entity.y + entity.h >= target.y) &&
		//	(entity.x + entity.w >= target.x)) {
		//	return true;
		//}
		//return false;
	}
}
