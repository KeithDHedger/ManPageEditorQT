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

	delete_aspell_config(this->aspellConfig);
	delete_aspell_speller(this->spellChecker);
	delete this->mpConv;
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
	this->mainNotebook=new NoteBookClass(this);
	this->menuBar=new QMenuBar;

//file menu
	this->fileMenu=new QMenu("&File");
	this->menuBar->addMenu(this->fileMenu);

//new page
	this->newMenuItem=this->makeMenuItemClass(FILEMENU,"New Manpage",QKeySequence::New,"document-new",NEWPAGEMENUITEM);

//new section
	this->newMenuItem=this->makeMenuItemClass(FILEMENU,"New Section",0,"document-new",NEWMENUITEM);

	this->fileMenu->addSeparator();

//open
	this->openMenuItem=this->makeMenuItemClass(FILEMENU,"Open Manpage",QKeySequence::Open,"document-open",OPENMENUITEM);

//import sys page
	this->openSysPageMenuItem=this->makeMenuItemClass(FILEMENU,"Open Sysytem Manpage",0,"document-open",OPENSYSPAGEMENUITEM);

//open Template
	this->openTemplateMenuItem=this->makeMenuItemClass(FILEMENU,"Open Template",0,"document-open",OPENTEMPLATEMENUITEM);

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

	this->fileMenu->addSeparator();

//close page
	this->closeMenuItem=this->makeMenuItemClass(FILEMENU,"Close Manpage",QKeySequence::Close,"window-close",CLOSEPAGEMENUITEM);
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

	this->editMenu->addSeparator();

//spellcheck
	this->spellCheckMenuItem=this->makeMenuItemClass(EDITMENU,"Spell Check Section",0,"tools-check-spelling",SPELLCHECKMENUITEM);

//find//TODO//
	this->findMenuItem=this->makeMenuItemClass(EDITMENU,"Find",QKeySequence::Find,"edit-find",FINDMENUITEM);

this->findMenuItem->setEnabled(false);

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

QTextEdit* ManPageEditorQT::makeNewTab(QString html,QString sectname,bool issub,int pos)
{
	QString		fh;
	QTextEdit	*te=new QTextEdit;
	QTextCursor	cursor(te->document());

	cursor.select(QTextCursor::Document);
	te->setFont(QFont(this->fontName,this->fontSize));
	te->setHtml(html);
	fh=te->toHtml();
	fh=fh.replace("Monospace",this->fontName);
	te->setHtml(fh);

	if(issub==false)
		{
			this->mainNotebook->insertTab(pos,te,sectname.toUpper());
			te->setStatusTip(".SH "+sectname);
		}
	else
		{
			this->mainNotebook->insertTab(pos,te,sectname.toLower());
			te->setStatusTip(".SS "+sectname);
		}
	te->setLineWrapMode(this->lineWrap);

	QObject::connect(te,&QTextEdit::cursorPositionChanged,[this,te]()
		{
			this->hiliteLine(te,this->lineHiliteColour);
		});
	return(te);
}

void ManPageEditorQT::spellCheckDoc(QTextEdit *te)
{
	QString							qstr;
	QTextCursor						cursor;
	QByteArray						bytearray;
	QTextCharFormat					format;
	QList<QTextEdit::ExtraSelection>	extraSelections;
	QString							form;
	QString							opts;
	QStringList						slist;
	QTextCursor						mc;
	QTextEdit::ExtraSelection		selection;
	AspellCanHaveError				*ret;
	AspellDocumentChecker			*checker;
	AspellToken						token;
	AspellWordList					*suggestions;
	AspellStringEnumeration			*elements;
	char*							line;
	int								docstart=0;
	const char						*suggestedword;

	cursor=te->textCursor();
	qstr=te->toPlainText();
	bytearray=qstr.toUtf8();
	line=(char*)bytearray.constData();

	/* Set up the document checker */
	ret=new_aspell_document_checker(this->spellChecker);
	if (aspell_error(ret)!=0)
		{
			printf("Error: %s\n",aspell_error_message(ret));
			return;
		}

	checker=to_aspell_document_checker(ret);
	  /* First process the line */
	aspell_document_checker_process(checker,line,-1);
	  /* Now find the misspellings in the line */

	while(token=aspell_document_checker_next_misspelling(checker),token.len!=0)
		{
			prefsClass						newprefs;
	    /* Pay particular attention to how token.offset and diff is used */
			this->goodWord="";
			this->cancelCheck=false;
			this->returnWord=true;
			cursor.setPosition(docstart+token.offset);
			cursor.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);
			this->badWord=cursor.selectedText();

			extraSelections.clear();
			format.setBackground(QColor(this->extraHiliteColour)); // Set highlight color
			format.setForeground(QColor(newprefs.bestFontColour(this->extraHiliteColour))); // Set highlight color
			selection.format=format;
			selection.cursor=cursor;
			selection.cursor.clearSelection();
			selection.cursor.setPosition(docstart+token.offset);
			selection.cursor.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);

//move selection into view
			mc=selection.cursor;
			mc.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,2);
			mc.movePosition(QTextCursor::StartOfBlock);
			te->setTextCursor(mc);
			mc.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor,2);
			mc.movePosition(QTextCursor::StartOfBlock);
			te->setTextCursor(mc);

			extraSelections.append(selection);
//			// Apply the highlight
			te->setExtraSelections(extraSelections);

			form="";
			opts="";
			slist.clear();

			suggestions=(AspellWordList*)aspell_speller_suggest(spellChecker,this->badWord.toStdString().c_str(),-1);
			elements=aspell_word_list_elements(suggestions);

			while((suggestedword=aspell_string_enumeration_next(elements))!=NULL)
				opts+=QString("%1@").arg(suggestedword);
			delete_aspell_string_enumeration(elements);

			if(opts.isEmpty()==true)
				continue;
			//no default entry
			opts="@"+opts;
			form=QString("edit@Unknown Word@%2@combostart@Correct Word To@%1comboend@").arg(opts).arg(this->badWord);

			QDialogButtonBox::StandardButton	dbutton=(QDialogButtonBox::StandardButton)((int)QDialogButtonBox::Close|(int)QDialogButtonBox::Ignore|(int)QDialogButtonBox::Apply);

			newprefs.paged=false;
			newprefs.useSavedPrefs=false;
			newprefs.bb->setStandardButtons(dbutton);
			newprefs.dialogPrefs.valid=false;

			slist=form.split('@');
			newprefs.createDialog(QString("Spell Check"),slist,QSize(400,-1));//TODO//
			//newprefs.createDialog(QString("Spell Check"),slist);

			if(newprefs.dialogPrefs.valid==true)
				{
					if(newprefs.button==QDialogButtonBox::Apply)
						{
							comboTuple ct=newprefs.getComboValue("Correct Word To");
							cursor.insertText(ct.value);
							qstr=te->toPlainText();
							bytearray=qstr.toUtf8();
							line=(char*)bytearray.constData();
							aspell_document_checker_process(checker,line,-1);
							continue;
						}

					if(newprefs.button==QDialogButtonBox::Close)
						{
							delete_aspell_document_checker(checker);
							this->hiliteLine(te,this->lineHiliteColour);
							return;
						}

					if(newprefs.button==QDialogButtonBox::Ignore)
						{
							this->hiliteLine(te,this->lineHiliteColour);
							aspell_speller_add_to_session(this->spellChecker,this->badWord.toStdString().c_str(),-1);
							continue;
						}
				}
		}		
	delete_aspell_document_checker(checker);
}

void ManPageEditorQT::hiliteLine(QTextEdit *te,QColor colour)
{
	QTextCursor						cursor=te->textCursor();
	QTextCharFormat					format;
	QList<QTextEdit::ExtraSelection>	extraSelections;
	QTextEdit::ExtraSelection		selection;

	format.setBackground(QColor(colour)); // Set highlight color
	// Highlight the current line
	selection.format=format;
	selection.cursor=cursor;
	selection.cursor.clearSelection();
	selection.cursor.movePosition(QTextCursor::StartOfBlock);
	selection.cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
	extraSelections.append(selection);
	// Apply the highlight
	te->setExtraSelections(extraSelections);
}

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
			if(te->document()->isModified()==true)
				{
					if(this->confirmClose(te)==false)
						return;
				}
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

	AspellCanHaveError	*possible_err;
	this->aspellConfig=new_aspell_config();
	possible_err=new_aspell_speller(this->aspellConfig);

	if(aspell_error_number(possible_err)!= 0)
		puts(aspell_error_message(possible_err));
	else
		this->spellChecker=to_aspell_speller(possible_err);
//
//TODO//

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

//application

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

//document
////application

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
}

bool ManPageEditorQT::confirmClose(QTextEdit *te)
{
	if(te->document()->isModified()==true)
		{
			if(QMessageBox::question(nullptr,"Document Modified","Document has been modified, please confirm close...")==QMessageBox::No)
				return(false);
		}
	return(true);
}

bool ManPageEditorQT::closeTabs(bool all)
{
	bool docdirty=false;
	for(int j=0;j<this->mainNotebook->count();j++)
		{
			QTextEdit	*te=this->getDocumentForTab(j);
			if(te->document()->isModified()==true)
				docdirty=true;
		}

	if(docdirty==true)
		{
			if(QMessageBox::question(nullptr,"Document Modified","Document has been modified, please confirm close...")==QMessageBox::No)
				return(false);
		}

	while(this->mainNotebook->count()>0)
		{
			QTextEdit	*te=this->getDocumentForTab(0);
			this->mainNotebook->removeTab(0);
			delete te;
		}

	this->mpConv->manString=mpclass->getProperties();
	return(true);
}

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

			if(create==false)
				{
					te=this->getDocumentForTab(-1);
					if(subchk->isChecked()==false)
						{
							this->mainNotebook->setTabText(this->mainNotebook->currentIndex(),tabname.trimmed().toUpper());
							te->setStatusTip(".SH "+tabname.trimmed());
						}
					else
						{
							this->mainNotebook->setTabText(this->mainNotebook->currentIndex(),tabname.trimmed().toLower());
							te->setStatusTip(".SS "+tabname.trimmed());
						}
				}
			else
				{
					this->makeNewTab("",tabname,subchk->isChecked(),this->mainNotebook->currentIndex()+1);
				}
		}
}

void ManPageEditorQT::doBold(void)
{
	QTextEdit		*te=this->getDocumentForTab(this->mainNotebook->currentIndex());
	QTextCharFormat	fmt;

	fmt.setFontWeight(QFont::Normal);			
	fmt.setFontUnderline(false);			

	fmt.setFontWeight(QFont::Bold);			
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
	te->setCurrentCharFormat(fmt);
}

void ManPageEditorQT::doClear(void)
{
	QTextEdit		*te=this->getDocumentForTab(this->mainNotebook->currentIndex());
	QTextCharFormat	fmt;

	fmt.setFontWeight(QFont::Normal);			
	fmt.setFontUnderline(false);			
	fmt.setFontItalic(false);			
	te->setCurrentCharFormat(fmt);			
}

void ManPageEditorQT::doPreView(void)
{
	runExternalProcClass	rp;

	this->mpConv->exportManpage(this->tmpFolderName+"/preview",true);
	rp.runExternalCommands(QString("%1 \"man '%2/preview'\"").arg(this->terminalCommand,this->tmpFolderName).toStdString(),false,"/dev/null");
}

void ManPageEditorQT::doPrefs(void)
{
	prefsClass	newprefs;

	QDialogButtonBox::StandardButton	dbutton=(QDialogButtonBox::StandardButton)((int)QDialogButtonBox::Ok|(int)QDialogButtonBox::Cancel);

	newprefs.paged=false;
	newprefs.bb->setStandardButtons(dbutton);
	newprefs.autoshowDialog=true;
	newprefs.dialogPrefs.valid=false;

	newprefs.createDialog("ManpageQT Prefs",QString("%1/%2").arg(DATADIR).arg("docs/prefs.config"),QSize(450,-1));
	if(newprefs.dialogPrefs.valid==true)
		{
			newprefs.saveCurrentPrefs();
	
			QTextEdit	*te;
			prefsClass	newprefs;
			stringTuple	st;
			boolTuple	bt;
			QString		fh;
			QString		oldname=this->fontName;

			st=newprefs.getStringValue("teminal_command");
			if(st.valid==true)
				mpclass->terminalCommand=st.value;

			st=newprefs.getStringValue("highlight_colour");
				if(st.valid==true)
					mpclass->lineHiliteColour=st.value;

			st=newprefs.getStringValue("extra_highlight_colour");
				if(st.valid==true)
					mpclass->extraHiliteColour=st.value;

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
						this->lineWrap=QTextEdit::WidgetWidth;
					else
						this->lineWrap=QTextEdit::NoWrap;
				}

			st=newprefs.getStringValue("main_font");
			if(st.valid==true)
				{
					QFont fnt;
					fnt.fromString(st.value);
					this->fontName=fnt.family();
					this->fontSize=fnt.pointSize();
				}

			for(int j=0;j<this->mainNotebook->count();j++)
				{
					te=this->getDocumentForTab(j);
						te->setLineWrapMode(this->lineWrap);

					fh=te->toHtml();
					QTextCursor	cursor(te->document());
					cursor.select(QTextCursor::Document);
					te->setFont(QFont(this->fontName,this->fontSize));
					te->setHtml(fh);
					fh=te->toHtml();
					fh=fh.replace(oldname,this->fontName);
					fh=fh.replace(QRegularExpression("font-size:(.*)pt",QRegularExpression::InvertedGreedinessOption),QString("font-size:%1pt").arg(this->fontSize));
					if(mpclass->useUnderline==true)
						fh=fh.replace(R"foo(<span style=" font-style:italic;">)foo",R"foo(<span style=" text-decoration: underline;">)foo");
					else
						fh=fh.replace(R"foo(<span style=" text-decoration: underline;">)foo",R"foo(<span style=" font-style:italic;">)foo");
					te->setHtml(fh);
					te->setLineWrapMode(this->lineWrap);
				}
		}
}

#if 0

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
}

#endif