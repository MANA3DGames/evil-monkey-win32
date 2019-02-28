#ifndef APPFRAME_H
#define APPFRAME_H

#include "stdwx.h"

#include <math.h>

#include "drawEngine.h"
#include "level.h"
#include "mage.h"

enum ID_List
{
	ID_New = 400,
	ID_Load,
	ID_About,
	ID_Exit,

	ID_Timer
};


enum LevelType
{
	RANDOM_NEW_LEVEL = 0
};

enum GameState
{
	STATE_NULL = 0,
	STATE_GAME_IN_PROGRESS,
	STATE_GAME_OVER,
	STATE_PLAYER_WON
};


class AppFrame : public wxFrame
{
public:
	AppFrame( const wxString& title, const wxPoint& pos, const wxSize& size );
	~AppFrame();

	void OnNew( wxCommandEvent &event );
	void OnLoad( wxCommandEvent &event );
	void OnAbout( wxCommandEvent &event );
	void OnExit( wxCommandEvent &event );
	
	void OnSize( wxSizeEvent &event );
	void OnPaint( wxPaintEvent &event );

	void OnTimer( wxTimerEvent &event );

	void OnKey( wxKeyEvent &event );

private:
	bool startNewLevel( LevelType type );
	void updateView( void );
	void updateGame( void );

	void drawInformation( void );
	void stretchGameView( void );
	void flipBackBuffer( void );

private:
	// Controls
	wxPanel *m_panel;
	wxPanel *m_info_panel;

	wxWindow *gameWindow;

	wxStaticText *stPlayerLives;
	wxStaticText *stCurrentLevel;
	wxStaticText *stNumEnemies;

	wxTimer *m_timer;

	// Game variables
	Level *level;
	Mage *player;

	DrawEngine drawArea;

	wxBitmap *backBuffer;	// prevent flickering, also we don't need to re-draw everything
	wxBitmap *finalBackBuffer;	// this will be created in the OnSize event

	int currentLevel;

	GameState gameState;

private:
	DECLARE_EVENT_TABLE()
};

#endif