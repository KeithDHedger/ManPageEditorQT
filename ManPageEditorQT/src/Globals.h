/*
 *
 * ©K. D. Hedger. Sat  2 Mar 13:40:59 GMT 2024 keithdhedger@gmail.com

 * This file (Globals.h) is part of ManPageEditorQT.

 * ManPageEditorQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * ManPageEditorQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ManPageEditorQT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _GLOBALS_
#define _GLOBALS_

#include <QUrl>

#include "qtincs.h"
#include "internet.h"

class runExternalProcClass;
class prefsClass;
class LFSTK_prefsClass;
class AboutBoxClass;
class NoteBookClass;
class ManPageEditorQT;
class MenuItemClass;
class ManpageConvertClass;

#define MARGINS 4

struct manProps
{
	QString manString=R"foo("myprogram" "1" "0.0.0" "Me" "My set of programs")foo";
	QString name="myprogram";
	QString section="1";
	QString version="0.0.0";
	QString author="Me";
	QString catagory="My set of programs";
};

#include "LFSTKPrefsClass.h"
#include "runExternalProc.h"
#include "prefsClass.h"
#include "QT_AboutBox.h"
#include "ManPageEditorQT.h"
#include "QT_notebook.h"
#include "QT_menuitem.h"
#include "Callbacks.h"
#include "ManpageConvert.h"

extern ManPageEditorQT	*mpclass;

#endif
