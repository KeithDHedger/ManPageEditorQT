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
#include "prefsDefaultConfig.h"

ManPageEditorQT::ManPageEditorQT(QApplication *app)
{
	this->application=app;
	this->initApp();
	this->buildWordCheckQt();
}

ManPageEditorQT::~ManPageEditorQT()
{
	QDir		fold(this->tmpFolderName);
	fold.removeRecursively();

	delete_aspell_config(this->aspellConfig);
	delete_aspell_speller(this->spellChecker);
	delete this->mpConv;
	delete this->fc;
	delete this->spellCheckWord;
}

MenuItemClass* ManPageEditorQT::makeMenuItemClass(int mainmenu,const QString name,const QKeySequence key,const QString iconname,int userdata)
{
	MenuItemClass	*menuitem=new MenuItemClass(name,this->mainWindow);
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

void ManPageEditorQT::reHighLight(void)
{
	QTextEdit	*te=this->getDocumentForTab(-1);

	if(te==NULL)
		return;

	this->fc->te=te;
	if(this->fc->findReplaceDialog->isVisible()==true && this->fc->highlightAll==true)
		{
			this->fc->resetHighLights();
			this->hiliteLine(te,this->lineHiliteColour);
			this->fc->highLightAllMatches();
		}
	else
		this->hiliteLine(te,this->lineHiliteColour);
}

void ManPageEditorQT::buildMainGui(void)
{
	this->mainNotebook=new NoteBookClass(this,this->mainWindow);
	QObject::connect(this->mainNotebook,&QTabWidget::currentChanged,[this](int index)
		{
			this->reHighLight();
		});

	this->menuBar=new QMenuBar(this->mainWindow);

//file menu
	this->fileMenu=new QMenu("&File",this->menuBar);
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
	this->editMenu=new QMenu("&Edit",this->menuBar);
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
	if(this->spellChecker!=NULL)
		this->spellCheckMenuItem=this->makeMenuItemClass(EDITMENU,"Spell Check Section",0,"tools-check-spelling",SPELLCHECKMENUITEM);

//find//TODO//
	this->findMenuItem=this->makeMenuItemClass(EDITMENU,"Find",QKeySequence::Find,"edit-find",FINDMENUITEM);

	this->editMenu->addSeparator();

//format menu
	this->formatMenu=new QMenu("&Formating",this->menuBar);
	this->menuBar->addMenu(this->formatMenu);
	this->boldMenuItem=this->makeMenuItemClass(FORMATMENU,"Bold",QKeySequence::fromString("Ctrl+B"),"format-text-bold",BOLDMENUITEM);
	this->italicMenuItem=this->makeMenuItemClass(FORMATMENU,"Italic",QKeySequence::fromString("Ctrl+I"),"format-text-italic",ITALICMENUITEM);
	this->clearMenuItem=this->makeMenuItemClass(FORMATMENU,"Clear",QKeySequence::fromString("Ctrl+R"),"edit-clear",CLEARMENUITEM);

//help
	this->helpMenu=new QMenu("&Help",this->menuBar);
	this->menuBar->addMenu(this->helpMenu);
//
//about
	this->makeMenuItemClass(HELPMENU,"About",0,"help-about",ABOUTMENUITEM);

//help
	this->makeMenuItemClass(HELPMENU,"Help",0,"help-contents",HELPMENUITEM);

	this->setUpToolBar();
	this->mainWindow->setMenuBar(qobject_cast<QMenuBar*>(this->menuBar));
	this->mainWindow->addToolBar(&this->toolBar);
	this->mainWindow->setCentralWidget(this->mainNotebook);
}

QTextEdit* ManPageEditorQT::makeNewTab(QString html,QString sectname,bool issub,int pos)
{
	QString		fh;
	QTextEdit	*te=new QTextEdit;
	te->setCurrentFont(QFont(this->fontName,this->fontSize));
	te->setFont(QFont(this->fontName,this->fontSize));
	QTextCursor	cursor(te->document());

	cursor.select(QTextCursor::Document);

	te->setHtml(html);
	fh=te->toHtml();

	fh=fh.replace(QRegularExpression("(<span style.*)font-family:'.*'(.*</span>)",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),"\\1font-family:'"+this->fontName+"'\\2");

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
			this->reHighLight();
		});
	return(te);
}

void ManPageEditorQT::doChangeWord(QTextEdit *te)
{
	QString	qstr;

	if(te->textCursor().hasSelection()==true)
		badWord=te->textCursor().selectedText();

	goodWord=wordListDropbox->currentText();
	te->textCursor().insertText(goodWord);

	this->blockFlag=false;
}

void ManPageEditorQT::buildWordCheckQt(void)
{
	QVBoxLayout	*vlayout=new QVBoxLayout;
	QWidget		*hbox;
	QHBoxLayout	*hlayout;
	QPushButton	*button;
	QSettings	prefs("KDHedger",PACKAGE_NAME);

	this->spellCheckWord=new QDialog(this->mainWindow);
	this->spellCheckWord->setWindowTitle("Check Section");

	hlayout=new QHBoxLayout;
	hbox=new QWidget(spellCheckWord);
	hbox->setLayout(hlayout);

	badWordLabel=new QLabel(QString("Change <i><b>%1</b></i> to: ").arg(badWord),spellCheckWord);
	hlayout->addWidget(badWordLabel);
	this->wordListDropbox=new QComboBox(spellCheckWord);
	hlayout->addWidget(wordListDropbox);
	vlayout->addWidget(hbox);

//buttons
	hlayout=new QHBoxLayout;
	hbox=new QWidget(spellCheckWord);
	hbox->setLayout(hlayout);

	button=new QPushButton("&Apply",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("dialog-ok"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			QTextEdit *te=this->getDocumentForTab(this->mainNotebook->currentIndex());
			doChangeWord(te);
		});
	hlayout->addWidget(button);

	button=new QPushButton("&Ignore",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("list-remove"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			aspell_speller_add_to_session(this->spellChecker,qPrintable(this->badWord),-1);
			this->badWord.clear();
			this->blockFlag=false;
		});
	hlayout->addWidget(button);

	button=new QPushButton("A&dd",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("list-add"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			aspell_speller_add_to_personal(this->spellChecker,qPrintable(this->badWord),-1);
			aspell_speller_save_all_word_lists(this->spellChecker);
			this->badWord.clear();
			this->blockFlag=false;
		});
	hlayout->addWidget(button);

	button=new QPushButton("&Close",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("dialog-cancel"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			QSettings	prefs("KDHedger",PACKAGE_NAME);
			prefs.setValue("spell/spellgeometry",this->spellCheckWord->saveGeometry());

			this->badWord.clear();
			this->goodWord.clear();
			this->cancelCheck=true;
			this->blockFlag=false;
			this->spellCheckWord->hide();
		});
	hlayout->addWidget(button);

	vlayout->addWidget(hbox);
	spellCheckWord->setLayout(vlayout);
	spellCheckWord->setModal(true);
	spellCheckWord->restoreGeometry(prefs.value("spell/spellgeometry").toByteArray());
}

bool ManPageEditorQT::checkTheWord(QString word)
{
	int						correct;
	AspellWordList			*suggestions=NULL;
	AspellStringEnumeration	*elements=NULL;
	const char				*suggestedword=NULL;
	int						wordcnt=0;
	bool						retval=false;

	wordListDropbox->clear();
	correct=aspell_speller_check(spellChecker,qPrintable(word),-1);
	if(correct==0)
		{
			badWord=word;
			cancelCheck=false;
			badWordLabel->setText(QString("Change <i><b>%1</b></i> to: ").arg(badWord));
			suggestions=(AspellWordList*)aspell_speller_suggest(spellChecker,qPrintable(word),-1);
			if(suggestions==NULL)
				return(false);
			elements=aspell_word_list_elements(suggestions);
			if(elements==NULL)
				return(false);
			while((suggestedword=aspell_string_enumeration_next(elements))!=NULL)
				{
					wordListDropbox->addItem(suggestedword);
					wordcnt++;
					suggestedword=NULL;
				}
			if(wordcnt==0)
				return(false);

			retval=true;
			delete_aspell_string_enumeration(elements);
			spellCheckWord->show();
			while(blockFlag==true)
				qApp->processEvents();
		}
	return(retval);
}


void ManPageEditorQT::doSpellCheckDoc(QTextEdit *te)
{
	QList<QTextEdit::ExtraSelection>	extraSelections;
	QTextCharFormat					format;
	QTextEdit::ExtraSelection		selection;
	QTextCursor						mc;
	QTextCursor						cursor;
	QString							xbadword	;
	QSettings						defaults("KDHedger",PACKAGE_NAME);
	AspellCanHaveError				*ret;
	AspellDocumentChecker			*checker;
	AspellToken						token;
	int								diff;
	unsigned int						goodwordlen;
	char								*word_begin;
	char								*line=NULL;
	int								buffdiff=0;

	/* Set up the document checker */
	if(spellChecker==NULL)
		return;
	ret=new_aspell_document_checker(spellChecker);
	if (aspell_error(ret)!=0)
		{
			printf("Error: %s\n",aspell_error_message(ret));
			return;
		}

	line=strdup(qPrintable(te->toPlainText()));
	if(line==NULL)
		return;
	checker=to_aspell_document_checker(ret);
	  /* First process the line */
	aspell_document_checker_process(checker,line,-1);
	diff=0;
	  /* Now find the misspellings in the line */
	while(token=aspell_document_checker_next_misspelling(checker),token.len!=0)
		{
			if(this->spellCheckWord->isHidden()==true)
				this->spellCheckWord->show();
			free(line);
			line=strdup(qPrintable(te->toPlainText()));
			/* Pay particular attention to how token.offset and diff is used */
			xbadword=QString(line).mid(token.offset+diff,token.len);
			goodWord.clear();
			cursor=te->textCursor();
			cursor.setPosition(token.offset-buffdiff,QTextCursor::MoveAnchor);
			te->setTextCursor(cursor);
			te->ensureCursorVisible();

			extraSelections.clear();
			format.setBackground(QColor(this->extraHiliteColour)); // Set highlight color
			selection.format=format;
			selection.cursor=te->textCursor();
			selection.cursor.clearSelection();
			selection.cursor.setPosition(token.offset-buffdiff,QTextCursor::MoveAnchor);
			selection.cursor.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);

			extraSelections.append(selection);
			// Apply the highlight
			te->setExtraSelections(extraSelections);

			this->blockFlag=true;
			checkTheWord(xbadword);
			if(cancelCheck==true)
				{
					delete_aspell_document_checker(checker);
					te->setPlainText(line);
					free(line);
					return;
				}
			word_begin=line+token.offset+diff;
			if(goodWord.isEmpty()==false)
				{
					goodwordlen=goodWord.length();
					/* Replace the misspelled word with the replacement */
					diff+=goodwordlen-token.len;
					memmove(word_begin+goodwordlen,word_begin+token.len,strlen(word_begin+token.len)+1);
					memcpy(word_begin,qPrintable(goodWord),goodwordlen);
					te->setPlainText(line);
					buffdiff+=badWord.length()-goodWord.length();
				}
		}
	if(this->spellCheckWord->isHidden()==false)
				this->spellCheckWord->hide();

	delete_aspell_document_checker(checker);
//replace all text in check document
	te->setPlainText(line);
	free(line);
	defaults.setValue("spell/spellgeometry",this->spellCheckWord->saveGeometry());
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
	QDir		tdir;
	QString	tstr;
	QFile	file;

	this->homeFolder=QString("%1").arg(tdir.homePath());
	this->realDataDir=QString("%1%2").arg(getenv("APPDIR")).arg(DATADIR);
	this->realBinDir=QString("%1%2").arg(getenv("APPDIR")).arg(BINDIR);

	this->tmpFolderName=mkdtemp(tmpfoldertemplate);
	if(this->tmpFolderName.isEmpty()==true)
		{
			qDebug()<<"Can't create temporary folder, quitting ...";
			exit (100);
		}
	this->mainWindow=new QMainWindow;
	this->mpConv=new ManpageConvertClass(this);

	QIcon::setThemeSearchPaths(QStringList()<<QString("%1/usr/share/icons").arg(getenv("APPDIR"))<<QString("/usr/share/icons")<<QString("%1/.icons").arg(getenv("HOME")) <<QString("%1/icons").arg(this->realDataDir) );
	QIcon::setFallbackSearchPaths(QStringList()<<QString("%1/usr/share/icons").arg(getenv("APPDIR"))<<QString("/usr/share/icons")<<QString("%1/.icons").arg(getenv("HOME"))  <<QString("%1/icons").arg(this->realDataDir));
	QIcon::setFallbackThemeName("manpageedqticons");

	AspellCanHaveError	*possible_err;
	this->aspellConfig=new_aspell_config();

	possible_err=new_aspell_speller(this->aspellConfig);
	if(aspell_error_number(possible_err)!=0)
		{
			aspell_config_replace(this->aspellConfig,"dict-dir","/lib/aspell");
			possible_err=new_aspell_speller(this->aspellConfig);
		}

	if(aspell_error_number(possible_err)!= 0)
		{
			qDebug()<<aspell_error_message(possible_err);
			qDebug()<<"Install some dictionary's in /lib/aspell ...";
		}
	else
		this->spellChecker=to_aspell_speller(possible_err);

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

	this->fc=new QT_FindClass(this->mainWindow);
	this->fc->autoHighlightAll=false;

	if(this->prefs.contains("app/geometry")==true)
		this->mainWindow->restoreGeometry(prefs.value("app/geometry").toByteArray());
	else
		this->mainWindow->setGeometry(100,100,800,400);

//	this->setToolbarSensitive();//TODO//
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
//editor
	this->prefs.setValue("app/geometry",this->mainWindow->saveGeometry());
	this->prefs.setValue("editor/lastsavedir",this->lastSaveDir);
	this->prefs.setValue("editor/lastloaddir",this->lastLoadDir);
}

void ManPageEditorQT::readConfigs(void)
{
//editor
	this->lastSaveDir=this->prefs.value("editor/lastsavedir","").toString();
	this->lastLoadDir=this->prefs.value("editor/lastloaddir","").toString();
}

bool ManPageEditorQT::confirmClose(QTextEdit *te)
{
	if(te->document()->isModified()==true)
		{
			if(QMessageBox::question(nullptr,"Document Modified","Document has been modified, please confirm close...",QMessageBox::Cancel|QMessageBox::Close)==QMessageBox::Cancel)
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
			if(QMessageBox::question(nullptr,"Document Modified","Document has been modified, please confirm close...",QMessageBox::Cancel|QMessageBox::Close)==QMessageBox::Cancel)
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
	fmt.clearProperty(QTextFormat::TextUnderlineStyle);
	te->setCurrentCharFormat(fmt);			
}

void ManPageEditorQT::doPreView(void)
{
	runExternalProcClass	rp;
//	rp.showCli=true;

	this->mpConv->exportManpage(this->tmpFolderName+"/preview",true);
	rp.runExternalCommands(QString("%1 man '%2/preview'").arg(this->terminalCommand).arg(this->tmpFolderName).toStdString(),false,"/dev/null");
}

void ManPageEditorQT::doPrefs(void)
{
	prefsClass	newprefs;

	newprefs.createDialog(PACKAGE_STRING,configStr);
	if(newprefs.dialogPrefs.valid==true)
		{
			QTextEdit	*te;
			QString		fh;
			QFont		fnt;

			mpclass->terminalCommand=newprefs.dialogPrefs.editBoxes[TERMBOX]->text();
			mpclass->lineHiliteColour=newprefs.dialogPrefs.colourBoxes[HIGHLIGHTBOX]->text();
			mpclass->extraHiliteColour=newprefs.dialogPrefs.colourBoxes[SPELLBOX]->text();
			mpclass->zipPages=newprefs.dialogPrefs.checkBoxes[GZIPBOX]->isChecked();
			if(newprefs.dialogPrefs.checkBoxes[WRAPBOX]->isChecked()==true)
				mpclass->lineWrap=QTextEdit::WidgetWidth;
			else
				mpclass->lineWrap=QTextEdit::NoWrap;

			if(newprefs.dialogPrefs.checkBoxes[ITALICBOX]->isChecked()==true)
				mpclass->italicMenuItem->setAppearance("format-text-underline","Underline","Ctrl+U");
			else
				mpclass->italicMenuItem->setAppearance("format-text-italic","Italic","Ctrl+I");
			mpclass->useUnderline=newprefs.dialogPrefs.checkBoxes[ITALICBOX]->isChecked();
		
			fnt.fromString(newprefs.dialogPrefs.fontBoxes[FONTBOX]->text());
			mpclass->fontName=fnt.family();
			mpclass->fontSize=fnt.pointSize();

			for(int j=0;j<this->mainNotebook->count();j++)
				{
					te=this->getDocumentForTab(j);
					te->setLineWrapMode(this->lineWrap);
					te->setFont(QFont(this->fontName,this->fontSize));
					te->setCurrentFont(QFont(this->fontName,this->fontSize));

					fh=te->toHtml();
					fh=fh.replace(QRegularExpression("(<span style.*)font-family:'.*'(.*</span>)",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),"\\1font-family:'"+this->fontName+"'\\2");

					QTextCursor	cursor(te->document());
					cursor.select(QTextCursor::Document);
					fh=fh.replace(QRegularExpression("font-size:(.*)pt",QRegularExpression::InvertedGreedinessOption),QString("font-size:%1pt").arg(this->fontSize));
					if(mpclass->useUnderline==true)
						fh=fh.replace(QRegularExpression("(<span style.*)font-style:italic(.*</span>)",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),"\\1text-decoration: underline\\2");
					else
						fh=fh.replace(QRegularExpression("(<span style.*)text-decoration: underline(.*</span>)",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),"\\1font-style:italic\\2");

					te->setHtml(fh);
					te->setLineWrapMode(this->lineWrap);
				}
		}
}

//TODO//
#if 0

void ManPageEditorQT::setToolbarSensitive(void)

#endif