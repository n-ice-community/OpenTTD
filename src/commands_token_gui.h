/* $Id: commands_token_gui.h 23663 2011-12-23 18:01:50  me $ */

/** @file commands_token_gui.h GUI related to community server commands. */

#ifndef COMMANDS_TOKEN_GUI_H
#define COMMANDS_TOKEN_GUI_H

#include "window_gui.h"
#include "company_base.h"

void ShowCCCommands();
NWidgetBase *MakeServerButtons1(int *biggest_index);
bool GetServerInfo(int number, bool findonly);

#endif // COMMANDS_TOKEN_GUI_H
