#include "simple_logger.h"
#include "collisions.h"

#include "fountain.h"
#include "level.h"

#define ES_DEAD 1

void fountain_think(Zentity *self)
{
	if (!self) return;

}

void fountain_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	SDL_Rect bounds = { 0, 0, 1200, 720 };
	Zentity *underlevel = level_new("images/backgrounds/zeldaworld.png", bounds);

}

Zentity *fountain_new(Vector2D position)
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

	return self;
}