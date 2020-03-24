#include "simple_logger.h"
#include "collisions.h"

#include "shield.h"
#include "level.h"

#define ES_DEAD 1

Uint32 lifetime;
int direction;
Entity *owner;

void bonfire_think(Entity *self)
{
	if (!self) return;

	
}

void bonfire_touch(Entity *self, Entity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	if (other->isPlayer) // Heal the player to full health when they're in the vicinity.
	{
		other->health = other->maxHealth;
		other->magic = other->maxMagic;
	}

}


Entity *bonfire_new(Vector2D position)
{
	Entity *self;
	self = entity_new();
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"images/lonk2.png",
		128,
		128,
		16);
	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;
	self->think = bonfire_think;
	self->touch = bonfire_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	self->frame = 28

	return self;
}