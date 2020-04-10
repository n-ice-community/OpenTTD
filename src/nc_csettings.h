#ifndef NC_CSETTINGS_H
#define NC_CSETTINGS_H

#include "nc_receiver.h"
#include "nc_util.h"
#include "stdafx.h"
#include "gfx_type.h"
#include "ini_type.h"
#include "network/network.h"
#include "network/network_func.h"
#include "misc/fixedsizearray.hpp"
#include <string>
#include <ctime>
// TODO better encapsulation
class Observer;

struct Server
{
	Server();
	NetworkAddress GetAddress();
	char* display_name;
	char* ipv4 ;
	char* ipv6 ;
	int port ;
	uint uid ;
};

class ServerList : public Receiver
{
private:
	const char *link ;
	const char *file;
	FixedSizeArray<Server*,50> data;
	time_t modified ;
	bool out_of_sync ;
public:
	Server *Get(const char*);
	Server *Get(uint i);
	Server *GetByUid(uint i);
	uint GetPosByUid(uint i);
	uint Size();
	void Load(IniFile*,bool);
	void Save();
	bool isOutOfSync();
	
	void CheckIntegrity();
	/*virtual*/void ReceiveModified(bool);
	virtual void DataReceived(std::string);
	virtual void Failed();
	ServerList(char *,char*);
};
/*struct Admin
{
	char* name = NULL;
	char* role = NULL;
};*/
struct ServerGroup
{
	const char* display_name;
	FixedSizeArray<const char*,5> tabs_name;
	FixedSizeArray<Colours,5> tabs_colour;
	FixedSizeArray<Colours,5> bg_colour;
	FixedSizeArray<uint,5> servers_within_group;
	FixedSizeArray<uint,50> server_uid;
	FixedSizeArray<Colours,50> server_colour;
	bool selected ;
	uint getServerUid(uint, uint);
	uint getServer(uint, uint);
	ServerGroup();
};
class ServerGroupList : public Receiver
{
private:
	const char *link ;
	const char *file ;
	FixedSizeArray<ServerGroup*,10> data;
	time_t modified;
	bool out_of_sync;
	int selected ;
public:
	ServerGroup *Get(uint i);
	ServerGroup *GetSelected();
	int SelectedIndex();
	const char* SelectedName();
	uint Size();
	void Load(IniFile*,bool);
	void Save();
	void SetSelected(int);
	bool isOutOfSync();
	
	void CheckIntegrity();
	/*virtual*/void ReceiveModified(bool);
	virtual void DataReceived(std::string);
	virtual void Failed();
	ServerGroupList(char *,char*);
};

enum LoginMethod
{
	TOKEN,
	TOKEN_MD5,
	PRV_TO_SRV,
};
class CSettings;

class Community : public Receiver
{
private:
	CSettings* _instance ; //XXX for real? yes
	char* linklist_url ;
	char* serverlist_url ;
	char* servergrouplist_url ;
	char* adminlist_url ;
	time_t modified ;
	bool out_of_sync ;
	
	ServerList* server_list ;
	ServerGroupList* servergroup_list ;
//	AdminList admin_list;
public:
	char* display_name ;
	char* group_name ;
	char* username;
	char* password;
	char* admin;
	char* adminpass;
	char* login_url;
	char* login_command;
	enum LoginMethod login_method;
	bool HasLoginCredentials();
	ServerList* GetServerList();
	ServerGroup* GetServerGroup();
	ServerGroupList* GetGroups();
	void SetGroup(int);
	void CheckIntegrity();
	bool ContainsIp(char*);
	
	void AttemptLogin();
	//
	Community(CSettings*);
	bool Load(IniGroup*,bool);
	void Load(const char*, const char*);
	void Save(IniFile*,bool);
	bool isOutOfSync();
	
	const char* GetName();
	long int LastModified();
	
	/*virtual*/void ReceiveModified(bool);
	virtual void DataReceived(std::string);
	virtual void Failed();
};


class CSettings 
{
private:
	char *community_file;
	FixedSizeArray<Community*,10> community_list ;
	Observer* observer; // TODO for now 1 is enough
	int selected;
	
	CSettings();
	CSettings(CSettings const&);
	void operator=(CSettings const&); 
	void CheckIntegrity();
public:
	static CSettings& get();

	bool isThereConnection();

	int Size();
	Community *GetSelected();
	void SetSelected(int i);
	const char* GetSelectedName();
	void SetSelectedByIP(char*);
	char *GetCommunityName(int);
	int Selected(); 
	
	void Regist(Observer*);
	void RegistUpdate();
	void UnRegist(Observer*);
	
	void LoadCommunities();
	void SaveCommunities();
	void AddCommunity(const char*, const char*);
};
#endif
