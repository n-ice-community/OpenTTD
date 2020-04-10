#include "nc_intro_gui.h"
#include "nc_csettings.h"
#include "strings_func.h"
#include "settings_type.h"
#include "table/strings.h"
#include "error.h"
#include "rev.h"
#include "settings_func.h" //saveconfig
#include "textbuf_gui.h"
#include "stdafx.h"
#include "3rdparty/md5/md5.h"

#include "network/network.h"
#include "network/network_func.h"
#include "network/network_base.h"
#include "network/network_type.h"

#include "strings_func.h"

#include "window_gui.h"
#include "widget_type.h"
#include "window_func.h"
#include "querystring_gui.h"
/******
	Intro Gui Support Windows
*/
/*
	ACCOUNT
*/
enum AccountMenuWidgets
{
	WID_AM_CAPTION,
	WID_AM_USER_SET,
	WID_AM_PASSWORD_SET,
	WID_AM_AUSER_SET,
	WID_AM_APASSWORD_SET,
	WID_AM_SAVE,
};
class AccountMenuGui : public Window
{
	QueryString name_editbox; 
	QueryString pass_editbox;
	bool pass_changed;
	QueryString aname_editbox; 
	QueryString apass_editbox;
	bool apass_changed;
	public:
	AccountMenuGui(WindowDesc *desc, int window_number) : Window(desc),name_editbox(15),pass_editbox(15),aname_editbox(15),apass_editbox(15), pass_changed(false), apass_changed(false)
	{
		this->querystrings[WID_AM_USER_SET] = &this->name_editbox;
		this->querystrings[WID_AM_PASSWORD_SET] = &this->pass_editbox;
		this->querystrings[WID_AM_AUSER_SET] = &this->aname_editbox;
		this->querystrings[WID_AM_APASSWORD_SET] = &this->apass_editbox;
		Community* selected = CSettings::get().GetSelected();
		if(selected == NULL)
		{
			NOT_REACHED();
			return; /// XXX Can't be here
		}
		this->name_editbox.text.Assign(selected->username == NULL ? "" : selected->username);
		this->pass_editbox.text.Assign(selected->password == NULL ? "" : "SAVED");
		this->aname_editbox.text.Assign(selected->admin == NULL ? "" : selected->admin);
		this->apass_editbox.text.Assign(selected->adminpass == NULL ? "" : "SAVED");
		this->InitNested(window_number);
		this->SetFocusedWidget(WID_AM_USER_SET);
	}
	virtual void DrawWidget(const Rect &r, int widget) const
	{
	}
	virtual void SetStringParameters(int widget) const
	{
		Community* selected = CSettings::get().GetSelected();
		if(selected == NULL)
		{
			NOT_REACHED();
			return; /// XXX Can't be here
		}
		switch (widget) 
		{
			case WID_AM_CAPTION:
				SetDParamStr(0,selected->display_name);
			break;
			case WID_AM_USER_SET:
			break;
			case WID_AM_PASSWORD_SET:
			break;
			case WID_AM_AUSER_SET:
			break;
			case WID_AM_APASSWORD_SET:
			break;
			case WID_AM_SAVE:
			break;
			default:
				NOT_REACHED();
			break;
		}
	}
	virtual void OnEditboxChanged(int widget)
	{
		switch(widget)
		{
			case WID_AM_PASSWORD_SET:
				pass_changed = true;
			break;
			case WID_AM_APASSWORD_SET:
				apass_changed = true;
			break;
		}
	}
	virtual void OnClick(Point pt, int widget, int click_count)
	{
		switch (widget)
		{
			case WID_AM_SAVE:
			{
				Community* selected = CSettings::get().GetSelected();
				if(selected == NULL)
				{
					NOT_REACHED();
					return;
				}
				selected->username = stredup(name_editbox.GetText(),NULL);
				if(pass_changed)
				{
					if(selected->login_method == TOKEN_MD5)
					{
						Md5 password;
						password.Append(pass_editbox.GetText(), strlen(pass_editbox.GetText()));
						uint8 digest[16];
						char hex_output[16 * 2 + 1];
						password.Finish(digest);
						for (int di = 0; di < 16; ++di) 
						{
							seprintf(hex_output + di * 2, lastof(hex_output), "%02x", digest[di]);
						}
						selected->password = stredup(hex_output,NULL);
					}
					else
						selected->password = stredup(pass_editbox.GetText(),NULL);
				}
				selected->admin = stredup(aname_editbox.GetText(),NULL);
				if(apass_changed)
					selected->adminpass = stredup(apass_editbox.GetText(),NULL);
				CSettings::get().SaveCommunities();
			}
			default:
			break;
		}
	}
};
static const NWidgetPart _nested_account_menu_widgets[] = {
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_BROWN),
		NWidget(WWT_CAPTION, COLOUR_BROWN,WID_AM_CAPTION), SetDataTip(STR_NC_ACCOUNT_DETAILS_CAPTION , STR_NULL),
	EndContainer(),
	NWidget(WWT_PANEL, COLOUR_GREY),
	NWidget(NWID_VERTICAL),SetPadding(5),
		NWidget(WWT_TEXT,COLOUR_ORANGE),SetDataTip(STR_NC_REGULAR_USER,STR_NULL),
		NWidget(NWID_HORIZONTAL),SetPadding(0,0,0,20),
			NWidget(WWT_TEXT,COLOUR_ORANGE),SetDataTip(STR_NC_USER,STR_NULL),
			NWidget(WWT_EDITBOX, COLOUR_GREY, WID_AM_USER_SET), SetMinimalSize(256, 12), SetFill(1, 1), SetPadding(2, 2, 2, 2),
		EndContainer(),
		NWidget(NWID_HORIZONTAL),SetPadding(5,0,0,20),
			NWidget(WWT_TEXT,COLOUR_ORANGE),SetDataTip(STR_NC_PASSWORD,STR_NULL),
			NWidget(WWT_EDITBOX, COLOUR_GREY, WID_AM_PASSWORD_SET), SetMinimalSize(256, 12), SetFill(1, 1), SetPadding(2, 2, 2, 2),
		EndContainer(),
	EndContainer(),
	NWidget(WWT_PANEL, COLOUR_GREY),SetFill(1,0),
	NWidget(NWID_VERTICAL),SetPadding(5),
		NWidget(WWT_TEXT,COLOUR_ORANGE),SetDataTip(STR_NC_ADMIN_USER,STR_NULL),
		NWidget(NWID_HORIZONTAL),SetPadding(0,0,0,20),
			NWidget(WWT_TEXT,COLOUR_ORANGE),SetDataTip(STR_NC_USER,STR_NULL),
			NWidget(WWT_EDITBOX, COLOUR_GREY, WID_AM_AUSER_SET), SetMinimalSize(256, 12), SetFill(1, 1), SetPadding(2, 2, 2, 2),
		EndContainer(),
		NWidget(NWID_HORIZONTAL),SetPadding(5,0,0,20),
			NWidget(WWT_TEXT,COLOUR_ORANGE),SetDataTip(STR_NC_PASSWORD,STR_NULL),
			NWidget(WWT_EDITBOX, COLOUR_GREY, WID_AM_APASSWORD_SET), SetMinimalSize(256, 12), SetFill(1, 1), SetPadding(2, 2, 2, 2),
		EndContainer(),
		NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_AM_SAVE),SetDataTip(STR_SAVE_PRESET_SAVE,STR_NULL),SetPadding(5),
	EndContainer(),
};
static WindowDesc _account_menu_desc(
	WDP_CENTER, NULL, 0, 0,
	WC_NC_ACCOUNT_MENU, WC_NONE,
	0,
	_nested_account_menu_widgets, lengthof(_nested_account_menu_widgets)
);

void ShowAccountMenu()
{
	AllocateWindowDescFront<AccountMenuGui>(&_account_menu_desc, 0);
}
/*
	LINKS
*/
enum LinksListWidgets
{
	WID_LL_CAPTION,
	WID_LL_WEBSITE,
	WID_LL_WEBSITE_TXT,
	WID_LL_FORUM,
	WID_LL_FORUM_TXT,
	WID_LL_WIKI,
	WID_LL_WIKI_TXT,
	WID_LL_RULES,
	WID_LL_RULES_TXT,
	WID_LL_SERVERS,
	WID_LL_SERVERS_TXT,
	WID_LL_CONTACTS,
	WID_LL_CONTACTS_TXT,	
};
class LinksListGui : public Window
{
	public:
	LinksListGui(WindowDesc *desc, int window_number) : Window(desc)
	{
		this->InitNested(window_number);
	}
	virtual void DrawWidget(const Rect &r, int widget) const
	{
	}
	virtual void OnClick(Point pt, int widget, int click_count)
	{
		extern void OpenBrowser(const char *url);
		switch(widget)
		{	
		  /*case WID_LL_WEBSITE:
				if(_settings_client.gui.community == Community::N_ICE)
					OpenBrowser("http://www.n-ice.org/openttd");
				else
					OpenBrowser("http://openttd.btpro.nl");
			break;
	      case WID_LL_FORUM:
				if(_settings_client.gui.community == Community::N_ICE)
					OpenBrowser("http://www.n-ice.org/openttd/forum/index.php");
				else
					OpenBrowser("http://openttd.btpro.nl/index.php/forum/index");
	      break;
	      case WID_LL_WIKI:
				if(_settings_client.gui.community == Community::N_ICE)
					OpenBrowser("http://wiki.x-base.org/wakka.php?wakka=OpenTTD/Rules");
				else
					OpenBrowser("http://openttd.btpro.nl/wiki/index.php/Main_Page");
	      break;
			case WID_LL_RULES:
				if(_settings_client.gui.community == Community::N_ICE)
					OpenBrowser("http://n-ice.org/openttd/rules.php");
				else
					OpenBrowser("http://openttd.btpro.nl/index.php/forum/11-server-rules/10-server-rules");
			break;
	      case WID_LL_SERVERS:
				if(_settings_client.gui.community == Community::N_ICE)
					OpenBrowser("http://openttd.n-ice.org/servers.php");
				else
					OpenBrowser("http://openttd.btpro.nl/index.php?option=com_content&view=article&id=79&Itemid=514");
			break;
	      case WID_LL_CONTACTS:	
				if(_settings_client.gui.community == Community::N_ICE)
					OpenBrowser("http://openttd.n-ice.org/contact.php");
				else
					OpenBrowser("http://openttd.btpro.nl/index.php?option=com_content&view=article&id=102&Itemid=560");
			break;*/
			default:
			break;
		}
	}
	
	virtual void SetStringParameters(int widget) const
	{
		Community* selected = CSettings::get().GetSelected();
		if(selected == NULL)
			return;
		switch (widget) 
		{
			case WID_LL_RULES_TXT:
			case WID_LL_SERVERS:
			case WID_LL_CONTACTS_TXT:
			case WID_LL_WIKI_TXT:
 			case WID_LL_WIKI:
			case WID_LL_FORUM:
			case WID_LL_WEBSITE:
			case WID_LL_CAPTION:
			{
				SetDParamStr(0,selected->display_name);
			}
			break;
			default:
			break;
		}
	}
	
};
static const NWidgetPart _nested_links_list_widgets[] = {
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_BROWN),
		NWidget(WWT_CAPTION, COLOUR_BROWN,WID_LL_CAPTION), SetDataTip(STR_NC_LINKS_CAPTION, STR_NULL),
	EndContainer(),
	/*NWidget(WWT_PANEL, COLOUR_GREY),
	NWidget(NWID_VERTICAL),SetPadding(7),
		NWidget(NWID_HORIZONTAL),
			NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_LL_WEBSITE),SetDataTip(STR_NC_WEBSITE,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
			NWidget(NWID_SPACER),SetFill(0,1),
		EndContainer(),
		NWidget(WWT_TEXT,COLOUR_BEGIN,WID_LL_WEBSITE_TXT),SetDataTip(STR_NC_WEBSITE_DESC,STR_NULL),SetPadding(0,0,0,15),
		NWidget(NWID_SPACER),SetMinimalSize(0,6),SetFill(1,0),
		NWidget(NWID_HORIZONTAL),
			NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_LL_FORUM),SetDataTip(STR_NC_FORUM,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
			NWidget(NWID_SPACER),SetFill(0,1),
		EndContainer(),
		NWidget(WWT_TEXT,COLOUR_BEGIN,WID_LL_FORUM_TXT),SetDataTip(STR_NC_FORUM_DESC,STR_NULL),SetPadding(0,0,0,15),
		NWidget(NWID_SPACER),SetMinimalSize(0,6),SetFill(1,0),
		NWidget(NWID_HORIZONTAL),
			NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_LL_WIKI),SetDataTip(STR_NC_WIKI,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
			NWidget(NWID_SPACER),SetFill(0,1),
		EndContainer(),
		NWidget(WWT_TEXT,COLOUR_BEGIN,WID_LL_WIKI_TXT),SetDataTip(STR_NC_WIKI_DESC,STR_NULL),SetPadding(0,0,0,15),
		NWidget(NWID_SPACER),SetMinimalSize(0,6),SetFill(1,0),
		NWidget(NWID_HORIZONTAL),
			NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_LL_RULES),SetDataTip(STR_NC_RULES,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
			NWidget(NWID_SPACER),SetFill(0,1),
		EndContainer(),
		NWidget(WWT_TEXT,COLOUR_BEGIN,WID_LL_RULES_TXT),SetDataTip(STR_NC_RULES_DESC,STR_NULL),SetPadding(0,0,0,15),
		NWidget(NWID_SPACER),SetMinimalSize(0,6),SetFill(1,0),
		NWidget(NWID_HORIZONTAL),
			NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_LL_SERVERS),SetDataTip(STR_NC_SERVERS,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
			NWidget(NWID_SPACER),SetFill(0,1),
		EndContainer(),
		NWidget(WWT_TEXT,COLOUR_BEGIN,WID_LL_SERVERS_TXT),SetDataTip(STR_NC_SERVERS_DESC,STR_NULL),SetPadding(0,0,0,15),
		NWidget(NWID_SPACER),SetMinimalSize(0,6),SetFill(1,0),
		NWidget(NWID_HORIZONTAL),
			NWidget(WWT_TEXTBTN,COLOUR_BROWN,WID_LL_CONTACTS),SetDataTip(STR_NC_CONTACTS,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
			NWidget(NWID_SPACER),SetFill(0,1),
		EndContainer(),
		NWidget(WWT_TEXT,COLOUR_BEGIN, WID_LL_CONTACTS_TXT),SetDataTip(STR_NC_CONTACTS_DESC,STR_NULL),SetPadding(0,0,0,15),
		NWidget(NWID_SPACER),SetMinimalSize(0,6),SetFill(1,0),	
	EndContainer(),*/
};
static WindowDesc _links_list_desc(
	WDP_CENTER, NULL, 0, 0,
	WC_NC_LINKS_LIST, WC_NONE,
	0,
	_nested_links_list_widgets, lengthof(_nested_links_list_widgets)
);

void ShowUsefulLinksList()
{
	AllocateWindowDescFront<LinksListGui>(&_links_list_desc, 0);
} 
/*
	ADD COMMUNITY
*/
enum AddCommunityWidgets
{
	WID_AC_CAPTION,
	WID_AC_NAME,
	WID_AC_LINK,
	WID_AC_CONFIRM,	
};
class AddCommunityGui : public Window
{
	QueryString link_editbox;    ///< Editbox.
	public:
	AddCommunityGui(WindowDesc *desc, int window_number) : Window(desc), link_editbox(200) 
	{
		this->querystrings[WID_AC_LINK] = &this->link_editbox;
		this->InitNested(window_number);
		this->SetFocusedWidget(WID_AC_LINK);
	}
	virtual void DrawWidget(const Rect &r, int widget) const
	{
	}
	virtual void OnClick(Point pt, int widget, int click_count)
	{
		switch(widget)
		{
			case WID_AC_CONFIRM:
				CSettings::get().AddCommunity("Undefined",link_editbox.GetText());
			break;
		}
	}
	

	
};
static const NWidgetPart _nested_add_comm_widgets[] = {
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_ORANGE),
		NWidget(WWT_CAPTION, COLOUR_ORANGE,WID_LL_CAPTION), SetDataTip(STR_NC_ADD_COMM, STR_NULL),SetMinimalSize(300,20),
	EndContainer(),
	NWidget(WWT_PANEL, COLOUR_WHITE),
	NWidget(NWID_VERTICAL),SetPadding(7),
		NWidget(WWT_TEXT,COLOUR_BEGIN),SetDataTip(STR_NC_LINK,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
		NWidget(WWT_EDITBOX, COLOUR_GREY, WID_AC_LINK), SetMinimalSize(256, 12), SetFill(1, 1), SetPadding(2, 2, 2, 2),
		NWidget(WWT_TEXTBTN,COLOUR_ORANGE,WID_AC_CONFIRM),SetDataTip(STR_NC_CONFIRM,STR_NULL),SetPadding(5),SetMinimalSize(0,20),
	EndContainer(),
};
static WindowDesc _add_comm_desc(
	WDP_CENTER, NULL, 0, 0,
	WC_NC_ADD_COMM, WC_NONE,
	0,
	_nested_add_comm_widgets, lengthof(_nested_add_comm_widgets)
);

void ShowAddCommunity()
{
	AllocateWindowDescFront<AddCommunityGui>(&_add_comm_desc, 0);
} 
