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

							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-weight:700;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fB\\1\\fR");
							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-style:italic;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");
							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-family:'Monospace';">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\1");

							res1=res1.replace(QRegularExpression(R"RX(<p style="-qt-paragraph-type:empty; .*">)RX",QRegularExpression::InvertedGreedinessOption),"");

							res1=res1.replace(QRegularExpression(R"RX(<p style=" margin-top.* -qt-block-indent:0; text-indent:0px;">(.*)</p>)RX",QRegularExpression::InvertedGreedinessOption),"\\1");	
							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-family:'Monospace'; font-weight:700;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fB\\1\\fR");
							res1=res1.replace(QRegularExpression(R"RX(<span style=" font-family:'Monospace'; font-style:italic;">(.*)</span>)RX",QRegularExpression::InvertedGreedinessOption),"\\fI\\1\\fR");

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
			QTextStream out(&data);
			out<<prevpage;
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
	QString		htmlstr;
	char			buffer[2048]= {0,};
	char			commandBuffer[2048]= {0,};
	FILE*		fp;

	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			this->mainClass->lastLoadDir=QFileInfo(filepath).dir().absolutePath();
			this->mainClass->currentFilePath=filepath;
			content=QString::fromUtf8(file.readAll());

			sprintf(commandBuffer,"cat %s| sed -n '/^.TH/p'",filepath.toStdString().c_str());
			content="";
			fp=popen(commandBuffer,"r");
			if(fp!=NULL)
				{
					while(fgets((char*)&buffer[0],2048,fp))
						content+=buffer;
					pclose(fp);
				}
			this->manString=this->mainClass->getProperties(content);

			sprintf(commandBuffer,"echo -e '\n.SH \"\"'|cat '%s' -|sed 's/^\\(\\.S[Hh]\\) \\(.*\\)/\\n@SECTION@--\\2--\\n\\1 \\2/g;s/^\\(\\.S[Ss]\\) \\(.*\\)/\\n@section@++\\2++\\n\\1 \\2/g'|GROFF_SGR=1 MANWIDTH=2000 MAN_KEEP_FORMATTING=1 man -l --no-justification --no-hyphenation -|ul|head -n -4",filepath.toStdString().c_str());

			content="";
			fp=popen(commandBuffer,"r");
			if(fp!=NULL)
				{
					while(fgets((char*)&buffer[0],2048,fp))
						content+=buffer;
					pclose(fp);
				}
			
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
					htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[4m(.*)\e\[m)RX",QRegularExpression::InvertedGreedinessOption),"<i>\\1</i>");
					htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[1m)RX"),"");
					htmlstr=htmlstr.replace(QRegularExpression(R"RX(\x1b\[m)RX"),"");
					htmlstr+="</pre>\n";

					te=new QTextEdit;
					te->setHtml(htmlstr);
					this->mainClass->mainNotebook->addTab(te,cname);
					if(issub==false)
						te->setStatusTip(".SH "+cname);
					else
						te->setStatusTip(".SS "+cname);
					te->setLineWrapMode(QTextEdit::WidgetWidth);
				}
		}
}
