#include <iostream>
#include <fstream>
#include "emptyobject.h"
#include "proxysettings.h"

namespace utils{


	bool ProxySettings::existsApp( string const &appName )
	{
		return data.hasSection ( appName ) ;

	}		

	// first call existsApp() is suggested to check if such an item exists in the map
	attributes & ProxySettings::operator[] (string const &appName)
	{
		return data [ appName ];	
	}

	
	// first call existsApp() is suggested to check if such an item exists in the map
	attributes const & ProxySettings::operator[] (string const &appName) const
	{
		return data [ appName ];
	}

	ProxySettings :: const_iterator  ProxySettings::begin () const
	{
		 return data.begin();
	}

	ProxySettings :: const_iterator  ProxySettings::end () const
	{
		 return data.end();
	}

	ProxySettings :: iterator  ProxySettings::begin ()
    {
		return data.begin();
    }

    ProxySettings ::iterator  ProxySettings::end ()
    {
        return data.end();
    }

	void ProxySettings::saveData( string const &fileName  ) 
	{
		fix( data, fileName );
	}

	void ProxySettings::loadData( string const &fileName )
	{
		std::ifstream file(fileName.c_str());
		
		if (file){
			data = readData(file);	
		}	
	}

	void ProxySettings::fix( IniData const &idata, string const &fileName ) const
	{
		std::ofstream file(fileName.c_str());
		if (file){
			writeData(file, idata);	
		}		
	}

	std::ostream& operator<<( std::ostream &os, ProxySettings const & ps ) 
	{
		os << ps.data;
		return os;
	}

}
