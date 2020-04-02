#include "simple_logger.h"
#include "collisions.h"

#include "shield.h"
#include "level.h"

#define ES_DEAD 1

Uint32 lifetime;
int direction;
Zentity *owner;

Uint32 bashTime;

void shield_think(Zentity *self)
{
	float mx, my;

	if (!self) return;

	if (self->souls > 0)
	{
		owner->souls += self->souls;
		self->souls = 0;
	}

	if (SDL_GetTicks() > lifetime || self->state == ES_DEAD)
		Zentity_free(self);

	/*
	if (collide_circle(self->position, self->radius, vector2d(mx, my), 1))
	{
		self->state = ES_DEAD;
		vector2d_set(self->velocity, 0, 0);
		return;
	}*/
}

void shield_touch(Zentity *self, Zentity *other)
{
	if ((!self) || (!other) || self->state == ES_DEAD)return;

	//slog("Hit for %i damage", self->damage);

	if (other->canBash)
	{
		//other->isBashed;
		if (direction == 1)
		{
			//vector2d_set(other->velocity, 0, -3);
			vector2d_set(other->position, other->position.x, other->position.y - 40);
		}
		if (direction == 2)
		{
			//vector2d_set(other->velocity, 0, 3);
			vector2d_set(other->position, other->position.x, other->position.y + 40);
		}
		if (direction == 3)
		{
			//vector2d_set(other->velocity, -3, 0);
			vector2d_set(other->position, other->position.x - 40, other->position.y);
		}
		if (direction == 4)
		{
			//vector2d_set(other->velocity, 3, 0);
			vector2d_set(other->position, other->position.x + 40, other->position.y);
		}
	}
	//self->state = ES_DEAD;
	// Do damage

	vector2d_set(self->velocity, 0, 0);
}


Zentity *shield_new(Vector2D position, int dir, Zentity *own)
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