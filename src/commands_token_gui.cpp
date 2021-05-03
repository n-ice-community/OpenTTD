#include "stdafx.h"
#include "widget_type.h"
#include "gfx_type.h"
#include "gfx_func.h"
#include "window_func.h"
#include "strings_func.h"
#include "commands_token_gui.h"
#include "network/network.h"
#include "network/network_func.h"
#include "network/network_base.h"
#include "network/network_type.h"
#include "table/strings.h"
#include "textbuf_gui.h"
#include "debug.h"
#include "error.h"
#include "gui.h"
#include "console_func.h" //IConsolePrintF
#include "base64.h"
#include <iostream>

#include "safeguards.h"

using namespace std;
std::string _server_list_text_names1;

enum CCCommandsWidgets {
	NIC_CAPTION,
	NIC_RULES1,
	NIC_RULES2,
	NIC_RULES3,
	NIC_RULES4,
	NIC_RULES5,
	NIC_RULES6,
	NIC_RULES7,
	NIC_RULES8,
	NIC_RULES9,
	NIC_RULES10,
	NIC_RULES11,
	NIC_RULES12,
	NIC_RULES13,
	NIC_RULES_ALL,
	NIC_RULES_WEBSITE,
	NIC_RULES_WEBSITE_DETAILED,
	NIC_LINKS_CC,
	NIC_LINKS_CC_TTD,
	NIC_LINKS_TTDWIKI,
	NIC_LINKS_CCWIKI,
	NIC_LINKS_TUTORIAL,
	NIC_LINKS_MECHANICS,
	NIC_LINKS_SIGNALS,
	NIC_LINKS_GAME_LOGIN,
	NIC_VARIOUS_SERVER,
	NIC_VARIOUS_INFO,
	NIC_VARIOUS_VOTE_RESTART,
	NIC_VARIOUS_VOTE_RESTART_MAP,
	NIC_VARIOUS_PLAYERS,
	NIC_VARIOUS_COMPANIES,
	NIC_VARIOUS_VOTE,
	NIC_VARIOUS_VOTE_STATUS,
	NIC_VARIOUS_GOODBYE,
	NIC_OTHER_ADMIN_ACK,
	NIC_OTHER_ADMIN_SOLVE,
	NIC_OTHER_TOKEN_LOGIN,
	NIC_OTHER_TOKEN_LOGOUT,
	NIC_OTHER_COORDINATES,
	NIC_OTHER_ADMIN_LOGIN,
	NIC_OTHER_ADMIN_LOGOUT,
	NIC_OTHER_NEWSTICKET,
	NIC_OTHER_SERVER,
	NIC_NICE_USER,
	NIC_BTPRO_USER,
	NIC_SERVERS,
	NIC_SERVERS1,
	NIC_SERVERS2,
	NIC_SERVERS3,
	NIC_SERVERS4,
	NIC_SERVERS5,
	NIC_SERVERS6,
	NIC_SERVERS7,
	NIC_SERVERS8,
	NIC_SERVERS9,
	NIC_SERVERS10,
	NIC_SERVERS11,
	NIC_SERVERS12,
	NIC_SERVERS13,
	NIC_SERVERS14,
	NIC_SERVERS15,
	NIC_SERVERS16,
	NIC_SERVERS17,
	NIC_SERVERS18,
	NIC_SERVERS19,
	NIC_SERVERS20,
	NIC_SERVERS21,
	NIC_SERVERS22,
	NIC_SERVERS23,
	NIC_SERVERS24,
	NIC_SERVERS25,
	NIC_SERVERS26,
	NIC_SERVERS27,
	NIC_SERVERS28,
	NIC_SERVERS29,
	NIC_SERVERS30
};

enum CCCommandsQuery {
	NIQ_COORDINATES,
	NIQ_NEWSTICKET,
};

enum CCCommandsHotkeys {
//	MTHK_SCLAUNCH,
};

static const NWidgetPart _nested_CC_commands_widgets[] = {
	/* Title Bar with close box, title, shade and stick boxes */
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_GREY),
		NWidget(WWT_CAPTION, COLOUR_GREY), SetDataTip(STR_CC_GLOBAL_COMMANDS_TITLE, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
		NWidget(WWT_SHADEBOX, COLOUR_GREY),
		NWidget(WWT_DEFSIZEBOX, COLOUR_GREY),
		NWidget(WWT_STICKYBOX, COLOUR_GREY),
	EndContainer( ),
	NWidget( NWID_VERTICAL ),
    NWidget(NWID_SPACER), SetMinimalSize(2, 0), SetFill(0, 1),
    NWidget(WWT_PANEL, COLOUR_GREY), SetFill(0, 1),
      NWidget( NWID_HORIZONTAL ),
        NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_GLOBAL_RULES, STR_NULL), SetMinimalSize(70, 16),
        NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 12), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_SERVER_TEXT, STR_NULL), SetMinimalSize(20, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_SPACES, STR_NULL), SetMinimalSize(4, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN, NIC_OTHER_SERVER), SetDataTip(STR_CC_SERVER_TEXT_SERVER, STR_NULL), SetMinimalSize(40, 16),
      EndContainer( ), 
      NWidget( NWID_HORIZONTAL ), SetPIP(4, 1, 4),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES1 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES2 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES3 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES4 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES5 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES6 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES7 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES8 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES9 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES10 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES11 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES12 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES13 ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_STRING, STR_CC_GLOBAL_RULES_STRING_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES_ALL ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_ALL, STR_CC_GLOBAL_RULES_ALL ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES_WEBSITE ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_WEBSITE, STR_CC_GLOBAL_RULES_WEBSITE_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_RULES_WEBSITE_DETAILED ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_RULES_WEBSITE_DETAILED, STR_CC_GLOBAL_RULES_WEBSITE_DETAILED_TOOLTIP ),
        EndContainer( ),
      EndContainer( ),
      NWidget(NWID_SPACER), SetMinimalSize(0, 4), SetFill(0, 1),
    EndContainer( ),
    NWidget(WWT_PANEL, COLOUR_GREY), SetFill(0, 1),
      NWidget( NWID_HORIZONTAL ),
        NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_CC_WHITE, STR_NULL), SetMinimalSize(70, 16),
      EndContainer( ), 
      NWidget( NWID_HORIZONTAL, NC_EQUALSIZE ), SetFill(1, 0), SetPIP(4, 1, 4),
        NWidgetFunction(MakeServerButtons1),
      EndContainer( ),
      NWidget(NWID_SPACER), SetMinimalSize(0, 4), SetFill(0, 1),
    EndContainer( ),
    NWidget(WWT_PANEL, COLOUR_GREY), SetFill(0, 1),
      NWidget( NWID_HORIZONTAL ),
        NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_GLOBAL_LINKS, STR_NULL), SetMinimalSize(70, 16),
      EndContainer( ), 
      NWidget( NWID_HORIZONTAL ), SetPIP(4, 1, 4),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_CC ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_CC, STR_CC_GLOBAL_LINKS_CC ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_CC_TTD ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_CC_TTD, STR_CC_GLOBAL_LINKS_CC_TTD ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_TTDWIKI ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_TTDWIKI, STR_CC_GLOBAL_LINKS_TTDWIKI ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_CCWIKI ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_CCWIKI, STR_CC_GLOBAL_LINKS_CCWIKI ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_TUTORIAL ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_TUTORIAL, STR_CC_GLOBAL_LINKS_TUTORIAL ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_MECHANICS ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_MECHANICS, STR_CC_GLOBAL_LINKS_MECHANICS ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_SIGNALS ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_SIGNALS, STR_CC_GLOBAL_LINKS_SIGNALS ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_LINKS_GAME_LOGIN ),SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_LINKS_GAME_LOGIN, STR_CC_GLOBAL_LINKS_GAME_LOGIN ),
        EndContainer( ),
      EndContainer( ),
      NWidget(NWID_SPACER), SetMinimalSize(0, 4), SetFill(0, 1),
    EndContainer( ),
    NWidget(WWT_PANEL, COLOUR_GREY), SetFill(0, 1),
      NWidget( NWID_HORIZONTAL ),
        NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_GLOBAL_VARIOUS, STR_NULL), SetMinimalSize(70, 16),
      EndContainer( ), 
      NWidget( NWID_HORIZONTAL ), SetPIP(4, 1, 4),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_PLAYERS ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_PLAYERS, STR_XI_PLAYERS ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_COMPANIES ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_COMPANIES, STR_XI_COMPANIES ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_SERVER ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_SERVER, STR_CC_GLOBAL_VARIOUS_SERVER ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_INFO ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_INFO, STR_CC_GLOBAL_VARIOUS_INFO ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_VOTE_RESTART ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_VOTE_RESTART, STR_CC_GLOBAL_VARIOUS_VOTE_RESTART ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_VOTE_RESTART_MAP ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_VOTE_RESTART_MAP, STR_CC_GLOBAL_VARIOUS_VOTE_RESTART_MAP ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_VOTE ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_VOTE, STR_CC_GLOBAL_VARIOUS_VOTE ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_VOTE_STATUS ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_VOTE_STATUS, STR_CC_GLOBAL_VARIOUS_VOTE_STATUS ),
        EndContainer( ),
      EndContainer( ),
      NWidget( NWID_HORIZONTAL ), SetPIP(4, 1, 4),
		NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
			NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
			NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_VARIOUS_GOODBYE ), SetMinimalSize(470, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_GLOBAL_VARIOUS_GOODBYE, STR_CC_GLOBAL_VARIOUS_GOODBYE ),
			NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
		EndContainer( ),
	  EndContainer( ),
    EndContainer( ),
    NWidget(WWT_PANEL, COLOUR_GREY), SetFill(0, 1),
      NWidget( NWID_HORIZONTAL ),
        NWidget(NWID_SPACER), SetMinimalSize(4, 0), SetFill(0, 1),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_OTHER_ADMIN, STR_NULL), SetMinimalSize(70, 16),
      EndContainer( ), 
      NWidget( NWID_HORIZONTAL ),
        NWidget(NWID_SPACER), SetMinimalSize(3, 0),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_NICE_USER_TEXT, STR_NULL), SetMinimalSize(20, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_SPACES, STR_NULL), SetMinimalSize(4, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN, NIC_NICE_USER), SetDataTip(STR_CC_NICE_USER_WHITE, STR_NULL), SetMinimalSize(80, 16),
        NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 12), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_BTPRO_USER_TEXT, STR_NULL), SetMinimalSize(20, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_SPACES, STR_NULL), SetMinimalSize(4, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN, NIC_BTPRO_USER), SetDataTip(STR_CC_BTPRO_USER_WHITE, STR_NULL), SetMinimalSize(80, 16),
        NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 12), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_SERVER_TEXT, STR_NULL), SetMinimalSize(20, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_CC_SPACES, STR_NULL), SetMinimalSize(4, 16),
        NWidget(WWT_TEXT, COLOUR_BROWN, NIC_OTHER_SERVER), SetDataTip(STR_CC_SERVER_TEXT_SERVER, STR_NULL), SetMinimalSize(40, 16),
        NWidget(NWID_SPACER), SetMinimalSize(3, 0),
      EndContainer( ), 
      NWidget( NWID_HORIZONTAL ), SetPIP(4, 1, 4),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_ADMIN_ACK ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_OTHER_ADMIN_ACKNOWLEDGE, STR_CC_OTHER_ADMIN_ACKNOWLEDGE ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_ADMIN_SOLVE ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_OTHER_ADMIN_SOLVE, STR_CC_OTHER_ADMIN_SOLVE ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_TOKEN_LOGIN ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_OTHER_TOKEN_LOGIN, STR_CC_OTHER_TOKEN_LOGIN_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_TOKEN_LOGOUT ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_OTHER_TOKEN_LOGOUT, STR_CC_OTHER_TOKEN_LOGOUT_TOOLTIP ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_COORDINATES ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_SCROLL_TO_COORDS, STR_XI_SCROLL_TO_COORDS_TOOLTIP ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_NEWSTICKET ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_XI_GLOBAL_NEWSTICKET_BUTTON, STR_XI_GLOBAL_NEWSTICKET_BUTTON_TOOLTIP ),
        EndContainer( ),
        NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_ADMIN_LOGIN ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_OTHER_ADMIN_LOGIN, STR_CC_OTHER_ADMIN_LOGIN ),
          NWidget( WWT_PUSHTXTBTN, COLOUR_ORANGE, NIC_OTHER_ADMIN_LOGOUT ), SetMinimalSize(50, 18),SetFill(1, 0) ,   SetDataTip( STR_CC_OTHER_ADMIN_LOGOUT, STR_CC_OTHER_ADMIN_LOGOUT ),
        EndContainer( ),
      EndContainer( ),
	  NWidget( NWID_HORIZONTAL ), SetPIP(4, 1, 4),
		NWidget( NWID_VERTICAL ), SetPIP(1, 1, 1),
			NWidget(NWID_SPACER), SetMinimalSize(3, 3),
		EndContainer ( ),
	  EndContainer( ),
    EndContainer( ),
    NWidget(NWID_SPACER), SetMinimalSize(2, 0), SetFill(0, 1),
  EndContainer( ),
};

/* Window Descriptor */
static WindowDesc _CC_commands_desc(
	WDP_CENTER, "commands_token_gui", 0, 0,
	WC_CC_COMMANDS, WC_NONE,
	0,
	_nested_CC_commands_widgets, lengthof(_nested_CC_commands_widgets)
);

/** Class Constructor */
class CCCommand : public Window
{
	protected:	
		CCCommandsQuery query_widget;

	public:
		CCCommand(WindowDesc *desc, int window_number) : Window(desc)
		{
			this->InitNested(window_number);
		}
		
	virtual void SetStringParameters(int widget) const
	{
		switch (widget) {
			case NIC_OTHER_SERVER:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "n-ice" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "BTPro" ); }
					else { SetDParamStr( 0, "Unknown" ); }
				} 
				else { SetDParamStr( 0, "Unknown (SP)" ); }
				break;
			case NIC_NICE_USER:
				SetDParamStr( 0, _settings_client.network.community_user[0] );
				break;
			case NIC_BTPRO_USER:
				SetDParamStr( 0, _settings_client.network.community_user[1] );
				break;
			case NIC_RULES1:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "1: OpenTTD Rules" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "1: Language" ); } 
					else { SetDParamStr( 0, "1: Language" ); }
				} 
				else { SetDParamStr( 0, "1: Language" ); }
				break;
			case NIC_RULES2:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "2: Competition" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "2: Spamming" ); } 
					else { SetDParamStr( 0, "2: Spamming" ); }
				} 
				else { SetDParamStr( 0, "2: Spamming" ); }
				break;
			case NIC_RULES3:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "3: ST Spread" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "3: Competition" ); } 
					else { SetDParamStr( 0, "3: Competition" ); }
				} 
				else { SetDParamStr( 0, "3: Competition" ); }
				break;
			case NIC_RULES4:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "4: Blocking" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "4: Self Comp." ); } 
					else { SetDParamStr( 0, "4: Self Comp." ); }
				}
				else { SetDParamStr( 0, "4: Self Comp." ); }
				break;
			case NIC_RULES5:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "5: Destroy towns" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "5: Blocking" ); } 
					else { SetDParamStr( 0, "5: Blocking" ); }
				} 
				else { SetDParamStr( 0, "5: Blocking" ); }
				break;
			case NIC_RULES6:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "6: Crossings" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "6: Town Dest." ); } 
					else { SetDParamStr( 0, "6: Town Dest." ); }
				} 
				else { SetDParamStr( 0, "6: Town Dest." ); }
				break;
			case NIC_RULES7:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "7: Swearing" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "7: ST Spread" ); } 
					else { SetDParamStr( 0, "7: ST Spread" ); }
				} 
				else { SetDParamStr( 0, "7: ST Spread" ); }
				break;
			case NIC_RULES8:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "8: Racism" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "8: ST apart" ); } 
					else { SetDParamStr( 0, "8: ST apart" ); }
				} 
				else { SetDParamStr( 0, "8: ST apart" ); }
				break;
			case NIC_RULES9:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "9: Spamming" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "9: Water Resources" ); } 
					else { SetDParamStr( 0, "9: Water Resources" ); }
				} 
				else { SetDParamStr( 0, "9: Water Resources" ); }
				break;
			case NIC_RULES10:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "10: Language" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "10: CB Golden" ); } 
					else { SetDParamStr( 0, "10: CB Golden" ); }
				} 
				else { SetDParamStr( 0, "10: CB Golden" ); }
				break;
			case NIC_RULES11:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "11: No Revenge" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "11: ..." ); } 
					else { SetDParamStr( 0, "11: ..." ); }
				} 
				else { SetDParamStr( 0, "11: ..." ); }
				break;
			case NIC_RULES12:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "12: Spread Cheat" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "12: ..." ); } 
					else { SetDParamStr( 0, "12: ..." ); }
				} 
				else { SetDParamStr( 0, "12: ..." ); }
				break;
			case NIC_RULES13:
				if (_networking) {
					if (_settings_client.gui.community == 1) { SetDParamStr( 0, "13: Rules Intention" ); } 
					else if (_settings_client.gui.community == 2) { SetDParamStr( 0, "13: ..." ); } 
					else { SetDParamStr( 0, "13: ..." ); }
				} 
				else { SetDParamStr( 0, "13: ..." ); }
				break;
		}
	}

	virtual void OnPaint()
	{
		this->DrawWidgets();
	}

	virtual void DrawWidget(const Rect &r, int widget) const
	{
		char* _cc_name; //server name - for the button
		char name[16];
		char sn[64];
		switch (widget) {
			case NIC_SERVERS:				
				// do nothing
				break;
			default:
				if(widget >= NIC_SERVERS1){
					if(widget - NIC_SERVERS1 + 1 < 10){
						seprintf(name, lastof(name), "NAME0%i", widget - NIC_SERVERS1 + 1);
					}
					else {
						seprintf(name, lastof(name), "NAME%i", widget - NIC_SERVERS1 + 1);
					}
					size_t posname = _server_list_text_names1.find(name);
					std::string sname = _server_list_text_names1.substr(posname + 8, _server_list_text_names1.find(";", posname + 8) - posname - 8);
					strecpy(sn, sname.c_str(), lastof(sn));
					_cc_name = sn;
					
					SetDParamStr(0, _cc_name);
					DrawString(r.left, r.right, r.top + 3, STR_NETWORK_DIRECT_JOIN_GAME, TC_FROMSTRING, SA_CENTER);
				}
				break;
		}
	}

	virtual void OnClick(Point pt, int widget, int click_count)
	{
		char msg[128];
		std::string decoded;
		std::string ti;
		extern void OpenBrowser(const char *url);
		
		switch (widget) {
			/*rules*/
			case NIC_RULES1:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 1");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 1");
					}
				} 
				else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES2:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 2");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 2");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES3:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 3");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 3");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES4:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 4");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 4");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES5:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 5");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 5");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES6:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 6");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 6");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES7:
			if (_networking) {
				if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 7");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 7");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES8:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 8");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 8");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES9:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 9");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 9");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES10:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 10");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 10");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES11:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 11");
					} else if (_settings_client.gui.community == 2) {
						//NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 11");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES12:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 12");
					} else if (_settings_client.gui.community == 2) {
						//NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 12");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES13:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules 13");
					} else if (_settings_client.gui.community == 2) {
						//NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules 13");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES_ALL:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!rules *");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!arules");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_RULES_WEBSITE:
				if (_settings_client.gui.community == 1) {
					OpenBrowser("http://www.n-ice.org/openttd/rules.php");
				  } else if (_settings_client.gui.community == 2) {
					OpenBrowser("http://openttd.btpro.nl/index.php/forum/11-server-rules/10-server-rules");
				  }
				break;
			case NIC_RULES_WEBSITE_DETAILED:
				if (_settings_client.gui.community == 1) {
					OpenBrowser("https://wiki.x-base.info/OpenTTD/Rules");
				  } else if (_settings_client.gui.community == 2) {
					OpenBrowser("http://openttd.btpro.nl/wiki/index.php/Server_Rules");
				  }
				break;

			/*web links*/
			case NIC_LINKS_CC:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://www.n-ice.org/openttd");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://openttd.btpro.nl");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_CC_TTD:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://www.n-ice.org/openttd/forum");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://openttd.btpro.nl");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_TTDWIKI:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://wiki.openttd.org");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_CCWIKI:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "https://wiki.x-base.info/OpenTTD");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://openttd.btpro.nl/wiki/index.php/Main_Page");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_TUTORIAL:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://wiki.openttd.org/Tutorial");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_MECHANICS:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://wiki.openttd.org/Game_Mechanics");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_SIGNALS:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://wiki.openttd.org/Signals");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_LINKS_GAME_LOGIN:
				if (_networking) {
					if (_settings_client.gui.community == 1) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://www.n-ice.org/openttd  -  login at website and read instructions in 'Game Login' link");
					} else if (_settings_client.gui.community == 2) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "http://openttd.btpro.nl  -  login at website and read instructions in 'Game Login' link");
					}
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;

			/*various commands*/
			case NIC_VARIOUS_PLAYERS:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!players");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_COMPANIES:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!companies");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_SERVER:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "!server");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_INFO:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "!info");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_VOTE_RESTART:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "!voterestart");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_VOTE_RESTART_MAP:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "!voterestartgame");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_VOTE:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "!vote");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_VOTE_STATUS:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , "!votestatus");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_VARIOUS_GOODBYE:
				if (_networking) {
					seprintf(msg, lastof(msg), "%s", _settings_client.network.luck_goodbye);
					NetworkClientSendChat(NETWORK_ACTION_CHAT, DESTTYPE_BROADCAST, 0 , msg);
					AskExitToGameMenu();
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;


			/*admin commands*/
			case NIC_OTHER_ADMIN_ACK:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!ack");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_OTHER_ADMIN_SOLVE:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!solve");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_OTHER_TOKEN_LOGIN:
				if ((_networking) && (_settings_client.gui.community != 0)) {
					CommunityLoginManagerSend();
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_OTHER_TOKEN_LOGOUT:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!logout");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_OTHER_COORDINATES:
				this->query_widget = NIQ_COORDINATES;
				SetDParam(0, this->window_number);
				ShowQueryString(STR_EMPTY, STR_XI_SCROLL_TO_COORDS_TITLE, 25, this, CS_ALPHANUMERAL, QSF_NONE);
				break;
			case NIC_OTHER_NEWSTICKET:
				if (_networking) {
					this->query_widget = NIQ_NEWSTICKET;
					SetDParam(0, this->window_number);
					ShowQueryString(STR_EMPTY, STR_XI_GLOBAL_NEWSTICKET, 250, this, CS_ALPHANUMERAL, QSF_NONE);
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_OTHER_ADMIN_LOGIN:
				if (_networking) {
					const char *np;
					std::string ti;
					std::string decoded;
					if (_settings_client.gui.community == 1) {
						ti = _settings_client.network.community_admin_password[0];
						decoded = base64_decode(ti);
						np = decoded.c_str();
						seprintf(msg, lastof(msg), "!alogin %s %s", _settings_client.network.community_user[0], np);
					} else if (_settings_client.gui.community == 2) {
						ti = _settings_client.network.community_admin_password[1];
						decoded = base64_decode(ti);
						np = decoded.c_str();
						seprintf(msg, lastof(msg), "!alogin %s %s", _settings_client.network.community_user[1], np);
					}
					if (_settings_client.gui.community != 0) {
						NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
					} else { ShowErrorMessage(STR_CC_ADMIN_LOGIN_WRONG_SERVER, INVALID_STRING_ID, WL_ERROR); }
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			case NIC_OTHER_ADMIN_LOGOUT:
				if (_networking) {
					NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , "!alogout");
				} else { ShowErrorMessage(STR_NETWORK_BT_AVAILABLE_NETWORK, INVALID_STRING_ID, WL_ERROR); }
				break;
			default:
				if(widget >= NIC_SERVERS1){
					/*get info of server from network.cpp and join*/
					if (GetCommunityServer(widget - NIC_SERVERS1 + 1)) {
						if (_ctrl_pressed) {
							NetworkClientConnectGame(_cc_address, _cc_porti, COMPANY_NEW_COMPANY);
						} else {
							NetworkClientConnectGame(_cc_address, _cc_porti, COMPANY_SPECTATOR);
						}
					}        
				} else { ShowErrorMessage(STR_CC_SERVER_DISABLED, INVALID_STRING_ID, WL_ERROR); }
				break;
		}
	}

	void OnQueryTextFinished(char *str) 
	{	
		if (str == NULL) return;
		char msg[512];
		
		switch (this->query_widget) {
			default: NOT_REACHED();
			case NIQ_COORDINATES:
				seprintf(msg, lastof(msg), "scrollto %s", str);
				IConsoleCmdExec(msg);
				break;
			case NIQ_NEWSTICKET:
				seprintf(msg, lastof(msg), "!news %s", str);
				NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg);
				break;
		}
	}
};

NWidgetBase *MakeServerButtons1(int *biggest_index)
{
	NWidgetVertical *ver = new NWidgetVertical;
	_server_list_text_names1 = _server_list_text;

	if(_settings_client.gui.community == 0 || _server_list_text_names1.empty()){
		NWidgetBackground *leaf = new NWidgetBackground(WWT_PANEL, COLOUR_GREY, NIC_SERVERS, NULL);		
		ver->Add(leaf);
		return ver;
	}
   
    /* check for disabled server from serverlist file */ 
	int active = 0, aactive[50], s_max = 0;
	if (_settings_client.gui.community == 1) s_max = 50; //for n-ice
	if (_settings_client.gui.community == 2) s_max = 30; //for btpro
	for (int i = 0; i < s_max; i++) {
        aactive[i] = GetCommunityServer(i + 1, true) ? (i + 1) : 0; //server disabled?
        active++;
    }
    	
	NWidgetHorizontal *hor = new NWidgetHorizontal;
	int i1 = 0, i2 = 0;
	for (int i = 0; i < s_max; i++) {
		if ((aactive[i] == 0) && (_settings_client.gui.community == 1)) continue; //hide button if disabled - for n-ice only
		i2++;
		if ((i1 == 5) || (i1 == 10) || (i1 == 15) || (i1 == 20) || (i1 == 25) || (i1 == 30) || (i1 == 35) || (i1 == 40) || (i1 == 45) || (i1 == 50)) {
			i2=0;
			NWidgetSpacer *spce = new NWidgetSpacer(3, 0);
			spce->SetFill(1, 0);
			hor->Add(spce);
			ver->Add(hor);
			NWidgetSpacer *spc = new NWidgetSpacer(0, 4);
			spc->SetFill(1, 0);
			ver->Add(spc);
			hor = new NWidgetHorizontal();
		}
		NWidgetSpacer *spce = new NWidgetSpacer(3, 0);
		spce->SetFill(1, 0);
		hor->Add(spce);
		NWidgetBackground *leaf = new NWidgetBackground(WWT_PANEL, COLOUR_ORANGE, NIC_SERVERS1 + i, NULL);
		if(aactive[i] == 0) leaf->SetDisabled(true);
		leaf->SetDataTip(STR_NETWORK_DIRECT_JOIN_GAME, STR_NETWORK_DIRECT_JOIN_GAME_TOOLTIP);
		leaf->SetMinimalSize(90, 15);
		hor->Add(leaf);
		i1++;
	}
	
	/* arrange buttons @ last line */
	if (i2==0) i2=375;
	if (i2==1) i2=282;
	if (i2==2) i2=189;
	if (i2==3) i2=96;
	if (i2==4) i2=3;
	NWidgetSpacer *spce = new NWidgetSpacer(i2, 0);
	spce->SetFill(1, 0);
	hor->Add(spce);			
	ver->Add(hor);
	*biggest_index = NIC_SERVERS1 - 1 + active;
	return ver;
}

void ShowCCCommands()
{	
	AllocateWindowDescFront<CCCommand>(&_CC_commands_desc, 0);	
}
