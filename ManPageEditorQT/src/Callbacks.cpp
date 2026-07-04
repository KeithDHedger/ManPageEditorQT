/*
 *
 * ©K. D. Hedger. Sat  2 Mar 16:07:00 GMT 2024 keithdhedger@gmail.com

 * This file (Callbacks.cpp) is part of ManPageEditorQT.

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

#include "Callbacks.h"

void ManPageEditorQT::doFileMenuItems(MenuItemClass *mc)
{
	switch(mc->getMenuID())
		{
//file menu
			case NEWPAGEMENUITEM:
				this->closeTabs(true);
				this->mpConv->manString=mpclass->getProperties();
				this->currentFilePath="";
				break;

			case NEWMENUITEM:
				this->buildSectionProps("NEW SECTION",false,true);
				this->mainNotebook->setCurrentIndex(this->mainNotebook->currentIndex()+1);
				break;
			case OPENMENUITEM:
				{
					QString	filepath="";
					filepath=QFileDialog::getOpenFileName(nullptr,"Open File",this->lastLoadDir,"",nullptr,QFileDialog::HideNameFilterDetails);
					if(filepath.isEmpty()==false)
						{
							if(this->closeTabs(true)==true)
								{
									this->lastLoadDir=QFileInfo(filepath).dir().absolutePath();
									this->currentFilePath=filepath;
									this->mpConv->importManpage(filepath);
								}
						}
				}
				break;

			case SAVEMENUITEM:
				if(this->currentFilePath.isEmpty()==false)
					{
						this->mpConv->exportManpage(this->currentFilePath);
						for(int j=0;j<this->mainNotebook->count();j++)
							{
								QTextEdit	*te=this->getDocumentForTab(j);
								te->document()->setModified(false);
							}
						break;
					}
			case SAVEASMENUITEM:
				{
					QString filepath;
					filepath=QFileDialog::getSaveFileName(nullptr,"Save File",this->lastSaveDir,"",nullptr,QFileDialog::HideNameFilterDetails);
					if(filepath.isEmpty()==false)
						{
							this->currentFilePath=filepath;
							this->lastSaveDir=QFileInfo(filepath).canonicalPath();
							this->mpConv->exportManpage(filepath);
							for(int j=0;j<this->mainNotebook->count();j++)
								{
									QTextEdit	*te=this->getDocumentForTab(j);
									te->document()->setModified(false);
								}
						}
					else
						this->currentFilePath="";
				}
				break;

			case PREVIEWMENUITEM:
				this->doPreView();
				break;
			case PROPSMENUITEM:
				this->buildProperties("");
				break;

			case CLOSEPAGEMENUITEM:
				this->closeTabs(true);
				break;

			case QUITMENUITEM:
				if(this->closeTabs(true)==true)
					{
						this->writeExitData();
						QApplication::quit();
					}
				break;
			case PREFSMENUITEM:
				this->doPrefs();
				break;
			case OPENSYSPAGEMENUITEM:
				{
					QString filepath;
					filepath=this->mpConv->buildOpenSystemPage();
					if(filepath.isEmpty()==false)
						{
							if(this->closeTabs(true)==true)
								this->mpConv->importManpage(filepath);
						}
				}
				break;
			case OPENTEMPLATEMENUITEM:
				if(this->closeTabs(true)==true)
					{
						this->mpConv->importManpage(QString("%1/docs/template.1").arg(this->realDataDir));
						this->currentFilePath="";
					}
				break;
		}
}

void ManPageEditorQT::doEditMenuItems(MenuItemClass *mc)
{
	QTextEdit	*te=this->getDocumentForTab(this->mainNotebook->currentIndex());
	if(te==NULL)
		return;
	switch(mc->getMenuID())
		{
			case UNDOMENUITEM:
				te->undo();
				break;
			case REDOMENUITEM:
				te->redo();
				break;
			case CUTMENUITEM:
				te->cut();
				break;
			case COPYMENUITEM:
				te->copy();
				break;
			case PASTEMENUITEM:
				te->paste();
				break;

			case SPELLCHECKMENUITEM:
				//this->spellCheckDoc(te);
				this->doSpellCheckDoc(te);
				break;

			case FINDMENUITEM://TODO//
				//this->findReplaceDialog->show();
			//	this->findReplaceDialog->activateWindow();
				break;
		}
}

void ManPageEditorQT::doFormatMenuItems(MenuItemClass *mc)
{
	switch(mc->getMenuID())
		{
			case BOLDMENUITEM:
				this->doClear();
				this->doBold();
				break;
			case ITALICMENUITEM:
				this->doClear();
				this->doItalic();
				break;
			case CLEARMENUITEM:
				this->doClear();
				break;
		}
}

void ManPageEditorQT::doHelpMenuItems(MenuItemClass *mc)
{
	switch(mc->getMenuID())
		{
			case ABOUTMENUITEM:
				{
					AboutBoxClass	about(this->mainWindow,QString("%1/pixmaps/%2.png").arg(this->realDataDir).arg(PACKAGE));
					QFile			file(QString("%1/docs/gpl-3.0.txt").arg(this->realDataDir));
							
					if(file.open(QIODevice::ReadOnly | QIODevice::Text))
						{
							QTextStream in(&file);
							about.licence=in.readAll();
							file.close();
						}
					about.credits=credits;
					about.setHomepage(QString("%1").arg(GLOBALWEBSITE),"Home Page");
					about.setBodyText("A Qt 5/6 Linux manpage editor");
					about.showAboutQtButton(true);
					about.showLicenceButton(true);
					about.showCreditsButton(true);

					about.runAbout();
				}
				break;
			case ABOUTQTMENUITEM:
				QMessageBox::aboutQt(nullptr);
				break;

			case HELPMENUITEM:
				{
					AboutBoxClass	about(this->mainWindow);
					about.showHelp(QString("%1/docs/help.html").arg(this->realDataDir));
				}
				break;
		}
}

void ManPageEditorQT::doLiveSearch(const QString text)
{
	QTextEdit	*doc=this->getDocumentForTab(-1);
	bool			retval;
	QTextCursor	savetc=doc->textCursor();

	if(this->application->queryKeyboardModifiers()==Qt::ShiftModifier)
		{
			QTextCursor tc=doc->textCursor();
			tc.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,text.length());
			doc->setTextCursor(tc);
			retval=doc->find(text,QTextDocument::FindBackward);
			if(retval==false)
				{
					QTextCursor tc=doc->textCursor();
					tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
					doc->setTextCursor(tc);
					retval=doc->find(text,QTextDocument::FindBackward);
				}
		}
	else
		{
			QTextCursor savetc=doc->textCursor();
			retval=doc->find(text);
			if(retval==false)
				{
					QTextCursor tc=doc->textCursor();
					tc.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
					doc->setTextCursor(tc);
					retval=doc->find(text);
				}
		}
	if(retval==false)
		doc->setTextCursor(savetc);
}
