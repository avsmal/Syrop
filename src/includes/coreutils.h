/*****************************************************************************************
 * Copyright (c) 2012 K. Krasheninnikova, M. Krinkin, S. Martynov, A. Smal, A. Velikiy   *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 *****************************************************************************************/

#ifndef _UTILS_COREUTILS_H_
#define _UTILS_COREUTILS_H_

#include <string>
#include <map>
#include <vector>

#include "fileinfo.h"

using std::string;
using std::vector;
using std::map;

namespace utils
{
	typedef vector<FileInfo> files_t;
	typedef map<string, string> plugins_t;

	const string HOME = ".syrop/";
	const string PLUGINS = "plugins/";
	const string CONFIGS = "configs/";
	const string SETUP = "/usr/share/syrop/";
	const string PLUGIN_EXTENSION = ".py";
	const string MAPPING_FILE = "mappings.conf";
	const string FIELDS_FILE = "fields";

	/**
	 * Function returns application HOME dir. Directory will be
	 * created if doesn't exist;
	 *
	 * @return string with directory name
	 * @throws std::runtime_error if an error occrred
	 */
	string const & application_dir();
	
	/**
	 * Function return mappings file name in HOME/.syrop
	 *
	 * @return string with file name
	 */
	string const & mapping_file();
	 

	/**
	 * Function returns application config dir. Directory will be
	 * created if doesn't exist;
	 *
	 * @return string with directory name
	 * @throws std::runtime_error if an error occrred
	 */
	string const & config_dir();

	/**
	 * Function returns standart plugin search path:
	 *	$HOME/.syrop/plugins
	 *	/usr/share/syrop/plugins
	 *
	 * @return vector of a string with path
	 * @throws std::runtime_error if an system error occurred
	 */
	vector<string> const & search_pathes(); // throws std::runtime_error
	
	/**
	 * Return map of plugins in a diretories specified in the path argument
	 *
	 * @param path vectror of search pathes
	 * @param plugins set of pairs (plugin name, main plugin file)
	 * @throws std::runtime_error if an system error occurred
	 */
	void list_plugins(vector<string> const & path, plugins_t & plugins); // throws std::runtime_error

} // namespace utils

#endif //_UTILS_COREUTILS_H_
