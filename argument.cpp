#include <cstring>
#include <iostream>
#include <string>

#include "argument.h"
#include "configuration.h"

using namespace std;

Argument::Argument() : m_status(true) {

}

std::string Argument::getDirectory() const {
	return this->m_directory;
}

std::string Argument::getFileConf() const {
	return this->m_fileConf;
}

std::string Argument::getFilePid() const {
	return this->m_filePid;
}

std::string Argument::getLevel() const {
        return this->m_level;
}

bool Argument::getStatus() const {
	return this->m_status;
}

std::string Argument::getUser() const {
	return this->m_user;
}

void Argument::parse(int total, char *table[]) {

	Argument argument;
	int counter;

	for(counter = 1; counter < total; counter++) {
		if(!strcmp(table[counter], "-C")) {
			//Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {
				cout << "\n-C requires an argument!\n" << endl;
				this->showHelp(table[0]);
				exit(EXIT_FAILURE);
			}
			//Save the custom configuration file name
	                this->m_fileConf = table[counter+1];
			counter += 1;
			continue;
                }
		else if(!strcmp(table[counter], "-d")) {
                        //Save the custom deamonize status
			this->m_status = !this->m_status;
			continue;
                }
		else if(!strcmp(table[counter], "-D")) {
                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {
                                cout << "\n-D requires an argument!\n" << endl;
                                this->showHelp(table[0]);
				exit(EXIT_FAILURE);
                        }
                        //Save the custom directory name
                        this->m_directory = table[counter+1];
			counter += 1;
			continue;
                }
		else if(!strcmp(table[counter], "-h")) {
			this->showHelp(table[0]);
			exit(EXIT_SUCCESS);
		}
		else if(!strcmp(table[counter], "-L")) {
                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {
                                cout << "\n-L requires an argument!\n" << endl;
                                this->showHelp(table[0]);
				exit(EXIT_FAILURE);
                        }
                        //Save the custom log level file
                        this->m_level = table[counter+1];
			counter += 1;
			continue;
                }
		else if(!strcmp(table[counter], "-P")) {
                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {
                                cout << "\n-P requires an argument!\n" << endl;
                                this->showHelp(table[0]);
				exit(EXIT_FAILURE);
                        }
                        //Save the custom pid file name
                        this->m_filePid = table[counter+1];
			counter += 1;
			continue;
                }
		else if(!strcmp(table[counter], "-U")) {
                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {
                                cout << "\n-U requires an argument!\n" << endl;
                                this->showHelp(table[0]);
				exit(EXIT_FAILURE);
                        }
                        //Save the custom user name
			this->m_user = table[counter+1];
			counter += 1;
			continue;
                }
		else if(!strcmp(table[counter], "-v")) {
                        this->showVersion(table[0]);
			exit(EXIT_SUCCESS);
                }
		else {
			cout << "\nIllegal command line argument: " << table[counter] << "\n" << endl;
			this->showHelp(table[0]);
			exit(EXIT_FAILURE);
		}
	}
}

void Argument::showHelp(string sh) const {
	cout << "Usage: " << sh << " [arguments]" << "\n" << endl;
	cout << "-C file		Define the specified configuration file instead of the default one" << endl;
	cout << "-d		Run " << sh << " not like a daemon" << endl;
	cout << "-D directory	Define the specified directory as the root" << endl;
	cout << "-h		Print this help" << endl;
	cout << "-L level	Record many messages depending on the level" << endl;
	cout << "		e for error" << endl;
	cout << "		w for warning" << endl;
	cout << "		i for info (option by default)" << endl;
	cout << "-P file		Use the specified name for the pid file instead of the default one"  << endl;
	cout << "-U user		Run as the specified user instead of " << sh << endl;
	cout << "-v		Print the version info of " << sh << "\n" << endl;
}

void Argument::showVersion(string sh) const {
	cout << sh << endl;
}
