#include "simple_logger.h"
#include "collisions.h"

#include "bug.h"
#include "level.h"

#define ES_DEAD 1

// thonk
void bug_think(Entity *self)
{
	if (!self) return;

	int mx, my;
	

	// Animate movement
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


	// Age the bug
	self->age++;

	// Are we dead or alive? Animate appropriately.
	switch (self->state)
	{
		case ES_DEAD:
			//self->frame = self->frame + 0.1;
			//if (self->frame > 63)
			entity_free(self);
			break;
		default:
			//self->frame = self->frame + 0.1;
			//if (self->frame > 10)self->frame = 0;
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
	
	/*
	if ((!self->madebabies) && (self->age > 100))
	{
		bug_new(vector2d(self->position.x + gfc_crandom() * 64, self->position.y + gfc_crandom() * 64), vector2d(gfc_crandom(), gfc_crandom()));
		self->madebabies = 1;
	}
	*/
	
}


// Create a brand new bug
Entity *bug_new(Vector2D position, Vector2D velocity)
{
	Entity *self;
	self = entity_new();
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/enemy.png",
		60,
		60,
		15);
	self->radius = 24;
	self->size.x = 32;
	self->size.y = 32;
	self->think = bug_think;
	self->touch = bug_touch;
	vector2d_copy(self->position, position);
	vector2d_copy(self->velocity, velocity);
	vector2d_set(self->drawOffset, -64, -78);

	// Define our hit-points and such
	self->maxHealth = 5;
	self->health = self->maxHealth;
	self->maxMagic = 0;
	self->magic = self->maxMagic;
	self->attack = 0;
	self->damage = 0;

	return self;
}