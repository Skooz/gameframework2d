#include "simple_logger.h"
#include "collisions.h"

#include "fountain.h"
#include "level.h"

#define ES_DEAD 1

void fountain_think(Zentity *self)
{
	if (!self) return;

	if (self->fountainType == 1)
	{
		self->frame = 31;
	}
	if (self->fountainType == 2)
	{
		self->frame = 32;
	}

}

void fountain_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	if (other->isPlayer)
	{
		if (self->fountainType == 1 && (other->health < other->maxHealth))
		{
			other->health += 1;
		}
		if (self->fountainType == 2 && (other->magic < other->maxMagic))
		{
			other->magic += 1;
		}
	}

}

Zentity *fountain_new(Vector2D position, int t)
{
	Zentity *self;
	self = Zentity_new();

	if (!self)return NULL;

	self->sprite = gf2d_sprite_load_all(
		"images/lonk2.png",
		60,
		60,
		6,
		false);
	self->radius = 30;
	self->size.x = 30;
	self->size.y = 30;
	self->think = fountain_think;
	self->touch = fountain_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	self->fountainType = t;
	//self->canBash = 0;

	return self;
}