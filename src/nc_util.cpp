#include "nc_util.h"
#include "stdafx.h"
#include "network/network_content.h"
#include "error.h"
#include "strings_func.h"
#include "table/strings.h"
#include "network/network.h"
#include "network/network_func.h"
#include "safeguards.h"
std::string char2hex( char dec )
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
	if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=97-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

std::string urlencode(const std::string &c)
{
	std::string escaped="";
	int max = c.length();
	for(int i=0; i<max; i++)
	{
		if ( (48 <= c[i] && c[i] <= 57) ||//0-9
		(65 <= c[i] && c[i] <= 90) ||//abc...xyz
		(97 <= c[i] && c[i] <= 122) || //ABC...XYZ
		(c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
		)
		{
			escaped.append( &c[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
		}
	}
	return escaped;
}

Login::Login()
{
	selected = CSettings::get().GetSelected();
	
	if(selected == NULL || selected->username == NULL || selected->password == NULL )
	{
		ShowErrorMessage(STR_NC_NO_USER_OR_PASS, INVALID_STRING_ID, WL_ERROR);
		delete this; // XXX is this safe in the contructor?
		return;
	}
	switch(selected->login_method)
	{
		case TOKEN:
		case TOKEN_MD5:
		{
			std::string username(selected->username);
			std::string password(selected->password);
			username = urlencode(username);
			password = urlencode(password);
			char addr[512];
			seprintf(addr, lastof(addr), selected->login_url, username.c_str(), password.c_str());
			new SimpleTxtDownloader(addr,this);
		}break;
		case PRV_TO_SRV:
		{
			char msg[512];
			seprintf(msg, lastof(msg), selected->login_command , selected->username, selected->password);
			NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , msg );
		}
	}
}
void Login::DataReceived(std::string token)
{
	if(selected != CSettings::get().GetSelected() || !(selected->login_method == TOKEN || selected->login_method == TOKEN_MD5))
		return;
	if(token.length() == 4) 
	{
		char b[16];
		seprintf(b, lastof(b), selected->login_command, token.c_str());
		NetworkClientSendChat(NETWORK_ACTION_CHAT_CLIENT, DESTTYPE_CLIENT, CLIENT_ID_SERVER , b);
	}else 
		ShowErrorMessage(STR_NC_OTHER_TOKEN_LOGIN_ERROR, INVALID_STRING_ID, WL_ERROR);
		
	delete this;
}
void Login::Failed()
{
	ShowErrorMessage(STR_NC_OTHER_TOKEN_LOGIN_ERROR, INVALID_STRING_ID, WL_ERROR);
}
