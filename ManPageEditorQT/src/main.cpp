/*
 *
 * ©K. D. Hedger. Wed 10 Sep 15:12:29 BST 2025 keithdhedger@gmail.com

 * This file (main.cpp) is part of ManPageEditorQT.

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

#include "Globals.h"

int main(int argc, char **argv)
{
	int				status;
	QApplication		app(argc,argv);
	prefsClass		newprefs;
	stringTuple		st;
	boolTuple		bt;

	LFSTK_prefsClass	prefs(QString(PACKAGE_NAME).toLower().toStdString(),VERSION);
	option			long_options[]=
		{
			{"showsyspage",no_argument,NULL,'p'},
			{0,0,0,0}
		};

	prefs.prefsMap=
		{
			{prefs.LFSTK_hashFromKey("showsyspage"),{TYPEBOOL,"showsyspage","Open system manpage dialog at start","",false,0}},
		};

	if(prefs.LFSTK_argsToPrefs(argc,argv,long_options,true)==false)
		return(0);

	app.setOrganizationName("KDHedger");
	app.setApplicationName(PACKAGE_NAME);
	app.setApplicationVersion(PACKAGE_VERSION);

	mpclass=new ManPageEditorQT(&app);
	mpclass->mpConv->manString=mpclass->getProperties();

	st=newprefs.getStringValue("main_font");
	if(st.valid==true)
		{
			QFont fnt;
			fnt.fromString(st.value);
			mpclass->fontName=fnt.family();
			mpclass->fontSize=fnt.pointSize();
		}

	st=newprefs.getStringValue("highlight_colour");
	if(st.valid==true)
		mpclass->hiliteColour=st.value;

	st=newprefs.getStringValue("teminal_command");
	if(st.valid==true)
		mpclass->terminalCommand=st.value;

	bt=newprefs.getBoolValue("italic_as_underline");
	if(bt.valid==true)
		{
			if(bt.value==true)
				mpclass->italicMenuItem->setAppearance("format-text-underline","Underline","Ctrl+U");
			else
				mpclass->italicMenuItem->setAppearance("format-text-italic","Italic","Ctrl+I");
			mpclass->useUnderline=bt.value;
		}

	bt=newprefs.getBoolValue("wrap_lines");
		if(bt.valid==true)
			{
				if(bt.value==true)
					mpclass->lineWrap=QTextEdit::WidgetWidth;
				else
					mpclass->lineWrap=QTextEdit::NoWrap;
			}

	if(prefs.LFSTK_getBool("showsyspage")==true)
		{
			QString filepath;
			filepath=mpclass->mpConv->buildOpenSystemPage();
			if(filepath.isEmpty()==false)
				{
					mpclass->mpConv->importManpage(filepath);
				}
		}
	else
		if(prefs.cliArgs.size()>0)
			mpclass->mpConv->importManpage(prefs.cliArgs.at(0).c_str());

	status=app.exec();

	delete mpclass;
	return status;
}