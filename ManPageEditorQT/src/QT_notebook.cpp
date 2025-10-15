/*
 *
 * ©K. D. Hedger. Fri 10 Oct 16:50:55 BST 2025 keithdhedger@gmail.com

 * This file (QT_notebook.cpp) is part of ManPageEditorQT.

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

#include "Globals.h"

NoteBookClass::~NoteBookClass()
{
}

NoteBookClass::NoteBookClass(ManPageEditorQT *mpclass,QWidget *parent): QTabWidget(parent)
{
	QIcon		qicon;
	QWidget		*container;
	QHBoxLayout	*layout;

	this->mainManPageEditorQTClass=mpclass;
	this->setAcceptDrops(true);
	this->scrollLeft=new QToolButton(this);
	this->scrollRight=new QToolButton(this);
	qicon=QIcon::fromTheme("go-previous");
	this->scrollLeft->setIcon(qicon);
	qicon=QIcon::fromTheme("go-next");
	this->scrollRight->setIcon(qicon);

	container=new QWidget(this);
	layout=new QHBoxLayout(container);
//scroll rite
	container->setFixedHeight(this->tabBar()->height());
	container->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed));
	layout->setContentsMargins(1,0,1,0);
	this->scrollRight->setFixedHeight(tabBar()->height());
	this->scrollRight->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	this->scrollRight->setToolButtonStyle(Qt::ToolButtonIconOnly);
	layout->addWidget(this->scrollRight);
	this-> setCornerWidget(container, Qt::TopRightCorner);

//scroll left
	container=new QWidget(this);
	layout=new QHBoxLayout(container);
	container->setFixedHeight(this->tabBar()->height());
	container->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed));
	layout->setContentsMargins(1,0,1,0);
	this->scrollLeft->setFixedHeight(tabBar()->height());
	this->scrollLeft->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	this->scrollLeft->setToolButtonStyle(Qt::ToolButtonIconOnly);
	layout->addWidget(this->scrollLeft);
	this-> setCornerWidget(container, Qt::TopLeftCorner);

	this->scrollLeft->setAutoRepeat(true);
	this->scrollRight->setAutoRepeat(true);

	QObject::connect(this->scrollLeft,&QPushButton::clicked,[this]()
		{
			this->scrollTabsLeft();
		});
	QObject::connect(this->scrollRight,&QPushButton::clicked,[this]()
		{
			this->scrollTabsRight();
		});

	this->tabBar()->setStyleSheet(QString("QTabBar::scroller{width: 0px;}"));
	this->setDocumentMode(true);
	this->setTabsClosable(true);
	this->setMovable(true);
}

void NoteBookClass::scrollTabsLeft(void)
{
	int	ctab=this->currentIndex()-1;

	while((ctab>-1) && (this->isTabVisible(ctab)==false))
		ctab--;

	if(ctab==-1)
		return;

	this->setCurrentIndex(ctab);
}

void NoteBookClass::scrollTabsRight(void)
{
	int	ctab=this->currentIndex()+1;

	while((ctab<this->count()) && (this->isTabVisible(ctab)==false))
		ctab++;

	if(ctab==this->count())
		ctab=this->count()-1;
	else
		this->setCurrentIndex(ctab);
}
