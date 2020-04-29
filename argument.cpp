#include <iostream>
#include <cstring>

#include "argument.h"


void Argument::parsing(int total, char *table[]) const {

	Argument argument;
	int counter;

	for(counter = 1; counter < total; ++counter) {
		if(!strcmp(table[counter], "-C")) {
			//Argument incompleted
			if(counter == --total) {
				std::cout << "-C requires an argument!\n"  << std::endl;
				showingHelp(table[0]);
			}
			//Save the custom configuration file name
			// = table[++counter];
                }
		else if(!strcmp(table[counter], "-d")) {
                        //Save the custom deamonize status
                        // = false;
                }
		else if(!strcmp(table[counter], "-D")) {
                        //Argument incompleted
                        if(counter == --total) {
                                std::cout << "-D requires an argument!\n"  << std::endl;
                                showingHelp(table[0]);
                        }
                        //Save the custom directory name
                        // = table[++counter];
                }
		else if(!strcmp(table[counter], "-h")) {
			showingHelp(table[0]);
		}
		else if(!strcmp(table[counter], "-L")) {
                        //Argument incompleted
                        if(counter == --total) {
                                std::cout << "-L requires an argument!\n"  << std::endl;
                                showingHelp(table[0]);
                        }
                        //Save the custom log level file
                        // ;
                }
		else if(!strcmp(table[counter], "-P")) {
                        //Argument incompleted
                        if(counter == --total) {
                                std::cout << "-P requires an argument!\n"  << std::endl;
                                showingHelp(table[0]);
                        }
                        //Save the custom pid file name
                        // = table[++counter];
                }
		else if(!strcmp(table[counter], "-U")) {
                        //Argument incompleted
                        if(counter == --total) {
                                std::cout << "-U requires an argument!\n"  << std::endl;
                                showingHelp(table[0]);
                        }
                        //Save the custom user name
                        // = table[++counter];
                }
		else if(!strcmp(table[counter], "-v")) {
                        showingVersion(table[0]);
                }
		else
		{
			std::cout << "Illegal command line argument: " << table[counter] << "\n" << std::endl;
			showingHelp(table[0]);
		}
	}

}

void Argument::showingHelp(const char *sh) const {

	std::cout << "Usage: " << sh << " [arguments]" << "\n" << std::endl;
	std::cout << "-C file		Define the specified configuration file instead of the default one" << std::endl;
	std::cout << "-d		Run " << sh << " not like a daemon" << std::endl;
	std::cout << "-D directory	Define the specified directory as the root" << std::endl;
	std::cout << "-h		Print this help" << std::endl;
	std::cout << "-L level	Record many messages depending on the level" << std::endl;
	std::cout << "		e for error" << std::endl;
	std::cout << "		w for warning" << std::endl;
	std::cout << "		i for info (option by default)" << std::endl;
	std::cout << "-P file		Use the specified name for the pid file instead of the default one"  << std::endl;
	std::cout << "-U user		Run as the specified user instead of " << sh << std::endl;
	std::cout << "-v		Print the version info of " << sh << "\n" << std::endl;

}

void Argument::showingVersion(const char *sh) const {

	std::cout << sh << std::endl;

}
