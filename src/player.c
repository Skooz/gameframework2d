#include "simple_logger.h"
#include "gf2d_draw.h"
#include "collisions.h"
#include "SDL_timer.h"
#include "player.h"
#include "level.h"
#include "arrow.h"
#include "shield.h"
#include "sword.h"
#include "bloodstain.h"

#define ES_DEAD 1

void player_think(Zentity *self);
void player_touch(Zentity *self, Zentity *other);

Uint32 nextAttack; // Used with SDL_GetTicks() to create a delay between attacks.
Uint32 nextMessage;
Zentity* lastBonfire;
Zentity* playerBloodstain;

// Create a player
Zentity *player_new(Vector2D position)
{
	Zentity *self;
	self = Zentity_new();
	if (!self) return NULL;

	self->sprite = gf2d_sprite_load_all(
		"images/lonk2.png",
		60,
		60,
		6,
		false);

	self->isPlayer = 1;
	self->maxHealth = 100;
	self->health	= self->maxHealth;
	self->maxMagic	= 100;
	self->magic		= self->maxMagic;
	self->attack	= 1;
	self->souls;

	self->radius = 15;
	self->size.x = 30;
	self->size.y = 30;

	self->think = player_think;
	self->touch = player_touch;

	vector2d_copy(self->position, position);
	vector2d_set(self->drawOffset, -30, -30);

	return self;
}

// thonk
void player_think(Zentity *self)
{
	if (!self) return;

	static int direction; // 1=up, 2=down, 3=left, 4=right
	const Uint8 * keys;

	keys = SDL_GetKeyboardState(NULL);

	// ** STATS ** 
	if (keys[SDL_SCANCODE_F1] && nextMessage < SDL_GetTicks()) // Level Health
	{
		nextMessage = SDL_GetTicks() + 500;
		if (self->souls >= 100)
		{
			self->maxHealth += 10;
			self->souls -= 100;
			slog("Health increased +10.\n-100 Souls.");
		}
		else
			slog("Not enough souls.");
	}
	if (keys[SDL_SCANCODE_F2] && nextMessage < SDL_GetTicks()) // Level Magic
	{
		nextMessage = SDL_GetTicks() + 500;
		if (self->souls >= 100)
		{
			self->maxMagic += 10;
			self->souls -= 100;
			slog("Magic increased +10.\n-100 Souls.");
		}
		else
			slog("Not enough souls.");
	}
	if (keys[SDL_SCANCODE_F3] && nextMessage < SDL_GetTicks()) // Level Attack
	{
		nextMessage = SDL_GetTicks() + 500;
		if (self->souls >= 100)
		{
			self->attack += 1;
			self->souls -= 100;
			slog("Attack increased +1.\n-100 Souls.");
		}
		else
			slog("Not enough souls.");
	}
	if (keys[SDL_SCANCODE_1] && nextMessage < SDL_GetTicks()) // Print Health
	{
		nextMessage = SDL_GetTicks() + 500;
		slog("Health: %i / %i", self->health, self->maxHealth);
	}
	if (keys[SDL_SCANCODE_2] && nextMessage < SDL_GetTicks()) // Print Magic
	{
		nextMessage = SDL_GetTicks() + 500;
		slog("Magic: %i / %i", self->magic, self->maxMagic);
	}
	if (keys[SDL_SCANCODE_3] && nextMessage < SDL_GetTicks()) // Print Attack
	{
		nextMessage = SDL_GetTicks() + 500;
		slog("Attack: %i", self->attack);
	}
	if (keys[SDL_SCANCODE_4] && nextMessage < SDL_GetTicks()) // Print Souls
	{
		nextMessage = SDL_GetTicks() + 500;
		slog("Souls: %i", self->souls);
	}

	// ** MOVEMENT **
	if (keys[SDL_SCANCODE_W]) // Up
	{
		direction = 1;
		vector2d_set(self->velocity, 0, -2);
		self->frame = self->frame + 0.05;
		if (self->frame < 12 || self->frame >= 14) self->frame = 12;
	}
	if (keys[SDL_SCANCODE_S]) // Down
	{
		direction = 2;
		vector2d_set(self->velocity, 0, 2);
		self->frame = self->frame + 0.05;
		if (self->frame < 0 || self->frame >= 2) self->frame = 0;
	}
	if (keys[SDL_SCANCODE_A]) // Left
	{
		direction = 3;
		vector2d_set(self->velocity, -2, 0);
		self->frame = self->frame + 0.05;
		if (self->frame < 6 || self->frame >= 8) self->frame = 6;
	}
	if (keys[SDL_SCANCODE_D]) // Right
	{
		direction = 4;
		vector2d_set(self->velocity, 2, 0);
		self->frame = self->frame + 0.05;
		if (self->frame < 18 || self->frame >= 20) self->frame = 18;
	}
	if (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) // Not moving
	{
		vector2d_set(self->velocity, 0, 0);
	}


	// ** ATTACKS **
	if (keys[SDL_SCANCODE_Q]) // Shield Bash - Knock enemies back - Need a shield Zentity
	{
		Zentity *shield;
		if (direction == 1)
		{
			self->frame = 15;
			if (self->frame < 14 || self->frame >= 16) self->frame = 14;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				shield = shield_new(vector2d(self->position.x, self->position.y - 50), direction, self);
			}
		}
		if (direction == 2)
		{
			self->frame = 3;
			if (self->frame < 2 || self->frame >= 4) self->frame = 2;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				shield = shield_new(vector2d(self->position.x, self->position.y + 50), direction, self);
			}
		}
		if (direction == 3)
		{
			self->frame = 9;
			if (self->frame < 8 || self->frame >= 10) self->frame = 8;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				shield = shield_new(vector2d(self->position.x - 50, self->position.y), direction, self);
			}
		}
		if (direction == 4)
		{
			self->frame = 21;
			if (self->frame < 20 || self->frame >= 22) self->frame = 20;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				shield = shield_new(vector2d(self->position.x + 50, self->position.y), direction, self);
			}
		}
	}
	if (keys[SDL_SCANCODE_E]) // Sword
	{
		Zentity *sword_swipe;
		if (direction == 1)
		{
			self->frame = 15;
			if (self->frame < 14 || self->frame >= 16) self->frame = 14;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				sword_swipe = sword_new(vector2d(self->position.x, self->position.y - 50), self);
			}
		}
		if (direction == 2)
		{
			self->frame = 3;
			if (self->frame < 2 || self->frame >= 4) self->frame = 2;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				sword_swipe = sword_new(vector2d(self->position.x, self->position.y + 50), self);
			}
		}
		if (direction == 3)
		{
			self->frame = 9;
			if (self->frame < 8 || self->frame >= 10) self->frame = 8;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				sword_swipe = sword_new(vector2d(self->position.x - 50, self->position.y), self);
			}
		}
		if (direction == 4)
		{
			self->frame = 21;
			if (self->frame < 20 || self->frame >= 22) self->frame = 20;
			if (SDL_GetTicks() > nextAttack)
			{
				nextAttack = SDL_GetTicks() + 300;
				sword_swipe = sword_new(vector2d(self->position.x + 50, self->position.y), self);
			}
		}
	}
	if (keys[SDL_SCANCODE_R]) // Arrow - Launch a projectile
	{
		if (SDL_GetTicks() > nextAttack)
		{
			nextAttack = SDL_GetTicks() + 300;
			Zentity *arrow;
			if (direction == 1)
			{
				arrow = arrow_new(vector2d(self->position.x, self->position.y-30), vector2d(0, -5), direction, self);
			}
			if (direction == 2)
			{
				arrow = arrow_new(vector2d(self->position.x, self->position.y+30), vector2d(0, 5), direction, self);
			}
			if (direction == 3)
			{
				arrow = arrow_new(vector2d(self->position.x-30, self->position.y), vector2d(-5, 0), direction, self);
			}
			if (direction == 4)
			{
				arrow = arrow_new(vector2d(self->position.x+30, self->position.y), vector2d(5, 0), direction, self);
			}
		}
	}
	if (keys[SDL_SCANCODE_F]) // Sword spin - Might want a unique Zentity for this instead.
	{
		if (SDL_GetTicks() > nextAttack)
		{
			if (self->magic > 10)
			{
				self->magic -= 10;
				nextAttack = SDL_GetTicks() + 1000;
				Zentity *sword;
				sword = sword_new(vector2d(self->position.x, self->position.y - 50), self);
				sword_new(vector2d(self->position.x+30, self->position.y - 30), self);
				sword_new(vector2d(self->position.x-30, self->position.y - 30), self);
				sword_new(vector2d(self->position.x, self->position.y + 50), self);
				sword_new(vector2d(self->position.x - 30, self->position.y + 30), self);
				sword_new(vector2d(self->position.x + 30, self->position.y + 30), self);
				sword_new(vector2d(self->position.x - 50, self->position.y), self);
				sword_new(vector2d(self->position.x + 50, self->position.y), self);
			}
		}
	}
	if (keys[SDL_SCANCODE_T]) // Magic - Heal
	{
		if (self->health < self->maxHealth)
		{
			if (self->magic > 10)
			{
				self->magic -= 20;
				self->health += 10;
				slog("Healed!", self->health);
			}
		}
	}

	// ** DEATH **
	if (self->health <= 0 || keys[SDL_SCANCODE_K])
	{
		playerBloodstain = bloodstain_new(self->position, self->souls);

		if (lastBonfire)
			self->position = lastBonfire->position;
		else
			self->position = vector2d(600, 600);

		// Reset Stats
		self->health = self->maxHealth;
		self->magic = self->maxMagic;
		self->souls = 0;
	}
}


void player_touch(Zentity *self, Zentity *other)
{
	if (!self || !other) return;

	if (other->isBonfire == 1 && other->bonfireUsed == 0)
	{
		if (lastBonfire)
		{
			lastBonfire->bonfireUsed = 0;
		}
		lastBonfire = other;
		slog("touched bonfire");
	}

}