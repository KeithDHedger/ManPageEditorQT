/*
 *
 * ©K. D. Hedger. Fri 15 May 11:46:27 BST 2026 keithdhedger@gmail.com

 * This file (prefsDefaultConfig.h) is part of ManPageEditorQT.

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

#ifndef _PREFSDEFAULTCONFIG_H_
#define _PREFSDEFAULTCONFIG_H_

#define HIGHLIGHTBOX 0
#define SPELLBOX 1
#define TERMBOX 0
#define FONTBOX 0
#define GZIPBOX 0
#define WRAPBOX 1
#define ITALICBOX 2

static const QStringList	configStr={
"edit",
"Teminal Command",
"xterm -geometry 96x28  -e ",
"colour",
"Highlight Colour",
"#30000000",
"colour",
"Spell Check Colour",
"#30ff0000",
"check",
"GZip Pages",
"0",
"check",
"Wrap Lines",
"1",
"check",
"Italic As Underline",
"0",
"font",
"Main Font",
"Monospace,12,-1,5,400,0,0,0,0,0,0,0,0,0,0,1",
};

#endif
