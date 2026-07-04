/*
 *
 * ©K. D. Hedger. Wed 10 Sep 15:12:29 BST 2025 keithdhedger@gmail.com

 * This file (main.cpp) is part of ManPageEditorQT.

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

#include "globals.h"

int main(int argc, char **argv)
{
	QApplication		app(argc,argv);
	QVariant			qvar;

	prefsClass		newprefs(QString("%1").arg(PACKAGE_NAME));
	prefsClass		cliargs;	
	bool				parse;	
	int				status;

	option			long_options[]=
		{
			{"selectpage",no_argument,NULL,'p'},
			{0,0,0,0}
		};

	app.setOrganizationName("KDHedger");
	app.setApplicationName(PACKAGE_NAME);
	app.setApplicationVersion(PACKAGE_VERSION);

	mpclass=new ManPageEditorQT(&app);
	mpclass->mpConv->manString=mpclass->getProperties();
	mpclass->mpConv->appFontName=app.font().family();

	qvar=newprefs.getSavedPrefValue("main_font");
	if(qvar.isValid()==true)
		{
			QFont fnt;
			fnt.fromString(qvar.toString());
			mpclass->fontName=fnt.family();
			mpclass->fontSize=fnt.pointSize();
		}

	qvar=newprefs.getSavedPrefValue("highlight_colour");
	if(qvar.isValid()==true)
		mpclass->lineHiliteColour=qvar.toString();

	qvar=newprefs.getSavedPrefValue("spell_check_colour");
	if(qvar.isValid()==true)
		mpclass->extraHiliteColour=qvar.toString();

	qvar=newprefs.getSavedPrefValue("teminal_command");
	if(qvar.isValid()==true)
		mpclass->terminalCommand=qvar.toString();

	qvar=newprefs.getSavedPrefValue("italic_as_underline");
	if(qvar.isValid()==true)
		{
			if(qvar.toBool()==true)
				mpclass->italicMenuItem->setAppearance("format-text-underline","Underline","Ctrl+U");
			else
				mpclass->italicMenuItem->setAppearance("format-text-italic","Italic","Ctrl+I");
			mpclass->useUnderline=qvar.toBool();
		}

	qvar=newprefs.getSavedPrefValue("wrap_lines");
	if(qvar.isValid()==true)
		{
			if(qvar.toBool()==true)
				mpclass->lineWrap=QTextEdit::WidgetWidth;
			else
				mpclass->lineWrap=QTextEdit::NoWrap;
		}

	parse=cliargs.doCliArgs(argc,argv,long_options);
	if(parse==false)
		{
			fprintf(stderr,"%s","HELP!!!");
			exit(0);
		}

	if(cliargs.extraCliArgs.size()>0)
		{
			std::string			sfp;
			runExternalProcClass	rp;

			rp.trimOP=true;
			sfp=rp.runExternalCommands(QString("man -w %1").arg(cliargs.extraCliArgs.constLast()).toStdString(),true);
			mpclass->mpConv->importManpage(QString::fromStdString(sfp));;
		}
	else if(cliargs.prefsData.contains(cliargs.hashFromKey("selectpage")))
		{
					QString filepath;
					filepath=mpclass->mpConv->buildOpenSystemPage();
					if(filepath.isEmpty()==false)
						mpclass->mpConv->importManpage(filepath);
		}

	status=app.exec();

	delete mpclass;
	return status;
}