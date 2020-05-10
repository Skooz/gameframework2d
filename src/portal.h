#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "entity.h"
#include "level.h"

/**
* @brief test if a circle is touching or exceeding the level bounds
* @param pL the currently active level to pass
* @param wT the world number we're going to move to
*/
Zentity *portal_new(Vector2D position, Level *pL, int wT, int rad);


#endif