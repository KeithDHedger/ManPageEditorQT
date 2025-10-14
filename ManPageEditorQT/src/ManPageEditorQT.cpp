/*
 *
 * ©K. D. Hedger. Sat  2 Mar 13:30:43 GMT 2024 keithdhedger@gmail.com

 * This file (ManPageEditorQT.cpp) is part of ManPageEditorQT.

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

#include "ManPageEditorQT.h"

ManPageEditorQT::ManPageEditorQT(QApplication *app)
{
	this->application=app;
	this->initApp();
}

ManPageEditorQT::~ManPageEditorQT()
{
	QDir		fold(this->tmpFolderName);
	fold.removeRecursively();
}

MenuItemClass* ManPageEditorQT::makeMenuItemClass(int mainmenu,const QString name,const QKeySequence key,const QString iconname,int userdata)
{
	MenuItemClass	*menuitem=new MenuItemClass(name);
	QIcon			itemicon=QIcon::fromTheme(iconname);

	menuitem->setMenuID(userdata);
	menuitem->setIcon(itemicon);
	if(key!=0)
		menuitem->setShortcut(key);

	switch(mainmenu)
		{
			case FILEMENU:
				this->fileMenu->addAction(menuitem);
				QObject::connect(menuitem,&MenuItemClass::triggered,[this,menuitem]()
					{
						this->doFileMenuItems(menuitem);
					});
			
				break;
			case EDITMENU:
				this->editMenu->addAction(menuitem);
				QObject::connect(menuitem,&MenuItemClass::triggered,[this,menuitem]()
					{
						this->doEditMenuItems(menuitem);
					});
				break;
			case FORMATMENU:
				this->formatMenu->addAction(menuitem);
				QObject::connect(menuitem,&MenuItemClass::triggered,[this,menuitem]()
					{
						this->doFormatMenuItems(menuitem);
					});
				break;
			case HELPMENU:
				this->helpMenu->addAction(menuitem);
				QObject::connect(menuitem,&MenuItemClass::triggered,[this,menuitem]()
					{
						this->doHelpMenuItems(menuitem);
					});
				break;
			case NOMENU:
				break;
		}

	return(menuitem);
}

void ManPageEditorQT::buildMainGui(void)
{
	//MenuItemClass	*menuItemSink;

	this->mainNotebook=new NoteBookClass(this);
	this->menuBar=new QMenuBar;

//file menu
	this->fileMenu=new QMenu("&File");
	this->menuBar->addMenu(this->fileMenu);

//new page
	this->newMenuItem=this->makeMenuItemClass(FILEMENU,"New Manpage",QKeySequence::New,"document-new",NEWPAGEMENUITEM);

//new
	this->newMenuItem=this->makeMenuItemClass(FILEMENU,"New Section",0,"document-new",NEWMENUITEM);

//open
	this->openMenuItem=this->makeMenuItemClass(FILEMENU,"Open Manpage",QKeySequence::Open,"document-open",OPENMENUITEM);

//import sys page
	this->openSysPageMenuItem=this->makeMenuItemClass(FILEMENU,"Open Sysytem Manpage",0,"document-open",OPENSYSPAGEMENUITEM);

	this->fileMenu->addSeparator();

//save
	this->saveMenuItem=this->makeMenuItemClass(FILEMENU,"Save Manpage",QKeySequence::Save,"document-save",SAVEMENUITEM);

//savas
	this->saveAsMenuItem=this->makeMenuItemClass(FILEMENU,"Save Manpage As",QKeySequence("Ctrl+Shift+S"),"document-save-as",SAVEASMENUITEM);

	this->fileMenu->addSeparator();

//preview
	this->previewMenuItem=this->makeMenuItemClass(FILEMENU,"Preview Manpage",0,"edit-find",PREVIEWMENUITEM);
//property
	this->propsMenuItem=this->makeMenuItemClass(FILEMENU,"Manpage Properties",0,"preferences-system",PROPSMENUITEM);

//printfile
	this->printMenuItem=this->makeMenuItemClass(FILEMENU,"Print Manpage",QKeySequence::Print,"document-print",PRINTMENUITEM);

	this->fileMenu->addSeparator();

//close
	this->closeMenuItem=this->makeMenuItemClass(FILEMENU,"Close",QKeySequence::Close,"window-close",CLOSEMENUITEM);
	this->fileMenu->addSeparator();

//prefs
	this->makeMenuItemClass(FILEMENU,"Preferences",0,"preferences-desktop",PREFSMENUITEM);
	this->fileMenu->addSeparator();

//quit
	this->makeMenuItemClass(FILEMENU,"Quit",QKeySequence::Quit,"application-exit",QUITMENUITEM);

//edit menu
	this->editMenu=new QMenu("&Edit");
	this->menuBar->addMenu(this->editMenu);

//undo
	this->undoMenuItem=this->makeMenuItemClass(EDITMENU,"Undo",QKeySequence::Undo,"edit-undo",UNDOMENUITEM);

//redo
	this->redoMenuItem=this->makeMenuItemClass(EDITMENU,"Redo",QKeySequence::Redo,"edit-redo",REDOMENUITEM);

	this->editMenu->addSeparator();

//cut
	this->cutMenuItem=this->makeMenuItemClass(EDITMENU,"Cut",QKeySequence::Cut,"edit-cut",CUTMENUITEM);
//copy
	this->copyMenuItem=this->makeMenuItemClass(EDITMENU,"Copy",QKeySequence::Copy,"edit-copy",COPYMENUITEM);
//paste
	this->pasteMenuItem=this->makeMenuItemClass(EDITMENU,"Paste",QKeySequence::Paste,"edit-paste",PASTEMENUITEM);
//find
	this->findMenuItem=this->makeMenuItemClass(EDITMENU,"Find",QKeySequence::Find,"edit-find",FINDMENUITEM);

	this->editMenu->addSeparator();

//format menu
	this->formatMenu=new QMenu("&Formating");
	this->menuBar->addMenu(this->formatMenu);
	this->boldMenuItem=this->makeMenuItemClass(FORMATMENU,"Bold",QKeySequence::fromString("Ctrl+B"),"format-text-bold",BOLDMENUITEM);
	this->italicMenuItem=this->makeMenuItemClass(FORMATMENU,"Italic",QKeySequence::fromString("Ctrl+I"),"format-text-italic",ITALICMENUITEM);
	this->clearMenuItem=this->makeMenuItemClass(FORMATMENU,"Clear",QKeySequence::fromString("Ctrl+R"),"edit-clear",CLEARMENUITEM);

//help
	this->helpMenu=new QMenu("&Help");
	this->menuBar->addMenu(this->helpMenu);
//
//about
	this->makeMenuItemClass(HELPMENU,"About",0,"help-about",ABOUTMENUITEM);
//aboutqt
	this->makeMenuItemClass(HELPMENU,"About QT",0,"help-about",ABOUTQTMENUITEM);

//help
	this->makeMenuItemClass(HELPMENU,"Help",0,"help-contents",HELPMENUITEM);

	this->setUpToolBar();
	this->mainWindow->setMenuBar(qobject_cast<QMenuBar*>(this->menuBar));
	this->mainWindow->addToolBar(&this->toolBar);
	this->mainWindow->setCentralWidget(this->mainNotebook);

//	this->statusText=new QLabel;	
//	this->statusText->setText("Line 0\tCol 0");
// 	this->statusBar=this->mainWindow->statusBar();
 //	this->statusBar->addWidget(this->statusText);
}
//#include <sys/stat.h>
//#include <sys/types.h>
void ManPageEditorQT::initApp(void)
{
	char		tmpfoldertemplate[]="/tmp/ManPageEditorQT-XXXXXX";
	QRect	r(0,0,1024,768);
	QDir		tdir;
	QString	tstr;
	QFile	file;

	this->homeFolder=QString("%1").arg(tdir.homePath());

	this->tmpFolderName=mkdtemp(tmpfoldertemplate);
	if(this->tmpFolderName.isEmpty()==true)
		{
			qDebug()<<"Can't create temporary folder, quitting ...";
			exit (100);
		}
	this->mainWindow=new QMainWindow;
	this->mpConv=new ManpageConvertClass(this);

	this->readConfigs();
	this->buildMainGui();

	this->mainNotebook->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(this->mainNotebook,&QTabWidget::tabCloseRequested,[this](int index)
		{
			QTextEdit	*te=this->getDocumentForTab(index);
			this->mainNotebook->removeTab(index);
			delete te;
		});
	QObject::connect(this->mainNotebook,&QTabWidget::customContextMenuRequested,[this](const QPoint p)
		{
			QTextEdit	*te=this->getDocumentForTab(-1);
			QString		ss=te->statusTip();
			bool			issub=false;
			if(ss.startsWith(".SS"))
				issub=true;
			ss=ss.mid(4);
			this->buildSectionProps(ss,issub,false);
		});


//	this->buildPrefsWindow();

//	this->buildFindReplace();
//#ifdef _ASPELL_
//	AspellCanHaveError	*possible_err;
//	this->aspellConfig=new_aspell_config();
//	possible_err=new_aspell_speller(this->aspellConfig);
//
//	if(aspell_error_number(possible_err)!= 0)
//		puts(aspell_error_message(possible_err));
//	else
//		spellChecker=to_aspell_speller(possible_err);
//
//	this->spellCheckMenuItem=new MenuItemClass("Spell Check");
//	QIcon	itemicon=QIcon::fromTheme("tools-check-spelling");
//	this->spellCheckMenuItem->setMenuID(SPELLCHECKMENUITEM);
//	this->spellCheckMenuItem->setIcon(itemicon);
//	QObject::connect(this->spellCheckMenuItem,SIGNAL(triggered()),this,SLOT(doOddMenuItems()));
//	this->buildSpellCheckerGUI();
//#endif
//
	r=this->prefs.value("app/geometry",QVariant(QRect(50,50,1024,768))).value<QRect>();
	this->mainWindow->setGeometry(r);

//	this->setToolbarSensitive();
	this->mainWindow->show();
}

void ManPageEditorQT::setUpToolBar(void)
{
	QHBoxLayout	*hbox=new QHBoxLayout;
	QWidget		*widg=new QWidget;

	this->toolBar.addAction(qobject_cast<QAction*>(this->newMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->openMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->saveMenuItem));
	this->toolBar.addSeparator();
	this->toolBar.addAction(qobject_cast<QAction*>(this->cutMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->copyMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->pasteMenuItem));
	this->toolBar.addSeparator();
	this->toolBar.addAction(qobject_cast<QAction*>(this->undoMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->redoMenuItem));
	this->toolBar.addSeparator();
	this->toolBar.addAction(qobject_cast<QAction*>(this->findMenuItem));
	this->toolBar.addSeparator();
	this->toolBar.addAction(qobject_cast<QAction*>(this->boldMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->italicMenuItem));
	this->toolBar.addAction(qobject_cast<QAction*>(this->clearMenuItem));

	hbox->addStretch(0);

//livesearch
	this->liveSearchWidget=new QLineEdit;
	this->liveSearchWidget->setToolTip("Live Search");
	QObject::connect(this->liveSearchWidget,&QLineEdit::textChanged,[this](QString text)
		{
			this->doLiveSearch(text);
		});
	QObject::connect(this->liveSearchWidget,&QLineEdit::returnPressed,[this]()
		{
			this->doLiveSearch(this->liveSearchWidget->text());
		});
	hbox->addWidget(this->liveSearchWidget,1);

	hbox->addStretch(0);
	widg->setLayout(hbox);
	this->toolBar.addWidget(widg);
}

QTextEdit* ManPageEditorQT::getDocumentForTab(int tabnum)
{
	if(tabnum==-1)
		return(qobject_cast<QTextEdit*>(this->mainNotebook->currentWidget()));
	else
		return(qobject_cast<QTextEdit*>(this->mainNotebook->widget(tabnum)));
}

QString ManPageEditorQT::openFileDialog(QString title,QString dir)
{
	QString	filepath="";
	filepath=QFileDialog::getOpenFileName(nullptr,title,dir,"",nullptr,QFileDialog::HideNameFilterDetails);
	return(filepath);
}

void ManPageEditorQT::writeExitData(void)
{
	QRect rg;
	QRect rf;

//editor
	rg=this->mainWindow->geometry();
	rf=this->mainWindow->frameGeometry();
	rf.setHeight(rf.height()-(rf.height()-rg.height()));
	rf.setWidth(rf.width()-(rf.width()-rg.width()));
	this->prefs.setValue("app/geometry",rf);
	this->prefs.setValue("editor/lastsavedir",this->lastSaveDir);
	this->prefs.setValue("editor/lastloaddir",this->lastLoadDir);

//	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
//	this->prefs.setValue("editor/maxtabchars",this->prefsMaxTabChars);
//	this->prefs.setValue("editor/terminalcommand",this->prefsTerminalCommand);
//	this->prefs.setValue("editor/printcommand",this->prefsPrintCommand);
	
//document
//	this->prefs.setValue("document/indent",this->prefsIndent);
//	this->prefs.setValue("document/wrap",this->prefsLineWrap);
//	this->prefs.setValue("document/tabwidth",this->prefsTabWidth);
//	this->prefs.setValue("document/font",this->prefsDocumentFont);

//application
//	this->prefs.setValue("app/shortcuts",this->defaultShortCutsList);

//find
//	this->setSearchPrefs();
//	this->findList=this->tailStringList(this->findList,this->maxFRHistory);
//	this->replaceList=this->tailStringList(this->replaceList,this->maxFRHistory);
//	this->prefs.setValue("find/findlist",this->findList);
//	this->prefs.setValue("find/replacelist",this->replaceList);
//	this->prefs.setValue("find/wrapsearch",this->wrapSearch);
//	this->prefs.setValue("find/findinallfiles",this->findInAllFiles);
//	this->prefs.setValue("find/insensitivesearch",this->insensitiveSearch);
//	this->prefs.setValue("find/useregex",this->useRegex);
//	this->prefs.setValue("find/hightlightall",this->hightlightAll);
//	this->prefs.setValue("find/replaceall",this->replaceAll);
//	this->prefs.setValue("find/searchback",this->searchBack);
//	this->prefs.setValue("find/findafterreplace",this->findAfterReplace);
//	this->prefs.setValue("find/maxfrhistory",this->maxFRHistory);
}

void ManPageEditorQT::readConfigs(void)
{
//editor
	this->lastSaveDir=this->prefs.value("editor/lastsavedir","").toString();
	this->lastLoadDir=this->prefs.value("editor/lastloaddir","").toString();

//	this->prefsToolBarLayout=this->prefs.value("editor/toolbarlayout","NSOsURsBWsFGsE9ELEDEE").toString();
//	this->prefsMaxTabChars=this->prefs.value("editor/maxtabchars",20).toInt();
//	this->prefsTerminalCommand=this->prefs.value("editor/terminalcommand","xterm -e").toString();
//	this->prefsPrintCommand=this->prefs.value("editor/printcommand","").toString();

//document
//	this->prefsDocumentFont=this->prefs.value("document/font",QVariant(QFont("Monospace",10))).value<QFont>();
//	this->prefsTabWidth=this->prefs.value("document/tabwidth",4).toInt();
//	this->prefsLineWrap=this->prefs.value("document/wrap",QVariant(bool(true))).value<bool>();
//	this->prefsIndent=this->prefs.value("document/indent",QVariant(bool(true))).value<bool>();

////application
//	this->defaultShortCutsList=this->prefs.value("app/shortcuts",QVariant(QStringList({"Ctrl+H","Ctrl+Y","Ctrl+?","Ctrl+K","Ctrl+Shift+H","Ctrl+D","Ctrl+Shift+D","Ctrl+L","Ctrl+M","Ctrl+Shift+M","Ctrl+@","Ctrl+'","Ctrl+Shift+C"}))).toStringList();

////find
//	this->findList=this->prefs.value("find/findlist").toStringList();
//	this->replaceList=this->prefs.value("find/replacelist").toStringList();
//	this->wrapSearch=this->prefs.value("find/wrapsearch",QVariant(bool(false))).value<bool>();
//	this->findInAllFiles=this->prefs.value("find/findinallfiles",QVariant(bool(false))).value<bool>();
//	this->insensitiveSearch=this->prefs.value("find/insensitivesearch",QVariant(bool(false))).value<bool>();
//	this->useRegex=this->prefs.value("find/useregex",QVariant(bool(false))).value<bool>();
//	this->hightlightAll=this->prefs.value("find/hightlightall",QVariant(bool(false))).value<bool>();
//	this->replaceAll=this->prefs.value("find/replaceall",QVariant(bool(false))).value<bool>();
//	this->searchBack=this->prefs.value("find/searchback",QVariant(bool(false))).value<bool>();
//	this->findAfterReplace=this->prefs.value("find/findafterreplace",QVariant(bool(false))).value<bool>();
//	this->maxFRHistory=this->prefs.value("find/maxfrhistory",5).toInt();
//
//	this->setAppShortcuts();	
}

bool ManPageEditorQT::closeTabs(void)
{
	while(this->mainNotebook->count()>0)
		{
			QTextEdit	*te=this->getDocumentForTab(0);
			this->mainNotebook->removeTab(0);
			delete te;
		}
	this->mpConv->manString=mpclass->getProperties();
	return(true);
}

/*
formReturnStruct FormsClass::formsDialog(QStringList items)
{
	formReturnStruct	prefs;
	QWidget					*hbox;
	QHBoxLayout				*hlayout;
	QVBoxLayout				*docvlayout=new QVBoxLayout;

	prefs.theDialog=new QDialog();

	this->data->theDialog=prefs.theDialog;//TODO//
	for(int j=0;j<items.size();j++)
		{
			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(new QLabel(items.at(j)),0,Qt::AlignLeft);
			prefs.boxes[j]=new QLineEdit(nullptr);

			hlayout->addWidget(prefs.boxes[j],1,Qt::AlignRight);
			docvlayout->addWidget(hbox);
		}

	docvlayout->addWidget(this->data->bb);
	docvlayout->setContentsMargins(MARGINS,MARGINS,MARGINS,MARGINS);

	prefs.theDialog->setLayout(docvlayout);
	prefs.theDialog->setWindowTitle(this->data->title);

	if(this->data->customSize==true)
		prefs.theDialog->resize(this->data->adjustBoxSize(256,-1));
	return(prefs);
}
*/
/*
struct manProps
{
	QString manString=R"foo("myprogram" "1" "0.0.0" "Me" "My set of programs")foo";
	QString name="myprogram";
	QString section="1";
	QString version="0.0.0";
	QString author="Me";
	QString catagory="My set of programs";
};
	QHash<int,QLineEdit*>	boxes;

*/
QString ManPageEditorQT::buildProperties(QString thstr)
{
	QString			propstr=thstr;
	QDialog			propsdialog;
	QWidget			*hbox;
	QHBoxLayout		*hlayout;
	QVBoxLayout		*docvlayout=new QVBoxLayout;
	QString			data="";
	QDialogButtonBox	*buttonBox=NULL;
	QFrame			separator(nullptr);

	const char	*propname[5]={"Name","Section","Version","Author","Catagory"};
	this->propBoxes.clear();
	for(int j=0;j<5;j++)
		{
			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(new QLabel(propname[j]),0,Qt::AlignLeft);
			switch(j)
				{
					case 0:
						data=this->pageProperties.name;
						break;
					case 1:
						data=this->pageProperties.section;
						break;
					case 2:
						data=this->pageProperties.version;
						break;
					case 3:
						data=this->pageProperties.author;
						break;
					case 4:
						data=this->pageProperties.catagory;
						break;
				}
			this->propBoxes.push_back(new QLineEdit(data));
			hlayout->addWidget(this->propBoxes[j],1,Qt::AlignRight);
			docvlayout->addWidget(hbox);
		}

	separator.setFrameStyle(QFrame::Sunken | QFrame::HLine);
	docvlayout->addWidget(&separator);
      
	buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	docvlayout->addWidget(buttonBox);
	QObject::connect(buttonBox,&QDialogButtonBox::accepted,[&propsdialog]()
		{
			propsdialog.done(0);
		});
	QObject::connect(buttonBox,&QDialogButtonBox::rejected,[&propsdialog]()
		{
			propsdialog.done(1);
		});

	docvlayout->setContentsMargins(MARGINS,MARGINS,MARGINS,MARGINS);
	propsdialog.setLayout(docvlayout);
	propsdialog.setWindowTitle("Page Properties");
	int ret=propsdialog.exec();
	delete buttonBox;
	if(ret==0)
		{
			this->pageProperties.name=propBoxes.at(0)->text();
			this->pageProperties.section=propBoxes.at(1)->text();
			this->pageProperties.version=propBoxes.at(2)->text();
			this->pageProperties.author=propBoxes.at(3)->text();
			this->pageProperties.catagory=propBoxes.at(4)->text();
			this->pageProperties.manString=QString("\"%1\" \"%2\" \"%3\" \"%4\" \"%5\"").arg(propBoxes.at(0)->text()).arg(propBoxes.at(1)->text()).arg(propBoxes.at(2)->text()).arg(propBoxes.at(3)->text()).arg(propBoxes.at(4)->text());
			this->mpConv->manString=".TH "+this->pageProperties.manString;
		}
	return(propstr);
}

QString ManPageEditorQT::getProperties(QString thstr)
{
	QString		propstr=thstr;
	wordexp_t	p;
	char			**w=NULL;
	int			wordexpret=0;

	this->pageProperties.manString=propstr;
	this->pageProperties.name="";
	this->pageProperties.section="";
	this->pageProperties.version="";
	this->pageProperties.author="";
	this->pageProperties.catagory="";
	wordexpret=wordexp(thstr.trimmed().toStdString().c_str(),&p,WRDE_NOCMD);

	if(wordexpret==0)
		{
			w=p.we_wordv;
			for (size_t i=0;i<p.we_wordc;i++)
				{
					switch(i)
						{
							case 1:
								this->pageProperties.name=w[i];
								break;
							case 2:
								this->pageProperties.section=w[i];
								break;
							case 3:
								this->pageProperties.version=w[i];
								break;
							case 4:
								this->pageProperties.author=w[i];
								break;
							case 5:
								this->pageProperties.catagory=w[i];
								break;
						}
				}
			wordfree(&p);
		}

	return(propstr);
}

void ManPageEditorQT::buildSectionProps(QString s,bool issubsec,bool create)
{
	QDialog			propsdialog;
	QWidget			*hbox;
	QHBoxLayout		*hlayout;
	QVBoxLayout		*docvlayout=new QVBoxLayout;
	QDialogButtonBox	*buttonBox=NULL;
	QFrame			separator(nullptr);
	QLineEdit		*nameedit;
	QCheckBox		*subchk;
	const char		*propname[]={"Name","Is Sub-Section"};

	hbox=new QWidget;
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox->setLayout(hlayout);
	hlayout->addWidget(new QLabel(propname[0]),0,Qt::AlignLeft);
	nameedit=new QLineEdit();
	nameedit->setText(s);
	nameedit->selectAll();
	hlayout->addWidget(nameedit,1,Qt::AlignRight);
	docvlayout->addWidget(hbox);

	hbox=new QWidget;
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox->setLayout(hlayout);
	subchk=new QCheckBox(propname[1]);
	subchk->setChecked(issubsec);
	hlayout->addWidget(subchk,1,Qt::AlignLeft);
	docvlayout->addWidget(hbox);

	separator.setFrameStyle(QFrame::Sunken | QFrame::HLine);
	docvlayout->addWidget(&separator);
      
	buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	docvlayout->addWidget(buttonBox);
	QObject::connect(buttonBox,&QDialogButtonBox::accepted,[&propsdialog]()
		{
			propsdialog.done(0);
		});
	QObject::connect(buttonBox,&QDialogButtonBox::rejected,[&propsdialog]()
		{
			propsdialog.done(1);
		});

	docvlayout->setContentsMargins(MARGINS,MARGINS,MARGINS,MARGINS);
	propsdialog.setLayout(docvlayout);
	propsdialog.setWindowTitle("Section Properties");
	int ret=propsdialog.exec();
	delete buttonBox;
	if(ret==0)
		{
			QTextEdit	*te;
			QString		tabname=nameedit->text();

			if(create==true)
				{
					te=new QTextEdit;
					te->setFont(QFont(this->fontName,this->fontSize));
				}
			else
				te=this->getDocumentForTab(-1);
		
			if(subchk->isChecked()==false)
				{
					if(create==true)
						this->mainNotebook->insertTab(this->mainNotebook->currentIndex()+1,te,tabname.trimmed().toUpper());
					else
						this->mainNotebook->setTabText(this->mainNotebook->currentIndex(),tabname.trimmed().toUpper());
					te->setStatusTip(".SH "+tabname.trimmed());
				}
			else
				{
					if(create==true)
						this->mainNotebook->insertTab(this->mainNotebook->currentIndex()+1,te,tabname.trimmed().toLower());
					else
						this->mainNotebook->setTabText(this->mainNotebook->currentIndex(),tabname.trimmed().toLower());
					te->setStatusTip(".SS "+tabname.trimmed());
				}
			te->setLineWrapMode(QTextEdit::WidgetWidth);
		}
}

void ManPageEditorQT::doBold(void)
{
	QTextEdit		*te=this->getDocumentForTab(this->mainNotebook->currentIndex());
	QTextCharFormat	fmt;

	fmt.setFontWeight(QFont::Normal);			
	fmt.setFontUnderline(false);			

	fmt.setFontWeight(QFont::Bold);			
	//te->mergeCurrentCharFormat(fmt);			
	te->setCurrentCharFormat(fmt);			
}

void ManPageEditorQT::doItalic(void)
{
	QTextEdit		*te=this->getDocumentForTab(this->mainNotebook->currentIndex());
	QTextCharFormat	fmt;

	fmt.setFontWeight(QFont::Normal);
	fmt.setFontItalic(false);
	fmt.setFontUnderline(false);

	if(this->useUnderline==false)
		fmt.setFontItalic(true);
	else
		fmt.setFontUnderline(true);
	//te->mergeCurrentCharFormat(fmt);
	te->setCurrentCharFormat(fmt);
}

void ManPageEditorQT::doClear(void)
{
	QTextEdit		*te=this->getDocumentForTab(this->mainNotebook->currentIndex());
	QTextCharFormat	fmt;

	fmt.setFontWeight(QFont::Normal);			
	fmt.setFontUnderline(false);			
	fmt.setFontItalic(false);			
	te->mergeCurrentCharFormat(fmt);			
}

void ManPageEditorQT::doPreView(void)
{
	this->mpConv->exportManpage(this->tmpFolderName+"/preview");
	QProcess::execute("xterm",QStringList()<<"-hold"<<"-e"<<"man "+this->tmpFolderName+"/preview");
}

#if 0
void ManPageEditorQT::switchPage(int index)
{
	DocumentClass	*doc=NULL;
	plugData			pd;

	if(this->sessionBusy==true)
		return;

	doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(index));
	if(doc==0)
		return;
	if(doc==NULL)
		return;

	doc->setStatusBarText();
	doc->clearHilites();
	if(doc->state==CHANGEDONDISKTAB)
		{
			doc->state=NORMALTAB;
			doc->setTabColourType(NORMALTAB);
		}
	this->rebuildFunctionMenu(index);
	this->rebuildTabsMenu();

	this->currentFilepath=doc->getFilePath();
	this->currentFilename=doc->getFileName();

//plugins
	pd.doc=doc;
	pd.tabNumber=this->mainNotebook->currentIndex();
	pd.userIntData1=index;
	pd.what=DOSWITCHPAGE;
	this->runAllPlugs(pd);
}

void ManPageEditorQT::rebuildBookMarkMenu()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	MenuItemClass	*menuItemSink;

	this->bookMarkMenu->clear();
	menuItemSink=this->makeMenuItemClass(BOOKMARKSMENU,"Remove All Bookmarks",0,"list-remove",REMOVEALLBOOKMARKS,REMOVEALLBOOKMARKSMENUITEM);
	menuItemSink=this->makeMenuItemClass(BOOKMARKSMENU,"Toggle Bookmark",QKeySequence::fromString("Ctrl+T"),DATADIR"/pixmaps/BookMark.png",TOGGLEBOOKMARK,TOGGLEBOOKMARKMENUITEM);

	this->bookMarkMenu->addSeparator();
	this->bookMarks.clear();
#pragma GCC diagnostic pop
}

void ManPageEditorQT::handleBMMenu(QWidget *widget,int what,QTextCursor curs)
{
	DocumentClass	*doc=this->pages.value(qobject_cast<DocumentClass*>(widget)->pageIndex);
	QTextCursor		cursor;
	bookMarkStruct	bms;

	if(curs.isNull()==true)
		cursor=doc->textCursor();
	else
		cursor=curs;

	switch(what)
		{
			case REMOVEBOOKMARKSFROMPAGE:
				foreach(bookMarkStruct value,this->bookMarks)
					{
						if(value.docIndex==doc->pageIndex)
							{
									this->bookMarkMenu->removeAction(value.menu);
									delete value.menu;
									this->bookMarks.remove(value.bmKey);
							}
					}
				return;
				break;
			case TOGGLEBOOKMARKMENUITEM:
				{
					foreach(bookMarkStruct value,this->bookMarks)
						{
							if((value.docIndex==doc->pageIndex) && (value.line==cursor.blockNumber()+1))
								{
									this->bookMarkMenu->removeAction(value.menu);
									this->bookMarks.remove(value.bmKey);
									return;
								}
						}

					bms.bmLabel=this->truncateWithElipses(cursor.block().text().simplified(),this->prefsMaxMenuChars);
					bms.line=cursor.blockNumber()+1;
					if(bms.bmLabel.isEmpty()==true)
						bms.bmLabel=doc->getFileName() + QString(" Line %1").arg(bms.line); 
					bms.docIndex=doc->pageIndex;
					bms.bmKey=this->bookMarksIndex;
					bms.menu=this->makeMenuItemClass(BOOKMARKSMENU,bms.bmLabel,0,NULL,"NOTNEEDED",this->bookMarksIndex);
					this->bookMarks[this->bookMarksIndex++]=bms;
				}
				break;

			default:
				this->history->pushToBackList(doc->getCurrentLineNumber(),doc->getFilePath());
				bms=this->bookMarks.value(what);
				doc=this->pages.value(bms.docIndex);
				this->mainNotebook->setCurrentWidget(doc);
				this->setTabVisibilty(this->mainNotebook->currentIndex(),true);
				this->gotoLine(bms.line);
				break;
		}
}

void ManPageEditorQT::initApp(int argc,char** argv)
{
	char		tmpfoldertemplate[]="/tmp/KKEditQT-XXXXXX";
	QRect	r(0,0,1024,768);
	QDir		tdir;
	QString	tstr;
	QFile	file;

	this->homeFolder=QString("%1").arg(tdir.homePath());
	this->homeDataFolder=QString("%1/%2").arg(this->homeFolder).arg(KKEDITFOLDER);
	this->sessionFolder=QString("%1/%2/%3").arg(this->homeFolder).arg(KKEDITFOLDER).arg("sesssions");
	this->toolsFolder=QString("%1/%2/%3").arg(this->homeFolder).arg(KKEDITFOLDER).arg("tools");
	this->recentFiles=new RecentMenuClass(this);
	this->theme=new ThemeClass(this);
	QObject::connect(this->fileWatch,&QFileSystemWatcher::fileChanged,[this](const QString &path)
		{
			this->fileChangedOnDisk(path);
		});

	tdir.mkpath(this->sessionFolder);
	for(int j=0;j<MAXSESSIONS;j++)
		{
			QProcess::execute("touch",QStringList()<<this->sessionFolder+"/Session-"+QString::number(j));

			file.setFileName(QString("%1/Session-%2").arg(this->sessionFolder).arg(j));
			if(file.open(QIODevice::Text | QIODevice::ReadOnly))
				{
					tstr=QTextStream(&file).readLine();
					file.close();
					if(tstr.isEmpty()==true)
						{
							if(file.open(QIODevice::Text | QIODevice::WriteOnly))
								{
									if(j>0)
										{
											QTextStream(&file) << "New Session-" << j <<Qt::endl;
											this->sessionNames[j]=QString("New Session-%1").arg(j);
										}
									else
										{
											QTextStream(&file) << "Default Session" <<Qt::endl;
											this->sessionNames[0]=QString("Default Session");
										}
									file.close();
								}
						}
					else
						this->sessionNames[j]=tstr;
				}
		}


//qDebug()<<tmpfoldertemplate;
		this->tmpFolderName=mkdtemp(tmpfoldertemplate);
		if(this->tmpFolderName==NULL)
			{
				qDebug()<<"Can't create temporary folder, quitting ...";
				exit (100);
			}

//qDebug()<<this->tmpFolderName;

	this->gotDoxygen=QProcess::execute("sh",QStringList()<<"-c"<<"which doxygen 2>&1 >/dev/null");
	this->gotManEditor=QProcess::execute("sh",QStringList()<<"-c"<<"which manpageeditor 2>&1 >/dev/null");

//	if(getuid()!=0)
//		styleName="classic";
//	else
//		styleName="Root Source";
	//this->highlightColour="#808080";
	this->mainWindow=new QMainWindow;

	for(int j=0;j<NOMORESHORTCUT;j++)
		this->appShortcuts[j]=new QShortcut(this->mainWindow);

	this->setAppShortcuts();
	this->readConfigs();
	if(this->queueID==-1)
		{
			if((this->queueID=msgget(this->sessionID,IPC_CREAT|0660))==-1)
				fprintf(stderr,"Can't create message queue, scripting wont work :( ...\n");
		}
	this->checkMessages=new QTimer();
	QObject::connect(this->checkMessages,SIGNAL(timeout()),this,SLOT(doTimer()));
	this->checkMessages->start(this->prefsMsgTimer);
	this->theme->loadTheme(this->prefStyleName);
	this->buildMainGui();
	this->buildPrefsWindow();
	this->buildToolOutputWindow();
	this->loadPlugins();

//#ifdef _BUILDDOCVIEWER_
	this->buildDocViewer();
//#endif

	this->buildFindReplace();
//#ifdef _ASPELL_
	AspellCanHaveError	*possible_err;
	this->aspellConfig=new_aspell_config();
	possible_err=new_aspell_speller(this->aspellConfig);

	if(aspell_error_number(possible_err)!= 0)
		puts(aspell_error_message(possible_err));
	else
		spellChecker=to_aspell_speller(possible_err);

	this->spellCheckMenuItem=new MenuItemClass("Spell Check");
	QIcon	itemicon=QIcon::fromTheme("tools-check-spelling");
	this->spellCheckMenuItem->setMenuID(SPELLCHECKMENUITEM);
	this->spellCheckMenuItem->setIcon(itemicon);
	QObject::connect(this->spellCheckMenuItem,SIGNAL(triggered()),this,SLOT(doOddMenuItems()));
	this->buildSpellCheckerGUI();
//#endif

	this->htmlFile=QString("%1/Docview-%2.html").arg(this->tmpFolderName).arg(this->randomName(6));
	this->htmlURI="file://"+this->htmlFile;

	this->recentFiles->updateRecents();

	if(this->forceDefaultGeom==false)
		r=this->prefs.value("app/geometry",QVariant(QRect(50,50,1024,768))).value<QRect>();
	this->mainWindow->setGeometry(r);

//this->onExitSaveSession //TODO//
	this->setToolbarSensitive();
	this->mainWindow->show();
}

QString ManPageEditorQT::randomName(int len)
{
	QString	retval="";

	const char alphanum[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(int i=0;i<len;++i)
		retval+=alphanum[rand()%(sizeof(alphanum)-1)];
   return(retval);
}

void ManPageEditorQT::readConfigs(void)
{
//editor
	this->prefsFunctionMenuLayout=this->prefs.value("editor/funcsort",4).toInt();
	this->prefsDepth=this->prefs.value("editor/prefsdepth",1).toInt();
	this->prefsToolBarLayout=this->prefs.value("editor/toolbarlayout","NSOsURsBWsFGsE9ELEDEE").toString();
	this->prefsMaxTabChars=this->prefs.value("editor/maxtabchars",20).toInt();
	this->prefsMaxMenuChars=this->prefs.value("editor/maxfuncchars",64).toInt();
	this->prefsTerminalCommand=this->prefs.value("editor/terminalcommand","xterm -e").toString();
	this->prefsRootCommand=this->prefs.value("editor/rootcommand","gtksu -- env QTWEBENGINE_DISABLE_SANDBOX=1 env QT_QPA_PLATFORMTHEME=qt5ct ").toString();
	this->prefsQtDocDir=this->prefs.value("editor/qtdocdir","/usr/share/doc/qt5").toString();
	this->prefsNoOpenduplicate=this->prefs.value("editor/noopendup",QVariant(bool(true))).value<bool>();
	this->prefsNoWarnings=this->prefs.value("editor/nowarnings",QVariant(bool(false))).value<bool>();
	this->recentFiles->maxFiles=this->prefs.value("editor/maxrecents",10).toInt();
	this->prefsPrintCommand=this->prefs.value("editor/printcommand","").toString();

//document
	this->prefsHighLightline=this->prefs.value("document/highlightline",QVariant(bool(true))).value<bool>();
	this->prefsShowLineNumbers=this->prefs.value("document/showlinenumbers",QVariant(bool(true))).value<bool>();
	this->prefsDocumentFont=this->prefs.value("document/font",QVariant(QFont("Monospace",10))).value<QFont>();
	this->prefsSyntaxHilighting=this->prefs.value("document/syntaxhilighting",QVariant(bool(true))).value<bool>();
	this->prefsTabWidth=this->prefs.value("document/tabwidth",4).toInt();
	this->prefsLineWrap=this->prefs.value("document/wrap",QVariant(bool(true))).value<bool>();
	this->prefsIndent=this->prefs.value("document/indent",QVariant(bool(true))).value<bool>();
	this->prefsAutoShowCompletions=this->prefs.value("document/autoshowcompletions",QVariant(bool(true))).value<bool>();
	this->autoShowMinChars=this->prefs.value("document/autoshowminchars",6).toInt();

//theme
	this->prefStyleName=this->prefs.value("theme/style","default").toString();
	this->prefStyleNameHold=this->prefStyleName;
	this->prefsHiLiteLineColor=this->prefs.value("theme/hilitelinecol",QVariant(QColor(0xff,0xff,0xff,0x40))).value<QColor>();
	this->prefsBookmarkHiLiteColor=this->prefs.value("theme/bmhilitecol",QVariant(QColor(0,0,0,0x40))).value<QColor>();

//application
	this->prefsMenuStyleString=this->prefs.value("app/prefsmenustylestring","QMenu{menu-scrollable: true;padding: 0px;margin: 0px}").toString();
	this->prefsMsgTimer=this->prefs.value("app/msgtimer",1000).toInt();
	this->prefsUseSingle=this->prefs.value("app/usesingle",QVariant(bool(true))).value<bool>();
	this->prefsNagScreen=this->prefs.value("app/bekind",QVariant(bool(false))).value<bool>();
	this->defaultShortCutsList=this->prefs.value("app/shortcuts",QVariant(QStringList({"Ctrl+H","Ctrl+Y","Ctrl+?","Ctrl+K","Ctrl+Shift+H","Ctrl+D","Ctrl+Shift+D","Ctrl+L","Ctrl+M","Ctrl+Shift+M","Ctrl+@","Ctrl+'","Ctrl+Shift+C"}))).toStringList();
	this->onExitSaveSession=this->prefs.value("app/onexitsavesession",QVariant(bool(true))).value<bool>();
	this->disabledPlugins=this->prefs.value("app/disabledplugins").toStringList();

//find
	this->findList=this->prefs.value("find/findlist").toStringList();
	this->replaceList=this->prefs.value("find/replacelist").toStringList();
	this->wrapSearch=this->prefs.value("find/wrapsearch",QVariant(bool(false))).value<bool>();
	this->findInAllFiles=this->prefs.value("find/findinallfiles",QVariant(bool(false))).value<bool>();
	this->insensitiveSearch=this->prefs.value("find/insensitivesearch",QVariant(bool(false))).value<bool>();
	this->useRegex=this->prefs.value("find/useregex",QVariant(bool(false))).value<bool>();
	this->hightlightAll=this->prefs.value("find/hightlightall",QVariant(bool(false))).value<bool>();
	this->replaceAll=this->prefs.value("find/replaceall",QVariant(bool(false))).value<bool>();
	this->searchBack=this->prefs.value("find/searchback",QVariant(bool(false))).value<bool>();
	this->findAfterReplace=this->prefs.value("find/findafterreplace",QVariant(bool(false))).value<bool>();
	this->maxFRHistory=this->prefs.value("find/maxfrhistory",5).toInt();

	this->setAppShortcuts();	
}

void ManPageEditorQT::tabContextMenu(const QPoint &pt)
{
	MenuItemClass	*menuitem;
	MenuItemClass	*menuitem1;
	int				tabIndex;
	QIcon			itemicon;
	DocumentClass	*doc=this->getDocumentForTab(-1);
	plugData			pd;

	if(pt.isNull())
		return;

	tabIndex=this->mainNotebook->tabBar()->tabAt(pt);

	if (tabIndex!=-1)
		{
			QMenu	menu(this->mainNotebook);
			QMenu	srcmenu(this->tabContextMenuItems[(SRCHILTIE-COPYFOLDERPATH)/0x100].label);
			QMenu	filemenu(this->tabContextMenuItems[(OPENFROMHERE-COPYFOLDERPATH)/0x100].label);
			for(int cnt=0;cnt<TABCONTEXTMENUCNT;cnt++)
				{
					if(cnt==(SRCHILTIE-COPYFOLDERPATH)/0x100)
						{
							menu.addMenu(&srcmenu);
							itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
							srcmenu.setIcon(itemicon);
							for(int j=0;j<doc->highlighter->langPlugins.count();j++)
								{
									menuitem1=new MenuItemClass(doc->highlighter->langPlugins[j].langName);
									srcmenu.addAction(menuitem1);
									QObject::connect(menuitem1,&QAction::triggered,[doc,j]()
										{
											doc->highlighter->setLanguage(doc->highlighter->langPlugins[j].langName);
											doc->highlighter->setTheme();
										});
								}
							continue;
						}

					if(cnt==(OPENFROMHERE-COPYFOLDERPATH)/0x100)
						{
							menu.addMenu(&filemenu);
							DocumentClass	*doc=this->getDocumentForTab(tabIndex);
							QDir				dir(doc->getDirPath());
							QStringList		filters;
							QStringList		flist=dir.entryList(QDir::Files);
							itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
							filemenu.setIcon(itemicon);
							for(int k=0;k<flist.count();k++)
								{
									if(flist.at(k).endsWith(".o")==false)
										{
											menuitem1=new MenuItemClass(flist.at(k));
											menuitem1->setMenuID(OPENFROMHERE+tabIndex);
											filemenu.addAction(menuitem1);
											QObject::connect(menuitem1,SIGNAL(triggered()),this,SLOT(doTabBarContextMenu()));
										}
								}
							continue;
						}
			
					menuitem=new MenuItemClass(this->tabContextMenuItems[cnt].label);
					menuitem->setMenuID(this->tabContextMenuItems[cnt].what+tabIndex);
					menu.addAction(menuitem);

					DocumentClass	*doc=this->getDocumentForTab(tabIndex);
					if(cnt==(LOCKCONTENTS-COPYFOLDERPATH)/0x100)
						{
							if(doc->isReadOnly()==true)
								{
									menuitem->setText("Unlock Contents");
									itemicon=QIcon::fromTheme("emblem-default");
								}
							else
								{
									itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
									menuitem->setText(this->tabContextMenuItems[cnt].label);
								}
						}
					else
						itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
					menuitem->setIcon(itemicon);
					QObject::connect(menuitem,SIGNAL(triggered()),this,SLOT(doTabBarContextMenu()));
				}
//plugins
			pd.menu=&menu;
			pd.tabNumber=tabIndex;
			pd.userStrData1=this->homeDataFolder;
			pd.userStrData2=this->getDocumentForTab(tabIndex)->getDirPath();
			pd.userStrData3=this->getDocumentForTab(tabIndex)->getFileName();
			pd.what=DOTABPOPUP;
			this->runAllPlugs(pd);
			menu.exec(this->mainNotebook->mapToGlobal(pt));
		}
}

void ManPageEditorQT::writeExitData(void)
{
	if(this->verySafeFlag==true)
		return;
//editor
	if(this->forceDefaultGeom==false)
		this->prefs.setValue("app/geometry",this->mainWindow->geometry());

//#ifdef _BUILDDOCVIEWER_
	this->prefs.setValue("app/viewergeometry",this->docView->geometry());
//#endif

	this->prefs.setValue("editor/funcsort",this->prefsFunctionMenuLayout);
	this->prefs.setValue("editor/prefsdepth",this->prefsDepth);
	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
	this->prefs.setValue("editor/maxtabchars",this->prefsMaxTabChars);
	this->prefs.setValue("editor/maxfuncchars",this->prefsMaxMenuChars);
	this->prefs.setValue("editor/terminalcommand",this->prefsTerminalCommand);
	this->prefs.setValue("editor/rootcommand",this->prefsRootCommand);
	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
	this->prefs.setValue("editor/qtdocdir",this->prefsQtDocDir);
	this->prefs.setValue("editor/noopendup",this->prefsNoOpenduplicate);
	this->prefs.setValue("editor/nowarnings",this->prefsNoWarnings);
	this->prefs.setValue("editor/maxrecents",this->recentFiles->maxFiles);
	this->prefs.setValue("editor/printcommand",this->prefsPrintCommand);
	
//document
	this->prefs.setValue("document/indent",this->prefsIndent);
	this->prefs.setValue("document/wrap",this->prefsLineWrap);
	this->prefs.setValue("document/tabwidth",this->prefsTabWidth);
	this->prefs.setValue("document/syntaxhilighting",this->prefsSyntaxHilighting);
	this->prefs.setValue("document/font",this->prefsDocumentFont);
	this->prefs.setValue("document/showlinenumbers",this->prefsShowLineNumbers);
	this->prefs.setValue("document/highlightline",this->prefsHighLightline);
	this->prefs.setValue("document/autoshowcompletions",this->prefsAutoShowCompletions);
	this->prefs.setValue("document/autoshowminchars",this->autoShowMinChars);

//theme
	this->prefs.setValue("theme/style",this->prefStyleName);
	this->prefs.setValue("theme/hilitelinecol",this->prefsHiLiteLineColor);
	this->prefs.setValue("theme/bmhilitecol",this->prefsBookmarkHiLiteColor);

//application
	this->prefs.setValue("app/prefsmenustylestring",this->prefsMenuStyleString);
	this->prefs.setValue("app/msgtimer",this->prefsMsgTimer);
	this->prefs.setValue("app/usesingle",this->prefsUseSingle);
	this->prefs.setValue("app/bekind",this->prefsNagScreen);
	this->prefs.setValue("app/toolsopgeometry",this->toolOutputWindow->geometry());
	this->prefs.setValue("app/shortcuts",this->defaultShortCutsList);
	this->prefs.setValue("app/onexitsavesession",this->onExitSaveSession);

//find
	this->setSearchPrefs();
	this->findList=this->tailStringList(this->findList,this->maxFRHistory);
	this->replaceList=this->tailStringList(this->replaceList,this->maxFRHistory);
	this->prefs.setValue("find/findlist",this->findList);
	this->prefs.setValue("find/replacelist",this->replaceList);
	this->prefs.setValue("find/wrapsearch",this->wrapSearch);
	this->prefs.setValue("find/findinallfiles",this->findInAllFiles);
	this->prefs.setValue("find/insensitivesearch",this->insensitiveSearch);
	this->prefs.setValue("find/useregex",this->useRegex);
	this->prefs.setValue("find/hightlightall",this->hightlightAll);
	this->prefs.setValue("find/replaceall",this->replaceAll);
	this->prefs.setValue("find/searchback",this->searchBack);
	this->prefs.setValue("find/findafterreplace",this->findAfterReplace);
	this->prefs.setValue("find/maxfrhistory",this->maxFRHistory);

	if(this->safeFlag==false)
		{
			this->disabledPlugins.clear();
			for(int j=0;j<this->plugins.count();j++)
				{
					if(this->plugins[j].loaded==false)
						this->disabledPlugins<<this->plugins[j].plugPath;
				}
		}
	this->prefs.setValue("app/disabledplugins",this->disabledPlugins);
}

void ManPageEditorQT::findFile(void)
{
	DocumentClass	*document=this->getDocumentForTab(-1);
	QString			selection;
	QString			filename;
	QString			results;
	QStringList		retval;

	if(document==NULL)
		return;

	selection=document->textCursor().block().text().trimmed();

	if((selection.isEmpty()==true) || (selection.startsWith('#')==false))
		return;

	filename=selection.replace(QRegExp("#.*include\\s*[\"<](.*)[\">]"),"\\1").trimmed();

	if(this->openFile(QString("%1/%2").arg(document->getDirPath()).arg(filename))==true)
		return;

	results=this->runPipeAndCapture(QString("find \"%1\" -iwholename \"%2\" 2>/dev/null").arg(document->getDirPath()).arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}

	results=this->runPipeAndCapture(QString("find \"/usr/include\" -iwholename \"/usr/include/%1\" 2>/dev/null").arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}

	results=this->runPipeAndCapture(QString("find \"/usr/include/\" -name \"%1\" 2>/dev/null").arg(filename));
	if(results.isEmpty()==true)
		return;

	retval=results.split("\n",Qt::SkipEmptyParts);
	for(int j=0;j<retval.count();j++)
		this->openFile(retval.at(j));
}

void ManPageEditorQT::showBarberPole(QString windowtitle,QString bodylabel,QString cancellabel,QString maxitems,QString controlfile)//TODO//
{
	QStringList	arguments;

//#ifdef _DEBUGCODE_
	QString		app="KKEditQT/app/KKEditQTProgressBar";
//#else
	QString		app="KKEditQTProgressBar";
//#endif
	arguments<<"-c"<<QString("\"%6\" \"%1\" \"%2\" \"%3\" \"%4\" \"%5\"").arg(windowtitle).arg(bodylabel).arg(cancellabel).arg(maxitems).arg(controlfile).arg(app);
	QProcess::startDetached("sh",arguments);
}

void ManPageEditorQT::buildDocs(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	struct stat		sb;
	FILE				*fp;
	char				line[4096];
	QString			pipecom;
	QFileInfo		fileinfo;
	QDir				currentdir;

	if(doc==NULL)
		return;

	currentdir=QDir::current();
	QDir::setCurrent(doc->getDirPath());
	stat("Doxyfile",&sb);
	if(!S_ISREG(sb.st_mode))
		{			
			QDialog		*diag=new QDialog();
			QVBoxLayout	*docvlayout=new QVBoxLayout;
			QLineEdit	*projectname;
			QLineEdit	*versionbox;
			QWidget		*hbox;
			QHBoxLayout	*hlayout;
			QPushButton	*cancelbutton=new QPushButton("&Cancel");
			QPushButton	*okbutton=new QPushButton("&Apply");

			QObject::connect(cancelbutton,&QPushButton::clicked,[this,diag]()
				{
					diag->reject();
				});
			QObject::connect(okbutton,&QPushButton::clicked,[this,diag]()
				{
					diag->accept();
				});
			projectname=new QLineEdit;
			versionbox=new QLineEdit;

			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(new QLabel("Project Name\t"),Qt::AlignLeft);
			hlayout->addWidget(projectname,Qt::AlignRight);
			docvlayout->addWidget(hbox);

			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(new QLabel("Project Version\t"),Qt::AlignLeft);
			hlayout->addWidget(versionbox,Qt::AlignRight);
			docvlayout->addWidget(hbox);

			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(cancelbutton);
			hlayout->addStretch(0);
			hlayout->addWidget(okbutton);

			docvlayout->addWidget(hbox);

			diag->setLayout(docvlayout);

			int res=diag->exec();
			if(res==1)
				{
					QProcess::execute("cp",QStringList()<<DATADIR "/docs/Doxyfile"<<".");
					runPipeAndCapture(QString("sed -i 's/^PROJECT_NAME=.*$/PROJECT_NAME=%1/;s/^PROJECT_NUMBER=.*$/PROJECT_NUMBER=%2/' '%3'").arg(projectname->text()).arg(versionbox->text()).arg("Doxyfile"));
				}
			else
				{
					return;
				}
		}

	this->showBarberPole("Building Docs","Please Wait","","0",QString("%1/progress").arg(this->tmpFolderName));

	fileinfo=QString("%1/html/index.html").arg(doc->getDirPath());
	fp=popen("doxygen Doxyfile","r");
	while(fgets(line,4095,fp))
		{
			line[strlen(line)-1]=0;
			this->runNoOutput(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg(line).arg(this->tmpFolderName));
		}
	pclose(fp);

	QString com=QString("/bin/echo '<meta http-equiv=\"refresh\" content=\"0; URL='file://%1'\" />' > %2").arg(fileinfo.absoluteFilePath()).arg(this->htmlFile);
	QProcess::execute("/bin/sh",QStringList()<<"-c"<<com);

	this->showWebPage("Doxygen Documentation","file://" + this->htmlFile);
	this->runNoOutput(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tmpFolderName));
	QDir::setCurrent(currentdir.canonicalPath());
}

void ManPageEditorQT::showDocs(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);

	if(doc==NULL)
		return;

	QFileInfo		fileinfo=QString("%1/html/index.html").arg(doc->getDirPath());

	if(fileinfo.exists()==false)
		this->buildDocs();
	else
		{
			QString com=QString("/bin/echo '<meta http-equiv=\"refresh\" content=\"0; URL='file://%1'\" />' > %2").arg(fileinfo.absoluteFilePath()).arg(this->htmlFile);
			QProcess::execute("/bin/sh",QStringList()<<"-c"<<com);
			this->showWebPage("Doxygen Documentation","file://" + this->htmlFile);
		}
}

void ManPageEditorQT::closeAllTabs(void)
{
	bool retval;
	this->sessionBusy=true;
	int	numtabs=(this->mainNotebook)->count();
	for(int loop=0; loop<numtabs; loop++)
		{
			closingAllTabs=true;
			retval=this->closeTab(0);
			if(retval==false)
				{
					this->sessionBusy=false;
					return;
				}
		}

	this->sessionBusy=false;
	this->rebuildBookMarkMenu();
	this->rebuildTabsMenu();
	this->currentSessionNumber=0xdeadbeef;
	this->closingAllTabs=false;
	this->setToolbarSensitive();
	this->pages.clear();
	this->newPageIndex=1;
	this->rebuildFunctionMenu(-1);
}

bool ManPageEditorQT::closeTab(int index)
{
	DocumentClass	*doc=NULL;
	int				thispage=index;
	QTextCursor		tc;
	bool				thisislasttab=false;
	plugData			pd;

	this->sessionBusy=true;

	if(this->closingAllTabs==true)
		thispage=0;
	else
		{
			if(index==-1)
				thispage=this->mainNotebook->currentIndex();
			else
				thispage=index;
		}

//plugins
	pd.doc=doc;
	pd.tabNumber=thispage;
	pd.what=DOCLOSE;
	this->runAllPlugs(pd);

	doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(thispage));
	if(doc!=0)
		{
			if(doc->dirty==true)
				{
					int result=this->askSaveDialog(doc->fileName);
					switch(result)
						{
							case QMessageBox::Save:
								this->saveFile(thispage,false);
       // Save was clicked
								break;
							case QMessageBox::Discard:
       // Don't Save was clicked
								break;
							case QMessageBox::Cancel:
       // Cancel was clicked
								this->sessionBusy=false;
								this->rebuildTabsMenu();
								return(false);
								break;
						}
				}

			if(this->closingAllTabs==false)
				this->handleBMMenu(this->mainNotebook->widget(thispage),REMOVEBOOKMARKSFROMPAGE,tc);

			if(thispage==this->mainNotebook->count()-1)
				thisislasttab=true;

			this->mainNotebook->removeTab(thispage);
			delete doc;
		}

	if(this->closingAllTabs==false)
		{
			if((thisislasttab==true) && (this->mainNotebook->count()>0))
				this->setTabVisibilty(this->mainNotebook->count()-1,true);
			this->sessionBusy=false;
			this->rebuildTabsMenu();
			this->setToolbarSensitive();
			this->rebuildFunctionMenu(-1);
		}

	return(true);
}

void ManPageEditorQT::shutDownApp()
{
//	if(this->forcedMultInst==false)//TODO//
	this->writeExitData();

	if(this->onExitSaveSession==true)
		this->doSessionsMenuItems();

	if(this->saveAllFiles(true)==true)
		{
//#ifdef _ASPELL_
			delete_aspell_config(this->aspellConfig);
			delete_aspell_speller(this->spellChecker);
//#endif
			QApplication::quit();
		}
}

QString ManPageEditorQT::truncateWithElipses(const QString str,int maxlen)
{
	QString newlabel;
	if(str.length()>maxlen)
		newlabel=str.left((maxlen-3)/2)+"..."+str.right((maxlen-3)/2);
	else
		newlabel=str;
	return(newlabel);
}

void ManPageEditorQT::setAppShortcuts(void)
{
	for(int j=0;j<NOMORESHORTCUT;j++)
		{
			if(this->appShortcuts[j]!=NULL)
				{
					delete this->appShortcuts[j];
					this->appShortcuts[j]=new QShortcut(this->mainWindow);
				}
			if(this->defaultShortCutsList.size()>j)
				{
					this->appShortcuts[j]->setKey(QKeySequence(this->defaultShortCutsList.at(j)));
					this->appShortcuts[j]->setObjectName(QString("%1").arg(j));
					QObject::connect(this->appShortcuts[j],SIGNAL(activated()),this,SLOT(doAppShortCuts()));
				}
			else
				{
					this->defaultShortCutsList<<"";
					this->appShortcuts[j]->setObjectName(QString("%1").arg(NOSHORTCUT));
				}
		}
}

void ManPageEditorQT::setToolbarSensitive(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	bool				override;
	bool				gotdoc=true;
	bool				hasselection=false;
	plugData			pd;

	if(this->sessionBusy==true)
		return;

	if(doc==NULL)
		{
			override=false;
			gotdoc=false;
		}
	else
		{
			override=doc->dirty;
			hasselection=doc->textCursor().hasSelection();
		}

	if(this->mainNotebook->count()==0)
		{
			this->saveSessionsMenu->setEnabled(false);
			this->currentSessionNumber=0xdeadbeef;
			this->mainNotebook->setScrollButtonStatus(BOTHSCROLLBUTTONS,false,false);
		}
	else
		{
			this->mainNotebook->setScrollButtonStatus(BOTHSCROLLBUTTONS,true,true);
			if(this->mainNotebook->currentIndex()==0)
				this->mainNotebook->setScrollButtonStatus(LEFTSCROLLBUTTON,false,true);
			else if(this->mainNotebook->currentIndex()==this->mainNotebook->count()-1)
				this->mainNotebook->setScrollButtonStatus(RIGHTSCROLLBUTTON,false,true);
			if(this->mainNotebook->count()==1)
				this->mainNotebook->setScrollButtonStatus(BOTHSCROLLBUTTONS,false,true);
			this->saveSessionsMenu->setEnabled(true);
		}

	if(this->currentSessionNumber==0xdeadbeef)
		this->saveCurrentSessionMenuItem->setEnabled(false);
	else
		this->saveCurrentSessionMenuItem->setEnabled(true);
		
	this->restoreDefaultSessionMenuItem->setEnabled(this->onExitSaveSession);

	this->pluginMenu->setEnabled(!this->pluginMenu->isEmpty());

	for(int j=0;j<this->prefsToolBarLayout.length();j++)
		{
			switch(this->prefsToolBarLayout.at(j).toLatin1())
				{
//new
					case 'N':
						this->newMenuItem->setEnabled(true);
						break;
//open+recent
					case 'O':
						this->newMenuItem->setEnabled(true);
						break;
//save
					case 'S':
						this->saveMenuItem->setEnabled(override);
						break;

//cut
					case 'X':
						this->cutMenuItem->setEnabled((gotdoc==true) && ((hasselection) & (!doc->isReadOnly())));
						break;
//copy
					case 'C':
						this->copyMenuItem->setEnabled(hasselection);
						break;
//paste
					case 'P':
						this->pasteMenuItem->setEnabled((gotdoc==true) && (doc->canPaste()));
						break;
//delete
					case 'd':
						this->deleteMenuItem->setEnabled((gotdoc==true) && (hasselection==true));
						break;
//undo
					case 'U':
						this->undoMenuItem->setEnabled((gotdoc==true) && (doc->gotUndo));
						this->undoAllMenuItem->setEnabled((gotdoc==true) && (doc->gotUndo));
						break;
//redo
					case 'R':
						this->redoMenuItem->setEnabled((gotdoc==true) && (doc->gotRedo));
						this->redoAllMenuItem->setEnabled((gotdoc==true) && (doc->gotRedo));
						break;
//find
					case 'F':
						this->findMenuItem->setEnabled(true);
						break;
//navigation
					case 'G':
						this->goToDefineMenuItem->setEnabled(hasselection);
						break;
//go back
					case 'B':
						this->goBackMenu->setEnabled(true);
						break;
//go foward
					case 'W':
						this->goFowardMenu->setEnabled(true);
						break;

					case '9':
						this->lineNumberWidget->setEnabled(gotdoc);
						this->goToLineDialogMenuItem->setEnabled(gotdoc);
						break;
//find in gtkdoc
					case 'A':
						this->findGtkApiWidget->setEnabled(true);
						break;

//find in qt5doc
					case 'Q':
						this->findQtApiWidget->setEnabled(true);
						break;
//find in function def
					case 'D':
						this->findDefWidget->setEnabled(true);
						break;
//livesearch
					case 'L':
						this->liveSearchWidget->setEnabled(gotdoc);
						break;
				}
		}
//plugins
	pd.tabNumber=this->mainNotebook->currentIndex();
	pd.what=DOSETSENSITVE;
	this->runAllPlugs(pd);
}

void ManPageEditorQT::runCLICommands(int quid)
{
	msgStruct	message;
	int			msglen;
	QString		opensessionname;
	QStringList	list;
	char			*pathtopwd;

	if(quid==-1)
		fprintf(stderr,"From KKeditQT Can't create message queue, scripting wont work :( ...\n");
	else
		{
			if(this->parser.isSet("quit"))
				{
 					msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s","quit");
					message.mType=QUITAPPMSG;
					msgsnd(quid,&message,msglen,0);
				}

			if(this->parser.isSet("restore-session"))
				{
					opensessionname=this->parser.value("restore-session");
 					msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s",opensessionname.toStdString().c_str());
					message.mType=RESTORESESSIONMSG;
					msgsnd(quid,&message,msglen,0);
				}

			pathtopwd=get_current_dir_name();
			list=this->parser.positionalArguments();
			for(int j=0;j<list.count();j++)
				{
					if(list.at(j).at(0)!="/")
						msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s/%s",pathtopwd,list.at(j).toStdString().c_str());
					else
						msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s",list.at(j).toStdString().c_str());
					message.mType=OPENFILEMSG;
					msgsnd(quid,&message,msglen,0);
				}
			msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s","ACTIVATE");
			message.mType=ACTIVATEAPPMSG;
			msgsnd(quid,&message,msglen,0);

			if(pathtopwd!=NULL)
				free(pathtopwd);
		}
}

void ManPageEditorQT::setDocMenu(void)
{
//#ifdef _BUILDDOCVIEWER_
	if(this->docView->isVisible()==true)//ugly hack!!//
		{
			this->toggleDocViewMenuItem->setText("Hide Docviewer");
			this->docviewerVisible=true;
		}
	else
		{
			this->toggleDocViewMenuItem->setText("Show Docviewer");
			this->docviewerVisible=false;
		}
//#endif
}

void ManPageEditorQT::showWebPage(QString windowtitle,QString url)
{
//#ifdef _BUILDDOCVIEWER_
	if(windowtitle.isEmpty()==false)
		this->docView->setWindowTitle(windowtitle);

	this->webEngView->load(QUrl::fromUserInput(url));
	this->docView->show();
	this->docView->activateWindow();
	this->docView->raise();

	this->docView->setWindowState(Qt::WindowNoState);//TODO//doesnt work

	this->setDocMenu();
//#else
	QDesktopServices::openUrl(QUrl(url));
//#endif
}

void ManPageEditorQT::printDocument(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);

	if(doc==NULL)
		return;

	if(this->prefsPrintCommand.isEmpty()!=true)
		{
			QProcess::startDetached(this->prefsPrintCommand,QStringList(doc->getFilePath()));
			return;
		}

//N.B. CUPS/QT5 has a problem with QT5/CUPS so only print to pdf is available, this aint my fault! 
	QPrinter		printer(QPrinter::HighResolution);
	printer.setOutputFileName(QString("%1/Documents/%2.pdf").arg(this->homeFolder).arg(doc->getFileName()));
 
    QPrintDialog	dialog(&printer,this->mainWindow);
	if(dialog.exec()!=QDialog::Accepted)
		return;

	doc->print(&printer);
}

void ManPageEditorQT::setCompWordList(void)
{
	QString				results;
	QString				command;
	QString				paths;
	DocumentClass		*doc;
	QAbstractItemModel	*model;

	for(int j=0;j<this->mainNotebook->count();j++)
		{
			doc=this->getDocumentForTab(j);
			paths+="'"+doc->getFilePath()+"' ";
		}

	command=QString("grep -Eho '[[:alpha:]_]{%1,}' %2|sort -u").arg(this->autoShowMinChars).arg(paths);
	results=this->runPipeAndCapture(command);
	this->completionWords=results.split("\n",Qt::SkipEmptyParts);

	if(this->completer!=NULL)
		delete this->completer;

    this->completer=new QCompleter(this->completionWords,this);
	this->completer->setCaseSensitivity(Qt::CaseInsensitive);
	model=new QStringListModel(this->completionWords,this->completer);
	this->completer->setCompletionMode(QCompleter::PopupCompletion);
	this->completer->setModel(model);
	this->completer->setWrapAround(false);
	QObject::connect(this->completer,QOverload<const QString &>::of(&QCompleter::activated),[=](const QString &text)
		{
			this->insertCompletion(text);
		});
}

void ManPageEditorQT::insertCompletion(const QString& completion)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	QTextCursor		tc;
	if(this->completer->widget()!=doc)
		return;
	tc=doc->textCursor();
	tc.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::NextWord,QTextCursor::KeepAnchor);
	tc.removeSelectedText();
    tc.insertText(completion);
    doc->setTextCursor(tc);
}

void ManPageEditorQT::loadPlugins(void)
{
	int				cnt=0;
	QDir				pluginsDir(this->homeDataFolder+"/plugins/");

	if(this->verySafeFlag==true)
		return;

//local plugins
	QDirIterator		lit(pluginsDir.canonicalPath() ,QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (lit.hasNext())
		{
			QString s=lit.next();
			pluginStruct	ps;

			ps.plugPath=s;
			ps.local=true;
			if(this->loadPlug(&ps)==false)
				qWarning()<<"Error loading plug > " << s;

			this->plugins[cnt++]=ps;
		}

//global plugins
	pluginsDir.setPath(QString("%1/plugins/").arg(DATADIR));
	QDirIterator				git(pluginsDir.canonicalPath() ,QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (git.hasNext())
		{
			QString			s=git.next();
			pluginStruct	ps;

			ps.plugPath=s;
			ps.local=false;
			if(this->loadPlug(&ps)==false)
				qWarning()<<"Error loading plug > " << s;

			this->plugins[cnt++]=ps;
		}
}

bool ManPageEditorQT::loadPlug(pluginStruct *ps,bool force)
{
	QObject	*plugininst=NULL;

	ps->statusChanged=false;

	if(ps->loaded==true)
		return(true);

	ps->pluginLoader=new QPluginLoader(ps->plugPath);
	plugininst=ps->pluginLoader->instance();
	if(plugininst!=nullptr)
		{
			ps->instance=qobject_cast<kkEditQTPluginInterface*>(plugininst);
			if((this->disabledPlugins.contains(ps->plugPath)==false) || (force==true))
				{
					if((this->safeFlag==false)||(force==true))
						{
							ps->instance->initPlug(this,ps->plugPath);//TODO//return false if cant init
							ps->loaded=true;
							ps->wants=ps->instance->plugWants();
						}
					else
						{
							ps->loaded=false;
							ps->wants=DONONE;
						}
				}
			else
				{
					ps->loaded=false;
					ps->wants=DONONE;
				}
					
			ps->plugName=ps->pluginLoader->metaData().value("MetaData").toObject().value("name").toString();
			ps->plugVersion=ps->pluginLoader->metaData().value("MetaData").toObject().value("version").toString();
		}
	else
		{
			ps->loaded=false;
			ps->broken=true;
			ps->plugName=QFileInfo(ps->plugPath).fileName();
			return(false);
		}

	return(true);
}

bool ManPageEditorQT::unloadPlug(pluginStruct *ps)
{
	ps->statusChanged=false;

	if(ps->loaded==false)
		return(true);

	ps->instance->unloadPlug();
	ps->pluginLoader->unload();
	delete ps->pluginLoader;
	ps->pluginLoader=NULL;
	ps->loaded=false;

	return(true);
}

void ManPageEditorQT::setTabVisibilty(int tab,bool visible)
{
	DocumentClass	*doc;
	int				tabnum=tab;
	bool				vis=visible;

	if(this->sessionBusy==false)
		if(tabnum==this->mainNotebook->count()-1)//ui bug fix no last tab invisible
			vis=true;

	this->mainNotebook->setTabVisible(tabnum,vis);
	doc=this->getDocumentForTab(tabnum);
	if(doc==NULL)
		return;

	doc->visible=vis;

	if(vis==false)//hacks for tab gliches
		{
			if(this->mainNotebook->currentIndex()==tabnum)
				this->mainNotebook->scrollTabsLeft();
			tabnum=this->mainNotebook->currentIndex();
			this->mainNotebook->setCurrentIndex(0);
			this->mainNotebook->setCurrentIndex(tabnum);
		}
	else
		{
			this->mainNotebook->setCurrentIndex(0);
			this->mainNotebook->setCurrentIndex(tabnum);
		}
}

void ManPageEditorQT::runAllPlugs(plugData pd)
{
	for(int j=0;j<this->plugins.count();j++)
		{
			if((this->plugins[j].loaded) && ((this->plugins[j].wants & pd.what)==pd.what))
				{
					pd.plugName=this->plugins[j].plugName;
					pd.plugPath=this->plugins[j].plugPath;
					pd.plugVersion=this->plugins[j].plugVersion;
//#ifdef _DEBUGCODE_
					pd.printIt();
//#endif
					this->plugins[j].instance->plugRun(&pd);
				}
		}
}

void ManPageEditorQT::setBit(int *data,int bit)
{
	*data=*data | (1<<bit);
}

void ManPageEditorQT::resetBit(int *data,int bit)
{
	*data=*data & ~(-1 & (1<<bit));
}

int ManPageEditorQT::getBit(int data,int bit)
{
	return((data & (1<<bit)) && true);
}

QStringList ManPageEditorQT::tailStringList(QStringList list,int maxsize)
{	
	QStringList tlist2=list;
	QStringList tlist;

	if(list.size()<maxsize)
		return(list);

	tlist2.removeDuplicates();
	for(int j=tlist2.size()-maxsize;j<tlist2.size();j++)
		tlist.append(tlist2.at(j));
	return(tlist);
}

void ManPageEditorQT::runNoOutput(QString command,bool sync,bool asroot)
{
	QStringList	args;
	QString		com;

	if(asroot==false)
		{
			com="sh";
			args<<"-c"<<QString("cd %1;%2").arg(this->toolsFolder).arg(command);
		}
	else
		{
			args=QProcess::splitCommand(this->prefsRootCommand);
			com=args.at(0);
			args.removeFirst();
			args<<"sh"<<"-c"<<QString("cd %1;%2").arg(this->toolsFolder).arg(command);
		}

	if(sync==true)
		QProcess::execute(com,args);
	else
		QProcess::startDetached(com,args);
}

void ManPageEditorQT::sendMessgage(QString msg)
{
	msgStruct	buffer;

	strncpy(buffer.mText,msg.toStdString().c_str(),MAXMSGSIZE-1);
	buffer.mType=SENDMSG;
	if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
		{
			qDebug()<<"Can't send message :"<<msg;
		}
}
#endif