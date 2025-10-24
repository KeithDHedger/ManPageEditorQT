/*
 *
 * ©K. D. Hedger. Tue 21 Oct 12:50:36 BST 2025 keithdhedger@gmail.com

 * This file (prefsClass.h) is part of ManPageEditorQT.

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

#ifndef _PREFSCLASS_
#define _PREFSCLASS_

#include "Globals.h"

#define RITESTRETCH 2

struct prefsReturnStruct
{
//simple
	QDialog					*theDialog=NULL;
	QHash<int,QLineEdit*>	editBoxes;
	QHash<int,QString>		editBoxesPrefsName;
	int						editBoxCnt=0;
	QHash<int,QComboBox*>	comboBoxes;
	QHash<int,QString>		comboBoxesPrefsName;
	int						comboBoxCnt=0;
	QHash<int,QCheckBox*>	checkBoxes;
	QHash<int,QString>		checkBoxesPrefsName;
	int						checkBoxCnt=0;
	QHash<int,QDoubleSpinBox*>		spinBoxes;
	QHash<int,QString>		spinBoxesPrefsName;
	int						spinBoxCnt=0;

//compound
	QHash<int,QLineEdit*>	colourBoxes;
	QHash<int,QString>		colourBoxesPrefsName;
	int						colourBoxCnt=0;
	QHash<int,QLineEdit*>	fontBoxes;
	QHash<int,QString>		fontBoxesPrefsName;
	int						fontBoxCnt=0;
	QHash<int,QLineEdit*>	fileBoxes;
	QHash<int,QString>		fileBoxesPrefsName;
	int						fileBoxCnt=0;

	bool						valid=false;
};

struct boolTuple
{
	bool		value;
	bool		valid;
};

struct stringTuple
{
	QString	value;
	bool		valid;
};

struct comboTuple
{
	QString	value;
	int		index;
	bool		valid;
};

struct spinTuple
{
	double	value;
	double	min;
	double	max;
	bool		valid;
};

class prefsClass
{
	public:
		prefsClass();
		~prefsClass();

	prefsReturnStruct		dialogPrefs;
	QStringList				prefsNames;
	bool						paged=false;
	QString					opSep="\n";
	QDialogButtonBox			*bb=NULL;
	bool						autoshowDialog=true;
	bool						useSavedPrefs=true;
	int						button=QDialogButtonBox::NoButton;

	bool						reUseDialog=false;

	void						createDialog(QString title,QStringList items,QSize sze=QSize(-1,-1));
	void						createDialog(QString title,QString filepath,QSize sze=QSize(-1,-1));
	QSize					adjustBoxSize(int defw,int defy);


//	void						setPrefs(QStringList items);//TODO//
//	void						setPrefValue(QString name,QVariant val);//TODO//
//	QVariant					getPrefValue(QString name);//TODO//
//	void						writePrefs(void);//TODO//
//	void						readPrefs(void);//TODO//
	void						printCurrentPrefs(void);
	void						saveCurrentPrefs(void);

	boolTuple				getCheckValue(QString prefsname);//TODO//
	stringTuple				getEditValue(QString prefsname);//TODO//
	comboTuple				getComboValue(QString prefsname);//TODO//
	spinTuple				getSpinValue(QString prefsname);//TODO//
	stringTuple				getColourValue(QString prefsname);//TODO//
	stringTuple				getFontValue(QString prefsname);//TODO//
	stringTuple				getFileValue(QString prefsname);//TODO//

	boolTuple				getBoolValue(QString prefsname);
	stringTuple				getStringValue(QString prefsname);
	spinTuple				getFloatValue(QString prefsname);


	int						findByName(QHash<int,QString> section,QString name);

	QString					bestFontColour(QString colour);

	protected:
	private:
		//unsigned long		hashFromKey(QString key);//TODO//
};

#endif