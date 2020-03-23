#include <SDL.h>
#include <SDL_ttf.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_font.h"

#include "entity.h"
#include "bug.h"
#include "level.h"



int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
	Sprite *mouse;
    
    int mx,my;
    float mf = 0;
    Vector4D mouseColor = {255,100,255,200};

	// Entity Test
	SDL_Rect bounds = { 0, 0, 1200, 720 };
	Level *level;
	Entity *bug;
	Entity *player;

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
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

    /*main game loop*/

	// Entities
	entity_manager_init(1024);
	player = player_new(vector2d(700, 500));
	bug = bug_new(vector2d(500, 300));

	
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
		// Entity Test
		entity_update_all();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            //gf2d_sprite_draw_image(sprite,vector2d(0,0));
			level_draw(level);
            
			// Entity Test
			entity_draw_all();


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
        

		// Entity Movement?
		if (keys[SDL_SCANCODE_UP]);
		if (keys[SDL_SCANCODE_DOWN]);
		if (keys[SDL_SCANCODE_LEFT]);
		if (keys[SDL_SCANCODE_RIGHT]);


        if (keys[SDL_SCANCODE_ESCAPE]) done = 1; // exit condition
		// Entity Test
		//if (keys[SDL_SCANCODE_SPACE]) entity_free(bug); // free bug
		//slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());

    }
	level_free(level);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
