#include <iostream>
#include <vector>

#include "system.h"
#include "sysexception.h"

int main(int argc, char **argv)
{
	try
	{
		std::vector<utils::FileInfo> f = utils::listDirEntries(std::string(argv[1]));
		for (std::vector<utils::FileInfo>::iterator it = f.begin(); it != f.end(); ++it)
			if (it->isFile())
				std::cout << it->getFullName() << std::endl;
	}
	catch (utils::SystemException e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}
