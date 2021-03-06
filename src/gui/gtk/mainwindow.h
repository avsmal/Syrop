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

#ifndef __GUI_GTK_MAIN_WINDOW_H__
#define __GUI_GTK_MAIN_WINDOW_H__

#include <boost/shared_ptr.hpp>
#include <string>
#include <gtkmm.h>

#include "networkslist.h"
#include "networkview.h"
#include "coreutils.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow(utils::files_t const &configs);

protected:
	virtual bool on_delete_event(GdkEventAny* event);

private:
	NetworkList myNetworks;
	boost::shared_ptr<NetworkView> myCurrentNetwork;
	Gtk::HPaned myLayout;

	void on_network_add     (std::string const &name);
	void on_network_delete  (std::string const &name);
	void on_network_selected(std::string const &name);
};

#endif //__GUI_GTK_MAIN_WINDOW_H__
