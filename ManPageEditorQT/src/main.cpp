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
	QPixmap			pixmap(DATADIR "/pixmaps/KKEditQT.png");

	app.setOrganizationName("KDHedger");
	app.setApplicationName("ManPageEditorQT");

	mpclass=new ManPageEditorQT(&app);
	mpclass->mpConv->manString=mpclass->getProperties();
	if(argc>1)
		mpclass->mpConv->importManpage(argv[1]);
	status=app.exec();

	delete mpclass;
	return status;
}