/* $Id$ */

/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file intro_gui.cpp The main menu GUI. */

#include "nc_csettings.h"
#include "nc_util.h"

#include "stdafx.h"
#include "error.h"
#include "gui.h"
#include "window_gui.h"
#include "textbuf_gui.h"
#include "network/network.h"
#include "genworld.h"
#include "network/network_gui.h"
#include "network/network_content.h"
#include "landscape_type.h"
#include "strings_func.h"
#include "fios.h"
#include "ai/ai_gui.hpp"
#include "gfx_func.h"
#include "core/geometry_func.hpp"
#include "language.h"
#include "rev.h"
#include "highscore.h"

#include "widgets/intro_widget.h"
#include "widgets/dropdown_type.h"
#include "widgets/dropdown_func.h"

#include "table/strings.h"
#include "table/sprites.h"

#include "network/network_func.h" //Required to connect to a server
#include "window_func.h" //deletewindowbyID
#include "console_func.h" //IConsolePrintF
#include "settings_func.h" //saveconfig
#include "nc_intro_gui.h"

#include "safeguards.h"

////////////////////////XXX
void ShowSelectGameWindow();
////////////////////////XXX

struct SelectGameWindow : public Window, public Observer 
{
	void notify()
	{
		//this->ReInit();
		DeleteWindowByClass(WC_SELECT_GAME);
		ShowSelectGameWindow();
	}
	DropDownList *BuildDropDownList(int widget, int *selected_index) const
	{
		DropDownList *list = NULL;
		switch (widget) 
		{
			case WID_NC_COMMUNITY_DROPDOWN:
				list = new DropDownList();
				*selected_index = CSettings::get().Selected() + 1;
				*list->Append() = new DropDownListCharStringItem("None", 0, false);
				for (int i = 0; i < CSettings::get().Size(); i++) {
					*list->Append() = new DropDownListCharStringItem(CSettings::get().GetCommunityName(i), i+1, false);
				}
			break;
			case WID_NC_GROUP_DROPDOWN:
			{
				list = new DropDownList();
				*list->Append() = new DropDownListCharStringItem("None", 0, false);
				if(CSettings::get().GetSelected() == NULL)
				{
					*selected_index = 0;
					return list;
				}
				ServerGroupList* groups = CSettings::get().GetSelected()->GetGroups();
				if(groups == NULL)
				{
					*selected_index = 0;
					return list;
				}
				*selected_index = groups->SelectedIndex() +1 ;
				for (uint i = 0;i < groups->Size(); i++) {
					*list->Append() = new DropDownListCharStringItem(groups->Get(i)->display_name, i+1, false);
				}
			}
			break;
			default:
			break;
		}
		return list;
	}

	SelectGameQuery query_widget;
	SelectGameWindow(WindowDesc *desc) : Window(desc)
	{
		this->CreateNestedTree();
		this->FinishInitNested(0);
		this->OnInvalidateData();
	}

	/**
	 * Some data on this window has become invalid.
	 * @param data Information about the changed data.
	 * @param gui_scope Whether the call is done from GUI scope. You may not do everything when not in GUI scope. See #InvalidateWindowData() for details.
	 */
	virtual void OnInvalidateData(int data = 0, bool gui_scope = true)
	{
		if (!gui_scope) return;
		this->SetWidgetLoweredState(WID_SGI_TEMPERATE_LANDSCAPE, _settings_newgame.game_creation.landscape == LT_TEMPERATE);
		this->SetWidgetLoweredState(WID_SGI_ARCTIC_LANDSCAPE,    _settings_newgame.game_creation.landscape == LT_ARCTIC);
		this->SetWidgetLoweredState(WID_SGI_TROPIC_LANDSCAPE,    _settings_newgame.game_creation.landscape == LT_TROPIC);
		this->SetWidgetLoweredState(WID_SGI_TOYLAND_LANDSCAPE,   _settings_newgame.game_creation.landscape == LT_TOYLAND);
	}

	virtual void OnInit()
	{
		bool missing = _current_language->missing >= _settings_client.gui.missing_strings_threshold && !IsReleasedVersion();
		this->GetWidget<NWidgetStacked>(WID_SGI_TRANSLATION_SELECTION)->SetDisplayedPlane(missing ? 0 : SZSP_NONE);
		CSettings::get().Regist(this);
	}
	
	virtual void OnPaint()
	{
		//this->DisableWidget(WID_NC_ORDER);
		this->DrawWidgets();
	}

	virtual void DrawWidget(const Rect &r, int widget) const
	{
		switch (widget) {
			case WID_SGI_TRANSLATION:
				SetDParam(0, _current_language->missing);
				DrawStringMultiLine(r.left, r.right, r.top,  r.bottom, STR_INTRO_TRANSLATION, TC_FROMSTRING, SA_CENTER);
				break;
			//case WID_NC_COMMUNITY_DROPDOWN:
			//	DrawString(r.left, r.right, r.top + 3, STR_NULL, TC_FROMSTRING, SA_CENTER);
			//	break;
			default:
			{
				Community* selected = CSettings::get().GetSelected();
				if(selected == NULL) break;
				ServerList* list = selected->GetServerList();
				if(list == NULL) return;
				if(widget >= WID_NC_LIST && widget <= WID_NC_LIST_END)
				{
					SetDParamStr(0, list->Get(widget - WID_NC_LIST)->display_name);
					DrawString(r.left, r.right, r.top + 3, STR_JUST_RAW_STRING, TC_BLACK, SA_CENTER);
				}else if(widget >= WID_NC_GROUPS && widget <= WID_NC_GROUPS_END)
				{
					SetDParamStr(0, CSettings::get().GetSelected()->GetServerGroup()->tabs_name[widget - WID_NC_GROUPS]);
					DrawString(r.left, r.right, r.top + 3, STR_JUST_RAW_STRING , TC_BLACK, SA_CENTER);
				}
			}
				break;
		}
	}
	virtual void SetStringParameters(int widget) const
	{
		Community *selected = CSettings::get().GetSelected();
		switch(widget)
		{
			case WID_NC_HEADER:
				SetDParamStr(0, selected == NULL ? "" : CSettings::get().GetSelected()->display_name);
			break;
			case WID_NC_COMMUNITY_DROPDOWN:   
				SetDParamStr(0, selected == NULL ? "None" :CSettings::get().GetSelectedName()); 
			break;
			case WID_NC_GROUP_DROPDOWN:
				SetDParamStr(0, selected == NULL || selected->GetGroups() == NULL ? "None" :selected->GetGroups()->SelectedName());
			break;
			default:
			break;
		}
	}
	virtual void UpdateWidgetSize(int widget, Dimension *size, const Dimension &padding, Dimension *fill, Dimension *resize)
	{
		switch (widget) {
			case WID_SGI_TRANSLATION: {
				SetDParam(0, _current_language->missing);
				int height = GetStringHeight(STR_INTRO_TRANSLATION, size->width);
				if (height > 3 * FONT_HEIGHT_NORMAL) {
					/* Don't let the window become too high. */
					Dimension textdim = GetStringBoundingBox(STR_INTRO_TRANSLATION);
					textdim.height *= 3;
					textdim.width -= textdim.width / 2;
					*size = maxdim(*size, textdim);
				} else {
					size->height = height + padding.height;
				}
				break;
			}
		}
	}
	virtual void OnDropdownSelect(int widget, int index)
	{
		switch (widget) {
			case WID_NC_COMMUNITY_DROPDOWN:
				CSettings::get().SetSelected(index-1);
				//this->ReInit();
				DeleteWindowByClass(WC_SELECT_GAME);
				ShowSelectGameWindow();
			break;
			case WID_NC_GROUP_DROPDOWN:
				CSettings::get().GetSelected()->SetGroup(index-1);
				//this->ReInit();
				DeleteWindowByClass(WC_SELECT_GAME);
				ShowSelectGameWindow();
			break;
		}
	}
	virtual void OnClick(Point pt, int widget, int click_count)
	{
#ifdef ENABLE_NETWORK
		/* Do not create a network server when you (just) have closed one of the game
		 * creation/load windows for the network server. */
		if (IsInsideMM(widget, WID_SGI_GENERATE_GAME, WID_SGI_EDIT_SCENARIO + 1)) _is_network_server = false;
#endif /* ENABLE_NETWORK */
		extern void OpenBrowser(const char *url);
		switch (widget) {
			case WID_SGI_GENERATE_GAME:
				if (_ctrl_pressed) {
					StartNewGameWithoutGUI(GENERATE_NEW_SEED);
				} else {
					ShowGenerateLandscape();
				}
				break;

			case WID_SGI_LOAD_GAME:      ShowSaveLoadDialog(SLD_LOAD_GAME); break;
			case WID_SGI_PLAY_SCENARIO:  ShowSaveLoadDialog(SLD_LOAD_SCENARIO); break;
			case WID_SGI_PLAY_HEIGHTMAP: ShowSaveLoadDialog(SLD_LOAD_HEIGHTMAP); break;
			case WID_SGI_EDIT_SCENARIO:  StartScenarioEditor(); break;

			case WID_SGI_PLAY_NETWORK:
				if (!_network_available) {
					ShowErrorMessage(STR_NETWORK_ERROR_NOTAVAILABLE, INVALID_STRING_ID, WL_ERROR);
				} else {
					ShowNetworkGameWindow();
				}
				break;

			case WID_SGI_TEMPERATE_LANDSCAPE: case WID_SGI_ARCTIC_LANDSCAPE:
			case WID_SGI_TROPIC_LANDSCAPE: case WID_SGI_TOYLAND_LANDSCAPE:
				SetNewLandscapeType(widget - WID_SGI_TEMPERATE_LANDSCAPE);
				break;

			case WID_SGI_OPTIONS:         ShowGameOptions(); break;
			case WID_SGI_HIGHSCORE:       ShowHighscoreTable(); break;
			case WID_SGI_SETTINGS_OPTIONS:ShowGameSettings(); break;
			case WID_SGI_GRF_SETTINGS:    ShowNewGRFSettings(true, true, false, &_grfconfig_newgame); break;
			case WID_SGI_CONTENT_DOWNLOAD:
				if (!_network_available) {
					ShowErrorMessage(STR_NETWORK_ERROR_NOTAVAILABLE, INVALID_STRING_ID, WL_ERROR);
				} else {
					ShowNetworkContentListWindow();
				}
				break;
			case WID_SGI_AI_SETTINGS:     ShowAIConfigWindow(); break;
			case WID_SGI_EXIT:            HandleExitGameRequest(); break;
			//community stuff
			case WID_NC_ACCOUNT:
				ShowAccountMenu();
			break;
			case WID_NC_LINKS:
				ShowUsefulLinksList();
			break;
			case WID_NC_ADD_COMM:
				ShowAddCommunity();
			break;
			default:
				Community* selected = CSettings::get().GetSelected();
				if(selected == NULL && CSettings::get().Size() == 0) break;
				if(widget >= WID_NC_LIST && widget <= WID_NC_LIST_END )
				{
					ServerList* list = selected->GetServerList();
					if(list == NULL) break;
					printf("Clicked\n");
					printf("%s\n", list->Get(widget - WID_NC_LIST)->ipv4);
					if (_ctrl_pressed) {
						NetworkClientConnectGame(list->Get(widget - WID_NC_LIST)->GetAddress(), COMPANY_NEW_COMPANY);
					}
					else{
						NetworkClientConnectGame(list->Get(widget - WID_NC_LIST)->GetAddress(), COMPANY_SPECTATOR);
					}
				}else if(widget == WID_NC_COMMUNITY_DROPDOWN || widget == WID_NC_GROUP_DROPDOWN)
				{
					int selected;
					DropDownList *list = this->BuildDropDownList(widget, &selected);
					if (list != NULL) {
						ShowDropDownList(this, list, selected, widget);
					}
				}
			break;
		}
	}
	virtual ~SelectGameWindow()
	{
		CSettings::get().UnRegist(this);
	}
};
NWidgetBase *MakeNCPart(int *biggest_index)
{
	Community* selected = CSettings::get().GetSelected();
	ServerList* list = NULL;
	ServerGroup* group = NULL;
	NWidgetVertical *main = new NWidgetVertical;
	if(selected == NULL && CSettings::get().Size() == 0) // There is no communities
	{
		NWidgetLeaf* cap = new NWidgetLeaf(WWT_CAPTION, COLOUR_BROWN, WID_NC_HEADER,0x0,STR_NULL);cap->SetDataTip(STR_NC_NO_COMMUNITY,STR_NULL);
		cap->SetFill(1,0);
		main->Add(cap);
		NWidgetBackground* bg = new NWidgetBackground(WWT_PANEL, COLOUR_BROWN, WID_NC_SERVERS, NULL);
		main->Add(bg);
		NWidgetLeaf* button = new NWidgetLeaf(WWT_PUSHTXTBTN, COLOUR_ORANGE,WID_NC_ADD_COMM,0x0,STR_NULL);
		button->SetDataTip(STR_NC_ADD_COMM,STR_NULL);button->SetMinimalSize(158,15);button->SetFill(1,0);
		main->Add(button);
		*biggest_index = WID_NC_ADD_COMM;
		return main;
	}

	NWidgetLeaf* cap = new NWidgetLeaf(WWT_CAPTION, COLOUR_BROWN, WID_NC_HEADER,0x0,STR_NULL);cap->SetDataTip(STR_NC_COMMUNITY_SECTION,STR_NULL);
	cap->SetFill(1,0);
	main->Add(cap);
	NWidgetBackground* bg = new NWidgetBackground(WWT_PANEL, COLOUR_BROWN,WID_NC_SERVERS, NULL);
	main->Add(bg);
	NWidgetVertical* ver = new NWidgetVertical(NC_EQUALSIZE);
	bg->Add(ver);
	NWidgetHorizontal* hor = new NWidgetHorizontal();
	NWidgetLeaf* drop = new NWidgetLeaf(WWT_DROPDOWN, COLOUR_WHITE, WID_NC_COMMUNITY_DROPDOWN, 0x0, STR_NULL);drop->SetDataTip(STR_BLACK_RAW_STRING,STR_NULL);
	drop->SetFill(1,0);drop->SetPadding(4,4,4,4);
	NWidgetLeaf* addc_btn = new NWidgetLeaf(WWT_PUSHTXTBTN, COLOUR_WHITE,WID_NC_ADD_COMM,0x0,STR_NULL);
	addc_btn->SetDataTip(STR_NC_ADD_COMM,STR_NULL);addc_btn->SetMinimalSize(0,15);
	hor->Add(drop);hor->Add(addc_btn);
	ver->Add(hor);
	
	if(selected == NULL)
	{
		*biggest_index = (WID_NC_LINKS);
		return main;
	}
	
	list = selected->GetServerList();
	group = selected->GetServerGroup();
	NWidgetLeaf* drop_mode = new NWidgetLeaf(WWT_DROPDOWN, COLOUR_WHITE, WID_NC_GROUP_DROPDOWN, 0x0, STR_NULL);
	drop_mode->SetDataTip(STR_BLACK_RAW_STRING,STR_NULL);drop_mode->SetFill(1,0);drop_mode->SetPadding(4,4,4,4);
	ver->Add(drop_mode);
	ver->SetPadding(10,5,10,5);

	if( !(list == NULL || list->Size() == 0))
	{
		if(group != NULL)
		{
			int line_counter = 0;
			for(uint k = 0; k < group->tabs_name.Length(); k++)
			{
				NWidgetLeaf *cap = new NWidgetLeaf(WWT_CAPTION,group->tabs_colour[k],WID_NC_GROUPS +k,0x0,STR_NULL);
				cap->SetFill(1,0);
				ver->Add(cap);
				NWidgetBackground* bg = new NWidgetBackground(WWT_PANEL,group->bg_colour[k],-1);
				ver->Add(bg);
				NWidgetVertical* cont = new NWidgetVertical(NC_EQUALSIZE);
				bg->Add(cont);
				line_counter = 0;
				hor = new NWidgetHorizontal;
				for(uint s = 0; s < group->servers_within_group[k]; s++)
				{
					if ( line_counter%5 == 0 && line_counter>0) 
					{
						cont->Add(hor);
						hor = new NWidgetHorizontal();
					}
					NWidgetLeaf *leaf = new NWidgetLeaf(WWT_PUSHTXTBTN, group->server_colour[group->getServer(k,s)], WID_NC_LIST + list->GetPosByUid(group->getServerUid(k,s)), 0x0,STR_NULL);
					leaf->SetMinimalSize(75, 15);
					leaf->SetPadding(3,2,0,2);
					hor->Add(leaf);
					line_counter++;
				}
				cont->Add(hor);
			}
		}else
		{
			int line_counter = 0;
			hor = new NWidgetHorizontal;
			for(uint s = 0; s < list->Size(); s++,line_counter++)
			{
				if ( line_counter%5 == 0 && line_counter>0) 
				{
					ver->Add(hor);
					hor = new NWidgetHorizontal();
				}
				NWidgetLeaf *leaf = new NWidgetLeaf(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_NC_LIST + s, 0x0,STR_NULL);
				leaf->SetMinimalSize(75, 15);
				leaf->SetPadding(3,2,0,2);
				hor->Add(leaf);
			}
			ver->Add(hor);
		}
	}
	
	main->Add(new NWidgetBackground(WWT_PANEL, COLOUR_BROWN,-1, NULL));
	hor = new NWidgetHorizontal(NC_EQUALSIZE);
	NWidgetLeaf* acc_btn = new 	NWidgetLeaf(WWT_PUSHTXTBTN, COLOUR_ORANGE,WID_NC_ACCOUNT,0x0,STR_NULL);
	acc_btn->SetDataTip(STR_NC_INTRO_ACCOUNT_DETAILS,STR_NULL);acc_btn->SetMinimalSize(158,15);acc_btn->SetFill(1,0);
	NWidgetLeaf* link_btn = new NWidgetLeaf(WWT_PUSHTXTBTN, COLOUR_ORANGE,WID_NC_LINKS,0x0,STR_NULL);link_btn->SetDataTip(STR_NC_INTRO_LINKS,STR_NULL);
	link_btn->SetMinimalSize(158,15);link_btn->SetFill(1,0);
	hor->Add(acc_btn);
	hor->Add(link_btn);
	main->Add(hor);
	*biggest_index = (WID_NC_LINKS);	
	return main;
}
static const NWidgetPart _nested_select_game_widgets[] = {
	NWidget(WWT_CAPTION, COLOUR_BROWN), SetDataTip(STR_INTRO_CAPTION, STR_NULL),
	NWidget(WWT_PANEL, COLOUR_BROWN),
	NWidget(NWID_SPACER), SetMinimalSize(0, 8),

	/* 'generate game' and 'load game' buttons */
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_GENERATE_GAME), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_NEW_GAME, STR_INTRO_TOOLTIP_NEW_GAME), SetPadding(0, 0, 0, 10), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_LOAD_GAME), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_LOAD_GAME, STR_INTRO_TOOLTIP_LOAD_GAME), SetPadding(0, 10, 0, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 6),

	/* 'play scenario' and 'play heightmap' buttons */
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_PLAY_SCENARIO), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_PLAY_SCENARIO, STR_INTRO_TOOLTIP_PLAY_SCENARIO), SetPadding(0, 0, 0, 10), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_PLAY_HEIGHTMAP), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_PLAY_HEIGHTMAP, STR_INTRO_TOOLTIP_PLAY_HEIGHTMAP), SetPadding(0, 10, 0, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 6),

	/* 'edit scenario' and 'play multiplayer' buttons */
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_EDIT_SCENARIO), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_SCENARIO_EDITOR, STR_INTRO_TOOLTIP_SCENARIO_EDITOR), SetPadding(0, 0, 0, 10), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_PLAY_NETWORK), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_MULTIPLAYER, STR_INTRO_TOOLTIP_MULTIPLAYER), SetPadding(0, 10, 0, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 7),

	/* climate selection buttons */
	NWidget(NWID_HORIZONTAL),
		NWidget(NWID_SPACER), SetMinimalSize(10, 0), SetFill(1, 0),
		NWidget(WWT_IMGBTN_2, COLOUR_ORANGE, WID_SGI_TEMPERATE_LANDSCAPE), SetMinimalSize(77, 55),
							SetDataTip(SPR_SELECT_TEMPERATE, STR_INTRO_TOOLTIP_TEMPERATE),
		NWidget(NWID_SPACER), SetMinimalSize(3, 0), SetFill(1, 0),
		NWidget(WWT_IMGBTN_2, COLOUR_ORANGE, WID_SGI_ARCTIC_LANDSCAPE), SetMinimalSize(77, 55),
							SetDataTip(SPR_SELECT_SUB_ARCTIC, STR_INTRO_TOOLTIP_SUB_ARCTIC_LANDSCAPE),
		NWidget(NWID_SPACER), SetMinimalSize(3, 0), SetFill(1, 0),
		NWidget(WWT_IMGBTN_2, COLOUR_ORANGE, WID_SGI_TROPIC_LANDSCAPE), SetMinimalSize(77, 55),
							SetDataTip(SPR_SELECT_SUB_TROPICAL, STR_INTRO_TOOLTIP_SUB_TROPICAL_LANDSCAPE),
		NWidget(NWID_SPACER), SetMinimalSize(3, 0), SetFill(1, 0),
		NWidget(WWT_IMGBTN_2, COLOUR_ORANGE, WID_SGI_TOYLAND_LANDSCAPE), SetMinimalSize(77, 55),
							SetDataTip(SPR_SELECT_TOYLAND, STR_INTRO_TOOLTIP_TOYLAND_LANDSCAPE),
		NWidget(NWID_SPACER), SetMinimalSize(10, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 7),
	NWidget(NWID_SELECTION, INVALID_COLOUR, WID_SGI_TRANSLATION_SELECTION),
		NWidget(NWID_VERTICAL),
			NWidget(WWT_EMPTY, COLOUR_ORANGE, WID_SGI_TRANSLATION), SetMinimalSize(316, 12), SetFill(1, 0), SetPadding(0, 10, 7, 10),
		EndContainer(),
	EndContainer(),

	/* 'game options' and 'advanced settings' buttons */
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_OPTIONS), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_GAME_OPTIONS, STR_INTRO_TOOLTIP_GAME_OPTIONS), SetPadding(0, 0, 0, 10), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_SETTINGS_OPTIONS), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_CONFIG_SETTINGS_TREE, STR_INTRO_TOOLTIP_CONFIG_SETTINGS_TREE), SetPadding(0, 10, 0, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 6),

	/* 'script settings' and 'newgrf settings' buttons */
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_AI_SETTINGS), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_SCRIPT_SETTINGS, STR_INTRO_TOOLTIP_SCRIPT_SETTINGS), SetPadding(0, 0, 0, 10), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_GRF_SETTINGS), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_NEWGRF_SETTINGS, STR_INTRO_TOOLTIP_NEWGRF_SETTINGS), SetPadding(0, 10, 0, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 6),

	/* 'online content' and 'highscore' buttons */
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_CONTENT_DOWNLOAD), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_ONLINE_CONTENT, STR_INTRO_TOOLTIP_ONLINE_CONTENT), SetPadding(0, 0, 0, 10), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_HIGHSCORE), SetMinimalSize(158, 12),
							SetDataTip(STR_INTRO_HIGHSCORE, STR_INTRO_TOOLTIP_HIGHSCORE), SetPadding(0, 10, 0, 0), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 6),

	/* 'exit program' button */
	NWidget(NWID_HORIZONTAL),
		NWidget(NWID_SPACER), SetFill(1, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_EXIT), SetMinimalSize(128, 12),
							SetDataTip(STR_INTRO_QUIT, STR_INTRO_TOOLTIP_QUIT),
		NWidget(NWID_SPACER), SetFill(1, 0),
	EndContainer(),

	NWidget(NWID_SPACER), SetMinimalSize(0, 8),

	EndContainer(),
	NWidgetFunction(MakeNCPart),
};

static WindowDesc _select_game_desc(
	WDP_CENTER, NULL, 0, 0,
	WC_SELECT_GAME, WC_NONE,
	0,
	_nested_select_game_widgets, lengthof(_nested_select_game_widgets)
);

void ShowSelectGameWindow()
{
	new SelectGameWindow(&_select_game_desc);
}

static void AskExitGameCallback(Window *w, bool confirmed)
{
	if (confirmed) _exit_game = true;
}

void AskExitGame()
{
#if defined(_WIN32)
		SetDParam(0, STR_OSNAME_WINDOWS);
#elif defined(__APPLE__)
		SetDParam(0, STR_OSNAME_OSX);
#elif defined(__BEOS__)
		SetDParam(0, STR_OSNAME_BEOS);
#elif defined(__HAIKU__)
		SetDParam(0, STR_OSNAME_HAIKU);
#elif defined(__MORPHOS__)
		SetDParam(0, STR_OSNAME_MORPHOS);
#elif defined(__AMIGA__)
		SetDParam(0, STR_OSNAME_AMIGAOS);
#elif defined(__OS2__)
		SetDParam(0, STR_OSNAME_OS2);
#elif defined(SUNOS)
		SetDParam(0, STR_OSNAME_SUNOS);
#elif defined(DOS)
		SetDParam(0, STR_OSNAME_DOS);
#else
		SetDParam(0, STR_OSNAME_UNIX);
#endif
	ShowQuery(
		STR_QUIT_CAPTION,
		STR_QUIT_ARE_YOU_SURE_YOU_WANT_TO_EXIT_OPENTTD,
		NULL,
		AskExitGameCallback
	);
}


static void AskExitToGameMenuCallback(Window *w, bool confirmed)
{
	if (confirmed) {
		_switch_mode = SM_MENU;
		ClearErrorMessages();
	}
}

void AskExitToGameMenu()
{
	ShowQuery(
		STR_ABANDON_GAME_CAPTION,
		(_game_mode != GM_EDITOR) ? STR_ABANDON_GAME_QUERY : STR_ABANDON_SCENARIO_QUERY,
		NULL,
		AskExitToGameMenuCallback
	);
}
