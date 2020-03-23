#include "simple_logger.h"
#include "collisions.h"

#include "sword.h"
#include "level.h"

#define ES_DEAD 1

Uint32 lifetime;
Entity *owner;

void sword_think(Entity *self)
{
	if (!self) return;

	int mx, my;

	if (self->souls > 0)
	{
		owner->souls += self->souls;
		self->souls = 0;
	}

	 
	if (SDL_GetTicks() > lifetime || self->state == ES_DEAD && self->souls == 0)
	{
		entity_free(self);
	}
	

	if (collide_circle(self->position, self->radius, vector2d(mx, my), 1))
	{
		self->state = ES_DEAD;
		vector2d_set(self->velocity, 0, 0);
		return;
	}
}

void sword_touch(Entity *self, Entity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	slog("Hit for %i damage", self->damage);

	self->state = ES_DEAD;

	vector2d_set(self->velocity, 0, 0);
}


Entity *sword_new(Vector2D position, Entity *own)
{
	Entity *self;
	self = entity_new();
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"",
		128,
		128,
		16);
	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;
	self->think = sword_think;
	self->touch = sword_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	// Who created us
	owner = own;

	// Damage
	self->damage = 1 * owner->attack;

	lifetime = SDL_GetTicks() + 50;

	return self;
}