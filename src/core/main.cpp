#include <string>
#include <iostream>
#include <cerrno>

#include "syrop.h"
#include "system.h"
#include "filelocks.h"
#include "coreutils.h"
#include "pluginrunner.h"

void printUsage()
{
	std::cout << "usage:" << std::endl
	          << "\t1. syrop [-c] (-a <network name> | -r) --- apply or roll back proxy parameters" << std::endl
	          << "\t2. syrop -h                            --- print this help" << std::endl << std::endl
		  << "\t\t-c or --create - create config file for network if doesn't exists" << std::endl
		  << "\t\t-a or --apply - apply proxy settings" << std::endl
		  << "\t\t-r or --roll_back" << std::endl;
}

void handleArgs(SyropControlArgs const *h)
{
	if (h->apply && h->rollback)
	{
		std::cerr << "--apply cannot be used with --roll_back" << std::endl;
		exit(1);
	}
	if (h->apply && h->network.empty())
	{
		std::cerr << "network name cannot be empty" << std::endl;
		exit(1);
	}
	if (h->help)
	{
		printUsage();
		exit(1);
	}
	if (!h->apply && !h->rollback)
	{
		std::cerr << "--apply, --roll_back needed or --help needed" << std::endl;
		exit(1);
	}
}

void parseArgs(int argc, char **argv, SyropControlArgs *h)
{
	int c;
	int dummy;
	
	h->apply = h->rollback = h->create = h->help = false;
	
	while ( (c = getopt_long(argc, argv, argsDecsripton, longOps, &dummy) ) != -1 )
	{
		switch (c)
		{
			case 'c':
				h->create = true;
				break;
			case 'a':
				h->apply = true;
				h->network = optarg;
				break;
			case 'r':
				h->rollback = true;
				break;
			case 'h':
			case '?':
			default:
				h->help = true;
				break;
		}
	}
	
	handleArgs(h);
}

void applyParameters(core::PluginRunner &runner)
{
	//TODO
}

void cancelParameters(core::PluginRunner &runner)
{
	//TODO
}

int main(int argc, char **argv)
{	
	try
	{
		SyropControlArgs handleParams;
		parseArgs(argc, argv, &handleParams);
		std::string appDir = core::getApplicationDir();
		core::PluginRunner runner;
	
		//test lock and call plugins
		if (handleParams.apply)
		{
			if (handleParams.create)
			{
				//create proxy config file if dosen't exists
				std::string configDir = appDir + core::configs;
				utils::createDir(configDir);
				utils::createFile(configDir + handleParams.network);
			}
			//call ini parser. TBD.
			if ( utils::locked(appDir, "applied") )
				cancelParameters(runner);
			else
				utils::lock(appDir, "applied");
			applyParameters(runner);
		}
		//release lock and roll back settings
		else if (handleParams.rollback && utils::locked(appDir, "applied") )
		{
			cancelParameters(runner);
			utils::unlock(appDir, "applied");
		}
	}
	catch (std::runtime_error const& e)
	{
		std::cerr << "error: " << e.what() << std::endl;
		exit(1);
	}
	
	return EXIT_SUCCESS;
}