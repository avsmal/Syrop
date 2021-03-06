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

#include <algorithm>

#include <boost/bind.hpp>

#include "system.h"
#include "coreutils.h"
#include "fileinfo.h"

using std::make_pair;

namespace utils
{

	namespace
	{

		bool test_plugin(FileInfo const &info)
		{
			//plugin is a direcotiry with a python script
			if (info.isDirectory())
			{
				//list files
				files_t children;
		
				list_dir_entries( info.getFullName(), children );
				
				return std::find_if( children.begin(), children.end(),
						boost::bind(&FileInfo::getName, _1)
							== (info.getName() + PLUGIN_EXTENSION)
						) != children.end();
			}
			return false;
		}

		//function adds plugins in a specified directory to map
		void list_plugins(string const &dir, plugins_t &plugins)
		{
			files_t children;
	
			list_dir_entries( dir, children );
			for (files_t::const_iterator it = children.begin(); it != children.end(); ++it)
			{
				if (test_plugin(*it))
					plugins[it->getName()] = it->getFullName();
			}	
		}

	} // unnamed namespace

/**
 * Function returns syrop directory in user home, if directory dosen't
 * exists function creates it
 */
std::string const& application_dir()
{
	static string const dirName(get_user_home_dir() + "/" + HOME);
	create_dir(dirName);
	create_dir(dirName + PLUGINS);
	create_dir(dirName + CONFIGS);
	return dirName;
}

/**
 * Function returns syrop directory with networks configuraton files,
 * if directory dosen't exists function creates it
 */
std::string const& config_dir()
{
	static string const dirName(application_dir() + CONFIGS);

	return dirName;
}

string const & mapping_file()
{
	static string const mappingFileName = (application_dir() + MAPPING_FILE);
	
	return mappingFileName;
}

/**
 * Returns vector of standard path for searching plugins:
 *  %HOME%/.syrop/plugins
 *  /usr/share/syrop/plugins
 */
vector<string> const& search_pathes()
{
	static vector<string> pathes;
	if (pathes.empty())
	{
		try
		{
			FileInfo info(SETUP + PLUGINS);
			pathes.push_back(info.getFullName());
		}
		catch (std::runtime_error const &e)
		{}

		try
		{
			FileInfo info(get_user_home_dir() + "/" + HOME + PLUGINS);
			pathes.push_back(info.getFullName());
		}
		catch (std::runtime_error const &e)
		{}
	}
	return pathes;
}

/**
 * List plugins in directories specified by pathes
 */
void list_plugins(vector<string> const& pathes, plugins_t &plugins)
{
	//for every path in a vector
	std::for_each( pathes.begin(), pathes.end(), boost::bind(
				static_cast<void (*)(string const &dir, plugins_t &plugins)>(list_plugins)
				, _1, boost::ref(plugins))
		);
}

} // namespace utils
