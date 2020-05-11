#include <iostream>
#include <regex>
#include <string>

#include "cargument.hpp"

using namespace std;

CArgument::CArgument(int total, char *arguments[]) : M_CDatabaseName(""), M_CDatabaseUsername(""), M_CDatabasePassword(""), M_CDirectory(""), M_CProcessFilename(""), M_CLogLevel(""), M_CIpMode(""), M_CInterface(""), M_CProcessUsername(""), M_CProcessStatus(true) {

	int counter;
        //Get only the executable name
        regex r_executable("^(.*)/");

        for(counter = 1; counter < total; counter++) {

                if(!strcmp(arguments[counter], "-!")) {

                        //Save the custom deamonize status
                        this->M_CProcessStatus = !this->M_CProcessStatus;
                        continue;

                } else if(!strcmp(arguments[counter], "-d")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(arguments[counter+1])==0) {

                                cout << "\n-d requires an argument!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom directory name
                        this->M_CDirectory = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(!strcmp(arguments[counter], "-h")) {

                        this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                        exit(EXIT_SUCCESS);

                } else if(!strcmp(arguments[counter], "-i")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(arguments[counter+1])==0) {

                                cout << "\n-i requires 1 argument minimum or 2 arguments maximum!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom ip mode
                        if(strcmp(arguments[counter+1], "ipv4") && strcmp(arguments[counter+1], "ipv6") && strcmp(arguments[counter+1], "ipv4-ipv6")) {

                                cout << "\n-i requires like first argument the possible values <ipv4>, <ipv6> or <ipv4-ipv6>!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }
                        this->M_CIpMode = arguments[counter+1];

                        //Save the specified interface name
                        if(!strcmp(arguments[counter+2], "-!") || !strcmp(arguments[counter+2], "-d") || !strcmp(arguments[counter+2], "-h") ||
                           !strcmp(arguments[counter+2], "-l") || !strcmp(arguments[counter+2], "-p") || !strcmp(arguments[counter+2], "-s") ||
                           !strcmp(arguments[counter+2], "-u") || !strcmp(arguments[counter+2], "-v"))
                                counter += 1;
                        else {

                                this->M_CInterface = arguments[counter+2];
                                counter += 2;

                        }
                        continue;

                } else if(!strcmp(arguments[counter], "-l")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(arguments[counter+1])==0) {

                                cout << "\n-l requires an argument!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom log level file
                        this->M_CLogLevel = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(!strcmp(arguments[counter], "-p")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(arguments[counter+1])==0) {

                                cout << "\n-p requires an argument!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom pid file name
                        this->M_CProcessFilename = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(!strcmp(arguments[counter], "-s")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(arguments[counter+1])==0) {

                                cout << "\n-s requires an argument!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Argument respects the regex uri database
                        regex r_databaseUri("(.*):(.*)@(.*)");
                        if(!regex_match(arguments[counter+1], r_databaseUri)) {

                                cout << "\n-s requires an argument with the format <database:username@password>!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Define the regex for gettinf all database configurations
                        regex r_databaseName(":(.*)$"), r_databaseUsername("^(.*):|@(.*)$"), r_databasePassword("^(.*)@");
                        //Get the database name
                        this->M_CDatabaseName = regex_replace(arguments[counter+1], r_databaseName, "$3") + ".db";
                        //Get the database username
                        this->M_CDatabaseUsername = regex_replace(arguments[counter+1], r_databaseUsername, "$3");
                        //Get the database password
                        this->M_CDatabasePassword = regex_replace(arguments[counter+1], r_databasePassword, "$3");

                        counter += 1;
                        continue;

                } else if(!strcmp(arguments[counter], "-u")) {

                        //Argument incompleted
                        if(counter == total-1 || strlen(arguments[counter+1])==0) {

                                cout << "\n-u requires an argument!\n" << endl;
                                this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom user name
                        this->M_CProcessUsername = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(!strcmp(arguments[counter], "-v")) {

                        this->showVersion(regex_replace(arguments[0], r_executable, "$3"));
                        exit(EXIT_SUCCESS);

                } else {

                        cout << "\nIllegal command line argument: " << arguments[counter] << "\n" << endl;
                        this->showHelp(regex_replace(arguments[0], r_executable, "$3"));
                        exit(EXIT_FAILURE);

                }

        }

}

string CArgument::getDatabaseName(void) const {

	return this->M_CDatabaseName;

}

string CArgument::getDatabasePassword(void) const {

        return this->M_CDatabasePassword;

}

string CArgument::getDatabaseUsername(void) const {

        return this->M_CDatabaseUsername;

}

string CArgument::getDirectory(void) const {

	return this->M_CDirectory;

}

string CArgument::getProcessFilename(void) const {

	return this->M_CProcessFilename;

}

string CArgument::getProcessUsername(void) const {

        return this->M_CProcessUsername;

}

bool CArgument::getProcessStatus(void) const {

        return this->M_CProcessStatus;

}

string CArgument::getInterface(void) const {

        return this->M_CInterface;

}

string CArgument::getIpMode(void) const {

        return this->M_CIpMode;

}

string CArgument::getLogLevel(void) const {

        return this->M_CLogLevel;

}

void CArgument::showHelp(string executable) const {

	cout << "Usage: " << executable << " [-!] [-d <directory>] [-h] [-i <mode> | <interface>] [-l <level>]" << endl;
	cout << "		[-s <database:user@password>] [-u <user>] [-v]\n" << endl;
	cout << "-!			      Run " << executable << " not like a daemon." << endl;
	cout << "-d <directory>		      Define the specified directory as the root." << endl;
	cout << "-h			      Print this help page." << endl;
	cout << "-i <mode> <interface>         Specify the ip mode and the network interface name" << endl;
	cout << "			      ipv4 / ipv6 / ipv4-ipv6 (mode by default)." << endl;
	cout << "-l <level>		      Record traces depending on the level" << endl;
	cout << "			      error / warning / info (level by default)." << endl;
	cout << "-p <file>		      Use the specified name for the pid file"  << endl;
	cout << "	                      instead of the default one."  << endl;
	cout << "-s <database:user@password>   Specify the database connection configuration." << endl;
	cout << "-u <user>		      Run as the specified user instead of " << executable << "." << endl;
	cout << "-v			      Print the version info of " << executable << ".\n" << endl;
	cout << "For bug reporting, please go: <https://github.com/j3suscri3/Protsion/issues>." << endl;

}

void CArgument::showVersion(string executable) const {

	cout << executable << " " << C_EXECUTABLEVERSION << " " << C_EXECUTABLEARCHITECTURE << " (GIT hash: " << C_EXECUTABLEHASH << ")" << endl;
	cout << "Compiled on " << C_HOSTNAME << " " << C_HOSTCATEGORY << " " << C_HOSTVERSION << " (" << C_HOSTCODE << ")" << " with " << C_COMPILATORNAME << " " << C_COMPILATORVERSION << "." << endl;
	cout << "The embedded database used is " << C_DATABASENAME << " " << C_DATABASEVERSION << ".\n" << endl;
	cout << "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>." <<endl;
	cout << "This is free software: you are free to change and redistribute it." << endl;
	cout << "There is NO WARRANTY, to the extent permitted by law.\n" << endl;

}
