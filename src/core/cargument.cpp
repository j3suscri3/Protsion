#include <QDebug>
#include <QRegExp>
#include <QString>

#include "cargument.hpp"

CArgument::CArgument(int total, char *arguments[]) : M_CDatabaseName(""), M_CDatabaseUsername(""), M_CDatabasePassword(""), M_CDirectory(""), M_CProcessFilename(""), M_CLogLevel(""), M_CIpMode(""), M_CInterface(""), M_CProcessUsername(""), M_CProcessStatus(true) {

	int counter;
        //Get only the executable name
        QRegExp r_executable("^(.*)/");

        for(counter = 1; counter < total; counter++) {

                if(QString::fromUtf8(arguments[counter]).compare("-!") == 0) {

                        //Save the custom deamonize status
                        this->M_CProcessStatus = !this->M_CProcessStatus;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-d") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-d requires an argument!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom directory name
                        this->M_CDirectory = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-h") == 0) {

                        this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                        exit(EXIT_SUCCESS);

                } else if(QString::fromUtf8(arguments[counter]).compare("-i") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-i requires 1 argument minimum or 2 arguments maximum!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom ip mode
                        if((QString::fromUtf8(arguments[counter+1]).compare("ipv4") != 0) && (QString::fromUtf8(arguments[counter+1]).compare("ipv6") != 0)
			&& (QString::fromUtf8(arguments[counter+1]).compare("ipv4-ipv6") != 0)) {

                                qInfo() << "\n-i requires like first argument the possible values <ipv4>, <ipv6> or <ipv4-ipv6>!\n";
                               	this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }
                        this->M_CIpMode = arguments[counter+1];

                        //Save the specified interface name
                        if((QString::fromUtf8(arguments[counter+2]).compare("-!") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-d") == 0)
			|| (QString::fromUtf8(arguments[counter+2]).compare("-h") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-l") == 0)
			|| (QString::fromUtf8(arguments[counter+2]).compare("-p") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-s") == 0)
			|| (QString::fromUtf8(arguments[counter+2]).compare("-u") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-v") == 0))
                                counter += 1;
                        else {

                                this->M_CInterface = arguments[counter+2];
                                counter += 2;

                        }
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-l") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-l requires an argument!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom log level file
                        this->M_CLogLevel = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-p") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-p requires an argument!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom pid file name
                        this->M_CProcessFilename = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-s") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-s requires an argument!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Argument respects the regex uri database
			QRegExp r_databaseUri("(.*):(.*)@(.*)");
                        if(!r_databaseUri.exactMatch(arguments[counter+1])) {

                                qInfo() << "\n-s requires an argument with the format <database:username@password>!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Define the regex for gettinf all database configurations
			QRegExp r_databaseName(":(.*)"), r_databaseUsername("^(.*):|@(.*)$"), r_databasePassword("^(.*)@");
                        //Get the database name
                        this->M_CDatabaseName = QString::fromUtf8(arguments[0]).replace(r_databaseName, "");
                        //Get the database username
                        this->M_CDatabaseUsername = QString::fromUtf8(arguments[0]).replace(r_databaseUsername, "");
                        //Get the database password
                        this->M_CDatabasePassword = QString::fromUtf8(arguments[0]).replace(r_databasePassword, "");

                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-u") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-u requires an argument!\n";
                                this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom user name
                        this->M_CProcessUsername = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-v") == 0) {

                        this->showVersion(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                        exit(EXIT_SUCCESS);

                } else {

                        qInfo() << "\nIllegal command line argument: " << arguments[counter] << "\n";
                        this->showHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                        exit(EXIT_FAILURE);

                }

        }

}

QString CArgument::getDatabaseName(void) const {

	return this->M_CDatabaseName;

}

QString CArgument::getDatabasePassword(void) const {

        return this->M_CDatabasePassword;

}

QString CArgument::getDatabaseUsername(void) const {

        return this->M_CDatabaseUsername;

}

QString CArgument::getDirectory(void) const {

	return this->M_CDirectory;

}

QString CArgument::getProcessFilename(void) const {

	return this->M_CProcessFilename;

}

QString CArgument::getProcessUsername(void) const {

        return this->M_CProcessUsername;

}

bool CArgument::getProcessStatus(void) const {

        return this->M_CProcessStatus;

}

QString CArgument::getInterface(void) const {

        return this->M_CInterface;

}

QString CArgument::getIpMode(void) const {

        return this->M_CIpMode;

}

QString CArgument::getLogLevel(void) const {

        return this->M_CLogLevel;

}

void CArgument::showHelp(QString executable) const {

	qInfo() << "Usage: " << executable << " [-!] [-d <directory>] [-h] [-i <mode> | <interface>] [-l <level>]";
	qInfo() << "		[-s <database:user@password>] [-u <user>] [-v]\n";
	qInfo() << "-!			      Run " << executable << " not like a daemon.";
	qInfo() << "-d <directory>		      Define the specified directory as the root.";
	qInfo() << "-h			      Print this help page.";
	qInfo() << "-i <mode> <interface>         Specify the ip mode and the network interface name";
	qInfo() << "			      ipv4 / ipv6 / ipv4-ipv6 (mode by default).";
	qInfo() << "-l <level>		      Record traces depending on the level";
	qInfo() << "			      error / warning / info (level by default).";
	qInfo() << "-p <file>		      Use the specified name for the pid file";
	qInfo() << "	                      instead of the default one.";
	qInfo() << "-s <database:user@password>   Specify the database connection configuration.";
	qInfo() << "-u <user>		      Run as the specified user instead of " << executable << ".";
	qInfo() << "-v			      Print the version info of " << executable << ".\n";
	qInfo() << "For bug reporting, please go: <https://github.com/j3suscri3/Protsion/issues>.";

}

void CArgument::showVersion(QString executable) const {

	qInfo() << executable + " " + C_EXECUTABLEVERSION + " " + C_EXECUTABLEARCHITECTURE + " (GIT hash: " + C_EXECUTABLEHASH + ")";
	qInfo() << "Compiled on" << C_HOSTNAME << C_HOSTCATEGORY << C_HOSTVERSION << "(" << C_HOSTCODE << ") with" << C_COMPILATORNAME << C_COMPILATORVERSION << ".";
	qInfo() << "The embedded database used is" << C_DATABASENAME << C_DATABASEVERSION << ".\n";
	qInfo() << "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.";
	qInfo() << "This is free software: you are free to change and redistribute it.";
	qInfo() << "There is NO WARRANTY, to the extent permitted by law.\n";

}
