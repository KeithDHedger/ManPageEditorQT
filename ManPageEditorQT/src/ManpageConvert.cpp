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
			thtml+="\n"+te->statusTip()+"\n";
			thtml+=te->toHtml()+"\n";
		}

	ss=thtml.split("\n");
	for(int j=0;j<ss.size();j++)
		{
			if(ss.at(j).startsWith(".SH"))
				htmlpage+=ss.at(j)+"\n.br\n";
			if(ss.at(j).startsWith(".SS"))
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
			if(td.contains(QRegularExpression("margin-left:..px;")))
				htmlpage+="\n.IP\n";

			if(ss.at(j).contains(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption))==true)
				{
					td=td.replace(QRegularExpression(".*<p.*\\\">(.*)</p>.*",QRegularExpression::InvertedGreedinessOption),"\\1");
					td=td.replace("</body></html>","");
					
					//td=td.replace(QRegularExpression(R"RX(<span style=" text-decoration: underline;">([^<]*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
					td=td.replace(QRegularExpression(R"RX(<span style=" font-style:italic;">([^<]*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
					td=td.replace(QRegularExpression(R"RX(<span style=" font-weight:.*;">([^<]*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fB\\1\\fR");

					td=td.replace("<br>","\n.br\n");
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
			this->mainClass->lastLoadDir=QFileInfo(filepath).dir().absolutePath();
			this->mainClass->currentFilePath=filepath;
			content=QString::fromUtf8(file.readAll());
			content=content.replace(QRegularExpression("^(.SS.*)$",QRegularExpression::MultilineOption|QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),"\\1\n@@issubsection@@\n");
			
			sections=content.split(QRegularExpression("\\.S[HS][[:space:]]*",QRegularExpression::DotMatchesEverythingOption|QRegularExpression::InvertedGreedinessOption),Qt::SkipEmptyParts);
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
							int			skipsub=1;
							QString		thisline="";
							bool			startindent=false;
							QString		htmlstr="";
							QString		origname=name;

							if(lines.at(1)==" @@issubsection@@")
								{
									name=name.toLower();
									skipsub=2;
								}
							else
								name=name.toUpper();

							for(int k=skipsub;k<lines.size();k++)
								{
									int spaces=40;
									if(lines.at(k)=="\n")
										continue;
									reform=lines.at(k);
									reform=reform.replace("<","&lt;");
									reform=reform.replace(">","&gt;");
									reform=reform.replace(QRegularExpression("^ .if . (.*)",QRegularExpression::InvertedGreedinessOption),"\\1<br>");
									reform=reform.replace("\\-","-");
									reform=reform.replace(" .SM","");
									QString t=reform;
									if(t.startsWith(" .TP "))
										{
											t=t.replace(QRegularExpression(" .TP (.*)"),"\\1");
											if(t.toInt()>0)
												spaces=t.toInt()*8;
											reform=" .IP";
											startindent=false;
										}
									if(t==(" .TP"))
										reform=" .br";
									reform=reform.replace(QRegularExpression(R"RX(^ \.BR[[:space:]]+(.*)$)RX"),QString("<b> \\1 </b>"));
									reform=reform.replace(".br","<br>",Qt::CaseInsensitive);
									reform=reform.replace(QRegularExpression(R"RX(^ \.B[[:space:]]+(.*)$)RX"),QString("<b> \\1 </b>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.I[[:space:]]+(.*)$)RX"),QString("<i> \\1 </i>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.IR[[:space:]]+(.*)$)RX"),QString("<i> \\1 </i>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.LP.*$)RX"),QString("<div><br></div>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.LP.*$)RX"),QString("<div><br></div>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.PP.*$)RX"),QString("<div><br></div>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.P.*$)RX"),QString("<div><br></div>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \.EX.*$)RX")," .IP");
									//reform=reform.replace(QRegularExpression(R"RX(^ \.EE.*$)RX")," .IP");
									if(reform.startsWith(" .EE")==true)
										{
											htmlstr+="</div>";
											startindent=false;	
										}

									if(reform.startsWith(" .IP")==true)
										{
											if(startindent==true)
												{
													htmlstr+="</div>";
													startindent=false;
												}
											else
												{
													htmlstr+="<div style=\"margin-left: "+QString("%1").arg(spaces)+"px\">";
													startindent=true;
												}
											continue;										
										}

reform=reform.replace(QRegularExpression(R"RX(^ \..*$)RX"),QString(""));
									
									reform=reform.replace(QRegularExpression(R"RX(\\fI([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::InvertedGreedinessOption),QString("<i>\\2</i>"));
									reform=reform.replace(QRegularExpression(R"RX(\\fB([[:space:]]*)([^\s].*)([[:space:]]*)\\f[RP])RX",QRegularExpression::InvertedGreedinessOption),QString("<b>\\2</b>"));


									reform=reform.replace(QRegularExpression(R"RX(^ \\fI)RX"),QString("<i>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \\fB)RX"),QString("<b>"));
									reform=reform.replace(QRegularExpression(R"RX(^ \\f[RP])RX"),QString("</b></i>"));




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
							if(skipsub==1)
								te->setStatusTip(".SH "+origname);
							else
								te->setStatusTip(".SS "+origname);
							//QTextStream(stderr)<<htmlstr<<Qt::endl;
							this->mainClass->mainNotebook->addTab(te,name);
						}
				}
		}
}
