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
	QString	prevpage=this->mainClass->getProperties(this->manString);

	prevpage+="\n";

	for(int j=0;j<this->mainClass->mainNotebook->count();j++)
		{
			QTextEdit	*te=this->mainClass->getDocumentForTab(j);
			prevpage+="\n"+te->statusTip()+"\n";
			QStringList ss=te->toHtml().split("\n");
			for(int j=0;j<ss.size();j++)
				{
					if(ss.at(j).startsWith("<p style="))
						{
							QString res1=ss.at(j);

							res1=res1.replace(QRegularExpression(R"RX(<br.*/>)RX",QRegularExpression::InvertedGreedinessOption),"\n.br\n");
							res1=res1.replace(QRegularExpression(R"RX(&quot;)RX",QRegularExpression::InvertedGreedinessOption),"\"");
							res1=res1.replace(QRegularExpression(R"RX(&lt;)RX",QRegularExpression::InvertedGreedinessOption),"<");
							res1=res1.replace(QRegularExpression(R"RX(&gt;)RX",QRegularExpression::InvertedGreedinessOption),">");

							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-weight:.*;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fB\\1\\fR");
							res1=res1.replace(QRegularExpression(R"RX(<span style=" text-decoration: underline;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-style:italic;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-family:'Monospace';">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\1");

							res1=res1.replace(QRegularExpression(R"RX(<p style="-qt-paragraph-type:empty; .*">)RX",QRegularExpression::InvertedGreedinessOption),"");

							res1=res1.replace(QRegularExpression(R"RX(<p style=" margin-top.* -qt-block-indent:0; text-indent:0px;">(.*)</p>)RX",QRegularExpression::InvertedGreedinessOption),"\\1");	

							prevpage+=res1+"\n.br\n";
						}
				}
		}

	prevpage=prevpage.replace(QRegularExpression(R"RX(\\fR)RX",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),R"RX(\\fR)RX");
	prevpage=prevpage.replace(QRegularExpression(R"RX(\\\")RX",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),R"RX(\\")RX");
	prevpage=prevpage.replace(QRegularExpression(R"RX(\\\$)RX",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),R"RX($)RX");
	prevpage=prevpage.replace(QRegularExpression(R"RX(([^\\])\\\\fR)RX",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),"\\1\\fR");
	prevpage=prevpage.replace(QRegularExpression(R"RX(<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">)RX",QRegularExpression::InvertedGreedinessOption|QRegularExpression::MultilineOption),"");
	prevpage=prevpage.replace(QRegularExpression(R"RX(<p style=" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">)RX",QRegularExpression::InvertedGreedinessOption),"");
	prevpage=prevpage.replace(QRegularExpression(R"RX(</p>)RX",QRegularExpression::InvertedGreedinessOption),"");
	prevpage=prevpage.replace(QRegularExpression(R"RX(</body></html>)RX",QRegularExpression::InvertedGreedinessOption),"");

	QFile data(filepath);
	if(data.open(QFile::WriteOnly|QFile::Truncate))
		{
			prefsClass	newprefs(QString("%1").arg(PACKAGE_NAME));
			boolTuple	bt;
			QTextStream	out(&data);
			out<<prevpage;
			data.close();

			bt=newprefs.getBoolValue("gzip_pages");
			if((bt.valid==true) && (bt.value==true) && (nozip==false))
				{
					runExternalProcClass	rp;
					rp.runExternalCommands(QString("gzip -f '%1'").arg(filepath).toStdString(),false,"/dev/null");
				}
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
	std::string			sfp;

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

	rp.trimOP=true;
	sfp=rp.runExternalCommands(QString("cat %1| sed -n '/^.TH/p'").arg(adjfp).toStdString(),true);

	this->manString=this->mainClass->getProperties(QString::fromStdString(sfp));
	setenv("GROFF_SGR","1",1);
	setenv("MANWIDTH","2000",1);
	setenv("MAN_KEEP_FORMATTING","1",1);
	sfp=rp.runExternalCommands(QString("echo -e '\n.SH \"\"'|cat '%1' -|sed 's/^\\(\\.S[Hh]\\) \\(.*\\)/\\n@SECTION@--\\2--\\n\\1 \\2/g;s/^\\(\\.S[Ss]\\) \\(.*\\)/\\n@section@++\\2++\\n\\1 \\2/g'|man -l --no-justification --no-hyphenation -|ul|head -n -4").arg(adjfp).toStdString(),true);
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
				}
			htmlstr="<style> pre {white-space: pre-wrap;word-wrap: break-word;overflow-x: auto;}</style>\n<pre>"+htmlstr;
			htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[1m(.*)\e\[m)RX",QRegularExpression::InvertedGreedinessOption),"<b>\\1</b>");
			if(this->mainClass->useUnderline==false)
				htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[4m(.*)\e\[m)RX",QRegularExpression::InvertedGreedinessOption),"<i>\\1</i>");
			else
				htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[4m(.*)\e\[m)RX",QRegularExpression::InvertedGreedinessOption),"<u>\\1</u>");
			htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[1m)RX"),"");
			htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[m)RX"),"");
			htmlstr+="</pre>\n";

			this->mainClass->makeNewTab(htmlstr,cname,issub);
		}
}
