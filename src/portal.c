#include "simple_logger.h"
#include "collisions.h"

#include "portal.h"
#include "level.h"

#define ES_DEAD 1

Level *level;
Zentity *player;
int worldTo;

void portal_think(Zentity *self)
{
	if (!self) return;

}

void portal_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || (!level) || (!other->isPlayer))return;

	Zentity_free_all();
	level_free(level);
	level = level_new(worldTo);
	

}

Zentity *portal_new(Vector2D position, Level *pL, int wT)
{
	Zentity *self;
	self = Zentity_new();

	if (!self)return NULL;

	/*
	self->sprite = gf2d_sprite_load_all(
		"images/lonk2.png",
		60,
		60,
		6,
		false);
	*/

	self->radius = 30;
	self->size.x = 30;
	self->size.y = 30;
	self->think = portal_think;
	self->touch = portal_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	worldTo = wT;
	level = pL;

	return self;
}