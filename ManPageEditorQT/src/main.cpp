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

	LFSTK_prefsClass	prefs(QString(PACKAGE_NAME).toLower().toStdString(),VERSION);
	option			long_options[]=
		{
			{"text-size",required_argument,NULL,'s'},
			{"as-underline",no_argument,NULL,'u'},
			{0,0,0,0}
		};

	std::string	configfile=getenv("HOME");
	configfile+="/.config/manpageeditqt.rc";

	prefs.prefsMap=
		{
			{prefs.LFSTK_hashFromKey("as-underline"),{TYPEBOOL,"as-underline","Show italic as underline ( as most terminals do )","",false,0}},
			{prefs.LFSTK_hashFromKey("text-size"),{TYPEINT,"text-size","Font size to use ( default 10 )","",false,10}},
		};

	prefs.LFSTK_loadVarsFromFile(configfile);
	if(prefs.LFSTK_argsToPrefs(argc,argv,long_options,true)==false)
		return(0);

	app.setOrganizationName("KDHedger");
	app.setApplicationName("PACKAGE_NAME");

	mpclass=new ManPageEditorQT(&app);
	mpclass->mpConv->manString=mpclass->getProperties();

	if(prefs.LFSTK_getBool("as-underline")==true)
		{
			mpclass->italicMenuItem->setAppearance("stock_underline","Underline","Ctrl+U");
			mpclass->useUnderline=true;
		}

	if(prefs.LFSTK_getInt("text-size")!=0)
		mpclass->fontSize=prefs.LFSTK_getInt("text-size");

	if(prefs.cliArgs.size()>0)
		mpclass->mpConv->importManpage(prefs.cliArgs.at(0).c_str());


	status=app.exec();

	delete mpclass;
	return status;
}