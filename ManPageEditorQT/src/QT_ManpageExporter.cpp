/*
 *
 * ©K. D. Hedger. Wed 13 May 15:42:47 BST 2026 keithdhedger@gmail.com

 * This file (QT_ManpageExporter.cpp) is part of ManPageEditorQT.

 * ManPageEditorQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation,either version 3 of the License,or
 * (at your option) any later version.

 * ManPageEditorQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ManPageEditorQT.  If not,see <http://www.gnu.org/licenses/>.
 *
 *Based on QBasicHtmlExporter available here:
 *https://gitlab.com/Open-App-Library/QBasicHtmlExporter
*/

#include "QT_ManpageExporter.h"

QManpageExporter::QManpageExporter(QTextDocument *_doc)
{
	this->doc=_doc;
	const QFont	defaultFont=this->doc->defaultFont();
	defaultCharFormat.setFont(defaultFont);
	// don't export those for the default font since we cannot turn them off with CSS
	defaultCharFormat.clearProperty(QTextFormat::FontUnderline);
	defaultCharFormat.clearProperty(QTextFormat::FontItalic);
	defaultCharFormat.clearProperty(QTextFormat::TextUnderlineStyle);
}

QString QManpageExporter::toMan()
{
	this->manStr=QLatin1String("");
	emitFrame(this->doc->rootFrame()->begin());

	// Remove newlines at beginning
	this->manStr.remove(QRegularExpression("^[\r\n]+"));

	return(this->manStr);
}

void QManpageExporter::emitFrame(const QTextFrame::Iterator &frameIt)
{
	if(!frameIt.atEnd())
		{
			QTextFrame::Iterator next=frameIt;
			++next;
			if(next.atEnd() && frameIt.currentFrame()==nullptr && frameIt.parentFrame() != this->doc->rootFrame() && frameIt.currentBlock().begin().atEnd())
				return;
		}

	for(QTextFrame::Iterator it=frameIt;!it.atEnd(); ++it)
		{
			if(it.currentBlock().isValid())
				emitBlock(it.currentBlock());
		}
}

void QManpageExporter::emitBlock(const QTextBlock &block)
{
	// save and later restore,in case we 'change' the default format by
	// emitting block char format information
	QTextCharFormat	oldDefaultCharFormat=this->defaultCharFormat;

	this->manStr+=QLatin1Char('\n');
	const QTextBlockFormat blockFormat=block.blockFormat();

	// Text
	QTextBlock::Iterator it=block.begin();
	for (;!it.atEnd();++it)
		emitFragment(it.fragment());
	block.next();

	this->defaultCharFormat=oldDefaultCharFormat;
}

void QManpageExporter::emitFragment(const QTextFragment &fragment)
{
	const QTextCharFormat format=fragment.charFormat();

	QString txt=fragment.text();
	QStringList closing_tags=emitCharFormatStyle(format);

	Q_ASSERT(!txt.contains(QChar::ObjectReplacementCharacter));
// split for [\n{LineSeparator}]
	QString forcedLineBreakRegExp=QString::fromLatin1("[\\na]");
	forcedLineBreakRegExp[3]=QChar::LineSeparator;
// space in BR on purpose for compatibility with old-fashioned browsers
	this->manStr+=txt.replace(QRegularExpression(forcedLineBreakRegExp),QLatin1String("<br />"));

	for(int i=0;i<closing_tags.length();i++)
		this->manStr+=closing_tags[i];
}

QStringList QManpageExporter::emitCharFormatStyle(const QTextCharFormat &format)
{
	QStringList	closing_tags;
	int			fontWeight=format.fontWeight();
	bool			isBold=fontWeight > defaultCharFormat.fontWeight();

// Not bold? Let's quickly check to make sure there is not a fontweight property.
	if(!isBold)
		{
			if (format.hasProperty(QTextFormat::FontWeight ) && format.fontWeight() != defaultCharFormat.fontWeight())
				isBold=true;
		}

	if(isBold)
		{
			this->manStr+=QLatin1String("\\fB");
			closing_tags<<"\\fR";
			return(closing_tags);
		}

	if(format.hasProperty(QTextFormat::FontItalic) && format.fontItalic() != defaultCharFormat.fontItalic())
		{
			this->manStr+=QLatin1String("\\fI");
			closing_tags<<"\\fR";
			return(closing_tags);
		}

	if (format.hasProperty(QTextFormat::FontUnderline) && format.fontUnderline() != defaultCharFormat.fontUnderline())
		{
			this->manStr+=QLatin1String("\\fI");
			closing_tags<<"\\fR";
			return(closing_tags);
		}

	if (format.hasProperty(QTextFormat::TextUnderlineStyle))
		{
			this->manStr+=QLatin1String("\\fI");
			closing_tags<<"\\fR";
		}

	return(closing_tags);
}
