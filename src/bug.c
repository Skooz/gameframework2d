#include "simple_logger.h"
#include "collisions.h"

#include "bug.h"
#include "level.h"

#define ES_DEAD 1

Uint32 nextMove;
int moveDir;

// thonk
void bug_think(Entity *self)
{
	if (!self) return;

	int mx, my;

	// Animate movement - Determines frames based on velocity.
	if (self->velocity.x == 0 && self->velocity.y < 0)
	{
		// Going up
		self->frame = 17;
	}
	if (self->velocity.x == 0 && self->velocity.y > 0)
	{
		// Going down
		self->frame = 15;
	}
	if (self->velocity.x < 0 && self->velocity.y == 0)
	{
		// Going left
		self->frame = 16;
	}
	if (self->velocity.x > 0 && self->velocity.y == 0)
	{
		// Going right
		self->frame = 18;
	}

	// Movement - Up and Down
	/*
	if (SDL_GetTicks() > nextMove)
	{
		nextMove = SDL_GetTicks() + 1500;
		if (moveDir == 1)
		{
			moveDir = 0;
			vector2d_set(self->velocity, 0, -1);
			return;
		}
		if (moveDir == 0)
		{
			moveDir = 1;
			vector2d_set(self->velocity, 0, 1);
			return;
		}
	}
	*/

	// Movement - Side to Side
	/*
	if (SDL_GetTicks() > nextMove)
	{
		nextMove = SDL_GetTicks() + 1500;
		if (moveDir == 1)
		{
			moveDir = 0;
			vector2d_set(self->velocity, -1, 0);
			return;
		}
		if (moveDir == 0)
		{
			moveDir = 1;
			vector2d_set(self->velocity, 1, 0);
			return;
		}
	}
	*/

	// Rectangular Movement
	/*
	if (SDL_GetTicks() > nextMove)
	{
		nextMove = SDL_GetTicks() + 1500;
		if (moveDir == 1)
		{
			moveDir = 2;
			vector2d_set(self->velocity, -1, 0);
			return;
		}
		if (moveDir == 2)
		{
			moveDir = 3;
			vector2d_set(self->velocity, 0, -1);
			return;
		}
		if (moveDir == 3)
		{
			moveDir = 4;
			vector2d_set(self->velocity, 1, 0);
			return;
		}
		if (moveDir == 4)
		{
			moveDir = 1;
			vector2d_set(self->velocity, 0, 1);
			return;
		}
	}
	*/

	// Stationary Movement - No velocity means we change directions here
	/**/
	if (SDL_GetTicks() > nextMove)
	{
		nextMove = SDL_GetTicks() + 1500;
		if (moveDir == 1)
		{
			moveDir = 2;
			// Going up
			self->frame = 17;
			return;
		}
		if (moveDir == 2)
		{
			moveDir = 3;
			// Going left
			self->frame = 16;
			return;
		}
		if (moveDir == 3)
		{
			moveDir = 4;
			// Going down
			self->frame = 15;
			return;
		}
		if (moveDir == 4)
		{
			moveDir = 1;
			// Going right
			self->frame = 18;
			return;
		}
	}

	


	// Are we dead or alive? Animate appropriately.
	if (self->state == ES_DEAD)
	{
		entity_free(self);
	}
	/*
	switch (self->state)
	{
		case ES_DEAD:
			self->frame = self->frame + 0.1;
			if (self->frame > 63)
			entity_free(self);
			break;
		default:
			self->frame = self->frame + 0.1;
			if (self->frame > 10)self->frame = 0;
	}
	*/
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

// T o u c h    t h e    b u g
void bug_touch(Entity *self, Entity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;
	
	self->health -= other->damage;

	if (self->health <= 0)
	{
		self->state = ES_DEAD;
		self->frame = 59;
		vector2d_set(self->velocity, 0, 0);
	}
	
}


// Create a brand new bug
Entity *bug_new(Vector2D position)
{
	Entity *self;
	self = entity_new();
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/enemy.png",
		60,
		60,
		15);

	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;

	self->think = bug_think;
	self->touch = bug_touch;

	vector2d_copy(self->position, position);
	//vector2d_set(self->velocity, 0, 0);
	vector2d_set(self->drawOffset, -30, -30);

	// Define our hit-points and such
	self->maxHealth = 5;
	self->health = self->maxHealth;
	self->maxMagic = 0;
	self->magic = self->maxMagic;
	self->attack = 0;
	self->damage = 0;

	nextMove = 0;
	moveDir = 1;

	return self;
}