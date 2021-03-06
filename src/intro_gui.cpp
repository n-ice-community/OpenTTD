/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file intro_gui.cpp The main menu GUI. */

#include "stdafx.h"
#include "error.h"
#include "gui.h"
#include "window_gui.h"
#include "textbuf_gui.h"
#include "network/network.h"
#include "genworld.h"
#include "network/network_func.h"
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
#include "3rdparty/md5/md5.h"

#include "widgets/intro_widget.h"

#include "table/strings.h"
#include "table/sprites.h"

#include "window_func.h" //deletewindowbyID
#include "console_func.h" //IConsolePrintF
#include "settings_func.h" //saveconfig
#include "base64.h"

#include "safeguards.h"

using namespace std;
std::string _server_list_text_names;

void ShowSelectGameWindow();
extern void OSOpenBrowser(const char *url);

struct SelectGameWindow : public Window {

  SelectGameQuery query_widget;

	SelectGameWindow(WindowDesc *desc) : Window(desc)
	{
		this->CreateNestedTree();
		this->FinishInitNested(0);
		this->OnInvalidateData();
		GetCommunityServerListText();
	}

  void OnQueryTextFinished(char *str) {
		if (str == NULL) return;		

    int comm = _settings_client.gui.community; //chosen community
		switch (query_widget) {
			case SGQ_CC_USER:
				if (Utf8StringLength(str) >= NETWORK_NAME_LENGTH) break;
				if(comm > 0 && comm <= 2){
					strecpy(_settings_client.network.community_user[comm-1], str, lastof(_settings_client.network.community_user[comm-1]));
				}
				//strecpy(_settings_client.network.client_name, str, lastof(_settings_client.network.client_name));
				SaveToConfig();
				this->SetDirty();
				break;

			case SGQ_CC_PASSWORD:{
				if (Utf8StringLength(str) >= NETWORK_PASSWORD_LENGTH) break;
				const char *np;
                if (comm == 1) {
                    Md5 password,salted_password;
                    password.Append(str, strlen(str));
                    uint8 digest[16];
                    char hex_output[16*2 + 1];
                    password.Finish(digest);
                    for (int di = 0; di < 16; ++di)
                    {
                        seprintf( hex_output + di * 2, lastof(hex_output), "%02x", digest[di]);
                    }
                    char tobe_salted[4+16*2+6+1] = {0};
                    strecat(tobe_salted, "nice", lastof(tobe_salted));
                    strecat(tobe_salted+4, hex_output, lastof(tobe_salted));
                    strecat(tobe_salted+4+16*2, "client", lastof(tobe_salted));
                    assert(strlen(tobe_salted) == (sizeof(tobe_salted)-1));
                    salted_password.Append(tobe_salted, strlen(tobe_salted));
                    salted_password.Finish(digest);
                    for (int di = 0; di < 16; ++di)
                    {
                        seprintf( hex_output + di * 2, lastof(hex_output), "%02x", digest[di]);
                    }
					strecpy(_settings_client.network.community_password[comm-1], hex_output, lastof(_settings_client.network.community_password[comm-1]));
                } else {
				    string s = str;
                    string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
                    np = encoded.c_str();

					strecpy(_settings_client.network.community_password[comm-1], np, lastof(_settings_client.network.community_password[comm-1]));
                }
				SaveToConfig();
				break;
			}
			default: NOT_REACHED();
		}
	}

	/**
	 * Some data on this window has become invalid.
	 * @param data Information about the changed data.
	 * @param gui_scope Whether the call is done from GUI scope. You may not do everything when not in GUI scope. See #InvalidateWindowData() for details.
	 */
	void OnInvalidateData(int data = 0, bool gui_scope = true) override
	{
		if (!gui_scope) return;
		this->SetWidgetLoweredState(WID_SGI_TEMPERATE_LANDSCAPE, _settings_newgame.game_creation.landscape == LT_TEMPERATE);
		this->SetWidgetLoweredState(WID_SGI_ARCTIC_LANDSCAPE,    _settings_newgame.game_creation.landscape == LT_ARCTIC);
		this->SetWidgetLoweredState(WID_SGI_TROPIC_LANDSCAPE,    _settings_newgame.game_creation.landscape == LT_TROPIC);
		this->SetWidgetLoweredState(WID_SGI_TOYLAND_LANDSCAPE,   _settings_newgame.game_creation.landscape == LT_TOYLAND);
	}

	void OnInit() override
	{
		bool missing_sprites = _missing_extra_graphics > 0 && !IsReleasedVersion();
		this->GetWidget<NWidgetStacked>(WID_SGI_BASESET_SELECTION)->SetDisplayedPlane(missing_sprites ? 0 : SZSP_NONE);

		bool missing_lang = _current_language->missing >= _settings_client.gui.missing_strings_threshold && !IsReleasedVersion();
		this->GetWidget<NWidgetStacked>(WID_SGI_TRANSLATION_SELECTION)->SetDisplayedPlane(missing_lang ? 0 : SZSP_NONE);
		if(_settings_client.gui.community == 1){
			this->GetWidget<NWidgetCore>(WID_SGI_CC_SELECT_NICE)->colour = COLOUR_YELLOW;
			this->DisableWidget(WID_SGI_SERVER_VIP); //disable VIP-button for n-ice
		}
		else if(_settings_client.gui.community == 2) this->GetWidget<NWidgetCore>(WID_SGI_CC_SELECT_BTPRO)->colour = COLOUR_YELLOW;
		else{
			this->DisableWidget(WID_SGI_CC_PASSWORD);
			this->DisableWidget(WID_SGI_CC_USER_ENTER);
			this->DisableWidget(WID_SGI_WEBSITE);
			this->DisableWidget(WID_SGI_SERVERS_FORUM);
			this->DisableWidget(WID_SGI_IRC_CHAT);
			this->DisableWidget(WID_SGI_IRC_SERVERS_CHAT);
			this->DisableWidget(WID_SGI_SERVER_RULES);
			this->DisableWidget(WID_SGI_SERVER_WIKI);
			this->DisableWidget(WID_SGI_SERVER_VIP);
		}
	}

	void DrawWidget(const Rect &r, int widget) const override
	{
    char* _cc_name; //server name - for the button
    char name[16];
    char sn[64];
		switch (widget) {
			case WID_SGI_BASESET:
				SetDParam(0, _missing_extra_graphics);
				DrawStringMultiLine(r.left, r.right, r.top,  r.bottom, STR_INTRO_BASESET, TC_FROMSTRING, SA_CENTER);
				break;

			case WID_SGI_TRANSLATION:
				SetDParam(0, _current_language->missing);
				DrawStringMultiLine(r.left, r.right, r.top,  r.bottom, STR_INTRO_TRANSLATION, TC_FROMSTRING, SA_CENTER);
				break;

			case WID_SGI_SERVERS:				
				//DrawString(r.left, r.right, r.top + 40, STR_CC_SERVER_FREELANCER, TC_FROMSTRING, SA_CENTER);
				DrawStringMultiLine(r.left, r.right, r.top,  r.bottom, STR_CC_SERVER_FREELANCER, TC_FROMSTRING, SA_CENTER);
				break;
			default:
				if(widget >= WID_SGI_CC1){
					if(widget - WID_SGI_CC1 + 1 < 10){
						seprintf(name, lastof(name), "NAME0%i", widget - WID_SGI_CC1 + 1);
					}
					else {
						seprintf(name, lastof(name), "NAME%i", widget - WID_SGI_CC1 + 1);
					}
					size_t posname = _server_list_text_names.find(name);
					std::string sname = _server_list_text_names.substr(posname + 8, _server_list_text_names.find(";", posname + 8) - posname - 8);
					strecpy(sn, sname.c_str(), lastof(sn));
					_cc_name = sn;
					
					SetDParamStr(0, _cc_name);
					DrawString(r.left, r.right, r.top + 3, STR_NETWORK_DIRECT_JOIN_GAME, TC_FROMSTRING, SA_CENTER);
				}
				break;
		}
	}

	void UpdateWidgetSize(int widget, Dimension *size, const Dimension &padding, Dimension *fill, Dimension *resize) override
	{
		StringID str = 0;
		switch (widget) {
			case WID_SGI_BASESET:
				SetDParam(0, _missing_extra_graphics);
				str = STR_INTRO_BASESET;
				break;

			case WID_SGI_TRANSLATION:
				SetDParam(0, _current_language->missing);
				str = STR_INTRO_TRANSLATION;
				break;
		}

		if (str != 0) {
			int height = GetStringHeight(str, size->width);
			if (height > 3 * FONT_HEIGHT_NORMAL) {
				/* Don't let the window become too high. */
				Dimension textdim = GetStringBoundingBox(str);
				textdim.height *= 3;
				textdim.width -= textdim.width / 2;
				*size = maxdim(*size, textdim);
			} else {
				size->height = height + padding.height;
			}
		}
	}

	void SetStringParameters(int widget) const override
	{
		switch(widget){
			case WID_SGI_CC_USER:
        if (_settings_client.gui.community != 0) {
          SetDParamStr(0, _settings_client.network.community_user[_settings_client.gui.community-1]);
        } else {
          SetDParamStr(0, " ");
        }
				break;

			case WID_SGI_CC_HEADER:
				SetDParam(0, STR_NETWORK_SERVER_LIST_CC_DEFAULT + (uint16)_settings_client.gui.community);
				break;
		}
	}

	void OnClick(Point pt, int widget, int click_count) override
	{
		/* Do not create a network server when you (just) have closed one of the game
		 * creation/load windows for the network server. */
		if (IsInsideMM(widget, WID_SGI_GENERATE_GAME, WID_SGI_EDIT_SCENARIO + 1)) _is_network_server = false;

		switch (widget) {
			case WID_SGI_GENERATE_GAME:
				if (_ctrl_pressed) {
					StartNewGameWithoutGUI(GENERATE_NEW_SEED);
				} else {
					ShowGenerateLandscape();
				}
				break;

			case WID_SGI_LOAD_GAME:      ShowSaveLoadDialog(FT_SAVEGAME, SLO_LOAD); break;
			case WID_SGI_PLAY_SCENARIO:  ShowSaveLoadDialog(FT_SCENARIO, SLO_LOAD); break;
			case WID_SGI_PLAY_HEIGHTMAP: ShowSaveLoadDialog(FT_HEIGHTMAP,SLO_LOAD); break;
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
			case WID_SGI_CC_SELECT_NICE:
				_settings_client.gui.community = _settings_client.gui.community != 1 ? 1 : 0;
				IConsolePrintF(CC_DEFAULT, "icomm: '%i'", _settings_client.gui.community);
				GetCommunityServerListText();
				DeleteWindowByClass(WC_SELECT_GAME);
				ShowSelectGameWindow();
				break;
			case WID_SGI_CC_SELECT_BTPRO:
				_settings_client.gui.community = _settings_client.gui.community != 2 ? 2 : 0;
				IConsolePrintF(CC_DEFAULT, "icomm: '%i'", _settings_client.gui.community);
				GetCommunityServerListText();
				DeleteWindowByClass(WC_SELECT_GAME);
				ShowSelectGameWindow();
				break;
			case WID_SGI_WEBSITE:
				if(_settings_client.gui.community == 1) OSOpenBrowser("http://www.n-ice.org");
				else if(_settings_client.gui.community == 2) OSOpenBrowser("https://openttd.btpro.nl");
				break;	
			case WID_SGI_SERVERS_FORUM:
				if(_settings_client.gui.community == 1) OSOpenBrowser("http://www.n-ice.org/openttd/forum/index.php");
				else if(_settings_client.gui.community == 2) OSOpenBrowser("https://openttd.btpro.nl/index.php/forum/recent");
				break;
			case WID_SGI_IRC_CHAT:
				if(_settings_client.gui.community == 1) OSOpenBrowser("https://chat.mibbit.com/?url=irc%3A%2F%2Firc.boxor.net:6668%2FOpenTTD.Chat");
				else if(_settings_client.gui.community == 2) OSOpenBrowser("https://chat.mibbit.com/?url=irc%3A%2F%2Firc.boxor.net:6668%2Fbtpro-chat");
				break;
			case WID_SGI_IRC_SERVERS_CHAT:
				if(_settings_client.gui.community == 1) OSOpenBrowser("https://chat.mibbit.com/?url=irc%3A%2F%2Firc.boxor.net:6668%2FOpenTTD");
				else if(_settings_client.gui.community == 2) OSOpenBrowser("https://chat.mibbit.com/?url=irc%3A%2F%2Firc.boxor.net:6668%2Fbtpro-openttd");
				break;
			case WID_SGI_SERVER_RULES:
				if(_settings_client.gui.community == 1) OSOpenBrowser("http://n-ice.org/openttd/rules.php");
				else if(_settings_client.gui.community == 2) OSOpenBrowser("https://openttd.btpro.nl/index.php/forum/11-server-rules/10-server-rules");
				break;
			case WID_SGI_SERVER_WIKI:
				if(_settings_client.gui.community == 1) OSOpenBrowser("https://wiki.x-base.info/OpenTTD/Rules");
				else if(_settings_client.gui.community == 2) OSOpenBrowser("https://openttd.btpro.nl/wiki/index.php/Main_Page");
				break;
			case WID_SGI_SERVER_VIP:
				if(_settings_client.gui.community == 2) OSOpenBrowser("https://openttd.btpro.nl/contact-us/vip-membership");
				else this->DisableWidget(widget);
				break;
			case WID_SGI_CC_USER_ENTER: // Enter User name
				this->query_widget = SGQ_CC_USER;
				SetDParamStr(0, _settings_client.network.community_user[_settings_client.gui.community-1]);
				ShowQueryString(STR_CC_USER_WHITE, STR_CC_USER_ENTER, 40, this, CS_ALPHANUMERAL, QSF_NONE);
				break;
			case WID_SGI_CC_PASSWORD: // Enter user Password
				this->query_widget = SGQ_CC_PASSWORD;
				ShowQueryString(STR_EMPTY, STR_CC_PASSWORD_ENTER, 40, this, CS_ALPHANUMERAL, QSF_NONE);
				break;
			// directly to server #number
			default:
				if(widget >= WID_SGI_CC1){
					if (GetCommunityServer(widget - WID_SGI_CC1 + 1)) {
						if (_ctrl_pressed) {
							NetworkClientConnectGame(_cc_address, _cc_porti, COMPANY_NEW_COMPANY);
						} else {
							NetworkClientConnectGame(_cc_address, _cc_porti, COMPANY_SPECTATOR);
						}
                        delete _cc_address;
					}
				} else {
					ShowErrorMessage(STR_CC_SERVER_DISABLED, INVALID_STRING_ID, WL_ERROR);
				}
				break;
		}
	}
};

NWidgetBase *MakeServerButtons(int *biggest_index)
{
	NWidgetVertical *ver = new NWidgetVertical;
    _server_list_text_names = _server_list_text;
	if(_settings_client.gui.community == 0 || _server_list_text.empty()){
		NWidgetBackground *leaf = new NWidgetBackground(WWT_PANEL, COLOUR_BROWN, WID_SGI_SERVERS, NULL);		
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
			NWidgetSpacer *spce = new NWidgetSpacer(8, 0);
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
		NWidgetBackground *leaf = new NWidgetBackground(WWT_PANEL, COLOUR_ORANGE, WID_SGI_CC1 + i, NULL);
		if(aactive[i] == 0) leaf->SetDisabled(true);
		leaf->SetDataTip(STR_NETWORK_DIRECT_JOIN_GAME, STR_NETWORK_DIRECT_JOIN_GAME_TOOLTIP);
		leaf->SetMinimalSize(90, 15);
		hor->Add(leaf);
		i1++;
	}
	
	/* arrange buttons @ last line */
	if (i2==0) i2=380;
	if (i2==1) i2=287;
	if (i2==2) i2=194;
	if (i2==3) i2=101;
	if (i2==4) i2=8;
	NWidgetSpacer *spce = new NWidgetSpacer(i2, 0);
	spce->SetFill(1, 0);
	hor->Add(spce);			
	ver->Add(hor);
	*biggest_index = WID_SGI_CC1 - 1 + active;
	return ver;
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
	NWidget(NWID_SELECTION, INVALID_COLOUR, WID_SGI_BASESET_SELECTION),
		NWidget(NWID_VERTICAL),
			NWidget(WWT_EMPTY, COLOUR_ORANGE, WID_SGI_BASESET), SetMinimalSize(316, 12), SetFill(1, 0), SetPadding(0, 10, 7, 10),
		EndContainer(),
	EndContainer(),
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

	// community servers
	NWidget(WWT_CAPTION, COLOUR_BROWN, WID_SGI_CC_HEADER), SetDataTip(STR_NETWORK_SERVER_LIST_CC_HEADER, STR_CC_SERVERS_TOOLTIP),
	NWidget(WWT_PANEL, COLOUR_BROWN),
	NWidget(NWID_SPACER), SetMinimalSize(0, 3),
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
		NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(20, 12), SetDataTip(STR_CC_USER_TEXT, STR_NULL),
		NWidget(WWT_PUSHTXTBTN, COLOUR_BROWN, WID_SGI_CC_USER_ENTER), SetMinimalSize(15, 12), SetDataTip(STR_CC_USER_CHANGE, STR_CC_USER_CHANGE_TOOLTIP),
		NWidget(NWID_SPACER), SetMinimalSize(3, 0),
		NWidget(WWT_TEXT, COLOUR_ORANGE, WID_SGI_CC_USER), SetMinimalSize(160, 12), SetDataTip(STR_CC_USER_WHITE, STR_NULL),
		NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(20, 12), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
		NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(20, 12), SetDataTip(STR_CC_PASSWORD, STR_NULL),
		NWidget(WWT_PUSHTXTBTN, COLOUR_BROWN, WID_SGI_CC_PASSWORD), SetMinimalSize(15, 12), SetDataTip(STR_CC_PASSWORD_CHANGE, STR_CC_PASSWORD_CHANGE_TOOLTIP),
		NWidget(NWID_SPACER), SetMinimalSize(175, 0),
		NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(20, 12), SetDataTip(STR_CC_BUILD, STR_NULL),
		NWidget(NWID_SPACER), SetMinimalSize(3, 0),
	EndContainer(),
	NWidget(NWID_SPACER), SetMinimalSize(0, 6),
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
		NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_WEBSITE), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_CC_WEBSITE, STR_NETWORK_SERVER_LIST_JOIN_GAME_CC_WEBSITE_TOOLTIP),
		NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_SGI_CC_SELECT_NICE), SetMinimalSize(235, 15), SetDataTip(STR_NETWORK_CC_SELECT_NICE, STR_NETWORK_CC_SELECT_NICE_TOOLTIP),
			NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_SGI_CC_SELECT_BTPRO), SetMinimalSize(235, 15), SetDataTip(STR_NETWORK_CC_SELECT_BTPRO, STR_NETWORK_CC_SELECT_BTPRO_TOOLTIP),
			NWidget(NWID_SPACER), SetMinimalSize(5, 0),
	EndContainer(),
	NWidget(NWID_SPACER), SetMinimalSize(0, 4),
	NWidget(NWID_HORIZONTAL, NC_EQUALSIZE),
		NWidget(NWID_VERTICAL, NC_EQUALSIZE), SetFill(1, 0),
			NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_SERVERS_FORUM), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_CC_SERVERS, STR_NETWORK_SERVER_LIST_JOIN_GAME_CC_SERVERS_TOOLTIP),
				NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			EndContainer(),
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_IRC_CHAT), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_IRC_CHAT, STR_NETWORK_SERVER_LIST_JOIN_GAME_IRC_CHAT_TOOLTIP),
				NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			EndContainer(),
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_IRC_SERVERS_CHAT), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_IRC_SERVERS_CHAT, STR_NETWORK_SERVER_LIST_JOIN_GAME_IRC_SERVERS_CHAT_TOOLTIP),
				NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			EndContainer(),
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_SERVER_RULES), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_SERVER_RULES, STR_NETWORK_SERVER_LIST_JOIN_GAME_SERVER_RULES_TOOLTIP),
				NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			EndContainer(),
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_SERVER_WIKI), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_SERVER_WIKI, STR_NETWORK_SERVER_LIST_JOIN_GAME_SERVER_WIKI_TOOLTIP),
				NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			EndContainer(),
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			NWidget(NWID_HORIZONTAL, NC_EQUALSIZE), SetFill(1, 0), SetPIP(5, 3, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_ORANGE, WID_SGI_SERVER_VIP), SetMinimalSize(55, 15), SetDataTip(STR_NETWORK_SERVER_LIST_JOIN_GAME_SERVER_VIP, STR_NETWORK_SERVER_LIST_JOIN_GAME_SERVER_VIP_TOOLTIP),
				NWidget(WWT_TEXT, COLOUR_ORANGE), SetMinimalSize(10, 0), SetDataTip(STR_CC_SEPARATOR1, STR_NULL),
			EndContainer(),
		EndContainer(),
		NWidgetFunction(MakeServerButtons),
	EndContainer(),
	NWidget(NWID_SPACER), SetMinimalSize(0, 6),
};

static WindowDesc _select_game_desc(
	WDP_CENTER, nullptr, 0, 0,
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
	ShowQuery(
		STR_QUIT_CAPTION,
		STR_QUIT_ARE_YOU_SURE_YOU_WANT_TO_EXIT_OPENTTD,
		nullptr,
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
		nullptr,
		AskExitToGameMenuCallback
	);
}
