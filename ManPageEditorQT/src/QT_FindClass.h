/*
 *
 * ©K. D. Hedger. Thu 16 Jul 15:03:16 BST 2026 keithdhedger@gmail.com

 * This file (QT_FindClass.h) is part of ManPageEditorQT.

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

#ifndef _QT_FINDCLASS_
#define _QT_FINDCLASS_

class QT_FindClass
{
	public:
		QT_FindClass(QMainWindow *window);
		~QT_FindClass();

#ifdef USEPLAIN
		QPlainTextEdit			*te=NULL;
#else
		QTextEdit				*te=NULL;
#endif

		QAction					findWrapped;
		QString					hiliteAllColour="#40ff0000";
		QDialog					*findReplaceDialog=NULL;
		bool						showMultiDoc=false;
		bool						autoHighlightAll=true;
		bool						searchBack=false;
		bool						highlightAll=false;
		
		void						showFindDialog(void);
		bool						checkForMatch(void);
		bool						findString(bool replace);
		void						highLightAllMatches(void);
		void						resetHighLights(void);
		void						resetSearchPositions(void);

	private:
//guis
		QMainWindow				*parent=NULL;
		QComboBox				*findDropBox=NULL;
		QComboBox				*replaceDropBox=NULL;
		QCheckBox				*caseCheck=NULL;
		QCheckBox				*regexCheck=NULL;
		QCheckBox				*findAfterCheck=NULL;
		QCheckBox				*wrapCheck=NULL;
		QCheckBox				*multiDocumentCheck=NULL;
		QCheckBox				*highlightAllCheck=NULL;
		QCheckBox				*replaceAllCheck=NULL;
		QCheckBox				*backwardsCheck=NULL;
		QPushButton				*replaceButton=NULL;
		QPushButton				*findButton=NULL;
		QStringList				findList;
		QStringList				replaceList;
		QSettings				prefs;

		QString					targetString;
		QString					replacementString;
		QTextCursor				holdCursor;
		QRegularExpressionMatch	match;

		bool						insensitiveSearch=false;
		bool						useRegex=false;
		bool						findAfterReplace=true;
		bool						wrapSearch=false;
		bool						findInMultDocument=false;
		bool						replaceAll=false;
        		
		int						startPosition=-1;
		int						holdStartPos=-1;
        		
		void						buildFindDialog(void);
		void						hideDialog(void);
        		
		void						setSearchStrings(void);
		void						replaceAllMatches(void);
		void						findButtonCB(void);
		void						replaceButtonCB(void);
		void						replaceMatched(QTextCursor tc);
		QRegularExpression		getRegex(void);
};

#endif
