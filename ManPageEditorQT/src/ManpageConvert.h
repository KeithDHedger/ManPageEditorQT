/*
 *
 * ©K. D. Hedger. Mon  8 Sep 10:59:37 BST 2025 keithdhedger@gmail.com

 * This file (ManpageConvert.h) is part of ManPageEditorQT.

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

#ifndef _MANPAGECONVERT_
#define _MANPAGECONVERT_

#include "Globals.h"

class ManpageConvertClass
{
	public:
						ManpageConvertClass(ManPageEditorQT *mainclass);
						~ManpageConvertClass();

		void				importManpage(QString filepath);
		void				exportManpage(QString filepath);

	private:
		QString			manString=R"foo("myprogram" "1" "0.0.0" "Me" "My set of programs")foo";
		ManPageEditorQT *mainClass=NULL;
};

#endif
