/*
 *
 * ©K. D. Hedger. Thu  2 Oct 13:06:22 BST 2025 keithdhedger@gmail.com
 *
 * This file (QT_runExternalProc.h) is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _RUNEXTERNALPROC_
#define _RUNEXTERNALPROC_

#include "Globals.h"

#define MAXARGS 50
#define BUFFER_SIZE 4096

// trim from both ends of string (right then left)
inline std::string& trimmed(std::string& s,const char *t=" \t\n\r\f\v")
{
	s.erase(0,s.find_first_not_of(t));
	s.erase(s.find_last_not_of(t)+1);
	return(s);
}

class runExternalProcClass
{
	public:
		runExternalProcClass();
		~runExternalProcClass();

//vars
		std::string	exitStatus="-1";
		bool			showCli=false;
		bool			trimOP=false;
//funcs
		std::string	runExternalCommands(std::string str,bool capture,std::string tofile="",bool append=false);

	private:
//vars
		wordexp_t	commandsVar[MAXARGS];
		char			buffer[BUFFER_SIZE];
};

#endif
