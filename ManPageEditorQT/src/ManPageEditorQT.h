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
enum {NEWMENUITEM=0x8000,NEWPAGEMENUITEM,OPENMENUITEM,OPENSYSPAGEMENUITEM,OPENTEMPLATEMENUITEM,SAVEMENUITEM,SAVEASMENUITEM,CLOSEPAGEMENUITEM,PROPSMENUITEM,PREVIEWMENUITEM,QUITMENUITEM,PREFSMENUITEM};
//edit
enum {UNDOMENUITEM=0x9000,REDOMENUITEM,CUTMENUITEM,COPYMENUITEM,PASTEMENUITEM,FINDMENUITEM};
//format
enum {BOLDMENUITEM=0xa000,ITALICMENUITEM,CLEARMENUITEM};

//help
enum {ABOUTMENUITEM=0xe000,ABOUTQTMENUITEM,HELPMENUITEM};

enum {FINDNEXT=1,FINDPREV,FINDREPLACE};

//od enums
enum {SPELLCHECKMENUITEM=0x2000,APPLYWORDBUTTON,IGNOREWORDBUTTON,ADDWORDBUTTON,CANCELSPELLCHECK,DOLIVESEARCH};

enum {FRCASE=0,FRUSEREGEX,FRREPLACEFIND,FRWRAP,FRALLFILES,FRHIGHLIGHTALL,FRREPLACEALL,FRSEARCHBACK,FRMAXSWITCHES};

#include "globals.h"

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
		QString						realDataDir;
		QString						realBinDir;
		QString						homeDataFolder;
		QString						tmpFolderName;
		QString						lastSaveDir="";
		QString						lastLoadDir="";
		QString						currentFilePath="";
		QString						terminalCommand="xterm -e ";
		QString						lineHiliteColour="#30000000";
		QString						extraHiliteColour="#30ff0000";
		QTextEdit::LineWrapMode		lineWrap=QTextEdit::WidgetWidth;
		QString						fontName="MonoSpace";
		int							fontSize=12;
		bool							useUnderline=false;
		bool							zipPages=false;

		QSettings					prefs;

//app functions
		void							initApp(void);
		void							buildMainGui(void);
		void							readConfigs(void);
		void							setAppShortcuts(void);
//		void							setToolbarSensitive(void);//TODO//
		void							writeExitData(void);
		bool							confirmClose(QTextEdit *te);
		QTextEdit*					makeNewTab(QString html,QString sectname,bool issub,int pos=-1);
		QTextEdit*					getDocumentForTab(int tabnum);
		void							doPrefs(void);
//spell check
		AspellConfig					*aspellConfig=NULL;
		AspellSpeller				*spellChecker=0;

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
//editor functions
		void							reHighLight(void);
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
		MenuItemClass				*spellCheckMenuItem;
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
//aspell
		void							doSpellCheckDoc(QTextEdit *te);

		QT_FindClass					*fc=NULL;

	protected:
	private:
//spellcheck
		QLabel						*badWordLabel=NULL;
		QDialog						*spellCheckWord=NULL;
		QComboBox					*wordListDropbox=NULL;
		QString						badWord;
		QString						goodWord;
		bool							blockFlag=false;
		bool							cancelCheck=false;

		void							buildWordCheckQt(void);
		bool							checkTheWord(QString word);
		void							doChangeWord(QTextEdit *te);
//app
		void							doFileMenuItems(MenuItemClass *mc);
		void							doLiveSearch(const QString text="");
		void							doEditMenuItems(MenuItemClass *mc);
		void							doFormatMenuItems(MenuItemClass *mc);
		void							doHelpMenuItems(MenuItemClass *mc);
		bool							closeTabs(bool all);
		void							hiliteLine(QTextEdit *te,QColor colour=QColor("#30000000"));

};

#endif

