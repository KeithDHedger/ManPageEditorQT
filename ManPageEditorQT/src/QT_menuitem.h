/*
 *
 * ©K. D. Hedger. Fri 10 Oct 16:51:46 BST 2025 keithdhedger@gmail.com

 * This file (QT_menuitem.h) is part of ManPageEditorQT.

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

#ifndef _QT_MENUITEM_
#define _QT_MENUITEM_

#include "Globals.h"

class MenuItemClass : public QAction
{
	public:
		explicit MenuItemClass(const QString &text, QWidget *parent = 0);
		~MenuItemClass();
		ManPageEditorQT	*mainManPageEditorQTClass=NULL;

		void				setMenuID(unsigned int id);
		unsigned int 	getMenuID(void);
		void				setMenuString(QString str);
		QString			getMenuString(void);
		void				setAppearance(QString iconname,QString actiontext,QString keys);

	private:
		int				menuID;
		QString			menuString;
};

#endif

