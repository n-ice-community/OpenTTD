/* $Id: commands_token_gui.cpp	23663 2011-12-23 18:01:50	me $ */

#include "stdafx.h"
#include "widget_type.h"
#include "window_func.h"
#include "strings_func.h"
#include "nc_community_window.h"
#include "network/network.h"
#include "network/network_func.h"
#include "network/network_base.h"
#include "network/network_type.h"
#include "table/strings.h"
#include "console_func.h"
#include "error.h"
#include "window_gui.h"
#include "window_func.h"
#include "company_base.h"
#include "nc_csettings.h"
#include "nc_util.h"

enum CommunityWindowWidgets {
	NIC_TOKEN_USER,
	NIC_TOKEN_LOGIN,
	NIC_TOKEN_COMMUNITY,
};

class CommunityWindowGui : public Window
{
	public:
	CommunityWindowGui(WindowDesc *desc, int window_number) : Window(desc)
	{
		this->InitNested(window_number);
	}

	virtual void OnPaint()
	{
		this->DrawWidgets();
	}

	virtual void SetStringParameters(int widget) const
	{
		switch(widget)
		{
			case NIC_TOKEN_USER:
				SetDParamStr(0, CSettings::get().GetSelected() == NULL ? "" : CSettings::get().GetSelected()->username);
			break;
			case NIC_TOKEN_COMMUNITY:
				SetDParamStr(0, CSettings::get().GetSelectedName());
			break;
			default:
				
			break;
		}

	}

	virtual void OnClick(Point pt, int widget, int click_count)
	{
		extern void OpenBrowser(const char *url);
		switch (widget) 
		{
			case NIC_TOKEN_LOGIN:
				if (_networking)
					new Login();
					DeleteWindowByClass(WC_NC_COMM_WINDOW);
				break;
		}
	}
};

static const NWidgetPart _nested_community_window_widgets[] = {
	/* Title Bar with close box, title, shade and stick boxes */
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_GREY),
		NWidget(WWT_CAPTION, COLOUR_GREY), SetDataTip(STR_NC_COMMUNITY_WINDOW, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
		NWidget(WWT_SHADEBOX, COLOUR_GREY),
		NWidget(WWT_STICKYBOX, COLOUR_GREY),
	EndContainer(),
	NWidget(WWT_PANEL, COLOUR_GREY), SetFill(0, 1), SetPIP(2, 2, 2),
		NWidget(NWID_VERTICAL), SetPadding(0, 5, 0, 5),
			NWidget(WWT_TEXT, COLOUR_BROWN, NIC_TOKEN_USER), SetDataTip(STR_NC_TOKEN_WELCOME, STR_NULL), SetMinimalSize(110, 16),
			NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_NC_TOKEN_CLICK_ME, STR_NULL), SetMinimalSize(110, 16),
			NWidget(NWID_HORIZONTAL), SetPIP(2, 2, 2),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, NIC_TOKEN_LOGIN), SetMinimalSize(105, 40),SetFill(1, 0), SetDataTip(STR_NC_TOKEN_LOGIN, STR_NC_TOKEN_LOGIN_TOOLTIP),
			EndContainer(),
			NWidget(WWT_TEXT, COLOUR_BROWN), SetDataTip(STR_NC_TOKEN_ENJOY, STR_NULL), SetMinimalSize(110, 16),
			NWidget(WWT_TEXT, COLOUR_BROWN, NIC_TOKEN_COMMUNITY), SetDataTip(STR_BLACK_RAW_STRING, STR_NULL), SetMinimalSize(110, 16),
		EndContainer(),
		NWidget(NWID_SPACER), SetMinimalSize(2, 2), SetFill(0, 1),
	EndContainer(),
};

static WindowDesc _community_window_desc(
	WDP_CENTER,"community_window", 0, 0,
	WC_NC_COMM_WINDOW, WC_NONE,
	0,
	_nested_community_window_widgets, lengthof(_nested_community_window_widgets)
);

void ShowCommunityWindow()
{	
	if(CSettings::get().GetSelected()->HasLoginCredentials())
		AllocateWindowDescFront<CommunityWindowGui>(&_community_window_desc, 0);
}
