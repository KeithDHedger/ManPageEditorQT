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
	//MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
qDebug()<<mc;
	switch(mc->getMenuID())
		{
//file menu
			case NEWMENUITEM:
				break;
			case OPENMENUITEM:
				{
					QString	filepath="";
					filepath=QFileDialog::getOpenFileName(nullptr,"Open File",this->lastLoadDir,"",nullptr,QFileDialog::HideNameFilterDetails);
					if(filepath.isEmpty()==false)
						{
							this->lastLoadDir=QFileInfo(filepath).dir().absolutePath();
							this->currentFilePath=filepath;
							this->mpConv->importManpage(filepath);
						}
				}
				break;

			case SAVEMENUITEM:
				if(this->currentFilePath.isEmpty()==true)
					break;
				this->mpConv->exportManpage(this->currentFilePath);
				break;
			case SAVEASMENUITEM:
				{
					QString filepath;
					filepath=QFileDialog::getSaveFileName(nullptr,"Save File",this->lastSaveDir,"",nullptr,QFileDialog::HideNameFilterDetails);
					if(filepath.isEmpty()==false)
						{
							this->currentFilePath=filepath;
							this->lastSaveDir=QFileInfo(filepath).canonicalPath();
							this->mpConv->exportManpage(filepath);
						}
				}
				break;

			case PRINTMENUITEM:
			//	this->printDocument();
				break;
			case CLOSEMENUITEM:
				this->closeTabs();
				break;
			case QUITMENUITEM:
				this->writeExitData();
				QApplication::quit();
				break;
		}
}

void ManPageEditorQT::doEditMenuItems(MenuItemClass *mc)
{
qDebug()<<mc;
	switch(mc->getMenuID())
		{
			case UNDOMENUITEM:
				//if(document!=NULL)
					//document->undo();
				break;
			case REDOMENUITEM:
				//if(document!=NULL)
				//	document->redo();
				break;
			case CUTMENUITEM:
				//if(document!=NULL)
				//	document->cut();
				break;
			case COPYMENUITEM:
				//if(document!=NULL)
				//	document->copy();
				break;
			case PASTEMENUITEM:
				//if(document!=NULL)
				//	document->paste();
				break;
			case FINDMENUITEM:
				//this->findReplaceDialog->show();
			//	this->findReplaceDialog->activateWindow();
				break;
			case PREFSMENUITEM:
				//this->doPrefs();
				break;
		}
}

void ManPageEditorQT::doFormatMenuItems(MenuItemClass *mc)
{
qDebug()<<mc;
	switch(mc->getMenuID())
		{
			case BOLDMENUITEM:
				this->doBold();
				break;
			case ITALICMENUITEM:
				this->doItalic();
				break;
			case CLEARMENUITEM:
				this->doClear();
				break;
		}
}

void ManPageEditorQT::doHelpMenuItems(MenuItemClass *mc)
{
qDebug()<<mc;
	switch(mc->getMenuID())
		{
			case ABOUTMENUITEM:
				break;
			case ABOUTQTMENUITEM:
				break;
			case HELPMENUITEM:
				break;
		}
}

void ManPageEditorQT::doLiveSearch(const QString text)
{
	qDebug()<<"doLiveSearch";
//	DocumentClass	*doc=this->getDocumentForTab(-1);
//	bool			retval;
//	QTextCursor		savetc=doc->textCursor();
//
//	if(this->application->queryKeyboardModifiers()==Qt::ShiftModifier)
//		{
//			QTextCursor tc=doc->textCursor();
//			tc.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,text.length());
//			doc->setTextCursor(tc);
//			retval=doc->find(text,QTextDocument::FindBackward);
//			if(retval==false)
//				{
//					QTextCursor tc=doc->textCursor();
//					tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
//					doc->setTextCursor(tc);
//					retval=doc->find(text,QTextDocument::FindBackward);
//				}
//		}
//	else
//		{
//			QTextCursor savetc=doc->textCursor();
//			retval=doc->find(text);
//			if(retval==false)
//				{
//					QTextCursor tc=doc->textCursor();
//					tc.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
//					doc->setTextCursor(tc);
//					retval=doc->find(text);
//				}
//		}
//	if(retval==false)
//		doc->setTextCursor(savetc);
}
