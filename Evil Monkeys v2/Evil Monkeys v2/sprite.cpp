#include "stdwx.h"

#include "sprite.h"

Sprite::Sprite( Level *l, DrawEngine *de, int s_index, float x, float y, int i_lives )
{
	level = l;

	drawArea = de;
	
	pos.x = x;
	pos.y = y;

	spriteIndex = s_index;

	numLives = i_lives;

	facingDirection.x = 1;
	facingDirection.y = 0;

	classID = SPRITE_CLASSID;

	delay = 80;
	lastTime = 0;
	bUpdateSprite = false;
}

Sprite::~Sprite()
{
	// erase the dying sprite
	erase( pos.x, pos.y );
}

void Sprite::setPosition( float x, float y )
{
	// erase sprite
	erase( pos.x, pos.y );

	facingDirection.x = facingDirection.y = -100;

	pos.x = x;
	pos.y = y;

	// draw sprite
	draw( pos.x, pos.y );
}

vector Sprite::getPosition( void )
{
	return pos;
}

float Sprite::getX( void )
{
	return pos.x;
}

float Sprite::getY( void )
{
	return pos.y;
}

void Sprite::addLives( int num )
{
	numLives += num;
}

int Sprite::getLives( void )
{
	return numLives;
}

bool Sprite::isAlive( void )
{
	return ( numLives > 0 );
}

bool Sprite::move( float x, float y )
{
	int xpos = (int)( pos.x + x );
	int ypos = (int)( pos.y + y );

	if ( isValidLevelMove( xpos, ypos ) )
	{
		// erase sprite
		erase( pos.x, pos.y );
		 
		pos.x += x;
		pos.y += y;

		facingDirection.x = x;
		facingDirection.y = y;

		// draw sprite
		draw( pos.x, pos.y );

		return true;
	}
	else
		facingDirection.x = facingDirection.y = -100;

	return false;
}

void Sprite::draw( float x, float y )
{
	drawArea->drawSprite( spriteIndex, (int)x, (int)y );
}

void Sprite::erase( float x, float y )
{
	drawArea->eraseSprite( (int)x, (int)y );
}

bool Sprite::isValidLevelMove( int xpos, int ypos )
{
	if ( xpos >= 0 && xpos < level->getWidth() &&
		 ypos >= 0 && ypos < level->getHeight() &&
		 level->level[xpos][ypos] != TILE_WALL )
		return true;

	return false;
}

void Sprite::idleUpdate( void )
{
	// this is for the inherited classes
}




void Sprite::setLevel( Level *lvl )
{
	level = lvl;
}

void Sprite::update( void )
{
	draw( pos.x, pos.y );
}

int Sprite::getID( void )
{
	return classID;
}

void Sprite::setSpeed( int new_speed )
{
	// take the new_speed and convert it into a delay (which is the amount of time between update)
	delay = 200 - ( new_speed / 100.0f ) * 200;	// the max delay is going to be 100 and the min delay is going to be 0, because the speed is between 0 -100
}

void Sprite::setCurrentTime( long time )
{
	if ( time - lastTime > delay )
	{
		lastTime = time;
		bUpdateSprite = true;
	}
}

bool Sprite::updateSprite( void )
{
	bool out = bUpdateSprite;
	bUpdateSprite = false;
	return out;
}