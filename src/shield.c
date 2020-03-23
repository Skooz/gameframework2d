#include "simple_logger.h"
#include "collisions.h"

#include "shield.h"
#include "level.h"

#define ES_DEAD 1

Uint32 lifetime;
int direction;
Entity *owner;

void shield_think(Entity *self)
{
	if (!self) return;

	int mx, my;

	if (self->souls > 0)
	{
		owner->souls += self->souls;
		self->souls = 0;
	}

	if (SDL_GetTicks() > lifetime || self->state == ES_DEAD)
		entity_free(self);

	if (collide_circle(self->position, self->radius, vector2d(mx, my), 1))
	{
		self->state = ES_DEAD;
		vector2d_set(self->velocity, 0, 0);
		return;
	}
}

void shield_touch(Entity *self, Entity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	slog("Hit for %i damage", self->damage);

	//self->state = ES_DEAD;

	if (direction == 1)
	{
		vector2d_set(other->velocity, 0, -3);
	}
	if (direction == 2)
	{
		vector2d_set(other->velocity, 0, 3);
	}
	if (direction == 3)
	{
		vector2d_set(other->velocity, -3, 0);
	}
	if (direction == 4)
	{
		vector2d_set(other->velocity, 3, 0);
	}
	

	// Do damage

	vector2d_set(self->velocity, 0, 0);
}


Entity *shield_new(Vector2D position, int dir, Entity *own)
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
	self->think = shield_think;
	self->touch = shield_touch;
	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	// Who created us
	owner = own;

	// Damage
	self->damage = 0 * owner->attack;

	direction = dir;
	lifetime = SDL_GetTicks() + 100;

	return self;
}