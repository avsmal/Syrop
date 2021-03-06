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

#ifndef _CORE_PLUGINRUNNER_
#define _CORE_PLUGINRUNNER_

#include <string>
#include <boost/python.hpp>
#include <boost/noncopyable.hpp>

#include "system.h"
#include "inidata.h"

using std::string;

namespace bpy = boost::python;
using utils::attributes;

namespace core
{

	/**
	 * Class initialize Python interpreter and call plugins specified by script name
	 */
	class PluginRunner : public boost::noncopyable
	{
	public:
		/**
		 * Constructor inititalize Python interpreter and prepare main module
		 */
		PluginRunner()
		{
			Py_Initialize();
			myMain = bpy::import("__main__").attr("__dict__");
			myMain["AppSettings"] = bpy::class_<wrapper>("AppSettings", bpy::no_init)
								.def("__getitem__", &wrapper::get)
								.def("__contains__", &wrapper::in);
		}
		
		/**
		 * Method calls setupSettings function with settings in the Python script
		 *
		 * @param script Python script name
		 * @param settings AppProxySettings
		 * @throws error_already_set if an Python interpreter error occurred
		 */
		void setupSettings(string const &script, attributes const &settings);
		
		/**
		 * Method calls celanupSettings function in the Python script
		 *
		 * @param script Python script name
		 * @throws error_already_set if an Python intepreter error occurred
		 */
		void cleanupSettings(string const &script);
		
		/**
		 * Destructor finalize Python interpreter
		 */
		~PluginRunner()
		{
			Py_Finalize();
		}

	private:
		bpy::object myMain;
		
		struct wrapper
		{
			wrapper(attributes const &settings)
				: mySettings(settings)
			{}
			
			bool in(string const &prot) const { return (mySettings.find(prot) != mySettings.end()); }
            
			string get(string const &prot) const 
            {
                attributes::const_iterator it = mySettings.find(prot);
                if (it != mySettings.end())
                    return it->second;
                return string(); 
            }
			
			attributes const mySettings;
		};
	};
	
} // namespace core
#endif //_CORE_PLUGINRUNNER_
