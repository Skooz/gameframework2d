#include "simple_logger.h"
#include "collisions.h"

#include "bloodstain.h"
#include "level.h"

#define ES_DEAD 1

void bloodstain_think(Zentity *self)
{
	if (!self) return;

}

void bloodstain_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;
	
	if (other->isPlayer)
	{
		other->souls += self->souls;
		Zentity_free(self);
	}

}

Zentity *bloodstain_new(Vector2D position, int droppedSouls)
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
	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;
	self->think = bloodstain_think;
	self->touch = bloodstain_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	self->souls = droppedSouls;

	self->frame = 30;

	return self;
}