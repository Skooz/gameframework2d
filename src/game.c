#include <SDL.h>
#include <SDL_ttf.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

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
#include "level.h"
#include "monster.h"
#include "player.h"
#include "bonfire.h"
#include "arrow.h"
#include "shield.h"
#include "sword.h"
#include "bloodstain.h"
#include "fountain.h"
#include "portal.h"

Window *ui;
Window* menu;

void startGame();
void exitGame();

int done;

int main(int argc, char * argv[])
{
    /*variable declarations*/
	done = 0;
	int levelchange = 0;
    const Uint8 * keys;
	
	//Sprite *mouse;

	char windowTitle[30];
    
	int mx,my;
    float mf = 0;
   
	//Vector4D mouseColor = {255,100,255,200};

	// Entities
	Level *level;
	Zentity *player;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/
	//mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, false);
	
	Zentity_manager_init(1024);
	
	// Player & Level
	level = level_new(0);
	player = player_new("saves/player.json");
	
	// UI & HUD
	gf2d_font_init("config/font.cfg");
	gfc_input_init("config/input.cfg");
	gf2d_action_list_init(128);
	gf2d_windows_init(32);
	ui = gf2d_window_new();
	ui = gf2d_window_load("config/ui.json");
	menu = gf2d_window_new();
	menu = gf2d_window_load("config/menu.json");
	gf2d_mouse_load("actors/mouse.actor");

	Element* startButton;
	Element* exitButton;
	startButton = gf2d_window_get_element_by_id(menu, 21);
	exitButton = gf2d_window_get_element_by_id(menu, 24);

	Element* hplabel;
	Element* mplabel;
	Element* soulslabel;
	Element* attacklabel;
	TextLine str;
	hplabel = gf2d_window_get_element_by_id(ui, 0);
	mplabel = gf2d_window_get_element_by_id(ui, 1);
	attacklabel = gf2d_window_get_element_by_id(ui, 2);
	soulslabel = gf2d_window_get_element_by_id(ui, 3);

	gfc_input_set_callbacks("startGame", startGame, NULL, NULL, NULL, menu);
	gfc_input_set_callbacks("exitGame", exitGame, NULL, NULL, NULL, menu);

	/*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
		gfc_input_update();

        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

		gf2d_mouse_update();
		gf2d_windows_update_all();

		// Update HUD
		sprintf(str, "HP: %i / %i", player->health, player->maxHealth); // Player Health UI
		gf2d_element_label_set_text(hplabel, str);
		sprintf(str, "MP: %i / %i", player->magic, player->maxMagic);	// Player Magic UI
		gf2d_element_label_set_text(mplabel, str);
		sprintf(str, "Attack: %i", player->attack);						// Player Attack UI
		gf2d_element_label_set_text(attacklabel, str);
		sprintf(str, "Souls: %i", player->souls);						// Player Souls UI
		gf2d_element_label_set_text(soulslabel, str);
		
		Zentity_update_all();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame   
			//backgrounds drawn first
			level_draw(level);
			Zentity_draw_all();
            //UI elements last
			gf2d_windows_draw_all();
			gf2d_mouse_draw();
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		
		/* Level change
		if (keys[SDL_SCANCODE_F7] && levelchange == 0)
		{
			levelchange = 1;
			player_save(player, "saves/player.json");
			Zentity_free_all();
			level_free(level);
			level = level_new(2);
			player = player_new("saves/player.json");
		}
		if (keys[SDL_SCANCODE_F8] && levelchange == 1)
		{
			levelchange = 0;
			player_save(player, "saves/player.json");
			Zentity_free_all();
			level_free(level);
			level = level_new(1);
			player = player_new("saves/player.json");
		}
		*/
		// exit condition(s)

		if (keys[SDL_SCANCODE_ESCAPE]) 
		{
			player_save(player, "saves/player.json");
			exitGame();
		}

		//slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
	level_free(level);
	Zentity_manager_close();

    slog("---==== END ====---");
    return 0;
}

void startGame()
{
	slog("haha");
}

void exitGame()
{
	//player_save(player, "saves/player.json");
	slog("done?");
	done = 1;
}

/*eol@eof*/
