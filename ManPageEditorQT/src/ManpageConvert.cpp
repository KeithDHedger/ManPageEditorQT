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
td=td.replace("<br />","<br>");

			if(ss.at(j).contains(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption))==true)
				{
					td=td.replace(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption),"\\1");
					td=td.replace("</body></html>","");
					
					//td=td.replace(QRegularExpression(R"RX(<span style=" text-decoration: underline;">([^<]*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
					td=td.replace(QRegularExpression(R"RX(<span style=" font-style:italic;">([^<]*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
					td=td.replace(QRegularExpression(R"RX(<span style=\" font-weight:.*;\">([^<]*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fB\\1\\fR");
td=td.replace("<br>","\n.br\n");
					QTextStream(stderr)<<td<<Qt::endl;
					htmlpage+=td+"\n.br\n";
				}
		}
//<span style=" font-style:italic;">STRING_ARG</span>
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
			this->mainClass->lastLoadDir=QFileInfo(filepath).dir().absolutePath();
			this->mainClass->currentFilePath=filepath;
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
							QString		reform="";
							QString		thissection=sections.at(j);
							thissection=thissection.replace(QRegularExpression("\n{2,}"),"\n.br\n");
							thissection=thissection.replace("\n","\f ");
							QStringList	lines=thissection.split("\f");
							QString		name=QString(lines.at(0)).replace(QRegularExpression("^.*\"(.*)\".*$"),"\\1").trimmed();
							QString		thisline="";
							bool			startindent=false;
							QString	htmlstr="";

							for(int k=1;k<lines.size();k++)
								{
									if(lines.at(k)=="\n")
										continue;
									reform=lines.at(k);
									reform=reform.replace("<","&lt;");
									reform=reform.replace(">","&gt;");
									reform=reform.replace(".br","<br>");
									reform=reform.replace(QRegularExpression(R"RX(^ \.B[[:space:]]+(.*)$)RX"),QString("<b>\\1</b>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.I[[:space:]]+(.*)$)RX"),QString("<i>\\1</i>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.LP.*$)RX"),QString("<div><br></div>"));
									if(reform.startsWith(" .IP")==true)
										{
											if(startindent==true)
												{
													htmlstr+="</div>";
													startindent=false;
												}
											else
												{
													htmlstr+="<div style=\"margin-left: 40px\">";
													startindent=true;
												}
											continue;										
										}
							
									reform=reform.replace(QRegularExpression(R"RX(\\fI([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::InvertedGreedinessOption),QString("<i>\\2</i>"));
									reform=reform.replace(QRegularExpression(R"RX(\\fB([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::InvertedGreedinessOption),QString("<b>\\2</b>"));

									reform=reform.replace(QRegularExpression("^[[:space:]]([[:space:]]+.*)"),"<span>\\1</span>");
									htmlstr+=reform;
								}
							htmlstr.replace(QRegularExpression("<br>[[:space:]]*<br>"),"<br>");
							htmlstr=htmlstr.replace("<br><br>","<br>");
							htmlstr=htmlstr.replace("<br> <br> <br>","<br><br>");
							htmlstr=htmlstr.replace("<br><div","<div");
							htmlstr=htmlstr.replace(" <br> <div","<div");
							htmlstr.replace(QRegularExpression("^ <br> "),"");
							if(startindent==true)
								htmlstr+="</div>";
							htmlstr="<style>span {white-space: pre-wrap;}</style>"+htmlstr;
							
							te=new QTextEdit;
							te->setAcceptRichText(true);
							te->setHtml(htmlstr);
							this->mainClass->mainNotebook->addTab(te,name);
						}
				}
		}
}
