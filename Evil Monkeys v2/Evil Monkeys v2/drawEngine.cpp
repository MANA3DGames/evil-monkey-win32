#include "stdwx.h"

#include "drawEngine.h"


#define GRID_SIZE 64

DrawEngine::DrawEngine( int xSize, int ySize )
{
	screenWidth = xSize;
	screenHeight = ySize;

	map = 0;
}

DrawEngine::~DrawEngine()
{
}

int DrawEngine::createSprite( int index, wxBitmap *sprite )
{
	if ( index >= 0 && index < 16 )
	{
		// first we need to convert the bitmap into an image
		wxImage img = sprite->ConvertToImage();
		// set the mask colour to white
		img.SetMaskColour( 255, 255, 255 );
		
		// add the image to the sprite image array
		spriteImage[index] = new wxBitmap( img );

		return index;
	}

	return -1;
}

void DrawEngine::deleteSprite( int index )
{
	// in this implementaion we don't need it
}

void DrawEngine::drawSprite( int index, int posX, int posY )
{
	// we want to draw on a dc (memory) not on a space like the console
	wxMemoryDC dc;

	// take a bitmap and associate it with a DC	//winCanvas is where we want to draw
	dc.SelectObject( *winCanvas );

	// now we want to draw the bitmap
	dc.DrawBitmap( *spriteImage[index], wxPoint( posX * GRID_SIZE, posY * GRID_SIZE ), true );	// true because we want to use the mask

	// to avoid memory leak, deassociate the bitmap
	dc.SelectObject( wxNullBitmap );
}

void DrawEngine::eraseSprite( int posX, int posY )
{
	// we want to draw on a dc (memory) not on a space like the console
	wxMemoryDC dc;

	// take a bitmap and associate it with a DC	//winCanvas is where we want to draw
	dc.SelectObject( *winCanvas );

	// now we want to draw the bitmap
	dc.DrawBitmap( *tileImage[0], wxPoint( posX * GRID_SIZE, posY * GRID_SIZE ), true );	// true because we want to use the mask

	// to avoid memory leak, deassociate the bitmap
	dc.SelectObject( wxNullBitmap );
}

void DrawEngine::setMap( char **data )
{
	map = data;
}

void DrawEngine::createBackgroundTile( int index, wxBitmap *tile )
{
	if ( index >= 0 && index < 16 )
	{
		// first we need to convert the bitmap into an image
		wxImage img = tile->ConvertToImage();
		// set the mask colour to white
		img.SetMaskColour( 255, 255, 255 );	// there is no need for this but just in case!
		
		// add the image to the sprite image array
		tileImage[index] = new wxBitmap( img );
	}
}

void DrawEngine::drawBackground( void )
{
	// we want to draw on a dc (memory) not on a space like the console
	wxMemoryDC dc;

	// take a bitmap and associate it with a DC	//winCanvas is where we want to draw
	dc.SelectObject( *winCanvas );

	if ( map )
	{
		for ( int y = 0; y < screenHeight; y++ )
		{
			// now we want to draw the bitmap
			for ( int x = 0; x < screenWidth; x++ )
				dc.DrawBitmap( *tileImage[map[x][y]], wxPoint( x * GRID_SIZE, y * GRID_SIZE ), true );	// true because we want to use the mask
		}
	}

	// to avoid memory leak, deassociate the bitmap
	dc.SelectObject( wxNullBitmap );
}

void DrawEngine::setWindow( wxBitmap *in, int width, int height )
{
	winCanvas = in;

	screenWidth = width;
	screenHeight = height;
}


