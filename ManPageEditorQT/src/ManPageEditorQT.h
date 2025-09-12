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

enum {FILEMENU=0x4000,EDITMENU,FORMATMENU,NAVMENU,FUNCTIONSMENU,BOOKMARKSMENU,TOOLSMENU,PLUGINSMENU,HELPMENU,SAVESESSIONSMENU,RESTORESESSIONSMENU,CURRENTSESSION,NOMENU};

//file
enum {NEWMENUITEM=0x8000,OPENMENUITEM,HEXDUMPMENUITEM,NEWADMINEDMENUITEM,NEWEDMENUITEM,MANPAGEEDMENUITEM,BUILDDOCSMENUITEM,SAVEMENUITEM,SAVEASMENUITEM,SAVEALLMENUITEM,SAVESESSIONMENUITEM,RESTORESESSIONMENUITEM,PRINTMENUITEM,CLOSEMENUITEM,CLOSEALLMENUITEM,REVERTMENUITEM,QUITMENUITEM};
//edit
enum {UNDOMENUITEM=0x9000,REDOMENUITEM,UNDOALLMENUITEM,REDOALLMENUITEM,EDSEP1,CUTMENUITEM,COPYMENUITEM,PASTEMENUITEM,DELETEMENUITEM,EDSEP2,SELECTALLMENUITEM,EDSEP3,FINDMENUITEM,FINDNEXTMENUITEM,EDSEP4,SORTTABSMENUITEM,SHOWALLTABSMENUITEM,SELECTTABMENUITEM,EDSEP5,PREFSMENUITEM,PLUGPREFSMENUITEM};
//format
enum {BOLDMENUITEM=0xa000,ITALICMENUITEM,CLEARMENUITEM};
//nav
enum {GOTODEFINEMENUITEM=0xb000,OPENINCLUDEMENUITEM,GOTOLINEMENUITEM,SEARCHFORDEFINEMENUITEM,SEARCHGTKDOCS,SEARCHQT5DOCS,SEARCHDOXYDOCS,GOBACKMENUITEM,GOFORWARDMENUITEM};
//func
//bms
enum {REMOVEALLBOOKMARKSMENUITEM=0xc000,TOGGLEBOOKMARKMENUITEM,REMOVEBOOKMARKSFROMPAGE};

//tools
enum {MANAGETOOLSMENUITEM=0xd000,TOOLNUMBER};
//plugins
//help
enum {ABOUTMENUITEM=0xe000,ABOUTQTMENUITEM,HELPMENUITEM};

enum	{AUTOINDENT=0,SHOWNUMS,WRAP,HIGHLIGHT,SYNTAXHILITE,USESINGLE,AUTOSAVE,NODUPLICATE,NOWARN,AUTOSHOW,BEKIND,MAXPREFSWIDGETS};
enum {MAXTABCHARS=0,MAXHISTORY,MAXFUNCDEPTH,COMPLETIONSIZE,TABWIDTH,MENUWIDTH,MAXRECENTS,MSGCHECKTIME,MAXPREFSINTWIDGETS};
enum {FUNCTIONCOMBO=0,THEMECOMBO,FONTNAMECOMBO,PREFSTERMCOMMAND,PREFSPRINTCOMMAND,PREFSROOTCOMMAND,PREFSQTDOCDIR,PREFSCURRENTFONT,BMHIGHLIGHTCOLOUR,CURRENTLINECOLOUR,SHORTCUTSCOMBO,PREFSMENUSTYLE,MAXPREFSOTHERWIDGETS};

enum {FINDNEXT=1,FINDPREV,FINDREPLACE};

//od enums
enum {SPELLCHECKMENUITEM=0x2000,APPLYWORDBUTTON,IGNOREWORDBUTTON,ADDWORDBUTTON,CANCELSPELLCHECK,CANCELPREFS,DOLINEBOX,DOLIVESEARCH,DOAPISEARCH,DOQT5SEARCH,DOGTKSEARCH,DOCVIEWERGOHOME,TOOLSSAVE,TOOLSCANCEL,TOOLSDELETE,TOOLSSAVEAS,TOOLSEDIT};

struct tabMenuStruct
{
	unsigned int		what;
	const char		*label;
	const char		*icon;
};

enum {COPYFOLDERPATH=0xf000,COPYFILEPATH=0xf100,COPYFILENAME=0xf200,SPELLCHECKDOC=0xf300,SRCHILTIE=0xf400,HIDETAB=0xf500,LOCKCONTENTS=0xf600,OPENFROMHERE=0xf700};

enum {TABCONTEXTMENUCNT=(OPENFROMHERE-COPYFOLDERPATH) / 0x100 +1};

enum {NONESRCCODE=0,CPPSRCCODE=0x100,CSRCCODE=0x200,BASHSRCCODE=0x300,PYTHONSRCCODE=0x400,GOSRCCODE=0x500,LUASRCCODE=0x600,YAMLSRCCODE=0x700,PHPSRCCODE=0x800,XMLSRCCODE=0x900,CSSSRCCODE=0xa00,JSSRCCODE=0xb00,MAKESRCCODE=0xc00};

enum {FRCASE=0,FRUSEREGEX,FRREPLACEFIND,FRWRAP,FRALLFILES,FRHIGHLIGHTALL,FRREPLACEALL,FRSEARCHBACK,FRMAXSWITCHES};

//class ThemeClass;
//class DocumentClass;
//class TabColours;
//class NoteBookClass;
//class MenuItemClass;
//class RecentMenuClass;
//class HistoryClass;
//class ToolBarClass;
//class kkEditQTPluginInterface;


//#include "kkedit-includes.h"
#include "Globals.h"
//#include "QT_themeClass.h"
//#include "QT_AboutBox.h"
//#include "QT_menuitem.h"
//#include "QT_historyClass.h"
//#include "QT_recentMenu.h"
//#include "QT_highlighter.h"
//#include "QT_document.h"
//#include "QT_toolbar.h"
//#include "QT_notebook.h"
//
//#include "kkeditqtPluginInterface.h"
//
//struct pluginStruct
//{
//	QPluginLoader					*pluginLoader=NULL;
//	QString							plugPath="";
//	QString							plugName="";
//	QString							plugVersion="";
//	kkEditQTPluginInterface			*instance=NULL;
//	unsigned int						wants=DONONE;
//	bool								loaded=false;
//	bool								statusChanged=false;
//	bool								broken=false;
//	bool								local=false;
//#ifdef _DEBUGCODE_
//	void				printIt(void)
//		{
//		//	qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{message}");
//			qDebug() << "pluginLoader" << pluginLoader;
//			qDebug() << "plugPath" << plugPath;
//			qDebug() << "plugName" << plugName;
//			qDebug() << "plugVersion" << plugVersion;
//			qDebug() << "instance" << instance;
//			qDebug() << "loaded" << loaded;
//			qDebug() << "statusChanged" << statusChanged;
//			qDebug() << "broken" << broken;
//
//			switch(wants)
//				{
//					case DONONE:
//						qDebug() << "wants DONONE";
//						break;
//					case DOSAVE:
//						qDebug() << "wants DOSAVE";
//						break;
//					case DOLOAD:
//						qDebug() << "wants DOLOAD";
//						break;
//					case DOCLOSE:
//						qDebug() << "wants DOCLOSE";
//						break;
//					case DORESSESSION:
//						qDebug() << "wants DORESSESSION";
//						break;
//					case DOSAVESESSION:
//						qDebug() << "wants DOSAVESESSION";
//						break;
//					case DOCONTEXTMENU:
//						qDebug() << "wants DOCONTEXTMENU";
//						break;
//					case DOTABPOPUP:
//						qDebug() << "wants DOTABPOPUP";
//						break;
//					case DOSETSENSITVE:
//						qDebug() << "wants DOSETSENSITVE";
//						break;
//					case DOSWITCHPAGE:
//						qDebug() << "wants DOSWITCHPAGE";
//						break;
//					case DONEWDOCUMENT:
//						qDebug() << "wants DONEWDOCUMENT";
//						break;
//					case DOSHUTDOWN:
//						qDebug() << "wants DOSHUTDOWN";
//						break;
//					case DOABOUT:
//						qDebug() << "wants DOABOUT";
//						break;
//					case DOSETTINGS:
//						qDebug() << "wants DOSETTINGS";
//						break;
//					default://qDebug("My hex number is: %x", hexnum);
//						qDebug() << "wants" << Qt::bin << Qt::showbase << wants;
//				}
//			qDebug()<<"";
//		};
//#endif
//};

class ManPageEditorQT : public QObject
{
	Q_OBJECT

	public:
		ManPageEditorQT(QApplication *app);
		~ManPageEditorQT();

//app vars
		QMainWindow					*mainWindow;
		NoteBookClass				*mainNotebook=NULL;
		ManpageConvertClass			*mpConv=NULL;


		QApplication					*application;
		QString						tmpFolderName;
		QString						htmlFile;
		QString						htmlURI;
		QSettings					prefs;
		QString						lastSaveDir="";
		QString						lastLoadDir="";
		QString						currentFilePath="";

		QTextEdit*					getDocumentForTab(int tabnum);
		QSplashScreen				*splash;
//recent files
//		RecentMenuClass				*recentFiles=NULL;

		QString						homeFolder;
		QString						homeDataFolder;


//app functions
		void							initApp(void);
		void							buildMainGui(void);
		void							readConfigs(void);

		void							setAppShortcuts(void);

//		void							setToolbarSensitive(void);
//
		void							writeExitData(void);
//		int							yesNoDialog(QString txt,QString info);


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
		int							autoShowMinChars=6;
		unsigned int 				untitledNumber=1;

//editor functions

//menubar
		QMenuBar						*menuBar;

//menu vars
//file menu
		QMenu						*fileMenu;
		MenuItemClass				*newMenuItem;
		MenuItemClass				*openMenuItem;
		MenuItemClass				*saveMenuItem;
		MenuItemClass				*saveAsMenuItem;
		MenuItemClass				*printMenuItem;

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
//		void							newFile(const QString data="",const QString filename="");
		//bool							openFile(QString filepath,bool addtorecents=true);
		QString						openFileDialog(QString title,QString dir);

		//bool							saveFile(QString filepath);
		//bool							saveFileAs(void);

//document vars
//document functions

//prefswindow
//prefswindow vars
//		QWidget						*prefsWidgets[MAXPREFSWIDGETS];
//		QWidget						*prefsIntWidgets[MAXPREFSINTWIDGETS];
//		QWidget						*prefsOtherWidgets[MAXPREFSOTHERWIDGETS];
//		QGridLayout					*table;
//		QDialog						*prefsWindow;
//		QListWidget					*listWidget;
//		QToolBar						*fromHBox;
//		QAction						*tool[20];
//		QDialog						*keyShortcutsWindow;
//
//prefswindow functions
//		void							makePrefsCheck(int widgnum,const QString label,bool onoff,int posx,int posy);
//		void							makePrefsDial(int widgnum,const QString label,int value,int minvalue,int maxvalue,int posy);
//		void							addIcon(const char* icon,const char* data,int toolnumber,const char* tooltip);
//		void							populateDnD(void);
//		void							buildPrefsWindow(void);
//		void							doPrefs(void);
//		void							addIconToList(const char* name,int type);
//		void							populateStore(void);
//		void							resetKeyCombo(void);

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

//search vars
//search functions

	public slots:
		//void							doFileMenuItems();
//		void							doFileMenuItems(MenuItemClass *mc);
//		void							doLiveSearch(const QString text="");
//		void							doEditMenuItems(MenuItemClass *mc);
//		void							doFormatMenuItems(MenuItemClass *mc);
//		void							doHelpMenuItems(MenuItemClass *mc);


//		void							doTimer(void);
//		void							doViewMenuItems();
//		void							doNavMenuItems();
//		void							doBookmarkMenuItems();
//		void							doHelpMenuItems();
//		void							doToolsMenuItems();
//		void							doTabBarContextMenu(void);
//		void							doOddMenuItems(void);
//		void							doOddButtons(void);
//
//		void							doDoubleClickPrefs(QListWidgetItem *item);
//		void							setPreferences(void);
//		void							setBMColour(void);
//		void							setLineColour(void);
//		void							setFont(void);
//		void							addToToolBar(void);
//		void							buildGetKeyShortCut(int index);
//
//		void							switchPage(int index);
//		bool							closeTab(int index);
//		void							closeAllTabs(void);
//
//		void							doFindButton(void);
//		void							tabContextMenu(const QPoint &pt);
//
//		void							doAppShortCuts(void);
//
//		void							doSessionsMenuItems(void);
//
//		void							setToolsData(int);
//
////search functions
//		void							setSearchPrefs(void);

	protected:
	private:
		void							doFileMenuItems(MenuItemClass *mc);
		void							doLiveSearch(const QString text="");
		void							doEditMenuItems(MenuItemClass *mc);
		void							doFormatMenuItems(MenuItemClass *mc);
		void							doHelpMenuItems(MenuItemClass *mc);
		bool							closeTabs(void);

};

#endif

