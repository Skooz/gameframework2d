#include "simple_logger.h"
#include "collisions.h"

#include "monster.h"
#include "level.h"

#define ES_DEAD 1

Uint32 moveDistance = 1300;
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
	// Enemy 5:
	if (self->velocity.x == 0 && self->velocity.y > 0)
	{
		// Going down
		if (self->monsterType == 1) self->frame = 15;
		if (self->monsterType == 2) self->frame = 19;
		if (self->monsterType == 3) self->frame = 94;
		if (self->monsterType == 4) self->frame = 90;
		//case 5:
	}
	if (self->velocity.x < 0 && self->velocity.y == 0)
	{
		// Going left
		if (self->monsterType == 1) self->frame = 16;
		if (self->monsterType == 2) self->frame = 20;
		if (self->monsterType == 3) self->frame = 95;
		if (self->monsterType == 4) self->frame = 91;
			//case 5:
	}
	if (self->velocity.x == 0 && self->velocity.y < 0)
	{
		// Going up
		if (self->monsterType == 1) self->frame = 17;
		if (self->monsterType == 2) self->frame = 21;
		if (self->monsterType == 3) self->frame = 96;
		if (self->monsterType == 4) self->frame = 92;
		//case 5:
	}
	if (self->velocity.x > 0 && self->velocity.y == 0)
	{
		// Going right
		if (self->monsterType == 1) self->frame = 18;
		if (self->monsterType == 2) self->frame = 22;
		if (self->monsterType == 3) self->frame = 97;
		if (self->monsterType == 4) self->frame = 93;
		//case 5:
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
				vector2d_set(self->velocity, -1, 0);
				return;
			}
			if (self->moveDir == 2)
			{
				self->moveDir = 3;
				vector2d_set(self->velocity, 0, -1);
				return;
			}
			if (self->moveDir == 3)
			{
				self->moveDir = 4;
				vector2d_set(self->velocity, 1, 0);
				return;
			}
			if (self->moveDir == 4)
			{
				self->moveDir = 1;
				vector2d_set(self->velocity, 0, 1);
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
				// Going up
				vector2d_set(self->velocity, 0, 0);
				self->frame = 92;
				return;
			}
			if (self->moveDir == 2)
			{
				self->moveDir = 3;
				// Going left
				vector2d_set(self->velocity, 0, 0);
				self->frame = 91;
				return;
			}
			if (self->moveDir == 3)
			{
				self->moveDir = 4;
				// Going down
				vector2d_set(self->velocity, 0, 0);
				self->frame = 90;
				return;
			}
			if (self->moveDir == 4)
			{
				self->moveDir = 1;
				// Going right
				vector2d_set(self->velocity, 0, 0);
				self->frame = 93;
				return;
			}
		}
	//case 5 :
		// Boss Movement/Behavior?
	}


	// DEATH
	if (self->state == ES_DEAD)
	{
		
		Zentity_free(self);
	}

	/*
	if (collide_circle(self->position, self->radius, vector2d(mx, my), 1))
	{
		self->state = ES_DEAD;
		self->frame = 59;
		vector2d_set(self->velocity, 0, 0);
		return;
	}
	*/
}

// If we touch something
void monster_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;
	
	self->health -= other->damage;

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

	self->think = monster_think;
	self->touch = monster_touch;

	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	// Which monster? (1 - 5)
	self->monsterType = type;

	self->canBash = 1;

	// Define our hit-points and such
	self->maxHealth = 5;
	self->health = self->maxHealth;
	self->maxMagic = 0;
	self->magic = self->maxMagic;
	self->attack = 0;
	self->damage = 0;

	self->nextMove = 0;
	self->moveDir = 1;

	return self;
}