/* $Id: watch_gui.h 17678 2009-10-07 20:54:05  muxy $ */

/** @file watch_gui.h GUI Functions related to watching. */

#ifndef WATCH_GUI_1_H
#define WATCH_GUI_1_H

#include "window_gui.h"
#include "company_base.h"

#define MAX_ACTIVITY 30

enum WatchCompany1Widgets {
	EWW_CAPTION1,
	EWW_PB_COMPANY_NUMBER_FIRST1,
	EWW_PB_COMPANY_NUMBER_LAST1  = EWW_PB_COMPANY_NUMBER_FIRST1 + MAX_COMPANIES - 1,
	EWW_PB_COMPANY_FIRST1,
	EWW_PB_COMPANY_LAST1  = EWW_PB_COMPANY_FIRST1 + MAX_COMPANIES - 1,
	EWW_HAS_CLIENT_FIRST1,
	EWW_HAS_CLIENT_LAST1 = EWW_HAS_CLIENT_FIRST1 + MAX_COMPANIES - 1,
	EWW_ACTIVITY_FIRST1,
	EWW_ACTIVITY_LAST1 = EWW_ACTIVITY_FIRST1 + MAX_COMPANIES - 1,
	EWW_PB_ACTION1_FIRST1,
	EWW_PB_ACTION1_LAST1 = EWW_PB_ACTION1_FIRST1 + MAX_COMPANIES - 1,
	EWW_WATCH1,
	EWW_ZOOMIN1,
	EWW_ZOOMOUT1,
	EWW_CENTER1,
	EWW_NEW_WINDOW1
};

class WatchCompany1 : public Window
{

protected:

	CompanyID watched_company;                            // Company ID beeing watched.
	CompanyID watched_company_window;                            // Company ID to open Company window.
	int company_activity[MAX_COMPANIES];                  // int array for activity blot.
	int company_count_client[MAX_COMPANIES];              // company client count.
	char company_name[MAX_LENGTH_COMPANY_NAME_CHARS];     // company name for title display

	void SetWatchWindowTitle( );
	void ScrollToTile( TileIndex tile );


public:

	WatchCompany1(WindowDesc *desc, int window_number, CompanyID company_to_watch );

	virtual void SetStringParameters(int widget) const;
	//virtual void OnPaint( );
	virtual void DrawWidget(const Rect &r, int widget) const;
	virtual void OnClick(Point pt, int widget, int click_count);
	virtual void OnResize( );
	virtual void OnScroll(Point delta);
	virtual void OnMouseWheel(int wheel);
	virtual void OnInvalidateData(int data, bool gui_scope );
	virtual void OnTick( );
	virtual void OnMouseOver(Point pt, int widget);
	
	void OnDoCommand( CompanyID company, TileIndex tile );
};

void ShowWatchWindow1( CompanyID company_to_watch );

#endif // WATCH_GUI_1_H
