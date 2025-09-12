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

void ManpageConvertClass::exportManpage(QString filepath)
{
QString htmlpage=R"foo(.TH "myprogram" "1" "0.0.0" "Me" "My set of programs")foo";

	QString manpage=R"foo(.TH "myprogram" "1" "0.0.0" "Me" "My set of programs")foo";
	manpage+="\n";
	for(int j=0;j<this->mainClass->mainNotebook->count();j++)
		{
			QTextEdit	*te=this->mainClass->getDocumentForTab(j);
			manpage+=".SH \""+this->mainClass->mainNotebook->tabText(j)+"\"\n";
			//QTextStream(stderr)<<">>>"<<te->toMarkdown()<<"<<<<"<<Qt::endl;
			manpage+=te->toMarkdown()+"\n";
			htmlpage+="\n.SH \""+this->mainClass->mainNotebook->tabText(j)+"\"\n";
			htmlpage+=te->toHtml();

		}
//manpage=manpage.replace("\f","\n");
//	manpage=manpage.replace(")\n","\\)\n");
{
QStringList ss=htmlpage.split("\n");
//QTextStream(stdout)<<".SH "<<<<Qt::endl;
for(int j=0;j<ss.size();j++)
	{
		QTextStream(stderr)<<QString(ss.at(j))<<Qt::endl;
		if(ss.at(j).startsWith(".SH"))
			QTextStream(stdout)<<ss.at(j)<<Qt::endl;
		if(ss.at(j).startsWith(".TH"))
			QTextStream(stdout)<<R"foo(.TH "myprogram" "1" "0.0.0" "Me" "My set of programs")foo"<<Qt::endl;

		QString td=QString(ss.at(j));
		td=td.replace("\f","");
		td=td.replace("&gt;",">");
		td=td.replace("&lt;","<");
		td=td.replace("&quot;","\"");
		td=td.replace("&amp;","&");
		td=td.replace("\\\"","\\\\\"");
		td=td.replace(QRegularExpression(".* href=\"(.*)\"><.*"),"\\1");
//QTextStream(stderr)<<td<<Qt::endl;
		//if(ss.at(j).contains(QRegularExpression("^<p.*\">(.*)</p>$"))==true)
		//if(ss.at(j).contains(QRegularExpression("<p.*\">(.*)</p>",QRegularExpression::InvertedGreedinessOption))==true)
		if(ss.at(j).contains(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption))==true)
			{
				td=td.replace(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption),"\\1");
		//	QTextStream(stderr)<<td<<Qt::endl;
				td=td.replace("</body></html>","");
				td=td.replace(QRegularExpression("(.*)<span style=.*weight.*\">(.*)</span>(.*)",QRegularExpression::InvertedGreedinessOption),"\\1\\fB\\2\\fR\\3");
				td=td.replace(QRegularExpression("(.*)<span style=.*underline.*>(.*)</span>(.*)",QRegularExpression::InvertedGreedinessOption),"\\1\\fI\\2\\fR\\3");
			//QTextStream(stderr)<<td<<Qt::endl;
				
				QTextStream(stdout)<<td<<"\n.br"<<Qt::endl;
			}
	}
}
	manpage=manpage.replace(QRegularExpression("\\*\\*([[:space:]]*)(\\S.*)([[:space:]]*)\\*\\*",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),"\\1\\fB\\2\\fR\\3");
	manpage=manpage.replace(QRegularExpression("\\b_([[:space:]]*)(\\S.*)([[:space:]]*)_\\b",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),"\\1\\fI\\2\\fR\\3");

//clean new lines
	manpage=manpage.replace("\n\n","\n");
	manpage=manpage.replace("\n","\n.br\n");
	manpage=manpage.replace("\f","");
	manpage=manpage.replace(".br\n\n.br\n",".br\n");
//QTextStream(stderr)<<manpage<<Qt::endl;

	QFile data(filepath);
	if(data.open(QFile::WriteOnly|QFile::Truncate))
		{
			QTextStream out(&data);
			out<<manpage;
			data.close();
		}
}

void ManpageConvertClass::importManpage(QString filepath)
{
	QTextEdit	*te;
	QFile		file(filepath);
	QString		content;
	QStringList	sections;
	bool			retval;

	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			content=QString::fromUtf8(file.readAll());
//			te=new QTextEdit;
//							te->setAcceptRichText(true);
//							te->setHtml(content);
//							this->mainClass->mainNotebook->addTab(te,filepath);
//			return;
			sections=content.split(QRegularExpression("\\.SH[[:space:]]*",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),Qt::SkipEmptyParts);
			for(int j=0;j<sections.size();j++)
				{
					if(j==0)
						{
							qDebug()<<sections.at(0);
						}
					else
						{
							QStringList	lines=sections.at(j).split("\n");
							QString		name=QString(lines.at(0)).replace(QRegularExpression("^.*\"(.*)\".*$"),"\\1").trimmed();
							QString		reform="";
							QString		thisline="";
							reform=sections.at(j);
							reform=reform.replace(QRegularExpression("^(.*)\\n",QRegularExpression::InvertedGreedinessOption|QRegularExpression::DotMatchesEverythingOption),"");



							reform=reform.replace(".br","  \n");
							reform=reform.replace("<","\\<");
							reform=reform.replace("\n ","\f ");
							reform=reform.replace("\n","  \n");
							reform=reform.replace("\n\t","\f\t");

//reform=reform.replace(QRegularExpression(R"RX(\\fI([[:space:]]*)([^\s].*)([[:space:]]*)\\fR)RX",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("%1_%2_%3").arg("\\1").arg("\\2").arg("\\3"));
reform=reform.replace(QRegularExpression(R"RX(\\fI([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("_\\2_"));

//reform=reform.replace(QRegularExpression(R"RX(\\fB([[:space:]]*)([^\s].*)([[:space:]]*)\\fR)RX",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("%1**%2**%3").arg("\\1").arg("\\2").arg("\\3"));
reform=reform.replace(QRegularExpression(R"RX(\\fB([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("**\\2**"));
							//reform=reform.replace("\n ","\f\t");

reform=reform.replace("\n","\n\n");
									//reform=reform.replace(QRegularExpression("\\\\fI(.*)\\\\fR",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("__%1__").arg("\\1"));
				//	QTextStream(stderr)<<reform<<Qt::endl;
								
							te=new QTextEdit;
							te->setAcceptRichText(true);
							te->setMarkdown(reform);
							this->mainClass->mainNotebook->addTab(te,name);
						}
				}
		}
}
