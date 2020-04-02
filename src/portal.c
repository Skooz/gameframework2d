#include "simple_logger.h"
#include "collisions.h"

#include "portal.h"
#include "level.h"

#define ES_DEAD 1

void portal_think(Zentity *self)
{
	if (!self) return;

}

void portal_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;
	
}

Zentity *portal_new(Vector2D position)
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
	self->think = portal_think;
	self->touch = portal_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	return self;
}