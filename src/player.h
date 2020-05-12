#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "level.h"

Zentity *player_new(char* saveFile);

void player_save(Zentity *self, char* saveFile);

#endif