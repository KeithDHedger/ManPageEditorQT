/*
 *
 * ©K. D. Hedger. Fri 10 Oct 16:51:35 BST 2025 keithdhedger@gmail.com

 * This file (QT_menuitem.cpp) is part of ManPageEditorQT.

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

MenuItemClass::~MenuItemClass()
{
}

MenuItemClass::MenuItemClass(const QString &text, QWidget *parent): QAction(parent)
{
	this->menuID=0;
	this->menuString=nullptr;
	this->setText(text);
}

void MenuItemClass::setMenuID(unsigned int id)
{
	this->menuID=id;
}

unsigned int MenuItemClass::getMenuID(void)
{
	return(this->menuID);
}

void MenuItemClass::setMenuString(QString str)
{
	this->menuString=str;
}

QString MenuItemClass::getMenuString(void)
{
	return(this->menuString);
}

void MenuItemClass::setAppearance(QString iconname,QString actiontext,QString keys)
{
	this->setIcon(QIcon::fromTheme(iconname));
	this->setText(actiontext);
	this->setShortcut(QKeySequence::fromString(keys));
}

