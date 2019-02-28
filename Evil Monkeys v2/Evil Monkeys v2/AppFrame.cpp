#include "stdwx.h"

#include "AppFrame.h"

#define GRID_SIZE	64
#define LEVEL_X		20
#define LEVEL_Y		10

#define UPDATE_TIME	20

BEGIN_EVENT_TABLE( AppFrame, wxFrame )
	EVT_MENU ( ID_New, AppFrame::OnNew )
	EVT_MENU ( ID_Load, AppFrame::OnLoad )
	EVT_MENU ( ID_About, AppFrame::OnAbout )
	EVT_MENU ( ID_Exit, AppFrame::OnExit )

	EVT_TIMER ( ID_Timer, AppFrame::OnTimer )
END_EVENT_TABLE()


AppFrame::AppFrame( const wxString& title, const wxPoint& pos, const wxSize& size )
: wxFrame( (wxFrame*)NULL, -1, title, pos, size )
{
	player = NULL;
	level = NULL;
	backBuffer = NULL;
	finalBackBuffer = NULL;

	// Create the Menu bar
	wxMenuBar *menu_bar = new wxMenuBar;

	wxMenu *menuFile = new wxMenu;

	menuFile->Append( ID_New, "&New" );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Load, "&Load" );
	menuFile->AppendSeparator();
	menuFile->Append( ID_About, "&About" );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Exit, "E&xit" );

	menu_bar->Append( menuFile, "&File" );

	SetMenuBar( menu_bar );

	// Create the Main Panel
	m_panel = new wxPanel( this, -1 );

	m_info_panel = new wxPanel( m_panel, -1, wxDefaultPosition , wxSize( 200, 400 ), wxBORDER_STATIC );

	gameWindow = new wxWindow( m_panel, -1, wxDefaultPosition, wxSize( 200, 200 ), wxBORDER_STATIC );
	gameWindow->Connect( -1, -1, wxEVT_PAINT, (wxObjectEventFunction) &AppFrame::OnPaint, NULL, this );
	gameWindow->Connect( -1, -1, wxEVT_KEY_DOWN, (wxObjectEventFunction) &AppFrame::OnKey, NULL, this );
	gameWindow->Connect( -1, -1, wxEVT_SIZE, (wxObjectEventFunction) &AppFrame::OnSize, NULL, this );
	
	stPlayerLives = new wxStaticText( m_info_panel, -1, _T( "Lives:" ), wxDefaultPosition, wxSize( 100, 15 ), wxST_NO_AUTORESIZE );
	stCurrentLevel = new wxStaticText( m_info_panel, -1, _T( "Level:" ), wxDefaultPosition, wxSize( 100, 15 ), wxST_NO_AUTORESIZE );
	stNumEnemies = new wxStaticText( m_info_panel, -1, _T( "Enemies:" ), wxDefaultPosition, wxSize( 100, 15 ), wxST_NO_AUTORESIZE );

	// add the relationship between the components
	wxBoxSizer *mainSizer = new wxBoxSizer( wxHORIZONTAL );

	mainSizer->Add( gameWindow, 1, wxGROW|wxALL, 5 );
	mainSizer->Add( m_info_panel, 0, wxGROW|wxALL|wxSTRETCH_NOT, 5 );

	wxBoxSizer *subSizer = new wxBoxSizer( wxVERTICAL );

	subSizer->Add( stPlayerLives, 0, wxLEFT|wxTOP, 5 );
	subSizer->Add( stCurrentLevel, 0, wxLEFT|wxTOP, 5 );
	subSizer->Add( stNumEnemies, 0, wxLEFT|wxTOP, 5 );

	m_panel->SetAutoLayout( TRUE );
	m_panel->SetSizer( mainSizer );

	m_info_panel->SetAutoLayout( TRUE );
	m_info_panel->SetSizer( subSizer );


	// we have to create the different tile and associate them with our drawEngine
	drawArea.createBackgroundTile( TILE_EMPTY, &wxBITMAP( EmptyTile ) );
	drawArea.createBackgroundTile( TILE_WALL, &wxBITMAP( WallTile ) );

	drawArea.createSprite( SPRITE_PLAYER, &wxBITMAP( PlayerTile ) );
	drawArea.createSprite( SPRITE_ENEMY, &wxBITMAP( EnemyTile ) );
	drawArea.createSprite( SPRITE_FIREBALL, &wxBITMAP( FireballTile ) );


	m_timer = new wxTimer( this, ID_Timer );
	m_timer->Start( UPDATE_TIME );

	srand( wxGetElapsedTime( false  ) );	// so we get a different value each time/ false -> because we don't want to reset the value of the Timer but we are going to reset it any way!!
	wxStartTimer();	// this will reset the timer

	// set the current state for the game
	gameState = STATE_NULL;
}

AppFrame::~AppFrame()
{
	// set the game to no game state
	gameState = STATE_NULL;

	// stop the game timer before delete it
	m_timer->Stop();

	// delete the timer
	delete m_timer;

	// delete the plaeyr
	if ( player )
		delete player;

	// delete the level
	if ( level )
		delete level;
}

// Event handler functions
void AppFrame::OnNew( wxCommandEvent &event )
{
	// First of stop any running game
	m_timer->Stop();

	// check if there is already a player
	if ( player )
		delete player;

	// create a new player
	player = new Mage( level, &drawArea, 0 );

	// delete the old level and create a new one
	startNewLevel( RANDOM_NEW_LEVEL );

	// add enmey to the level
	level->addEnemies( 3, 5 );

	// set the current level to Level 1
	currentLevel = 1;

	// set the current game state to Game in progress
	gameState = STATE_GAME_IN_PROGRESS;

	// start the game by starting the timer
	m_timer->Start( UPDATE_TIME );

}

void AppFrame::OnLoad( wxCommandEvent &event )
{

}

void AppFrame::OnAbout( wxCommandEvent &event )
{
	wxMessageBox( _T( "The Evil Monkeys game of doom!!!" ), _T( "About Evil Monkey" ) );
}

void AppFrame::OnExit( wxCommandEvent &event )
{
	Close(true);
}


void AppFrame::OnSize( wxSizeEvent &event )
{
	// event handling job
	event.Skip();

	// delete any finalBackBuffer
	if ( finalBackBuffer )
		delete finalBackBuffer;

	// get the client area of our current game window
	wxSize clientArea = gameWindow->GetClientSize();

	// new we can create finalBackBuffer
	finalBackBuffer = new wxBitmap( clientArea.GetWidth(), clientArea.GetHeight() );

	// re-draw everything by calling
	updateView();
}

void AppFrame::OnPaint( wxPaintEvent &event )
{
	// For the game window
	wxPaintDC dc( gameWindow );

	// set the bruch to black
	dc.SetBackground( *wxBLACK_BRUSH );
	// then we need to clean it
	dc.Clear();

	// draw  the bitmap
	//dc.DrawBitmap( *backBuffer, wxPoint( 0, 0 ) );
	updateView();
}

void AppFrame::OnTimer( wxTimerEvent &event )
{
	if ( gameState == STATE_GAME_IN_PROGRESS )
	{
		updateGame();
	}
}

void AppFrame::OnKey( wxKeyEvent &event )
{ 

	/*
	int ch = _getch();

	if ( ch == 0 || ch == 224 )
	{
		switch ( _getch() )
		{
		case 75:
			//cout << "LEFT Key" << endl;
			return move( -1, 0 ); 
			break;
		case 77:
			//cout << "RIGHT Key" << endl;
			return move( 1, 0 );
			break;
		case 72:
			//cout << "UP Key" << endl;
			return move( 0, -1 );
			break;
		case 80:
			//cout << "DOWN Key" << endl;
			return move( 0, 1 );
			break;
		}
	}

	return false;
	*/

	// check if we are in the game in progress state
	if ( gameState == STATE_GAME_IN_PROGRESS )
		level->keyPress( event.GetKeyCode() | 32 ); // we OR it with 32 to make sure it always gives us the lower value

	// update the game view
	updateView();
}


// gameplay functions
bool AppFrame::startNewLevel( LevelType type )
{
	// check if there is already a level
	if ( level )
		delete level;

	// check if there is already a backBuffer
	if ( backBuffer )
		delete backBuffer;

	// Check which type of level we startign here
	switch ( type )
	{
	case RANDOM_NEW_LEVEL:
		{
			// create a new level
			level = new Level( &drawArea, LEVEL_X, LEVEL_Y );
		}
	}

	// We need to create our Mage character and Level
	backBuffer = new wxBitmap( level->getWidth() * GRID_SIZE, level->getHeight() * GRID_SIZE );
	// we should also let the drawArea know about the backBuffer
	drawArea.setWindow( backBuffer, level->getWidth(), level->getHeight() );

	// add the player to the level
	level->addPlayer( player );

	// draw the level
	level->draw();
	
	// we should make the player looks for the correct level
	player->setLevel( level );
	// then we need to update the player
	player->update();

	return true;
}

void AppFrame::updateGame( void )
{
	// first, get all the strings
	wxString info_Lives = "Lives: " + wxString::Format( "%d", player->getLives() );
	wxString info_Level = "Level: " + wxString::Format( "%d", currentLevel );
	wxString info_NumEnemies = "Enemies: " + wxString::Format( "%d", level->numEnemies() );

	// now we have the labels
	stPlayerLives->SetLabel( info_Lives );
	stCurrentLevel->SetLabel( info_Level );
	stNumEnemies->SetLabel( info_NumEnemies );

	// check possible game states
	if ( level->numEnemies() == 0 )
	{
		// stop the timer (current game)
		m_timer->Stop();
		
		// advance t he current level
		currentLevel++;

		// start a new level
		startNewLevel( RANDOM_NEW_LEVEL );

		// calcualte a new number of enemies
		int numEnemies = currentLevel * 4;

		if ( numEnemies > 15 ) 
			numEnemies = 15;

		int newSpeed = pow( 2.0, currentLevel );

		if ( newSpeed > 100 )
			newSpeed = 100;

		// add enemies to level
		level->addEnemies( numEnemies, newSpeed ); 

		// reset player position
		level->setPlayerStart();

		// start the game
		m_timer->Start( UPDATE_TIME );
	}
	else if ( level->numEnemies() && !player->isAlive() )
	{
		gameState = STATE_GAME_OVER;

		m_timer->Stop();
	}
	else
		level->update( wxGetElapsedTime( false ) );

	updateView();
}

void AppFrame::stretchGameView( void )
{
	wxSize clientArea = gameWindow->GetClientSize();
	
	// this will take whatever inside clientArea and calculate width and height to have a stretch properly inside the viewing area
	wxSize stretchedSize;

	// simple proportion
	if ( clientArea.GetWidth() * level->getHeight() / level->getWidth() < clientArea.GetHeight() )
	{
		stretchedSize.Set( clientArea.GetWidth(), clientArea.GetWidth() * level->getHeight() / level->getWidth() );
	}
	else
	{
		stretchedSize.Set( clientArea.GetHeight() * level->getWidth() / level->getHeight(), clientArea.GetHeight() );
	}

	wxImage stretchedImage = backBuffer->ConvertToImage();

	stretchedImage = stretchedImage.Scale( stretchedSize.GetWidth(), stretchedSize.GetHeight() );

	//wxBitmap finalImage( clientArea.GetWidth(), clientArea.GetHeight() );

	wxMemoryDC finalDC;
	wxMemoryDC imageDC;

	finalDC.SelectObject( *finalBackBuffer );
	//imageDC.SelectObject( stretchedImage );
	imageDC.SelectObject( wxBitmap( stretchedImage, 32 ) );

	// first clear the final DC
	finalDC.SetBackground( *wxBLACK_BRUSH );
	finalDC.Clear();

	// we want to make sure that the image is centered
	wxPoint center;

	center.x = ( clientArea.GetWidth() - stretchedSize.GetWidth() ) / 2;
	center.y = ( clientArea.GetHeight() - stretchedSize.GetHeight() ) / 2;

	// Copy the stretched image into the final DC
	finalDC.Blit( center, stretchedSize, &imageDC, wxPoint( 0, 0 ) );

	// draw black border around it
	finalDC.SetBrush( *wxTRANSPARENT_BRUSH );
	finalDC.DrawRectangle( wxPoint( 0, 0 ), clientArea );

	imageDC.SelectObject( wxNullBitmap );
	finalDC.SelectObject( wxNullBitmap );

}

void AppFrame::flipBackBuffer( void )
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxMemoryDC finalDC;

	wxClientDC screenDC( gameWindow );

	finalDC.SelectObject( *finalBackBuffer );

	screenDC.Blit( wxPoint( 0, 0 ), clientArea, &finalDC, wxPoint( 0, 0 ) );

	// once you select an object , deselect
	finalDC.SelectObject( wxNullBitmap );
}

void AppFrame::drawInformation( void )
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxMemoryDC finalDC;

	finalDC.SelectObject( *finalBackBuffer );

	finalDC.Clear();

	finalDC.DrawRectangle( 0, 0, clientArea.GetWidth(), clientArea.GetHeight() );

	wxString message;

	if ( gameState == STATE_NULL )
		message = "Go to file > New to start a new game!";
	else if ( gameState == STATE_PLAYER_WON )
		message = "You have won the game!";
	else if ( gameState == STATE_GAME_OVER )
		message = "GAME OVER!";
	
	int textWidth, textHeight;

	finalDC.GetTextExtent( message, &textWidth, &textHeight );

	wxPoint center( (clientArea.GetWidth() - textWidth) / 2, ( clientArea.GetHeight() - textHeight ) / 2 );

	finalDC.DrawText( message, center );

	finalDC.SelectObject( wxNullBitmap );

}

void AppFrame::updateView( void )
{
	// draw the background, we cannot use wxPaint when it's outside the OnPaint()
	// so we declare wxClientDC
	wxClientDC area( gameWindow );

	// we should make sure that it doesn't update view if there is no backBuffer
	if ( backBuffer && gameState == STATE_GAME_IN_PROGRESS )
		stretchGameView();
	else if ( finalBackBuffer )
		drawInformation();

	flipBackBuffer();
}