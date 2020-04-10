#include "nc_csettings.h"
#include "network/network_content.h"
#include "fileio_func.h"
#include "safeguards.h"

#if defined(WIN32) || defined(WIN64)
//http://stackoverflow.com/questions/8512958/is-there-a-windows-variant-of-strsep
char* strsep(char** stringp, const char* delim)
{
	char* start = *stringp;
	char* p;

	p = (start != NULL) ? strpbrk(start, delim) : NULL;

	if (p == NULL)
	{
		*stringp = NULL;
	}
	else
	{
		*p = '\0';
		*stringp = p + 1;
	}

	return start;
}
#endif
Server::Server() : display_name(NULL), ipv4(NULL), ipv6(NULL), port(0), uid(0)
{
}
NetworkAddress Server::GetAddress()
{
	return NetworkAddress(ipv4,port);
}

char* _community_dir;

CSettings& CSettings::get()
{
	static CSettings instance;
	return instance;
}
const char* CSettings::GetSelectedName()
{
	return selected == -1 ? "None" : community_list[selected]->display_name;
}
CSettings::CSettings()
{
	community_file = str_fmt("%scommunities.cfg", _community_dir);
	this->selected = -1;
	if(FileExists(community_file)) // Load from file
		LoadCommunities();
}

void CSettings::CheckIntegrity()
{
	for(uint i = 0; i < community_list.Length();i++)
		community_list[i]->CheckIntegrity();
}
void CSettings::AddCommunity(const char* name, const char* link)
{
	for(uint i = 0; i < community_list.Length() ; i++)
	{
		if(strcmp(community_list[i]->group_name,name)==0)
			return;
	}
	Community* temp = new Community(this);
	temp->Load(name,link);
	if(community_list.Length() == 1)
		this->selected = 1;
	*community_list.Append() = temp;
	this->SaveCommunities();
}
void CSettings::LoadCommunities()
{
	IniFile *ini = new IniFile();
	ini->LoadFromDisk(community_file, BASE_DIR);
	
	IniGroup* cur = ini->group;
	while(&cur != ini->last_group && cur != NULL)// TODO Verify if it is correct
	{
		Community* temp = new Community(this);
		if(temp->Load(cur,false))
			this->selected = community_list.Length();
		*community_list.Append() = temp;
		cur = cur->next;
	}
	delete ini;
}

void CSettings::SaveCommunities()
{
	IniFile *ini = new IniFile();
	ini->LoadFromDisk(community_file, BASE_DIR);

	for(int i = 0; i < community_list.Length() ; i++)
	{
		if(strcmp(community_list[i]->group_name , "Undefined") != 0)
			community_list[i]->Save(ini,this->selected == i);
	}
	ini->SaveToDisk(community_file);
	RegistUpdate();
	delete ini;
}

int CSettings::Size()
{
	return community_list.Length();
}
void CSettings::Regist(Observer* o)
{
	observer = o;
}
void CSettings::RegistUpdate()
{
	// TODO BETTER IMPLEMENTATION
	if(observer != NULL)
		observer->notify();
}
void CSettings::UnRegist(Observer* o)
{
	observer = NULL;
}
Community *CSettings::GetSelected()
{
	CheckIntegrity();
	return community_list.Length() == 0 || selected == -1 ? NULL : community_list[selected];
}
int CSettings::Selected()
{
	return this->selected;
}
void CSettings::SetSelected(int i)
{
	this->selected = (i <= ((int)(community_list.Length())-1)) ? i : ((int)(community_list.Length())-1);
	this->SaveCommunities();
}
void CSettings::SetSelectedByIP(char* ip)
{
	for(uint i = 0; i < community_list.Length() ; i++)
	{
		if(community_list[i]->ContainsIp(ip))
		{
			this->selected = i;
			this->SaveCommunities();
			return;
		}
	}
	this->selected = -1;
	return;
}
char *CSettings::GetCommunityName(int i)
{
	return community_list[i]->display_name;
}
Community::Community(CSettings* s) : _instance(NULL), linklist_url(NULL), serverlist_url(NULL),servergrouplist_url(NULL),adminlist_url(NULL),modified(0),out_of_sync(false),server_list(NULL),servergroup_list(NULL),display_name(NULL),group_name(NULL),username(NULL),password(NULL),admin(NULL),adminpass(NULL)
{
	this->_instance = s;
}

bool Community::ContainsIp(char* ip)
{
	if(server_list == NULL)
		return false;
	for(int i = 0; i < server_list->Size(); i++)
		if(strcmp(server_list->Get(i)->ipv4,ip)==0)
			return true;
	return false;
}
void Community::CheckIntegrity()
{
	// TODO For now assume it was moddified every 5 min
	// TODO Later implement If-Modified-Since: HTTP Request
	if((difftime(std::time(nullptr),this->modified) >= 300 /*5min*/ ))
	{
		if(!out_of_sync)
			ReceiveModified(true);
	}
	if(server_list != NULL) server_list->CheckIntegrity();
	if(servergroup_list != NULL) servergroup_list->CheckIntegrity();
}

void Community::ReceiveModified(bool mod)
{
	if(mod)
	{
		new SimpleTxtDownloader(this->linklist_url , this);
		this->out_of_sync = true;
	}	
}

void Community::DataReceived(std::string s)
{
	IniFile* ini = new IniFile();
	ini->LoadFromString(s);
	IniGroup* cur = ini->group;
	
	this->Load(cur,true);
	this->out_of_sync = false;
	_instance->SaveCommunities();
	delete ini;
}
void Community::Failed()
{
}

bool Community::isOutOfSync()
{
	return out_of_sync;
}

const char* Community::GetName()
{
	return display_name;
}
void Community::Load(const char* name, const char* link)
{
	this->display_name = this->group_name = stredup(name,NULL);
	this->linklist_url = stredup(link,NULL);
}
bool Community::Load(IniGroup* group,bool update)
{
	bool result = false; 
	IniItem* item = group->GetItem("linklist",false);
	this->group_name = stredup(group->name,NULL);
	if(item == NULL)
	{
		printf("Linked list not found");
		//Error . Invalid
		return false;
	}
	this->linklist_url = stredup(item->value,NULL);
	
	this->display_name = (item = group->GetItem("display_name",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	item = group->GetItem("serverlist",false);
	
	this->serverlist_url = (item = group->GetItem("serverlist",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	this->servergrouplist_url = (item = group->GetItem("servergroup",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	this->adminlist_url = (item = group->GetItem("adminlist",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	if((item = group->GetItem("modified",false)) == NULL)
		if(update)
			this->modified = std::time(nullptr);
		else
			this->modified = 0;
	else
		this->modified = static_cast<time_t>(atol(item->value));
	this->login_url = (item = group->GetItem("login_url",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	this->login_method = (item = group->GetItem("login_method",false)) == NULL || item->value == NULL ? TOKEN : static_cast<enum LoginMethod>(atoi(item->value));
	this->login_command = (item = group->GetItem("login_command",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	
	if(update)
		return false;
	if((item =group->GetItem("selected",false)) != NULL && strcmp(item->value,"true") == 0)
		result = true;
	this->username = (item = group->GetItem("username",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	this->password = (item = group->GetItem("password",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	this->admin = (item = group->GetItem("admin",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
	this->adminpass = (item = group->GetItem("adminpass",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);

	return result;
}

void Community::Save(IniFile* ini, bool selected)
{
	IniGroup* group = ini->GetGroup(this->group_name,0,true);
	if(this->display_name != NULL) group->GetItem("display_name",true)->SetValue(this->display_name);
	if(this->linklist_url != NULL) group->GetItem("linklist",true)->SetValue(this->linklist_url);
	if(this->serverlist_url != NULL) group->GetItem("serverlist",true)->SetValue(this->serverlist_url);
	if(this->servergrouplist_url != NULL) group->GetItem("servergroup",true)->SetValue(this->servergrouplist_url);
	if(this->adminlist_url != NULL) group->GetItem("adminlist",true)->SetValue(this->adminlist_url);
	if(this->username != NULL) group->GetItem("username",true)->SetValue(this->username);
	if(this->password != NULL) group->GetItem("password",true)->SetValue(this->password);
	if(this->admin != NULL) group->GetItem("admin",true)->SetValue(this->admin);
	if(this->adminpass != NULL) group->GetItem("adminpass",true)->SetValue(this->adminpass);
	if(this->login_url != NULL) group->GetItem("login_url",true)->SetValue(this->login_url);
	if(this->login_command != NULL) group->GetItem("login_command",true)->SetValue(this->login_command);

	char temp[32];
	seprintf(temp, lastof(temp),"%ld",this->modified);
	group->GetItem("modified",true)->SetValue(temp);
	seprintf(temp, lastof(temp),"%i",(int)this->login_method);
	group->GetItem("login_method",true)->SetValue(temp);
	group->GetItem("selected",true)->SetValue( selected ? "true": "false");
}

long int Community::LastModified()
{
	return modified;
}
ServerGroupList* Community::GetGroups()
{
	return servergroup_list;
}
void Community::SetGroup(int i)
{
	this->servergroup_list->SetSelected(i);
}

void Community::AttemptLogin()
{
	new Login();
}
ServerList* Community::GetServerList() 
{
	if(server_list == NULL && serverlist_url != NULL)
	{
		server_list = new ServerList(str_fmt("%s%s_serverlist.cfg",_community_dir,this->group_name),this->serverlist_url);
		server_list->Load(NULL,false);
	}
	return  serverlist_url == NULL ? NULL : server_list;
}
ServerGroup* Community::GetServerGroup()
{
	if(servergroup_list == NULL && servergrouplist_url != NULL)
	{
		servergroup_list = new ServerGroupList(str_fmt("%s%s_servergroup.cfg",_community_dir,this->group_name),this->servergrouplist_url);
		servergroup_list->Load(NULL,false);
	}
	return this->servergroup_list == NULL ? NULL: this->servergroup_list->GetSelected();
}
bool Community::HasLoginCredentials()
{
	if(username != NULL && password != NULL && strlen(username) > 1 && strlen(password) > 1)
		return true;
	return false;
}
ServerList::ServerList(char *file,char *link) : link(NULL), file(NULL), modified(0), out_of_sync(false)
{
	this->file = file;
	this->link = link;
}
void ServerList::CheckIntegrity()
{
	// TODO For now assume it was moddified every 5 min
	// TODO Later implement If-Modified-Since: HTTP Request
	this->isOutOfSync();
	if((difftime(std::time(nullptr),this->modified) >= 300 /*5min*/ ))
	{
		if(!out_of_sync)
			ReceiveModified(true);
	}
}

void ServerList::ReceiveModified(bool mod)
{
	if(mod)
	{
		new SimpleTxtDownloader(this->link, this);
		this->out_of_sync = true;
	}	
}

void ServerList::DataReceived(std::string s)
{
	IniFile* ini = new IniFile();
	ini->LoadFromString(s);
	this->Load(ini,true);
	this->Save();
	this->out_of_sync = false;
	delete ini;
}
void ServerList::Failed()
{
}

bool ServerList::isOutOfSync()
{
	return this->out_of_sync;
}

void ServerList::Load(IniFile* ini,bool update)
{
	data.Clear();
	if(!update)
	{
		ini = new IniFile();
		ini->LoadFromDisk(file,BASE_DIR);
	}else
	{
		this->modified = std::time(nullptr);
	}
	IniGroup* group = ini->group;
	IniItem* item;
	for(;&group != ini->last_group && group != NULL;group = group->next)
	{
		if(strcmp(group->name,"details") == 0)
		{
			if((item = group->GetItem("modified",false)) == NULL)
				this->modified = 0;
			else
				this->modified = static_cast<time_t>(atol(item->value));
			continue;
		}
		Server* temp = new Server();
		temp->display_name =(item = group->GetItem("display_name",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		temp->ipv4 = (item = group->GetItem("ipv4",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		temp->ipv6 = (item = group->GetItem("ipv6",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		temp->port = (item = group->GetItem("port",false)) == NULL || item->value == NULL ? 0 : atoi(item->value);
		temp->uid = (item = group->GetItem("uid",false)) == NULL || item->value == NULL ? 0 : atoi(item->value);
		*this->data.Append() = temp;
	}
}

void ServerList::Save()
{
	IniFile* ini = new IniFile();
	IniGroup* group = ini->GetGroup("details",0,true);
	char temp[32];
	seprintf(temp, lastof(temp),"%ld",this->modified);
	group->GetItem("modified",true)->SetValue(temp);
	for(uint i = 0; i < data.Length(); i++)
	{
		group = ini->GetGroup(data[i]->display_name,0,true);
		if(data[i]->display_name != NULL) group->GetItem("display_name",true)->SetValue(data[i]->display_name);
		if(data[i]->ipv4 != NULL) group->GetItem("ipv4",true)->SetValue(data[i]->ipv4);
		if(data[i]->ipv6 != NULL) group->GetItem("ipv6",true)->SetValue(data[i]->ipv6);
		char temp[5];
		seprintf(temp,lastof(temp),"%d",data[i]->port);
		group->GetItem("port",true)->SetValue(temp);
		seprintf(temp,lastof(temp),"%d",data[i]->uid);
		group->GetItem("uid",true)->SetValue(temp);
	}
	ini->SaveToDisk(file);
	CSettings::get().RegistUpdate();
}

Server *ServerList::Get(uint i)
{
	return i <= data.Length()? data[i] : NULL;
}
Server *ServerList::GetByUid(uint u)
{
	for(uint i = 0; i < data.Length(); i++)
	{
		if(u == data[i]->uid)
			return this->Get(i);
	}	
	return NULL;
}
uint ServerList::GetPosByUid(uint u)
{
	for(uint i = 0; i < data.Length(); i++)
	{
		if(u == data[i]->uid)
			return i;
	}	
	return 0;
}
uint ServerList::Size()
{
	return data.Length();
}
ServerGroupList::ServerGroupList(char *file,char *link) : link(NULL), file(NULL), modified(0), out_of_sync(false),selected(-1)
{
	this->file = file;
	this->link = link;
}
void ServerGroupList::CheckIntegrity()
{
	// TODO For now assume it was moddified every 5 min
	// TODO Later implement If-Modified-Since: HTTP Request
	this->isOutOfSync();
	if((difftime(std::time(nullptr),this->modified) >= 300 /*5min*/ ))
	{
		if(!out_of_sync)
			ReceiveModified(true);
	}
}

void ServerGroupList::ReceiveModified(bool mod)
{
	if(mod)
	{
		new SimpleTxtDownloader(this->link, this);
		this->out_of_sync = true;
	}	
}

void ServerGroupList::DataReceived(std::string s)
{
	IniFile* ini = new IniFile();
	ini->LoadFromString(s);
	this->Load(ini,true);
	this->Save();
	this->out_of_sync = false;
	delete ini;
}
void ServerGroupList::Failed()
{
}

bool ServerGroupList::isOutOfSync()
{
	return this->out_of_sync;
}

void ServerGroupList::Load(IniFile* ini,bool update)
{
	data.Clear();
	if(!update)
	{
		ini = new IniFile();
		ini->LoadFromDisk(file,BASE_DIR);
	}else
	{
		this->modified = std::time(nullptr);
	}
	IniGroup* group = ini->group;
	IniItem* item;
	for(;&group != ini->last_group && group != NULL;group = group->next)
	{
		if(strcmp(group->name,"details") == 0)
		{
			if((item = group->GetItem("modified",false)) == NULL)
				this->modified = 0;
			else
				this->modified = static_cast<time_t>(atol(item->value));
			continue;
		}
		ServerGroup* temp = new ServerGroup();
		temp->display_name =(item = group->GetItem("display_name",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		temp->selected =((item = group->GetItem("selected",false)) == NULL || item->value == NULL) ? false : (strcmp(item->value,"true") == 0) ? true: false;
		if(temp->selected)
			this->selected = data.Length();
		char *token = (item = group->GetItem("tabs",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		while(token != NULL && token != '\0')
		{
			*temp->tabs_name.Append() = strsep(&token,"~");
		}
		token = (item = group->GetItem("tabscolour",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		while(token != NULL && token != '\0')
		{
			*temp->tabs_colour.Append() = (Colours)(atoi(strsep(&token,"~")));
		}
		token = (item = group->GetItem("bgcolour",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		while(token != NULL && token != '\0')
		{
			*temp->bg_colour.Append() = (Colours)(atoi(strsep(&token,"~")));
		}
		int count;
		char *ctk = (item = group->GetItem("buttons",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		while(ctk != NULL && ctk != '\0')
		{
			token = strsep(&ctk,"~");
			count = 0;
			while(token != NULL && token != '\0')
			{
				*temp->server_uid.Append() = atoi(strsep(&token,"^"));
				count++;
			}
			*temp->servers_within_group.Append() = count;
		}
		ctk = (item = group->GetItem("buttonscolour",false)) == NULL || item->value == NULL ? '\0' : stredup(item->value,NULL);
		while(ctk != NULL && ctk != '\0')
		{
			token = strsep(&ctk,"~");
			while(token != NULL && token != '\0')
			{
				*temp->server_colour.Append() = (Colours)(atoi(strsep(&token,"^")));
			}
		}
		*this->data.Append() = temp;
	}
}

void ServerGroupList::Save()
{
	IniFile* ini = new IniFile();
	IniGroup* group = ini->GetGroup("details",0,true);
	char temp[32];
	seprintf(temp, lastof(temp),"%ld",this->modified);
	group->GetItem("modified",true)->SetValue(temp);
	char info[512];
	uint length = 0;
	for(uint i = 0; i < data.Length(); i++)
	{
		group = ini->GetGroup(data[i]->display_name,0,true);
		if(data[i]->display_name != NULL) group->GetItem("display_name",true)->SetValue(data[i]->display_name);
		group->GetItem("selected",true)->SetValue((data[i]->selected ? "true" : "false"));
		if(data[i]->tabs_name.Length() != 0)
		{
			length = 0;
			for(uint j = 0; j < data[i]->tabs_name.Length(); j++)
			{
				length += seprintf(info+length, lastof(info), "%s~",data[i]->tabs_name[j] );
			}
			info[length-1] = '\0';
			group->GetItem("tabs",true)->SetValue(stredup(info));
		}
		if(data[i]->tabs_colour.Length() != 0)
		{
			length = 0;
			for(uint j = 0; j < data[i]->tabs_colour.Length(); j++)
			{
				length += seprintf(info+length,lastof(info), "%i~",(int)data[i]->tabs_colour[j]);
			}
			info[length-1] = '\0';
			group->GetItem("tabscolour",true)->SetValue(stredup(info));
		}
		if(data[i]->bg_colour.Length() != 0)
		{
			length = 0;
			for(uint j = 0; j < data[i]->bg_colour.Length(); j++)
			{
				length += seprintf(info+length,lastof(info), "%i~",(int)data[i]->bg_colour[j]);
			}
			info[length-1] = '\0';
			group->GetItem("bgcolour",true)->SetValue(stredup(info));
		}
		if(data[i]->server_uid.Length() != 0)
		{
			length = 0;
			for(uint j = 0,  k = 0, t = data[i]->servers_within_group[0]; j < data[i]->server_uid.Length(); j++)
			{
				if(j == t)
				{
					info[length-1] = '~';
					t += data[i]->servers_within_group[++k];
				}
				length += seprintf(info+length,lastof(info), "%i^",data[i]->server_uid[j]);
			}
			info[length-1] = '\0';
			group->GetItem("buttons",true)->SetValue(stredup(info));
		}
		if(data[i]->server_colour.Length() != 0)
		{
			length = 0;
			for(uint j = 0, k = 0,t = data[i]->servers_within_group[0]; j < data[i]->server_colour.Length(); j++)
			{
				if(j == t)
				{
					info[length-1] = '~';
					t += data[i]->servers_within_group[++k];
				}
				length += seprintf(info+length,lastof(info), "%i^",(int)data[i]->server_colour[j]);
			}
			info[length-1] = '\0';
			group->GetItem("buttonscolour",true)->SetValue(stredup(info));
		}
	}
	ini->SaveToDisk(file);
	CSettings::get().RegistUpdate();
}

ServerGroup *ServerGroupList::Get(uint i)
{
	return i <= data.Length()? data[i] : NULL;
}

ServerGroup *ServerGroupList::GetSelected()
{
	if(selected < 0)
		return NULL;
	return data[selected];
}
int ServerGroupList::SelectedIndex()
{
	return this->selected;
}
const char* ServerGroupList::SelectedName()
{
	if(this->selected < 0)	
		return "None";
	return data[selected]->display_name;
}
void ServerGroupList::SetSelected(int i)
{
	this->selected = i;
	for(uint j = 0; j <  data.Length(); j++) // TODO Remove redundancy
	{
		data[j]->selected = false; 
	}
	if(i >= 0)
		data[i>data.Length() ? data.Length():i]->selected = true; 
	this->Save();
}

uint ServerGroupList::Size()
{
	return data.Length();
}
ServerGroup::ServerGroup() : selected(false)
{
}
uint ServerGroup::getServerUid(uint g, uint s)
{
	int t = 0;
	for(uint i = 0; i < g; i++)
		t += this->servers_within_group[i];
	return this->server_uid[t+s];
	}
uint ServerGroup::getServer(uint g, uint s)
{
	int t = 0;
	for(uint i = 0; i < g; i++)
		t += this->servers_within_group[i];
	return t+s;
}
