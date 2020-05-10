
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "level.h"

#include "gf2d_entity.h"
#include "gf2d_windows.h"
#include "gf2d_elements.h"
#include "gf2d_font.h"
#include "gfc_input.h"
#include "gf2d_element_button.h"
#include "gf2d_element_label.h"
#include "gf2d_element_actor.h"
#include "gf2d_mouse.h"

#include "entity.h"
#include "monster.h"
#include "player.h"
#include "bonfire.h"
#include "arrow.h"
#include "shield.h"
#include "sword.h"
#include "bloodstain.h"
#include "fountain.h"
#include "portal.h"


static Level *THE_LEVEL = NULL;

Level *level_get_active()
{
	return THE_LEVEL;
}

Uint8 level_bounds_test_circle(Level *level, Vector2D center, float radius, Vector2D *normal)
{
	Uint8 hit = 0;
	if (!level)
	{
		slog("no level provided for test");
		return 0;
	}
	if (center.x - radius < level->bounds.x)
	{
		hit = 1;
		if (normal)normal->x = 1;
	}
	if (center.y - radius < level->bounds.y)
	{
		hit = 1;
		if (normal)normal->y = 1;
	}
	if (center.x + radius > level->bounds.x + level->bounds.w)
	{
		hit = 1;
		if (normal)normal->x = -1;
	}
	if (center.y + radius > level->bounds.y + level->bounds.h)
	{
		hit = 1;
		if (normal)normal->y = -1;
	}
	if ((hit) && (normal))
	{
		vector2d_normalize(normal);
	}
	return hit;
}

Level *level_new(int worldNum)
{
	char* backgroundFile;
	SDL_Rect bounds = { 0, 0, 1200, 720 };
	Level *level;

	// Background
	if (worldNum == 1) // 1 - Overworld
		backgroundFile = "images/backgrounds/zeldaworld.png";
	else if (worldNum == 2) // Underworld
		backgroundFile = "images/backgrounds/zeldaunder.png";
	else if (worldNum == 3)
		backgroundFile = "images/backgrounds/zeldaworld2.png";
	else
		backgroundFile = "images/backgrounds/zeldatitle.png";

	level = (Level*)gfc_allocate_array(sizeof(Level), 1);
	if (!level)return NULL;
	level->background = gf2d_sprite_load_image(backgroundFile);
	gfc_rect_set(level->bounds, bounds.x, bounds.y, bounds.w, bounds.h);
	THE_LEVEL = level;

	// Entities
	if (worldNum == 3) // 3 - Overworld 2
	{
		level->player = player_new("saves/player.json");
		level->player->position.x = 600;
		level->player->position.y = 550;

		Zentity *portal4;
		portal4 = portal_new(vector2d(600, 650), level, 1);

		Zentity *monster3;
		monster3 = monster_new(vector2d(600, 475), 3); // Rectangular
		Zentity *monster4;
		monster4 = monster_new(vector2d(700, 500), 4); // Stationary
	}
	else if (worldNum == 2) // 2 - Underworld
	{
		level->player = player_new("saves/player.json");
		level->player->position.x = 600;
		level->player->position.y = 550;

		Zentity *portal3;
		Zentity *monster5;
		Zentity *bonfire;

		portal3 = portal_new(vector2d(600, 650), level, 1);
		monster5 = monster_new(vector2d(600, 400), 5);
		bonfire = bonfire_new(vector2d(600, 300));
	}
	else if (worldNum == 1) // 1 - Default
	{
		level->player = player_new("saves/player.json");
		level->player->position.x = 350;
		level->player->position.y = 150;

		Zentity *monster1;
		monster1 = monster_new(vector2d(400, 550), 1); // Up-Down
		Zentity *monster2;
		monster2 = monster_new(vector2d(500, 300), 2); // Side-Side
		Zentity *monster3;
		monster3 = monster_new(vector2d(600, 500), 2); // Up-Down
		Zentity *monster4;
		monster4 = monster_new(vector2d(700, 525), 1); // Side-Side

		Zentity *bonfire;
		bonfire = bonfire_new(vector2d(400, 150));
		
		Zentity *fountain1;
		fountain1 = fountain_new(vector2d(900, 550), 1);
		Zentity *fountain2;
		fountain2 = fountain_new(vector2d(1000, 550), 2);
		
		Zentity *portal2;
		portal2 = portal_new(vector2d(600, 50), level, 3);
		Zentity *portal;
		portal = portal_new(vector2d(295, 50), level, 2);
		
	}
	else // 0 - Title
	{

	}

	return level;
}

void level_free(Level *level)
{
	if (!level)return;
	gf2d_sprite_free(level->background);
	free(level);
}

void level_draw(Level *level)
{
	if (!level)return;
	gf2d_sprite_draw_image(level->background, vector2d(0, 0));
	gf2d_draw_rect(level->bounds, vector4d(255, 0, 0, 255));
}