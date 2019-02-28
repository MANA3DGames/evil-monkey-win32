#ifndef LEVEL_H
#define LEVEL_H

#include "drawEngine.h"
//#include "character.h" // we can us -> class Character; to avoid Circular Referencing.
class Sprite;
class Character;	// This is a pre-definition, instead of using #include "character.h" because this will cause us a problem, (this mean Character is defined some where so the compiler believe us)

#include<list>
using std::list;


enum
{
	SPRITE_PLAYER,
	SPRITE_ENEMY,
	SPRITE_FIREBALL
};

enum
{
	TILE_EMPTY,
	TILE_WALL
};

class Level
{
public:
	Level( DrawEngine *de, int width = 30, int height = 20 );
	~Level();

	void addPlayer( Character *p );
	void update( long time );
	void draw( void );
	bool keyPress( char c );

	void addEnemies( int num, int speed = 5 );
	void setPlayerStart( void );
	void addNPC( Sprite *spr );

	friend class Sprite;	// which means it will have access on public & protected data



	int getWidth( void );
	int getHeight( void );
	int numEnemies( void );

protected:
	void createLevel( void );

private:
	int width;
	int height;

	int startX;
	int startY;

	char **level;
	
	Character *player;

	DrawEngine *drawArea;

public:
	list <Sprite *> npc;
	list <Sprite *>::iterator Iter;
};

#endif