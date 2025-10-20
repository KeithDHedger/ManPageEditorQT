/*
 *
 * ©K. D. Hedger. Sat  2 Mar 13:30:37 GMT 2024 keithdhedger@gmail.com

 * This file (ManPageEditorQT.h) is part of ManPageEditorQT.

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

#ifndef _KKEDITCLASS_
#define _KKEDITCLASS_

#define STATUSBARTIMEOUT 5000

enum {FILEMENU=0x4000,EDITMENU,FORMATMENU,HELPMENU,NOMENU};

//file
enum {NEWMENUITEM=0x8000,NEWPAGEMENUITEM,OPENMENUITEM,OPENSYSPAGEMENUITEM,OPENTEMPLATEMENUITEM,SAVEMENUITEM,SAVEASMENUITEM,CLOSEPAGEMENUITEM,PROPSMENUITEM,PREVIEWMENUITEM,QUITMENUITEM};
//edit
enum {UNDOMENUITEM=0x9000,REDOMENUITEM,UNDOALLMENUITEM,REDOALLMENUITEM,EDSEP1,CUTMENUITEM,COPYMENUITEM,PASTEMENUITEM,DELETEMENUITEM,EDSEP2,SELECTALLMENUITEM,EDSEP3,FINDMENUITEM,FINDNEXTMENUITEM,EDSEP4,EDSEP5,PREFSMENUITEM};
//format
enum {BOLDMENUITEM=0xa000,ITALICMENUITEM,CLEARMENUITEM};

//help
enum {ABOUTMENUITEM=0xe000,ABOUTQTMENUITEM,HELPMENUITEM};

enum {FINDNEXT=1,FINDPREV,FINDREPLACE};

//od enums
enum {SPELLCHECKMENUITEM=0x2000,APPLYWORDBUTTON,IGNOREWORDBUTTON,ADDWORDBUTTON,CANCELSPELLCHECK,CANCELPREFS,DOLINEBOX,DOLIVESEARCH,DOAPISEARCH,DOQT5SEARCH,DOGTKSEARCH,DOCVIEWERGOHOME,TOOLSSAVE,TOOLSCANCEL,TOOLSDELETE,TOOLSSAVEAS,TOOLSEDIT};

enum {FRCASE=0,FRUSEREGEX,FRREPLACEFIND,FRWRAP,FRALLFILES,FRHIGHLIGHTALL,FRREPLACEALL,FRSEARCHBACK,FRMAXSWITCHES};

#include "Globals.h"

class ManPageEditorQT : public QObject
{
	public:
		ManPageEditorQT(QApplication *app);
		~ManPageEditorQT();

//app vars
		QMainWindow					*mainWindow;
		NoteBookClass				*mainNotebook=NULL;
		ManpageConvertClass			*mpConv=NULL;

		QApplication					*application;

		QString						homeFolder;
		QString						homeDataFolder;
		QString						tmpFolderName;
		QString						lastSaveDir="";
		QString						lastLoadDir="";
		QString						currentFilePath="";
		QString						terminalCommand="xterm -hold -e";
		QSettings					prefs;
		QTextEdit*					getDocumentForTab(int tabnum);

//app functions
		void							initApp(void);
		void							buildMainGui(void);
		void							readConfigs(void);
		void							setAppShortcuts(void);
//		void							setToolbarSensitive(void);
		void							writeExitData(void);
		bool							confirmClose(QTextEdit *te);
		QTextEdit*					makeNewTab(QString html,QString sectname,bool issub,int pos=-1);
		void							doPrefs(void);

//editor vars
		QStatusBar					*statusBar;
		QLabel						*statusText;
		bool							wrapSearch=true;
		bool							insensitiveSearch=true;
		bool							useRegex=false;
		bool							replaceAll=false;
		bool							findInAllFiles=false;
		bool							hightlightAll=true;
		bool							searchBack=false;
		bool							findAfterReplace=false;
		int							currentPage=0;
		bool							closingAllTabs=false;

		QString						fontName="MonoSpace";
		int							fontSize=10;
		bool							useUnderline=false;
//editor functions
//menubar
		QMenuBar						*menuBar;

//menu vars
//file menu
		QMenu						*fileMenu;
		MenuItemClass				*newMenuItem;
		MenuItemClass				*openSysPageMenuItem;
		MenuItemClass				*openTemplateMenuItem;
		MenuItemClass				*openMenuItem;
		MenuItemClass				*saveMenuItem;
		MenuItemClass				*saveAsMenuItem;
		MenuItemClass				*previewMenuItem;
		MenuItemClass				*propsMenuItem;
		MenuItemClass				*closeMenuItem;

//edit menu
		QMenu						*editMenu;
		MenuItemClass				*undoMenuItem;
		MenuItemClass				*redoMenuItem;
		MenuItemClass				*cutMenuItem;
		MenuItemClass				*copyMenuItem;
		MenuItemClass				*pasteMenuItem;
		MenuItemClass				*findMenuItem;

//formating menu
		QMenu						*formatMenu;
		MenuItemClass				*boldMenuItem;
		MenuItemClass				*italicMenuItem;
		MenuItemClass				*clearMenuItem;

//help menu
		QMenu						*helpMenu;

//menu functions
		MenuItemClass				*makeMenuItemClass(int mainmenu,const QString name,const QKeySequence key,const QString iconname,int userdata);

//toolbar vars
		QToolBar						toolBar;
		QLineEdit					*liveSearchWidget=NULL;

//toolbar functions
		void							setUpToolBar(void);

//file vars
//file functions
		QString						openFileDialog(QString title,QString dir);

//document vars
		struct manProps				pageProperties;
		QVector<QLineEdit*>			propBoxes;
//document functions
		QString						buildProperties(QString thstr=R"foo(.TH "myprogram" "1" "0.0.0" "Me" "My set of programs")foo");
		QString						getProperties(QString thstr=R"foo(.TH "myprogram" "1" "0.0.0" "Me" "My set of programs")foo");
		void							buildSectionProps(QString s,bool issubsec,bool create);

		void							doBold(void);
		void							doItalic(void);
		void							doClear(void);
		void							doPreView(void);

//prefswindow
//prefswindow vars
//prefswindow functions

//find replace vars
//		QWidget						*findReplaceDialog;
//		QComboBox					*findDropBox;
//		QComboBox					*replaceDropBox;
//		QCheckBox					*frSwitches[FRMAXSWITCHES];
//		QPushButton					*frReplace;
//		QStringList					findList;
//		QStringList					replaceList;
//		unsigned int					maxFRHistory=5;
//		int							currentTab=0;
//		QString						correctedFind;
//		QString						correctedReplace;

//find replace functions
//		void							buildFindReplace(void);
//		void							doFindReplace(int response_id);

	protected:
	private:
		void							doFileMenuItems(MenuItemClass *mc);
		void							doLiveSearch(const QString text="");
		void							doEditMenuItems(MenuItemClass *mc);
		void							doFormatMenuItems(MenuItemClass *mc);
		void							doHelpMenuItems(MenuItemClass *mc);
		bool							closeTabs(bool all);
};

#endif

