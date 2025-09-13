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
	QString		htmlpage="";
	QString		thtml="";
	QStringList	ss;
	QString		td;

	htmlpage=this->mainClass->getProperties(this->manString);
	htmlpage+="\n";
	for(int j=0;j<this->mainClass->mainNotebook->count();j++)
		{
			QTextEdit	*te=this->mainClass->getDocumentForTab(j);
			thtml+="\n.SH \""+this->mainClass->mainNotebook->tabText(j)+"\"\n";
			thtml+=te->toHtml();
		}

	ss=thtml.split("\n");
	for(int j=0;j<ss.size();j++)
		{
			if(ss.at(j).startsWith(".SH"))
				htmlpage+=ss.at(j)+"\n.br\n";

			td=QString(ss.at(j));
			td=td.replace("\f","");
			td=td.replace("&gt;",">");
			td=td.replace("&lt;","<");
			td=td.replace("&quot;","\"");
			td=td.replace("&amp;","&");
			td=td.replace("\\\"","\\\\\"");
			td=td.replace(QRegularExpression(".* href=\"(.*)\"><.*"),"\\1");

			if(ss.at(j).contains(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption))==true)
				{
					td=td.replace(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption),"\\1");
					td=td.replace("</body></html>","");
					td=td.replace(QRegularExpression("(.*)<span style=.*weight.*\">(.*)</span>(.*)",QRegularExpression::InvertedGreedinessOption),"\\1\\fB\\2\\fR\\3");
					td=td.replace(QRegularExpression("(.*)<span style=.*underline.*>(.*)</span>(.*)",QRegularExpression::InvertedGreedinessOption),"\\1\\fI\\2\\fR\\3");
					htmlpage+=td+"\n.br\n";
				}
		}

	QFile data(filepath);
	if(data.open(QFile::WriteOnly|QFile::Truncate))
		{
			QTextStream out(&data);
			out<<htmlpage;
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
			sections=content.split(QRegularExpression("\\.SH[[:space:]]*",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),Qt::SkipEmptyParts);
			for(int j=0;j<sections.size();j++)
				{
					if(j==0)
						{
							this->manString=this->mainClass->getProperties(sections.at(0));
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

							reform=reform.replace(QRegularExpression(R"RX(\\fI([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("_\\2_"));
							reform=reform.replace(QRegularExpression(R"RX(\\fB([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),QString("**\\2**"));

							reform=reform.replace("\n","\n\n");
								
							te=new QTextEdit;
							te->setAcceptRichText(true);
							te->setMarkdown(reform);
							this->mainClass->mainNotebook->addTab(te,name);
						}
				}
		}
}
