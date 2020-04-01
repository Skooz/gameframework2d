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
#include "monster.h"
#include "player.h"
#include "level.h"

Window *ui;
Window* test;



int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
	Sprite *mouse;

	char windowTitle[30];
    
    int mx,my;
    float mf = 0;
    Vector4D mouseColor = {255,100,255,200};

	// Zentity Test
	SDL_Rect bounds = { 0, 0, 1200, 720 };
	Level *level;
	Zentity *monster1;
	Zentity *monster2;
	Zentity *monster3;
	Zentity *monster4;
	//Zentity *monster5;
	Zentity *player;
	Zentity *bonfire1;
	Zentity *bonfire2;

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
	level = level_new("images/backgrounds/zeldaworld.png", bounds);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16, false);

	// Entities
	Zentity_manager_init(1024);
	player = player_new(vector2d(600, 600));
	bonfire1 = bonfire_new(vector2d(300, 400));
	bonfire2 = bonfire_new(vector2d(800, 400));
	monster1 = monster_new(vector2d(400, 400), 1);
	monster2 = monster_new(vector2d(400, 500), 2);
	monster3 = monster_new(vector2d(600, 500), 3);
	monster4 = monster_new(vector2d(700, 500), 4);
	//monster5 = monster_new(vector2d(400, 400), 5);

	// Windows
	gf2d_font_init("config/font.cfg");
	// Input?
	gf2d_action_list_init(32);
	gf2d_windows_init(10);

	ui = gf2d_window_new();
	ui = gf2d_window_load("config/ui.json");

	Element* hplabel;
	Element* mplabel;
	Element* soulslabel;
	Element* attacklabel;
	TextLine str;

	hplabel = gf2d_window_get_element_by_id(ui, 0);
	mplabel = gf2d_window_get_element_by_id(ui, 1);
	attacklabel = gf2d_window_get_element_by_id(ui, 2);
	soulslabel = gf2d_window_get_element_by_id(ui, 3);

	//gf2d_window_add_element(ui, );

	/*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        
		/*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

		// Update UI
		// Player Health UI
		sprintf(str, "HP: %i / %i", player->health, player->maxHealth);
		gf2d_element_label_set_text(hplabel, str);
		// Player Magic UI
		sprintf(str, "MP: %i / %i", player->magic, player->maxMagic);
		gf2d_element_label_set_text(mplabel, str);
		// Player Attack UI
		sprintf(str, "Attack: %i", player->attack);
		gf2d_element_label_set_text(attacklabel, str);
		// Player Souls UI
		sprintf(str, "Souls: %i", player->souls);
		gf2d_element_label_set_text(soulslabel, str);
		
        
		// Zentity Test
		Zentity_update_all();
		gf2d_windows_update_all();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,vector2d(0,0));
			level_draw(level);
            
			// Zentity Test
			Zentity_draw_all();
			gf2d_windows_draw_all();


            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE]) done = 1; // exit condition

		//slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
	level_free(level);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
