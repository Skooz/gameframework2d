#include "simple_logger.h"
#include "collisions.h"

#include "arrow.h"
#include "level.h"

#define ES_DEAD 1

int direction;

void arrow_think(Entity *self)
{
	if (!self) return;

	int mx, my;
	
	if (self->state == ES_DEAD)
		entity_free(self);

	if (collide_circle(self->position, self->radius, vector2d(mx, my), 1))
	{
		self->state = ES_DEAD;
		self->frame = 60;
		vector2d_set(self->velocity, 0, 0);
		return;
	}
}

void arrow_touch(Entity *self, Entity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	self->state = ES_DEAD;

	// Do damage

	vector2d_set(self->velocity, 0, 0);
}


Entity *arrow_new(Vector2D position, Vector2D velocity, int dir)
{
	Entity *self;
	self = entity_new();
	direction = dir;
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/lonk2.png",
		128,
		128,
		16);
	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;
	self->think = arrow_think;
	self->touch = arrow_touch;
	vector2d_copy(self->position, position);
	vector2d_copy(self->velocity, velocity);
	vector2d_set(self->drawOffset, -30, -30);

	self->damage = 1;

	if (direction == 1)
	{
		self->frame = 26;
	}
	if (direction == 2)
	{
		self->frame = 24;
	}
	if (direction == 3)
	{
		self->frame = 25;
	}
	if (direction == 4)
	{
		self->frame = 27;
	}

	return self;
}