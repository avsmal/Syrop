#include <iostream>
#include <string>
#include <map>
#include "inidata.h"
#include "emptyobject.h"

using std::string;
using std::map;

namespace utils{

	void IniData::addSection( string const & sec )
	{
		data.insert( make_pair(sec, attributes()) );
	}

	void IniData::addAttribute( string const &sec, pair <string, string> const &params )
	{
		data[sec].insert(params);
	}

	//void IniData::removeSection(string const &);
	//void IniData::removeAttribute(string const &);
	
	bool IniData::hasSection( string const &sec ) const
	{
		return data.find(sec) != data.end();		
	}
	
	bool IniData::hasAttribute( string const &sec, string const &attr ) const
	{
		if ( hasSection( sec ) )
		{
			attributes attrs = getSection( sec );
			attributes::const_iterator  ait = attrs.find( attr );
			return ( ait != attrs.end() ) ;
		}		
		return false;
	}

	void IniData::removeSection( string const &sec ) 
	{
		map < string, attributes >::iterator dit = data.find( sec );
		if ( dit != data.end() ) data.erase ( dit );		
		return;
	}

	attributes IniData::getSection( string const &sec ) const
	{
		try
		{
			if ( data.find( sec ) == data.end() ) throw EmptyObjectException();
			return (data.find(sec))->second;
		}
		catch ( EmptyObjectException E )
		{
			std::cout << "IniData::getSection() " << E.showReason() << std::endl;
			throw ( E );
		}
		return attributes();
	}

	vector< string > IniData::getSectionsList() const
	{
		vector<string> secs;
		map <string, attributes>::const_iterator it = data.begin();
		for ( ; it != data.end() ; ++it ) {
			secs.push_back( it->first );
		}
		return secs;
	}
	//string IniData::getAttribute(string const &){
	//}

/*	void IniData::print() const {

		std::map<string, attributes>::const_iterator ait = data.begin();

		for ( ; ait != data.end(); ++ait){
			std::cout <<  " " << ait -> first << std::endl; 
			for (attributes::const_iterator sit = ait->second.begin() ; sit != ait->second.end() ; ++sit ) 
			 	std::cout << "   " << sit -> first  << " " << sit -> second << std::endl;			
		}

	}
*/

	std::ostream& operator<<( std::ostream &os, IniData const &idata ) 
	{

		std::map<string, attributes>::const_iterator ait = idata.data.begin();

		for ( ; ait != idata.data.end(); ++ait){
			os <<  " " << ait -> first << std::endl; 
			for (attributes::const_iterator sit = ait->second.begin() ; sit != ait->second.end() ; ++sit ) 
			 	os << "   " << sit -> first  << " " << sit -> second << std::endl;			
		}
		return os;
				

	}
	
}
