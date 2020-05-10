#include "simple_logger.h"
#include "collisions.h"

#include "sword.h"
#include "level.h"

#define ES_DEAD 1

Uint32 lifetime;
Zentity *owner;

void sword_think(Zentity *self)
{
	float mx, my;

	if (!self) return;

	if (self->souls > 0)
	{
		owner->souls += self->souls;
		self->souls = 0;
	}

	 
	if (SDL_GetTicks() > lifetime || self->state == ES_DEAD && self->souls == 0)
	{
		Zentity_free(self);
	}
	
	/*
	if (collide_circle(self->position, self->radius, vector2d(mx, my), 1))
	{
		self->state = ES_DEAD;
		vector2d_set(self->velocity, 0, 0);
		return;
	}*/
}

void sword_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	slog("Hit for %i damage", self->damage);

	self->state = ES_DEAD;

	vector2d_set(self->velocity, 0, 0);
}


Zentity *sword_new(Vector2D position, Zentity *own)
{
	Zentity *self;
	self = Zentity_new();
	if (!self)return NULL;
	self->sprite = gf2d_sprite_load_all(
		"",
		128,
		128,
		16,
		false);
	self->radius = 25;
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