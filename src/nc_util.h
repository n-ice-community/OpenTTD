#ifndef NC_UTIL_H
#define NC_UTIL_H

#include "nc_receiver.h"
#include "nc_csettings.h"
class Community;

class Login: public Receiver
{
	void DataReceived(std::string);
	void Failed();
	Community* selected;
public:
	Login();
};
class Observer
{
public:
	virtual void notify() = 0;
};
#endif
