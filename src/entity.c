#include <stdlib.h>
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "entity.h"
#include "level.h"
#include "collisions.h"

typedef struct
{
	Uint32 maxEnts;     /**<max entities supported by the system*/
	Zentity *ZentityList; /**<pointer to an allocated array of entities*/
}ZentityManager;

static ZentityManager Zentity_manager = { 0 };


void Zentity_collide_check(Zentity *Zentity);

void Zentity_manager_close()
{
	int i;
	for (i = 0; i < Zentity_manager.maxEnts; i++)
	{
		if (Zentity_manager.ZentityList[i]._inuse)
		{
			Zentity_free(&Zentity_manager.ZentityList[i]);
		}
	}
	Zentity_manager.maxEnts = 0;
	free(Zentity_manager.ZentityList);
	Zentity_manager.ZentityList = NULL;
	slog("Zentity manager closed");
}

void Zentity_manager_init(Uint32 maxEnts)
{
	if (Zentity_manager.ZentityList != NULL)
	{
		//TODO: cleanup
	}
	if (!maxEnts)
	{
		slog("cannot intialize a zero size Zentity list!");
		return;
	}
	Zentity_manager.ZentityList = (Zentity *)malloc(sizeof(Zentity)* maxEnts);
	if (Zentity_manager.ZentityList == NULL)
	{
		slog("failed to allocate %i entities for the Zentity manager", maxEnts);
		return;
	}
	Zentity_manager.maxEnts = maxEnts;
	memset(Zentity_manager.ZentityList, 0, sizeof(Zentity)*maxEnts);
	slog("Zentity manager initialized");
	atexit(Zentity_manager_close);
}


Zentity *Zentity_new()
{
	int i;
	for (i = 0; i < Zentity_manager.maxEnts; i++)
	{
		if (Zentity_manager.ZentityList[i]._inuse)continue;
		Zentity_manager.ZentityList[i]._inuse = 1;
		return &Zentity_manager.ZentityList[i];
	}
	slog("out of open Zentity slots in memory!");
	return NULL;
}


void Zentity_free(Zentity *self)
{
	if (!self)return;
	gf2d_sprite_free(self->sprite);
	memset(self, 0, sizeof(Zentity));
}

void Zentity_update(Zentity *self)
{
	Vector2D normal = { 0, 0 };
	if (!self)return;

	// Check level bounds
	vector2d_add(self->position, self->position, self->velocity);
	if (level_bounds_test_circle(level_get_active(), self->position, self->radius, &normal))
	{
		if (normal.x > 0)
		{
			//self->velocity.x = fabs(self->velocity.x);
			self->velocity.x = 0;
			self->position.x = self->position.x + 2;
		}
		if (normal.x < 0)
		{
			//self->velocity.x = -fabs(self->velocity.x);
			self->velocity.x = 0;
			self->position.x = self->position.x - 2;
		}
		if (normal.y > 0)
		{
			//self->velocity.y = fabs(self->velocity.y);
			self->velocity.y = 0;
			self->position.y = self->position.y + 2;
		}
		if (normal.y < 0)
		{
			//self->velocity.y = -fabs(self->velocity.y);
			self->velocity.y = 0;
			self->position.y = self->position.y - 2;
		}
	}
	Zentity_collide_check(self);
}

void Zentity_update_all()
{
	int i;
	for (i = 0; i < Zentity_manager.maxEnts; i++)
	{
		if (!Zentity_manager.ZentityList[i]._inuse)continue;
		if (Zentity_manager.ZentityList[i].think)
		{
			Zentity_manager.ZentityList[i].think(&Zentity_manager.ZentityList[i]);
		}
		Zentity_update(&Zentity_manager.ZentityList[i]);
	}
}

void Zentity_draw(Zentity *self)
{
	SDL_Rect rect;
	if (self == NULL)
	{
		slog("cannot draw sprite, NULL Zentity provided!");
		return;
	}
	gf2d_sprite_draw(
		self->sprite,
		vector2d(self->position.x + self->drawOffset.x, self->position.y + self->drawOffset.y),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	gf2d_draw_circle(self->position, self->radius, vector4d(255, 0, 255, 255));
	//gfc_rect_set(rect,self->position.x,self->position.y,self->size.x,self->size.y);
	//gf2d_draw_rect(rect, vector4d(255, 0, 255, 255));
}

void Zentity_Zentity_collide(Zentity *e1, Zentity *e2)
{
	if (collide_circle(e1->position, e1->radius, e2->position, e2->radius))
	{
		if (e1->touch)
		{
			e1->touch(e1, e2);
		}
	}
}

void Zentity_collide_check(Zentity *Zentity)
{
	int i;
	if (!Zentity)return;
	for (i = 0; i < Zentity_manager.maxEnts; i++)
	{
		if (!Zentity_manager.ZentityList[i]._inuse)continue;
		if (&Zentity_manager.ZentityList[i] == Zentity)continue;
		Zentity_Zentity_collide(Zentity, &Zentity_manager.ZentityList[i]);
	}
}

void Zentity_draw_all()
{
	int i;
	for (i = 0; i < Zentity_manager.maxEnts; i++)
	{
		if (!Zentity_manager.ZentityList[i]._inuse)continue;
		Zentity_draw(&Zentity_manager.ZentityList[i]);
	}
}



/*eol@eof*/