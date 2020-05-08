#include <cstring>
#include <iostream>
#include <regex>
#include <string>

#include "argument.hpp"
#include "configuration.hpp"

using namespace std;

Argument::Argument() : m_status(true) {

}

string Argument::getDatabase(void) const {

	return this->m_database;

}

string Argument::getDirectory(void) const {

	return this->m_directory;

}

string Argument::getFileConf(void) const {

	return this->m_fileConf;

}

string Argument::getFilePid(void) const {

	return this->m_filePid;

}

string Argument::getLevel(void) const {

        return this->m_level;

}

bool Argument::getStatus(void) const {

	return this->m_status;

}

string Argument::getUser(void) const {

	return this->m_user;

}

void Argument::parse(int total, char *table[]) {

	Argument argument;
	int counter;

	//Get only the executable name
	regex r_executable("^(.*)/");

	for(counter = 1; counter < total; counter++) {

		if(!strcmp(table[counter], "-!")) {

                        //Save the custom deamonize status
			this->m_status = !this->m_status;
			continue;

                } else if(!strcmp(table[counter], "-d")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {

                                cout << "\n-d requires an argument!\n" << endl;
                                this->showHelp(regex_replace(table[0], r_executable, "$3"));
				exit(EXIT_FAILURE);

                        }

                        //Save the custom directory name
                        this->m_directory = table[counter+1];
			counter += 1;
			continue;

                } else if(!strcmp(table[counter], "-h")) {

			this->showHelp(regex_replace(table[0], r_executable, "$3"));
			exit(EXIT_SUCCESS);

		} else if(!strcmp(table[counter], "-l")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {

                                cout << "\n-l requires an argument!\n" << endl;
                                this->showHelp(table[0]);
				exit(EXIT_FAILURE);

                        }

                        //Save the custom log level file
                        this->m_level = table[counter+1];
			counter += 1;
			continue;

                } else if(!strcmp(table[counter], "-p")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {

                                cout << "\n-p requires an argument!\n" << endl;
                                this->showHelp(regex_replace(table[0], r_executable, "$3"));
				exit(EXIT_FAILURE);

                        }

                        //Save the custom pid file name
                        this->m_filePid = table[counter+1];
			counter += 1;
			continue;

		} else if(!strcmp(table[counter], "-s")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {

                                cout << "\n-s requires an argument!\n" << endl;
                                this->showHelp(regex_replace(table[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom database coonexion configuration
                        this->m_database = table[counter+1];
                        counter += 1;
                        continue;

                } else if(!strcmp(table[counter], "-u")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(table[counter+1])==0) {

                                cout << "\n-u requires an argument!\n" << endl;
                                this->showHelp(regex_replace(table[0], r_executable, "$3"));
				exit(EXIT_FAILURE);

                        }

                        //Save the custom user name
			this->m_user = table[counter+1];
			counter += 1;
			continue;

                } else if(!strcmp(table[counter], "-v")) {

                        this->showVersion(regex_replace(table[0], r_executable, "$3"));
			exit(EXIT_SUCCESS);

                } else {

			cout << "\nIllegal command line argument: " << table[counter] << "\n" << endl;
			this->showHelp(regex_replace(table[0], r_executable, "$3"));
			exit(EXIT_FAILURE);

		}

	}

}

void Argument::showHelp(string sh) const {

	cout << "Usage: " << sh << " [-!] [-d <directory>] [-h] [-l <level>]" << endl;
	cout << "		[-s <database:user@password>] [-u <user>] [-v]\n" << endl;
	cout << "-!			      Run " << sh << " not like a daemon." << endl;
	cout << "-d <directory>		      Define the specified directory as the root." << endl;
	cout << "-h			      Print this help." << endl;
	cout << "-l <level>		      Record traces depending on the level" << endl;
	cout << "			      error / warning / info (option by default)." << endl;
	cout << "-p <file>		      Use the specified name for the pid file"  << endl;
	cout << "	                      instead of the default one."  << endl;
	cout << "-s <database:user@password>   Define the database connection configuration." << endl;
	cout << "-u <user>		      Run as the specified user instead of " << sh << "." << endl;
	cout << "-v			      Print the version info of " << sh << ".\n" << endl;
	cout << "For bug reporting, please go: <https://github.com/j3suscri3/Protsion/issues>." << endl;

}

void Argument::showVersion(string sh) const {

	cout << sh << " " << E_VERSION << " " << E_ARCHITECTURE << " (GIT hash: " << E_HASH << ")" << endl;
	cout << "Compiled on " << H_NAME << " " << H_CATEGORY << " " << H_VERSION << " (" << H_CODE << ") " << "with " << C_NAME << " " << C_VERSION << "." << endl;
	cout << "The embedded database used is " << D_NAME << " " << D_VERSION << ".\n" << endl;
	cout << "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>." <<endl;
	cout << "This is free software: you are free to change and redistribute it." << endl;
	cout << "There is NO WARRANTY, to the extent permitted by law.\n" << endl;

}
