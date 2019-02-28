#include "stdwx.h"

#include "AppFrame.h"


class EvilMonkeysApp : public wxApp
{
	virtual bool OnInit();
};

bool EvilMonkeysApp::OnInit() 
{
	// create the main window
	AppFrame *frame = new AppFrame( "Evil Monkeys", wxPoint( 100, 100 ), wxSize( 650,500 ) );

	// Actually display the game
	frame->Show(TRUE);

	// Set it to the top so that we see it
	SetTopWindow( frame );

	return true;
}


// Create the main function for the application
IMPLEMENT_APP( EvilMonkeysApp );
