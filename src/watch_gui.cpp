// search for "NetworkFindClientInfoFromClientID" e even "const NetworkClientInfo *ci" em network_gui.cpp
// or "NetworkClientInfo::GetByClientID"
/* $Id: watch_gui.cpp  17678 2009-10-07 20:54:05  muxy $ */

/** @file watch_gui.cpp GUI that follow other company building. */

#include "stdafx.h"
#include "watch_gui.h"
#include "watch_gui_1.h"
#include "widget_type.h"
#include "gfx_type.h"
#include "gfx_func.h"
#include "company_func.h"
#include "company_base.h"
#include "company_gui.h"
#include "viewport_func.h"
#include "window_func.h"
#include "strings_func.h"
#include "zoom_func.h"
#include "map_func.h"
#include "command_func.h"
//#include "console_internal.h"
#include "console_func.h"
#include "commands_token_gui.h"
#include "base64.h"
#include "debug.h"

#include "industry.h"
#include "town_map.h"
#include "station_gui.h"
#include "station_base.h"
#include "cargotype.h"
#include "widgets/viewport_widget.h"

#include "network/network.h"
#include "network/network_func.h"
#include "network/network_base.h"
#include "network/network_type.h"

#include "table/sprites.h"
#include "table/strings.h"

#include "textbuf_gui.h"

#include "safeguards.h"

int company_reset;

/**
 * Watch Company Window Widgets Array
 * The Company Button, Has Client Blot and Activity Blot Columns
 * Are made through a function regarding MAX_COMPANIES value
 */

static const NWidgetPart _nested_watch_company_widgets[] = {
	/* Title Bar with close box, title, shade and stick boxes */
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_GREY),
		NWidget(WWT_CAPTION, COLOUR_GREY, EWW_CAPTION), SetDataTip(STR_WATCH_WINDOW_TITLE, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
		NWidget(WWT_SHADEBOX, COLOUR_GREY),
		NWidget(WWT_DEFSIZEBOX, COLOUR_GREY),
		NWidget(WWT_STICKYBOX, COLOUR_GREY),
	EndContainer( ),
	NWidget( NWID_HORIZONTAL ),
		NWidget( NWID_VERTICAL ),
			NWidget(NWID_SELECTION, INVALID_COLOUR, EWW_ENABLE_SELECT),
        //NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_ENABLED ), SetDataTip( SPR_IMG_ROAD_ONE_WAY, STR_XI_ENABLE),
				NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_ENABLED ),SetMinimalSize(5, 18),SetFill(1, 0) , SetDataTip( STR_XI_ENABLE, STR_XI_ENABLE_TOOLTIP ),
				NWidget( NWID_HORIZONTAL ),
          NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_ENABLED ), SetDataTip( SPR_IMG_ROAD_ONE_WAY, STR_XI_ENABLE_TOOLTIP),
          //NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_ENABLED ),SetMinimalSize(5, 18),SetFill(1, 0) , SetDataTip( STR_XI_ENABLE, STR_XI_ENABLE_TOOLTIP ),
          NWidget( NWID_VERTICAL ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_JOIN ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_JOIN, STR_XI_JOIN_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_LOCK ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_LOCK, STR_XI_LOCK_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_KICKC ),SetMinimalSize(50, 18),SetFill(1, 0) ,  SetDataTip( STR_XI_KICKC, STR_XI_KICKC_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_EMPTY ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_EMPTY, STR_XI_EMPTY_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_KICK ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_KICK, STR_XI_KICK_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_NOTE_PLAYER ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_NOTE_PLAYER, STR_XI_NOTE_PLAYER_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_SUSPEND ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_SUSPEND, STR_XI_SUSPEND_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_UNSUSPEND ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_UNSUSPEND, STR_XI_UNSUSPEND_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_PLAYER_ATTENTION ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_PLAYER_ATTENTION, STR_XI_PLAYER_ATTENTION_TOOLTIP ),
            NWidget( WWT_PANEL, COLOUR_GREY ), EndContainer( ),
          EndContainer( ),
          NWidget( NWID_VERTICAL ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_RESET ),SetMinimalSize(50, 18),SetFill(1, 0) ,  SetDataTip( STR_XI_RESET, STR_XI_RESET_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_RESET_SPEC ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_RESET_SPEC, STR_XI_RESET_SPEC_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_RESET_KICK ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_RESET_KICK, STR_XI_RESET_KICK_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_RESET_TIMER5 ),SetMinimalSize(50, 18),SetFill(1, 0) ,  SetDataTip( STR_XI_RESET_TIMER5, STR_XI_RESET_TIMER5_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_RESET_TIMER ),SetMinimalSize(50, 18),SetFill(1, 0) ,  SetDataTip( STR_XI_RESET_TIMER, STR_XI_RESET_TIMER_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_CANCEL_RESET ),SetMinimalSize(50, 18),SetFill(1, 0) ,  SetDataTip( STR_XI_RESET_TIMER_CANCEL, STR_XI_RESET_TIMER_CANCEL_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_MOVE_PLAYER_TO ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_MOVE_PLAYER_TO, STR_XI_MOVE_PLAYER_TO_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_NEWSTICKET ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_PLAYER_NEWSTICKET_BUTTON, STR_XI_PLAYER_NEWSTICKET_BUTTON_TOOLTIP ),
            NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_AWARNING ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_AWARNING, STR_XI_AWARNING_TOOLTIP ),
            NWidget( WWT_PANEL, COLOUR_GREY ), EndContainer( ),
          EndContainer( ),
				EndContainer( ),
			EndContainer(),
      NWidget(NWID_HORIZONTAL),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_PLAYERS ), SetMinimalSize(23, 10), SetDataTip( SPR_IMG_COMPANY_GENERAL, STR_XI_PLAYERS),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_COMPANIES ),  SetMinimalSize(23, 10), SetDataTip( SPR_IMG_BUILDAIR, STR_XI_COMPANIES),
        /*NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_KNOWN ),  SetMinimalSize(23, 10), SetDataTip( SPR_IMG_QUERY, STR_XI_KNOWN_TOOLTIP),*/
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_COMPANYW ),  SetMinimalSize(23, 10), SetDataTip( SPR_IMG_COMPANY_LIST, STR_XI_COMPANYW_TOOLTIP),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_SHOW_NOTES ),  SetMinimalSize(23, 10), SetDataTip( SPR_IMG_STORY_BOOK, STR_XI_SHOW_NOTES_TOOLTIP ),
        NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_PUBLICP_MESSAGE ),  SetMinimalSize(23, 10), SetDataTip( STR_XI_PUBLIC_PLAYER_MESSAGE, STR_XI_PUBLIC_PLAYER_MESSAGE_TOOLTIP),
        NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_PUBLICC_MESSAGE ),  SetMinimalSize(23, 10), SetDataTip( STR_XI_PUBLIC_COMPANY_MESSAGE, STR_XI_PUBLIC_COMPANY_MESSAGE_TOOLTIP),
        NWidget( WWT_PANEL, COLOUR_GREY, EWW_NICE_COMMANDS ), SetMinimalSize(10, 10), SetResize( 0, 1 ), EndContainer( ),
      EndContainer( ),
      /* Button Zoom Out, In, Scrollto */
      NWidget(NWID_HORIZONTAL),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_COORDINATES ),  SetMinimalSize(23, 10), SetDataTip( SPR_IMG_SMALLMAP, STR_XI_SCROLL_TO_COORDS_TOOLTIP),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_ZOOMOUT ), SetMinimalSize(23, 10), SetDataTip( SPR_IMG_ZOOMOUT, STR_TOOLBAR_TOOLTIP_ZOOM_THE_VIEW_OUT),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_ZOOMIN ),  SetMinimalSize(23, 10), SetDataTip( SPR_IMG_ZOOMIN, STR_TOOLBAR_TOOLTIP_ZOOM_THE_VIEW_IN),
        NWidget( WWT_PUSHIMGBTN, COLOUR_GREY, EWW_CENTER ),  SetMinimalSize(23, 10), SetDataTip( SPR_CENTRE_VIEW_VEHICLE, STR_EXTRA_VIEW_MOVE_MAIN_TO_VIEW_TT),
        NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_PRIVATEP_MESSAGE ),  SetMinimalSize(23, 10), SetDataTip( STR_XI_PRIVATE_PLAYER_MESSAGE, STR_XI_PRIVATE_PLAYER_MESSAGE_TOOLTIP),
        NWidget( WWT_PUSHTXTBTN, COLOUR_GREY, EWW_PRIVATEC_MESSAGE ),  SetMinimalSize(23, 10), SetDataTip( STR_XI_PRIVATE_COMPANY_MESSAGE, STR_XI_PRIVATE_COMPANY_MESSAGE_TOOLTIP),
        NWidget( WWT_PANEL, COLOUR_GREY, EWW_NEW_WINDOW ),   SetMinimalSize(10, 10), SetDataTip( 0, 0 ), EndContainer( ),
      EndContainer( ),
			/* Background panel for resize purpose */
			NWidget( WWT_PANEL, COLOUR_GREY ), SetResize( 0, 1 ), EndContainer( ),
		EndContainer( ),
		/* Watch Pannel */
		NWidget(WWT_PANEL, COLOUR_GREY),
			NWidget(NWID_VIEWPORT, INVALID_COLOUR, EWW_WATCH), SetPadding(2, 2, 2, 2), SetResize(1, 1), SetFill(1, 1),
		EndContainer( ),
	EndContainer( ),
	/* Status Bar with resize buton */
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_PANEL, COLOUR_GREY), SetFill(1, 1), SetResize(1, 0), EndContainer(),
		NWidget(WWT_RESIZEBOX, COLOUR_GREY),
	EndContainer( ),
};

/**
 * Watch Company Window Descriptor
 */
static WindowDesc _watch_company_desc(
	WDP_AUTO, "watch_company_player", 450, 220,
	WC_WATCH_COMPANY, WC_NONE,
	0,
	_nested_watch_company_widgets, lengthof( _nested_watch_company_widgets )
);

/** Watch Company Class Constructor
 * @param desc Window Descriptor The Window Descriptor
 * @param window_number The window number for the class
 * @param company_to_watch Company ID for watching a particular company
 */
WatchCompany::WatchCompany(WindowDesc *desc, int window_number) : Window(desc)
{
	this->watched_client = window_number;
	NetworkClientInfo *ci = NetworkClientInfo::GetByClientID((ClientID)this->watched_client);
	if (ci) {
		this->watched_company = ci->client_playas;
	}
	else{
		this->watched_company = INVALID_COMPANY;
	}
	this->owner = this->watched_company;//!!
	this->buttons_enabled = false;
	this->InitNested(window_number);

	/* Init the viewport area */
	NWidgetViewport *nvp = this->GetWidget<NWidgetViewport>(EWW_WATCH);
	nvp->InitializeViewport(this, 0, ZOOM_LVL_OUT_8X);

	Point pt;
	/* the main window with the main view */
	const Window *w = FindWindowById(WC_MAIN_WINDOW, 0);

	/* center on same place as main window (zoom is maximum, no adjustment needed) */
	pt.x = w->viewport->scrollpos_x + w->viewport->virtual_width / 2;
	pt.y = w->viewport->scrollpos_y + w->viewport->virtual_height / 2;

	this->viewport->scrollpos_x = pt.x - this->viewport->virtual_width / 2;
	this->viewport->scrollpos_y = pt.y - this->viewport->virtual_height / 2;
	this->viewport->dest_scrollpos_x = this->viewport->scrollpos_x;
	this->viewport->dest_scrollpos_y = this->viewport->scrollpos_y;

	if ( this->watched_company != INVALID_COMPANY ) {
		Company *c = Company::Get( this->watched_company );
		this->ScrollToTile( c->last_build_coordinate );
	}

	this->InvalidateData( );
}
void WatchCompany::SetStringParameters(int widget) const
{
	switch (widget) {
		case EWW_CAPTION:
			if (!Company::IsValidHumanID(this->watched_company)){
				//this->watched_company == INVALID_COMPANY) {
				GetString( (char *)this->company_name, STR_JUST_NOTHING, lastof(this->company_name) );
			} else {
				const Company *c = Company::Get( this->watched_company );
				SetDParam( 0, c->index );
				GetString( (char *)this->company_name, STR_COMPANY_NAME, lastof(this->company_name) );
			}
			NetworkClientInfo *ci = NetworkClientInfo::GetByClientID((ClientID)this->watched_client);
			if(ci){
				strecpy((char *)this->client_name,ci->client_name,lastof(this->client_name));
			}
			else{
				GetString( (char *)this->client_name, STR_JUST_NOTHING, lastof(this->client_name) );
			}
			SetDParamStr( 0, this->client_name );
			SetDParamStr( 1, this->company_name );
			if (Company::IsValidHumanID(this->watched_company)){
				SetDParam( 2, this->watched_company + 1 );
			}
			else {
				SetDParam( 2, 255 );
			}
			break;
	}
}
void WatchCompany::OnPaint()
{
	if (!this->IsShaded()) {
		int plane = this->buttons_enabled ? EWP_ENABLE : EWP_DISABLE;
		NWidgetStacked *wi = this->GetWidget<NWidgetStacked>(EWW_ENABLE_SELECT);
		if (plane != wi->shown_plane) {
			wi->SetDisplayedPlane(plane);
			this->SetDirty();
			return;
		}
	}
	this->DrawWidgets();
}

void WatchCompany::OnResize()
{
	if (this->viewport != NULL) {
		NWidgetViewport *nvp = this->GetWidget<NWidgetViewport>(EWW_WATCH);
		nvp->UpdateViewportCoordinates(this);
	}
}

void WatchCompany::OnScroll(Point delta)
{
	const Viewport *vp = IsPtInWindowViewport(this, _cursor.pos.x, _cursor.pos.y);
	if (vp == NULL) return;

	this->viewport->scrollpos_x += ScaleByZoom(delta.x, vp->zoom);
	this->viewport->scrollpos_y += ScaleByZoom(delta.y, vp->zoom);
	this->viewport->dest_scrollpos_x = this->viewport->scrollpos_x;
	this->viewport->dest_scrollpos_y = this->viewport->scrollpos_y;
}

void WatchCompany::OnMouseWheel( int wheel )
{
	ZoomInOrOutToCursorWindow(wheel < 0, this);
}

static void WWCompanyResetCallback(Window *w, bool confirmed)
{
  if (confirmed) {
    char msg[128];
    seprintf(msg, lastof(msg), "!resetcompany %i",company_reset);
		NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
  }
}

static void WWCompanyResetSpecCallback(Window *w, bool confirmed)
{
	if (confirmed) {
		char msg[128];
    seprintf(msg, lastof(msg), "!resetcompanyspec %i",company_reset);
		NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
	}
}

static void WWCompanyResetKickCallback(Window *w, bool confirmed)
{
	if (confirmed) {
		char msg[128];
    seprintf(msg, lastof(msg), "!resetcompanykick %i",company_reset);
		NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
	}
}

void WatchCompany::OnClick(Point pt, int widget, int click_count)
{
	char msg[512];
	switch (widget) {
		case EWW_ZOOMOUT: DoZoomInOutWindow(ZOOM_OUT, this); break;
		case EWW_ZOOMIN: DoZoomInOutWindow(ZOOM_IN,  this); break;

		case EWW_CENTER: { // location button (move main view to same spot as this view) 'Center Main View'
			Window *w = FindWindowById(WC_MAIN_WINDOW, 0);
			int x = this->viewport->scrollpos_x; // Where is the watch looking at
			int y = this->viewport->scrollpos_y;

			/* set the main view to same location. Based on the center, adjusting for zoom */
			w->viewport->dest_scrollpos_x =  x - (w->viewport->virtual_width -  this->viewport->virtual_width) / 2;
			w->viewport->dest_scrollpos_y =  y - (w->viewport->virtual_height - this->viewport->virtual_height) / 2;
		} break;

		case EWW_NEW_WINDOW:
			ShowWatchWindow1(this->watched_company);
			break;

    case EWW_NICE_COMMANDS:
			ShowCCCommands();
			break;

    case EWW_RESET:
			company_reset = this->watched_company+1;
      ShowQuery(STR_XI_RESET_COMP, STR_XI_RESET_COMP_SURE, this, WWCompanyResetCallback);
      MarkWholeScreenDirty();
			break;
		case EWW_RESET_SPEC:
			company_reset = this->watched_company+1;
      ShowQuery(STR_XI_RESET_COMP, STR_XI_RESET_SPEC_SURE, this, WWCompanyResetSpecCallback);
      MarkWholeScreenDirty();
			break;
		case EWW_RESET_KICK:
			company_reset = this->watched_company+1;
      ShowQuery(STR_XI_RESET_COMP, STR_XI_RESET_KICK_SURE, this, WWCompanyResetKickCallback);
      MarkWholeScreenDirty();
			break;
		case EWW_EMPTY:
			seprintf(msg, lastof(msg), "!emptycompany %i",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_KICK:
			seprintf(msg, lastof(msg), "!kick #%i",this->watched_client);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_KICKC:
			seprintf(msg, lastof(msg), "!move #%i %i",this->watched_client,255);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_JOIN:
			seprintf(msg, lastof(msg), "!move #%i %i",_network_own_client_id,this->watched_company+1); //this->owner
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_RESET_TIMER:
      this->query_widget = EWQ_RESET_TIMER;
			SetDParam(0, this->window_number);
      //ShowQueryString(STR_EMPTY, STR_XI_RESET_TIMER_VALUE, 25, 100, this, CS_NUMERAL, QSF_NONE);  //OLD
      ShowQueryString(STR_EMPTY, STR_XI_RESET_TIMER_VALUE, 25, this, CS_NUMERAL, QSF_NONE);
			break;
		case EWW_RESET_TIMER5:
			seprintf(msg, lastof(msg), "!resetcompanytimer %i 300",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_LOCK:
			seprintf(msg, lastof(msg), "!lockcompany %i",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_SUSPEND:
			seprintf(msg, lastof(msg), "!suspend %i",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_UNSUSPEND:
			seprintf(msg, lastof(msg), "!unsuspend %i",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_AWARNING:
			seprintf(msg, lastof(msg), "!awarning %i",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_KNOWN:
			seprintf(msg, lastof(msg), "!known %i",this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_PLAYERS:
			seprintf(msg, lastof(msg), "!players");
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_COMPANIES:
			seprintf(msg, lastof(msg), "!companies");
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_COMPANYW:
         ShowCompany(this->watched_company);
			break;
		case EWW_PUBLICP_MESSAGE:
      this->query_widget = EWQ_PUBLICP_MESSAGE;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_PUBLIC_PLAYER_MESSAGE_TITLE, 512, this, CS_ALPHANUMERAL, QSF_NONE);
			break;
		case EWW_PUBLICC_MESSAGE:
      this->query_widget = EWQ_PUBLICC_MESSAGE;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_PUBLIC_COMPANY_MESSAGE_TITLE, 512, this, CS_ALPHANUMERAL, QSF_NONE);
			break;

    case EWW_PRIVATEP_MESSAGE:
      this->query_widget = EWQ_PRIVATEP_MESSAGE;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_PRIVATE_PLAYER_MESSAGE_TITLE, 512, this, CS_ALPHANUMERAL, QSF_NONE);
			break;
		case EWW_PRIVATEC_MESSAGE:
      this->query_widget = EWQ_PRIVATEC_MESSAGE;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_PRIVATE_COMPANY_MESSAGE_TITLE, 512, this, CS_ALPHANUMERAL, QSF_NONE);
			break;

		case EWW_NOTE_PLAYER:
      this->query_widget = EWQ_NOTE_PLAYER;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_NOTE_PLAYER_TITLE, 128, this, CS_ALPHANUMERAL, QSF_NONE);
			break;
		case EWW_SHOW_NOTES:
      this->query_widget = EWQ_SHOW_NOTES;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_SHOW_NOTES_TITLE, 64, this, CS_ALPHANUMERAL, QSF_NONE);
			break;
		case EWW_CANCEL_RESET:
			seprintf(msg, lastof(msg), "!cancelresetcompany %i", this->watched_company+1);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWW_PLAYER_ATTENTION:
      char buffer[128];
      if ((this->watched_company) != 255) {
        GetString(buffer, STR_COLOUR_DARK_BLUE + _company_colours[this->watched_company], lastof(buffer));
        seprintf(msg, lastof(msg), "%s (%s), %s", this->client_name, buffer, _settings_client.network.nice_attention);
      } else {
        seprintf(msg, lastof(msg), "%s (Spectator), %s", this->client_name, _settings_client.network.nice_attention);
      }
			NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , msg);
			break;
		case EWW_MOVE_PLAYER_TO:
      this->query_widget = EWQ_MOVE_PLAYER_TO;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_MOVE_PLAYER_TO_TITLE, 25, this, CS_NUMERAL, QSF_NONE);
			break;
		case EWW_NEWSTICKET:
      this->query_widget = EWQ_NEWSTICKET;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_PLAYER_NEWSTICKET, 250, this, CS_ALPHANUMERAL, QSF_NONE);
			break;
		case EWW_COORDINATES:
      this->query_widget = EWQ_COORDINATES;
			SetDParam(0, this->window_number);
      ShowQueryString(STR_EMPTY, STR_XI_SCROLL_TO_COORDS_TITLE, 25, this, CS_ALPHANUMERAL, QSF_NONE);
			break;
		case EWW_ENABLED:
			this->buttons_enabled = !this->buttons_enabled;
			break;
	}
}

void WatchCompany::OnQueryTextFinished(char *str)
{
	if (str == NULL) return;
	char msg[512];
	char buffer[128];
	char buffer2[128];
	int comp_player;
	switch (this->query_widget) {
		default: NOT_REACHED();

		case EWQ_RESET_TIMER:
			seprintf(msg, lastof(msg), "!resetcompanytimer %i %s", this->watched_company+1, str);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWQ_NOTE_PLAYER:
			seprintf(msg, lastof(msg), "!note %s %s", this->client_name, str);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWQ_SHOW_NOTES:
			seprintf(msg, lastof(msg), "!notes %s", str);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWQ_MOVE_PLAYER_TO:
			seprintf(msg, lastof(msg), "!move #%i %s", this->watched_client, str);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWQ_NEWSTICKET:
      if ((this->watched_company) != 255) {
        GetString(buffer, STR_COLOUR_DARK_BLUE + _company_colours[this->watched_company], lastof(buffer));
        SetDParam(0, this->watched_company);
        GetString(buffer2, STR_COMPANY_NAME, lastof(buffer2));
        seprintf(msg, lastof(msg), "!news %s (%s - %s), %s", this->client_name, buffer2, buffer, str);
      } else {
        seprintf(msg, lastof(msg), "!news %s (Spectator), %s", this->client_name, str);
      }
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
			break;
		case EWQ_COORDINATES:
			seprintf(msg, lastof(msg), "scrollto %s", str);
			IConsoleCmdExec(msg);
			break;
		case EWQ_PUBLICP_MESSAGE:
      if ((this->watched_company) != 255) {
        GetString(buffer, STR_COLOUR_DARK_BLUE + _company_colours[this->watched_company], lastof(buffer));
        SetDParam(0, this->watched_company);
        GetString(buffer2, STR_COMPANY_NAME, lastof(buffer2));
        seprintf(msg, lastof(msg), "%s (%s - %s), %s", this->client_name, buffer2, buffer, str);
      } else {
        seprintf(msg, lastof(msg), "%s (Spectator), %s", this->client_name, str);
      }
			NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , msg);
			break;
		case EWQ_PUBLICC_MESSAGE:
      if ((this->watched_company) != 255) {
        GetString(buffer, STR_COLOUR_DARK_BLUE + _company_colours[this->watched_company], lastof(buffer));
        SetDParam(0, this->watched_company);
        GetString(buffer2, STR_COMPANY_NAME, lastof(buffer2));
        seprintf(msg, lastof(msg), "%s (%s), %s", buffer2, buffer, str);
      } else {
        seprintf(msg, lastof(msg), "%s", str);
      }
			NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , msg);
			break;
		case EWQ_PRIVATEP_MESSAGE:
      if ((this->watched_company) != 255) {
        comp_player = this->watched_client;
        GetString(buffer, STR_COLOUR_DARK_BLUE + _company_colours[this->watched_company], lastof(buffer));
        seprintf(msg, lastof(msg), "(%s): %s", buffer, str);
      } else {
        seprintf(msg, lastof(msg), "%s", str);
      }
      DEBUG(net, 0, "player: %i", comp_player);
			DEBUG(net, 0, "msg: %s", msg);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, comp_player , msg);
			break;
		case EWQ_PRIVATEC_MESSAGE:
      if ((this->watched_company) != 255) {
        comp_player = this->watched_company;
        GetString(buffer, STR_COLOUR_DARK_BLUE + _company_colours[this->watched_company], lastof(buffer));
        seprintf(msg, lastof(msg), "(%s Team): %s", buffer, str);
      } else {
        seprintf(msg, lastof(msg), "%s", str);
      }
			NetworkClientSendChat(NETWORK_ACTION_CHAT_COMPANY, DESTTYPE_TEAM, comp_player , msg);
			DEBUG(net, 0, "company: %i", comp_player);
			DEBUG(net, 0, "msg: %s", msg);
			break;
	}
}

void WatchCompany::OnMouseOver(Point pt, int widget)
	{
		/* Show tooltip with last month production or town name */
		if (pt.x != -1) {
			TileIndex tile;
			const bool viewport_is_in_map_mode = (this->viewport->zoom > ZOOM_LVL_MAX);
			//const bool viewport_is_in_map_mode = (this->viewport->zoom >= ZOOM_LVL_MAX);
			//const bool viewport_is_in_map_mode = (this->viewport->zoom >= ZOOM_LVL_DRAW_MAP);
			if (viewport_is_in_map_mode) {
        // Disabled, for now :)
				/*NWidgetViewport *nvp = this->GetWidget<NWidgetViewport>(WID_EV_VIEWPORT);
				const int a = ((ScaleByZoom(pt.x - nvp->pos_x, this->viewport->zoom) + this->viewport->virtual_left) >> 2) / ZOOM_LVL_BASE;
				const int b = ((ScaleByZoom(pt.y - nvp->pos_y, this->viewport->zoom) + this->viewport->virtual_top) >> 1) / ZOOM_LVL_BASE;
				tile = TileVirtXY(b - a, b + a);*/
			} else {
				const Point p = GetTileBelowCursor();
				tile = TileVirtXY(p.x, p.y);
			}
			if (tile >= MapSize()) return;

			switch (GetTileType(tile)) {
				case MP_ROAD:
					if (IsRoadDepot(tile)) return;
					/* FALL THROUGH */
				case MP_HOUSE: {
					if (HasBit(_display_opt, DO_SHOW_TOWN_NAMES)) return; // No need for a town name tooltip when it is already displayed
					if (!viewport_is_in_map_mode) return;
					const TownID tid = GetTownIndex(tile);
					if (!tid) return;
					SetDParam(0, tid);
					GuiShowTooltips(this, STR_TOWN_NAME_TOOLTIP, 0, NULL, TCC_HOVER);
					break;
				}
				case MP_INDUSTRY: {
					const Industry *ind = Industry::GetByTile(tile);
					const IndustrySpec *indsp = GetIndustrySpec(ind->type);

					StringID str = STR_INDUSTRY_VIEW_TRANSPORTED_TOOLTIP;
					uint prm_count = 0;
					SetDParam(prm_count++, indsp->name);
					for (byte i = 0; i < lengthof(ind->produced_cargo); i++) {
						if (ind->produced_cargo[i] != CT_INVALID) {
							SetDParam(prm_count++, ind->produced_cargo[i]);
							SetDParam(prm_count++, ind->last_month_production[i]);
							SetDParam(prm_count++, ToPercent8(ind->last_month_pct_transported[i]));
							str++;
						}
					}
					if (prm_count <= 19) {
            GuiShowTooltips(this, str, 0, NULL, TCC_HOVER);
          }
					break;
				}
				case MP_STATION: {
          if (!IsRailWaypoint(tile)) {
            if (!HasTileWaterGround(tile)) {
              const Station *st = Station::GetByTile(tile);
              StringID str = STR_STATION_VIEW_TRANSPORTED_TOOLTIP;
              uint prm_count = 0;
              SetDParam(prm_count++, st->index);
              for (int i = 0; i < _sorted_standard_cargo_specs_size; i++) {
                const CargoSpec *cs = _sorted_cargo_specs[i];
                //const CargoSpec *cs = CargoSpec::Get(i);
                if(cs == NULL) continue;
                int cargoid = cs->Index();
                //if (HasBit(st->goods[i].status,GoodsEntry::GES_RATING)) {
                if (HasBit(st->goods[cargoid].status, GoodsEntry::GES_RATING)) {
                  SetDParam(prm_count++, cs->Index());
                  SetDParam(prm_count++, st->goods[cargoid].cargo.TotalCount());
                  SetDParam(prm_count++, ToPercent8(st->goods[cargoid].rating));
                  str++;
                  if (prm_count == 19) {
                    str++;
                    i = _sorted_standard_cargo_specs_size;
                  }
                }
              }
              if (prm_count <= 19) {
                GuiShowTooltips(this, str, 0, NULL, TCC_HOVER);
              }
            }
          }
					break;
				}
				default:
					return;
			}
		}
	}

void WatchCompany::OnInvalidateData(int data, bool gui_scope)
{
  //if (!gui_scope) return;
	if (data == 2) {
		delete this;
		return;
	}
	NetworkClientInfo *ci = NetworkClientInfo::GetByClientID((ClientID)this->watched_client);
	if(!ci) {
		delete this;
		return;
	}
	else {
		this->watched_company = ci->client_playas;
		this->owner = this->watched_company;
		if (ci->client_playas == 255) {
			this->SetWidgetDisabledState(EWW_RESET_KICK,true);
			this->SetWidgetDisabledState(EWW_RESET_SPEC,true);
			this->SetWidgetDisabledState(EWW_EMPTY,true);
			this->SetWidgetDisabledState(EWW_JOIN,true);
			this->SetWidgetDisabledState(EWW_KICKC,true);
			this->SetWidgetDisabledState(EWW_RESET,true);
			this->SetWidgetDisabledState(EWW_LOCK,true);
			this->SetWidgetDisabledState(EWW_SUSPEND,true);
			this->SetWidgetDisabledState(EWW_UNSUSPEND,true);
			this->SetWidgetDisabledState(EWW_AWARNING,true);
			this->SetWidgetDisabledState(EWW_KNOWN,true);
			this->SetWidgetDisabledState(EWW_COMPANYW,true);
			this->SetWidgetDisabledState(EWW_RESET_TIMER,true);
			this->SetWidgetDisabledState(EWW_RESET_TIMER5,true);
			this->SetWidgetDisabledState(EWW_CANCEL_RESET,true);
		}
		else {
			this->SetWidgetDisabledState(EWW_RESET_KICK,false);
			this->SetWidgetDisabledState(EWW_RESET_SPEC,false);
			this->SetWidgetDisabledState(EWW_EMPTY,false);
			this->SetWidgetDisabledState(EWW_JOIN,false);
			this->SetWidgetDisabledState(EWW_KICKC,false);
			this->SetWidgetDisabledState(EWW_RESET,false);
			this->SetWidgetDisabledState(EWW_LOCK,false);
			this->SetWidgetDisabledState(EWW_SUSPEND,false);
			this->SetWidgetDisabledState(EWW_UNSUSPEND,false);
			this->SetWidgetDisabledState(EWW_AWARNING,false);
			this->SetWidgetDisabledState(EWW_KNOWN,false);
			this->SetWidgetDisabledState(EWW_COMPANYW,false);
			this->SetWidgetDisabledState(EWW_RESET_TIMER,false);
			this->SetWidgetDisabledState(EWW_RESET_TIMER5,false);
			this->SetWidgetDisabledState(EWW_CANCEL_RESET,false);
		}
	}
	HandleZoomMessage(this, this->viewport, EWW_ZOOMIN, EWW_ZOOMOUT);
}


/** The Eart of the watch company window - Called by DoCommand
 *  @param company The company ID who's client is building
 *  @param tile The tile number where action took place
 */
void WatchCompany::ScrollToTile( TileIndex tile )
{
	/* Scroll window to the tile, only if not zero */
	if (tile != 0) {
		ScrollWindowTo( TileX(tile) * TILE_SIZE + TILE_SIZE / 2, TileY(tile) * TILE_SIZE + TILE_SIZE / 2, -1, this );
	}
}

void WatchCompany::OnDoCommand( CompanyID company, TileIndex tile )
{
	/* Check if its my company */
	if (this->watched_company == company)
	{
		/* Scroll window to the tile, only if not zero */
		if (tile != 0) {
			ScrollWindowTo( TileX(tile) * TILE_SIZE + TILE_SIZE / 2, TileY(tile) * TILE_SIZE + TILE_SIZE / 2, -1, this );
		}
	}
	/* set the company_activity to its max in order to pain the BLOT in read
	 * This will result by having the activity blot set to red for all companies
	 * even the one watched. To avoid this behaviour and not to light the blot of
	 * the watched company, the code can be moved just after the ScrollWindowTo call.
	 */
	if (tile != 0) {
		this->SetDirty();
	}
}

void ShowWatchWindow( int company_to_watch = INVALID_CLIENT_ID )
{
  AllocateWindowDescFront<WatchCompany>(&_watch_company_desc, company_to_watch);
}
