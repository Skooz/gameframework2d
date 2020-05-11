#include "simple_logger.h"
#include "collisions.h"

#include "bonfire.h"
#include "level.h"

#define ES_DEAD 1

Uint32 lifetime;
int direction;
Zentity *owner;

void bonfire_think(Zentity *self)
{
	if (!self) return;

	if (self->bonfireUsed == 1)
		self->frame = 29;
	else
		self->frame = 28;
	
}

void bonfire_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	if (other->isPlayer && self->bonfireUsed == 0) // Heal the player to full health when they're in the vicinity.
	{
		self->bonfireUsed = 1;
		other->health = other->maxHealth;
		other->magic = other->maxMagic;
	}

}

Zentity *bonfire_new(Vector2D position, int bid)
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
	self->think = bonfire_think;
	self->touch = bonfire_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	self->isBonfire = 1;
	self->bonfireUsed = 0;
	self->bonfireID = bid;

	self->frame = 28;

	return self;
}