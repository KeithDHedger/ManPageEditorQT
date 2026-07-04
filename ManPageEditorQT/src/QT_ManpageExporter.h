/*
 *
 * ©K. D. Hedger. Wed 13 May 15:31:19 BST 2026 keithdhedger@gmail.com

 * This file (QT_ManpageExporter.h) is part of ManPageEditorQT.

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
 *
 *Based on QBasicHtmlExporter available here:
 *https://gitlab.com/Open-App-Library/QBasicHtmlExporter
*/

#ifndef _QT_MANPAGEEXPORTER_H
#define _QT_MANPAGEEXPORTER_H

#include "globals.h"

class QManpageExporter
{
	public:
		QManpageExporter(QTextDocument *_doc);
		QString				toMan();

	private:
		QStringList			emitCharFormatStyle(const QTextCharFormat &format);
		void					emitFrame(const QTextFrame::Iterator &frameIt);
		void					emitBlock(const QTextBlock &block);
		void					emitFragment(const QTextFragment &fragment);

		QString				manStr;
		QTextCharFormat		defaultCharFormat;
		const QTextDocument	*doc;
};

#endif
