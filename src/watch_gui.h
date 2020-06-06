/* $Id: watch_gui.h 17678 2009-10-07 20:54:05  muxy $ */

/** @file watch_gui.h GUI Functions related to watching. */

#ifndef WATCH_GUI_H
#define WATCH_GUI_H

#include "window_gui.h"
#include "company_base.h"

enum WatchCompanyWidgets {
	EWW_CAPTION,
	EWW_WATCH,
	EWW_ZOOMIN,
	EWW_ZOOMOUT,
	EWW_CENTER,
	EWW_NEW_WINDOW,
	EWW_KICK,
	EWW_RESET_KICK,
  EWW_RESET_SPEC,
  EWW_RESET_TIMER5,
  EWW_RESET_TIMER,
  EWW_CANCEL_RESET,
	EWW_EMPTY,
	EWW_JOIN,
	EWW_KICKC,
	EWW_RESET,
	EWW_LOCK,
	EWW_SUSPEND,
	EWW_UNSUSPEND,
	EWW_AWARNING,
	EWW_KNOWN,
	EWW_COMPANYW,
	EWW_PUBLICP_MESSAGE,
	EWW_PUBLICC_MESSAGE,
	EWW_PRIVATEP_MESSAGE,
	EWW_PRIVATEC_MESSAGE,
	EWW_PLAYERS,
	EWW_COMPANIES,
	EWW_NOTE_PLAYER,
	EWW_SHOW_NOTES,
	EWW_PLAYER_ATTENTION,
	EWW_MOVE_PLAYER_TO,
	EWW_ENABLE_SELECT,
	EWW_NICE_COMMANDS,
	EWW_COORDINATES,
	EWW_NEWSTICKET,
	EWW_ENABLED,
};

enum WatchCompanyPlanes {
	EWP_DISABLE = 0,     
	EWP_ENABLE,
};

enum WatchCompanyQuery {
	EWQ_RESET_TIMER,
	EWQ_NOTE_PLAYER,
	EWQ_MOVE_PLAYER_TO,
	EWQ_SHOW_NOTES,
	EWQ_COORDINATES,
	EWQ_NEWSTICKET,
	EWQ_PUBLICP_MESSAGE,
	EWQ_PUBLICC_MESSAGE,
	EWQ_PRIVATEP_MESSAGE,
	EWQ_PRIVATEC_MESSAGE,
};

class WatchCompany : public Window
{
protected:
	void ScrollToTile( TileIndex tile );
	WatchCompanyQuery query_widget;

public:
	bool buttons_enabled;
	CompanyID watched_company;                            // Company ID beeing watched.
	int watched_client;
	char company_name[MAX_LENGTH_COMPANY_NAME_CHARS];     // company name for title display
	char client_name[NETWORK_CLIENT_NAME_LENGTH]; 
	WatchCompany(WindowDesc *desc,/* int window_number,*/ int client_to_watch );

	virtual void OnPaint( );
	//virtual void DrawWidget(const Rect &r, int widget) const;
	virtual void OnClick(Point pt, int widget, int click_count);
	virtual void OnQueryTextFinished(char *str);
	virtual void OnResize( );
	virtual void OnScroll(Point delta);
	virtual void OnMouseWheel(int wheel);
	virtual void OnInvalidateData(int data, bool gui_scope);
	virtual void SetStringParameters(int widget) const;
	virtual void OnMouseOver(Point pt, int widget);
	
	void OnDoCommand( CompanyID company, TileIndex tile );
};

void ShowWatchWindow( int client_to_watch );

#endif // COMPANY_GUI_H 
