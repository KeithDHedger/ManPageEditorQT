/*
 *
 * ©K. D. Hedger. Mon  8 Sep 10:59:37 BST 2025 keithdhedger@gmail.com

 * This file (ManpageConvert.cpp) is part of ManPageEditorQT.

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

#include "ManpageConvert.h"
#include "QT_ManpageExporter.h"

ManpageConvertClass::~ManpageConvertClass()
{
}

ManpageConvertClass::ManpageConvertClass(ManPageEditorQT *mainclass)
{
	this->mainClass=mainclass;
}

QString ManpageConvertClass::buildOpenSystemPage(void)
{
	QDialog			propsdialog;
	QWidget			*hbox;
	QHBoxLayout		*hlayout;
	QVBoxLayout		*docvlayout=new QVBoxLayout;
	QDialogButtonBox	*buttonBox=NULL;
	QFrame			separator(nullptr);
	QLineEdit		*nameedit;
	QComboBox		*section;
	const char		*propname[]={"Manpage name","Section"};

	hbox=new QWidget;
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox->setLayout(hlayout);
	nameedit=new QLineEdit();
	nameedit->setPlaceholderText(propname[0]);
	hlayout->addWidget(nameedit);
	docvlayout->addWidget(hbox);

	hbox=new QWidget;
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox->setLayout(hlayout);
	section=new QComboBox();
	section->addItems(QStringList()<<"Auto"<<"1 Executable programs or shell commands"<<"2 System calls (functions provided by the kernel)"<<"3 Library calls (functions within program libraries)"<<"4 Special files (usually found in /dev)"<<"5 File formats and conventions eg /etc/passwd"<<"6 Games"<<"7 Miscellaneous, e.g. man(7), groff(7)"<<"8 System administration commands (usually only for root)"<<"9 Kernel routines [Non standard]");

	hlayout->addWidget(section,1,Qt::AlignLeft);
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
	propsdialog.setWindowTitle("Open System Manpage");
	int ret=propsdialog.exec();
	delete buttonBox;
	if(ret==0)
		{
			QString				command;
			std::string			sfp;
			runExternalProcClass	rp;

			if(section->currentIndex()==0)
				command=QString("man -w %1").arg(nameedit->text());
			else
				command=QString("man -s%2 -w %1").arg(nameedit->text()).arg(section->currentIndex());

			rp.trimOP=true;
			sfp=rp.runExternalCommands(command.toStdString(),true);
			return(QString::fromStdString(sfp));
		}
	return("");
}

void ManpageConvertClass::exportManpage(QString filepath,bool nozip)
{
	QString	manpage=this->mainClass->getProperties(this->manString);
	QString os="";
	
	for(int j=0;j<this->mainClass->mainNotebook->count();j++)
		{
			QTextEdit *te=this->mainClass->getDocumentForTab(j);
			os+="\n"+te->statusTip()+"\n";
			os+=QManpageExporter(te->document()).toMan();
			os=os.trimmed();
		}

	os.replace(QRegularExpression("\n(?![\n[:space:]])"),"\n.br\n");
	os=manpage+"\n"+os;

	QFile data(filepath);
	if(data.open(QFile::WriteOnly|QFile::Truncate))
		{
			prefsClass	newprefs(QString("%1").arg(PACKAGE_NAME));
			//boolTuple	bt;
			bool bt;
			QTextStream	out(&data);
			out<<os;
			data.close();

//			newprefs.getSavedPrefValue("gzip_pages");
//			//bt=newprefs.getBoolValue("gzip_pages");
//			//if((bt.valid==true) && (bt.value==true) && (nozip==false))
//			if(bt==true) && (nozip==false))
//				{
//					runExternalProcClass	rp;
//					rp.runExternalCommands(QString("gzip -f '%1'").arg(filepath).toStdString(),false,"/dev/null");
//				}
		}
	else
		{
			QString filepath;
			filepath=QFileDialog::getSaveFileName(nullptr,"Save File",this->mainClass->lastSaveDir,"",nullptr,QFileDialog::HideNameFilterDetails);
			if(filepath.isEmpty()==false)
				{
					this->mainClass->currentFilePath=filepath;
					this->mainClass->lastSaveDir=QFileInfo(filepath).canonicalPath();
					this->exportManpage(filepath);
				}

			for(int j=0;j<this->mainClass->mainNotebook->count();j++)
				{
					QTextEdit	*te=this->mainClass->getDocumentForTab(j);
					te->document()->setModified(false);
				}
		}
}

void ManpageConvertClass::importManpage(QString filepath)
{
	QString				adjfp=filepath;
	runExternalProcClass	rp;

	if(QFile::exists(this->mainClass->tmpFolderName+"/adpage")==true)
		QFile::remove(this->mainClass->tmpFolderName+"/adpage");

	if(filepath.endsWith(".gz"))
		{
			rp.runExternalCommands(QString("gunzip -c %1").arg(filepath).toStdString(),false,QString("%2/adpage").arg(this->mainClass->tmpFolderName).toStdString());
			adjfp=this->mainClass->tmpFolderName+"/adpage";
			this->mainClass->currentFilePath="";
		}

	QFile				file(adjfp);
	QString				content;
	QStringList			sections;
	bool					retval;
	QString				htmlstr;
	QString				com;
	std::string			sfp="";

	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			this->mainClass->lastLoadDir=QFileInfo(adjfp).dir().absolutePath();
			this->mainClass->currentFilePath=adjfp;
			content=QString::fromUtf8(file.readAll());
			file.close();
			if(content.startsWith(".so"))
				{
					this->importManpage(this->mainClass->lastLoadDir+"/../"+content.mid(4).trimmed());
					return;
				}
		}

	if(filepath!=adjfp)
		{
			this->mainClass->lastLoadDir=QFileInfo(filepath).dir().absolutePath();
			this->mainClass->currentFilePath="";
		}

	if(QFileInfo::exists(adjfp)==false)
		{
			qDebug()<<"no such manpage ...";
			return;
		}

	rp.trimOP=true;
	sfp=rp.runExternalCommands(QString("cat %1| sed -n '/^.TH/p'").arg(adjfp).toStdString(),true);
	if(sfp.empty()==true)
		{
			sfp=rp.runExternalCommands(QString("cat %1| sed -n 's/^.Dt\\(.*\\)$/.TH \\1/p'").arg(adjfp).toStdString(),true);
			sfp=sfp+" "+rp.runExternalCommands(QString("cat %1|sed -n 's/.Dd[[:space:]]*\\(.*\\)$/\"\\1\"/p'").arg(adjfp).toStdString(),true);
		}

	this->manString=this->mainClass->getProperties(QString::fromStdString(sfp));
	this->mainClass->mainWindow->setWindowTitle(QString("Manpage for %1").arg(this->mainClass->pageProperties.name));

	setenv("GROFF_SGR","1",1);
	setenv("MANWIDTH","2000",1);
	setenv("MAN_KEEP_FORMATTING","1",1);

	com=QString("echo -e '\n.SH \"\"'|cat '%1' -|sed 's/^\\(\\.S[Hh]\\) \\(.*\\)/\\n@SECTION@--\\2--\\n\\1 \\2/g;s/^\\(\\.S[Ss]\\) \\(.*\\)/\\n@section@++\\2++\\n\\1 \\2/g'|man -l --no-justification --no-hyphenation -|ul|head -n -4").arg(adjfp);
	sfp=rp.runExternalCommands(com.toStdString(),true);
	if(sfp.empty()==true)
		{
			com=QString("echo -e '\n.SH \"\"'|cat '%1' -|sed 's/^\\(\\.S[Hh]\\) \\(.*\\)/\\n@SECTION@--\\2--\\n\\1 \\2/g;s/^\\(\\.S[Ss]\\) \\(.*\\)/\\n@section@++\\2++\\n\\1 \\2/g'|man -l --no-justification --no-hyphenation -|head -n -4|tee \"/tmp/man.log.raw\"").arg(adjfp);
			sfp=rp.runExternalCommands(com.toStdString(),true);
		}
	if(sfp.empty()==true)
		{
			com=QString("echo -e '\n.SH \"\"'|cat '%1'|sed 's/^\\(\\.S[Hh]\\) \\(.*\\)/\\n@SECTION@--\\2--\\n\\1 \\2/g;s/^\\(\\.S[Ss]\\) \\(.*\\)/\\n@section@++\\2++\\n\\1 \\2/g'|mandoc -l -O width=500|ul|head -n -2").arg(adjfp);
			sfp=rp.runExternalCommands(com.toStdString(),true);
		}

	unsetenv("GROFF_SGR");
	unsetenv("MANWIDTH");
	unsetenv("MAN_KEEP_FORMATTING");
	content=QString::fromStdString(sfp);

	sections=content.split(QRegularExpression("@SECTION@|@section@",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),Qt::KeepEmptyParts);

	for(int j=1;j<sections.size();j++)
		{
			QString tstr=sections.at(j);
			QStringList lines=sections.at(j).split("\n");
			QString name=lines.at(0);
			bool issub=false;
			if(name.startsWith("++")==true)
				issub=true;
			QString cname=QString(name).replace(QRegularExpression(R"RX((\+\+|--)(.*)(\+\+|--))RX"),"\\2");
			cname.replace(QRegularExpression("^\"|\"$"),"");
			htmlstr="";
			for (int k=3;k<lines.count();k++)
				{
					QString buf=lines.at(k);
					buf.remove(0,7);
					htmlstr+=buf+"\n";
					htmlstr=htmlstr.replace("<","&lt;");
					htmlstr=htmlstr.replace(">","&gt;");
					htmlstr=htmlstr.replace("\n.br","");
					htmlstr=htmlstr.replace("\n\n\n","\n");
				}
			htmlstr="<style> pre {white-space: pre-wrap;word-wrap: break-word;overflow-x: auto;}</style>\n<pre>"+htmlstr;

			htmlstr=htmlstr.replace(QRegularExpression(R"RX(\e\[1m(.*)\e\[.{,3}m)RX",QRegularExpression::InvertedGreedinessOption),"<b>\\1</b>");
			htmlstr=htmlstr.replace(QRegularExpression("\e\\[1m(.*)\e\\[m",QRegularExpression::InvertedGreedinessOption),"<b>\\1</b>");

			if(this->mainClass->useUnderline==false)
				htmlstr=htmlstr.replace(QRegularExpression(R"RX(\e\[4m(.*)\e\[.{,3}m)RX",QRegularExpression::InvertedGreedinessOption),"<i>\\1</i>");
			else
				htmlstr=htmlstr.replace(QRegularExpression(R"RX(\e\[4m(.*)\e\[.{,3}m)RX",QRegularExpression::InvertedGreedinessOption),"<u>\\1</u>");

			if(this->mainClass->useUnderline==false)
				htmlstr=htmlstr.replace(QRegularExpression(R"RX(\e\[22m(.*)\e\[.{,3}m)RX",QRegularExpression::InvertedGreedinessOption),"<i>\\1</i>");
			else
				htmlstr=htmlstr.replace(QRegularExpression(R"RX(\e\[22m(.*)\e\[.{,3}m)RX",QRegularExpression::InvertedGreedinessOption),"<u>\\1</u>");

			htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[1m)RX"),"");
			htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[m)RX"),"");
			htmlstr+="</pre>\n";

			this->mainClass->makeNewTab(htmlstr,cname,issub);
		}
}
