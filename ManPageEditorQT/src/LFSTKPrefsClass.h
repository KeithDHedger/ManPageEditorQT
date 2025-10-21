/*
 *
 * ©K. D. Hedger. Tue 21 Oct 17:19:01 BST 2025 keithdhedger@gmail.com

 * This file (LFSTKPrefsClass.h) is part of ManPageEditorQT.

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


#ifndef _LFSTKPREFSCLASS_
#define _LFSTKPREFSCLASS_

enum dataType {TYPEINVALID,TYPESTRING,TYPEBOOL,TYPEINT};

struct prefsData
{
	dataType		type;
	std::string	keyName;
	std::string	description;
	std::string	strData;
	bool			boolData;
	int			intData;
};

/**
 * \brief Line edit class for LFSToolKit.
*/
class LFSTK_prefsClass
{
	public:
		LFSTK_prefsClass(std::string name,std::string version);
		LFSTK_prefsClass();
		~LFSTK_prefsClass();
	
		std::map<unsigned long,struct prefsData> prefsMap;
		std::vector<std::string>	cliArgs;

		void					LFSTK_addToPrefs(const char *key,prefsData data);
		void					LFSTK_deleteFromPrefs(const char *key);
		const char			*LFSTK_boolToString(bool val);
		bool					LFSTK_stringToBool(std::string val);
		void					LFSTK_saveVarsToFile(std::string filepath);
		void					LFSTK_loadVarsFromFile(std::string filepath);
		std::string			LFSTK_strStrip(std::string haystack,std::string whitespace="\t \r\n");
		unsigned long		LFSTK_hashFromKey(std::string key);

//get/set
//strings
		const char			*LFSTK_getCString(const char *key);
		std::string			LFSTK_getString(const char *key);
		const std::string	*LFSTK_getStringObject(const char *key);
		void					LFSTK_setString(const char *key,const char *newstr);

//get/set
//boolean
		bool					LFSTK_getBool(const char *key);
		void					LFSTK_setBool(const char *key,bool val);

//get/set
		int					LFSTK_getInt(const char *key);
		void					LFSTK_setInt(const char *key,int val);

//get cli args
		bool					LFSTK_argsToPrefs(int argc, char **argv,option longoptions[],bool addhelp=true);

	private:
		const char			*bools[2]={"false","true"};
		std::string			appName="App";
		std::string			appVersion="0.0.0";
		void					printHelp(option longoptions[]);

};

#endif
