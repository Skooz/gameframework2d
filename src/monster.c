#include "simple_logger.h"
#include "collisions.h"
#include "gf2d_draw.h"
#include "monster.h"
#include "level.h"

#define ES_DEAD 1

Uint32 moveDistance = 1300;
Zentity *target;
int bashTime;

// thonk
void monster_think(Zentity *self)
{
	if (!self) return;

	/*
	if (self->isBashed)
	{	
		self->isBashed = 0;
		bashTime = SDL_GetTicks() + 400;
	}
	
	if (bashTime > SDL_GetTicks()) return;
	*/

	// Animate movement - Determines frames based on velocity / direction.
	// Frames : Down / Left / Up / Right
	// Enemy 1: 15 / 16 / 17 / 18
	// Enemy 2: 19 / 20 / 21 / 22
	// Enemy 3: 94 / 95 / 96 / 97
	// Enemy 4: 90 / 91 / 92 / 93
	// Enemy 5: 64 / 65 / 66 / 67
	if (self->velocity.x == 0 && self->velocity.y > 0)
	{
		// Going down
		gfc_rect_set(self->rect, self->position.x, self->position.y, 1, 200);
		if (self->monsterType == 1) self->frame = 15;
		if (self->monsterType == 2) self->frame = 19;
		if (self->monsterType == 3) self->frame = 94;
		if (self->monsterType == 4) self->frame = 90;
		if (self->monsterType == 5) self->frame = 64;
	}
	if (self->velocity.x < 0 && self->velocity.y == 0)
	{
		// Going left
		gfc_rect_set(self->rect, self->position.x-200, self->position.y, 200, 1);
		if (self->monsterType == 1) self->frame = 16;
		if (self->monsterType == 2) self->frame = 20;
		if (self->monsterType == 3) self->frame = 95;
		if (self->monsterType == 4) self->frame = 91;
		if (self->monsterType == 5) self->frame = 65;
	}
	if (self->velocity.x == 0 && self->velocity.y < 0)
	{
		// Going up
		gfc_rect_set(self->rect, self->position.x, self->position.y-200, 1, 200);
		if (self->monsterType == 1) self->frame = 17;
		if (self->monsterType == 2) self->frame = 21;
		if (self->monsterType == 3) self->frame = 96;
		if (self->monsterType == 4) self->frame = 92;
		if (self->monsterType == 5) self->frame = 66;
	}
	if (self->velocity.x > 0 && self->velocity.y == 0)
	{
		// Going right
		gfc_rect_set(self->rect, self->position.x, self->position.y, 200, 1);
		if (self->monsterType == 1) self->frame = 18;
		if (self->monsterType == 2) self->frame = 22;
		if (self->monsterType == 3) self->frame = 97;
		if (self->monsterType == 4) self->frame = 93;
		if (self->monsterType == 5) self->frame = 67;
	}

	// Patrolling Movement
	switch (self->monsterType)
	{
	case 1:
		if (SDL_GetTicks() > self->nextMove) // Movement - Up and Down
		{
			self->nextMove = SDL_GetTicks() + moveDistance;
			if (self->moveDir == 1)
			{
				self->moveDir = 0;
				vector2d_set(self->velocity, 0, -1);
				return;
			}
			if (self->moveDir == 0)
			{
				self->moveDir = 1;
				vector2d_set(self->velocity, 0, 1);
				return;
			}
		}
	case 2: 
		if (SDL_GetTicks() > self->nextMove) // Movement - Side to Side
		{
			self->nextMove = SDL_GetTicks() + moveDistance;
			if (self->moveDir == 1)
			{
				self->moveDir = 0;
				vector2d_set(self->velocity, -1, 0);
				return;
			}
			if (self->moveDir == 0)
			{
				self->moveDir = 1;
				vector2d_set(self->velocity, 1, 0);
				return;
			}
		}
	case 3:
		if (SDL_GetTicks() > self->nextMove) // Rectangular Movement
		{
			self->nextMove = SDL_GetTicks() + moveDistance;
			if (self->moveDir == 1)
			{
				self->moveDir = 2;
				vector2d_set(self->velocity, -0.5, 0);
				return;
			}
			if (self->moveDir == 2)
			{
				self->moveDir = 3;
				vector2d_set(self->velocity, 0, -0.5);
				return;
			}
			if (self->moveDir == 3)
			{
				self->moveDir = 4;
				vector2d_set(self->velocity, 0.5, 0);
				return;
			}
			if (self->moveDir == 4)
			{
				self->moveDir = 1;
				vector2d_set(self->velocity, 0, 0.5);
				return;
			}
		}
	case 4:
		if (SDL_GetTicks() > self->nextMove) // Stationary Movement - No velocity means we change directions here
		{
			self->nextMove = SDL_GetTicks() + moveDistance;
			if (self->moveDir == 1)
			{
				self->moveDir = 2;
				vector2d_set(self->velocity, 0.7, 0);
				return;
			}
			if (self->moveDir == 2)
			{
				self->moveDir = 3;
				vector2d_set(self->velocity, 0, 0.7);
				return;
			}
			if (self->moveDir == 3)
			{
				self->moveDir = 4;
				vector2d_set(self->velocity, -0.7, 0);
				return;
			}
			if (self->moveDir == 4)
			{
				self->moveDir = 1;
				vector2d_set(self->velocity, 0, -0.7);
				return;
			}
		}
	case 5:
		if (target && self->monsterType == 5) // Target Acquired
		{ // down left up right Enemy 5: 64 / 65 / 66 / 67
			int ty = target->position.y;
			int tx = target->position.x;
			int my = self->position.y;
			int mx = self->position.x;
			if (ty > my) // go up
			{
				self->position.y += 0.5;
				gfc_rect_set(self->rect, self->position.x, self->position.y, 1, 200);
				self->frame = 64;
			}
			if (ty < my) // go down
			{
				self->position.y -= 0.5;
				gfc_rect_set(self->rect, self->position.x, self->position.y - 200, 1, 200);
				self->frame = 66;
			}
			if (tx < mx) // go left
			{
				self->position.x -= 0.5;
				gfc_rect_set(self->rect, self->position.x - 200, self->position.y, 200, 1);
				self->frame = 65;
			}
			if (tx > mx) // go right
			{
				self->position.x += 0.5;
				gfc_rect_set(self->rect, self->position.x, self->position.y, 200, 1);
				self->frame = 67;
			}
		}
		else if (SDL_GetTicks() > self->nextMove && !target) // Patrol
		{
			self->nextMove = SDL_GetTicks() + moveDistance;
			if (self->moveDir == 1)
			{
				self->moveDir = 2;
				vector2d_set(self->velocity, 0.1, 0);
				return;
			}
			if (self->moveDir == 2)
			{
				self->moveDir = 3;
				vector2d_set(self->velocity, 0, 0.1);
				return;
			}
			if (self->moveDir == 3)
			{
				self->moveDir = 4;
				vector2d_set(self->velocity, -0.1, 0);
				return;
			}
			if (self->moveDir == 4)
			{
				self->moveDir = 1;
				vector2d_set(self->velocity, 0, -0.1);
				return;
			}
		}
	}


	// DEATH
	if (self->state == ES_DEAD)
	{
		Zentity_free(self);
	}

}

// If we see something
void monster_see(Zentity *self, Zentity *other)
{
	if (!self || !other) return;
	if (other->isPlayer && !target && self->monsterType == 5 && SDL_GetTicks() > 2500)
	{
		slog("I SEE YOU");
		target = other;
	}
		
}

// If we touch something
void monster_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;
	
	self->health -= other->damage;

	if (other->isPlayer)
		other->health -= 1;

	vector2d_set(self->velocity, 0, 0);

	if (self->health <= 0)
	{
		self->state = ES_DEAD;
		other->souls += 100;
		vector2d_set(self->velocity, 0, 0);
	}
	
}


// Create a new monster
Zentity *monster_new(Vector2D position, int type)
{
	Zentity *self;
	self = Zentity_new();

	if (!self)return NULL;

	self->sprite = gf2d_sprite_load_all(
		"images/enemy.png",
		60,
		60,
		15,
		false);

	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;

	self->see = monster_see;
	self->think = monster_think;
	self->touch = monster_touch;

	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	// Which monster? (1 - 5)
	self->monsterType = type;

	self->canBash = 1;

	// Define our hit-points and such
	if (self->monsterType == 5)
	{
		self->maxHealth = 30;
		self->health = self->maxHealth;
	}
	else
	{
		self->maxHealth = 5;
		self->health = self->maxHealth;
	}

	self->maxMagic = 0;
	self->magic = self->maxMagic;

	self->attack = 0;
	self->damage = 0;

	self->nextMove = 0;
	self->moveDir = 1;

	return self;
}