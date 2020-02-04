#include <stdlib.h>
#include "simple_logger.h"
#include "entity.h"

typedef struct
{
	Uint32 maxEnts; /**<max entities supported by the system*/
	Entity *entityList; /**<pointer to an allocated array of entitites*/
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_manager_close()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (entity_manager.entityList[i]._inuse)
		{
			entity_free(&entity_manager.entityList[i]);
		}
	}
	entity_manager.maxEnts = 0;
	free(entity_manager.entityList);
	entity_manager.entityList = NULL;
}

void entity_manager_init(Uint32 maxEnts)
{
	if (entity_manager.entityList != NULL)
	{
		//TODO: cleanup
	}
	if (!maxEnts)
	{
		slog("canot initialize a zero size entity list!");
		return;
	}
	entity_manager.entityList = (Entity *)malloc(sizeof(Entity));
	if (entity_manager.entityList == NULL)
	{
		slog("failed to allocate %i entities for the entity manager", maxEnts);
		return;
	}
	entity_manager.maxEnts = maxEnts;
	memset(entity_manager.entityList, 0, sizeof(Entity)*maxEnts);
	slog("entity manager initialized");
	atexit(entity_manager_close);
}

Entity *entity_new()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (entity_manager.entityList[i]._inuse) continue;
		entity_manager.entityList[i]._inuse = 1;
		return &entity_manager.entityList[i];
	}
	slog("out of open entity slots in memory!");
	return NULL;
}

void entity_free(Entity *self)
{
	if (!self) return;
	gf2d_sprite_free(self->sprite);
	memset(self, 0, sizeof(Entity));
}

void entity_update(Entity *self)
{
	if (!self) return;
	self->frame = self->frame + 0.1;
	self->frame;
}

void entity_update_all()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (!entity_manager.entityList[i]._inuse) continue;
		entity_draw(&entity_manager.entityList[i]);
	}
}

void entity_draw(Entity *self)
{
	if (self == NULL)
	{
		slog("cannot draw sprite, NULL entity provided");
		return;
	}
	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	
}

void entity_draw_all()
{
	int i;
	for (i = 0; i < entity_manager.maxEnts; i++)
	{
		if (!entity_manager.entityList[i]._inuse) continue;
		entity_draw(&entity_manager.entityList[i]);
	}
}