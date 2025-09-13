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

class NoteBookClass;
class ManPageEditorQT;
class MenuItemClass;
class ManpageConvertClass;

#include <QApplication>
#include <QMessageBox>
#include <QGuiApplication>
#include <QWidget>
#include <QMainWindow>
#include <QShortcut>
#include <QCommandLineParser>
#include <QSplashScreen>
#include <QTabWidget>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QPushButton>
#include <QTabBar>
#include <QMenuBar>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QDialog>
#include <QFileDialog>
#include <QTextEdit>
#include <QTextDocumentWriter>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QRegularExpression>
#include <QTextDocument>
#include <QProcess>
#include <QDebug>

#include <QSettings>
#include <QCompleter>
#include <QObject>

struct manProps
{
	QString manString=R"foo("myprogram" "1" "0.0.0" "Me" "My set of programs")foo";
	QString name="myprogram";
	QString section="1";
	QString version="0.0.0";
	QString author="Me";
	QString catagory="My set of programs";
};

#include "ManPageEditorQT.h"
#include "QT_notebook.h"
#include "QT_menuitem.h"
#include "Callbacks.h"
#include "ManpageConvert.h"



extern ManPageEditorQT	*mpclass;

#endif
