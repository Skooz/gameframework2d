#ifndef __Zentity_H__
#define __Zentity_H__

#include "gf2d_sprite.h"

typedef struct Zentity_S
{
	Uint8       _inuse;     /**<check if this Zentity in memory is active or not*/
	Sprite     *sprite;     /**<a pointer to the sprite that is used by this Zentity*/
	int         state;
	float       frame;      /**<current frame for the sprite*/

	Vector2D    position;   /**<where the Zentity is in 2D space*/
	Vector2D    velocity;

	Vector2D    drawOffset;
	float       radius;     /**<how wide this Zentity is*/
	Vector2D    size;

	SDL_Rect rect; // Rectangle to draw

	// Generic
	Uint32 birthday;

	// Monsters
	Uint32 nextMove;
	int moveDir;
	int monsterType;
	int canBash;
	int isBashed;

	//Players
	int isPlayer;	// Identify whether or not the Zentity is a player.
	int health;		// The player's health value.
	int maxHealth;	// The player's max health value.
	int magic;		// The player's magic value.
	int maxMagic;	// The player's max magic value.
	int attack;		// The player's attack damage modifier.
	int damage;		// The damage dealt by an Zentity.
	int souls;		// Souls currency.

	// Bonfires
	int isBonfire;
	int bonfireUsed;
	int bonfireID;

	// Bloodstains
	int isBloodstain;

	// Fountains
	int fountainType;
	
	// Portals
	int isPortal;
	int worldTo;

	void(*think)(struct Zentity_S *self);   /**<called when an Zentity draws*/
	void(*touch)(struct Zentity_S *self, struct Zentity_S *other);   /**<called when an Zentity touches another Zentity*/
	void(*see)(struct Zentity_S *self, struct Zentity_S *other);   /**<called when an Zentity touches another Zentity*/

}Zentity;

/**
* @brief get a pointer to a new Zentity
* @return NULL on out of memory or error, a pointer to a blank Zentity otherwise
*/
Zentity *Zentity_new();

/**
* @brief initialize the Zentity resource manager
* @param maxEnts upper bound of maximum concurrent entities to be supported
* @note must be called before creating a new Zentity
*/
void Zentity_manager_init(Uint32 maxEnts);

/**
* @brief free a previously allocated Zentity
* @param self a pointer to the Zentity to free
*/
void Zentity_free(Zentity *self);

/**
* @brief free all previously allocated Zentities
*/
void Zentity_free_all();

/**
* @brief update every active Zentity
*/
void Zentity_update_all();
/**
* @brief draww every active Zentity
*/
void Zentity_draw_all();

#endif