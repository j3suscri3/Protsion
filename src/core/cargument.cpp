#include <QDebug>
#include <QHash>
#include <QRegExp>
#include <QString>

#include "cargument.hpp"

CArgument::CArgument(int total, char *arguments[]) {

	//Initialize all hash entries
	this->M_Arguments["Database"]["Name"]	  = "protsion.db";
	this->M_Arguments["Database"]["Password"] = "protsion";
	this->M_Arguments["Database"]["User"]	  = "protsion";
	this->M_Arguments["Ip"]["Interface"]	  = "";
	this->M_Arguments["Ip"]["Mode"]		  = "ipv4-ipv6";
	this->M_Arguments["Log"]["Level"]	  = "info";
	this->M_Arguments["Process"]["Root"] 	  = "";
	this->M_Arguments["Process"]["File"]	  = "_protsion.pid";
	this->M_Arguments["Process"]["Mode"] 	  = "true";
	this->M_Arguments["Process"]["User"]	  = "protsion";

        //Get only the executable name
        QRegExp r_executable("^(.*)/");

	int counter;
        for(counter = 1; counter < total; counter++) {

                if(QString::fromUtf8(arguments[counter]).compare("-d") == 0) {

                        //Save the custom deamonize status
			this->M_Arguments["Process"]["Mode"] = "false";
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-h") == 0) {

                        this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                        exit(EXIT_SUCCESS);

                } else if(QString::fromUtf8(arguments[counter]).compare("-I") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-I requires 1 argument minimum or 2 arguments maximum!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom ip mode
                        if((QString::fromUtf8(arguments[counter+1]).compare("ipv4") != 0) && (QString::fromUtf8(arguments[counter+1]).compare("ipv6") != 0)
			&& (QString::fromUtf8(arguments[counter+1]).compare("ipv4-ipv6") != 0)) {

                                qInfo() << "\n-I requires like first argument the possible values <ipv4>, <ipv6> or <ipv4-ipv6>!\n";
                               	this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }
                        this->M_Arguments["Ip"]["Mode"] = arguments[counter+1];

                        //Save the specified interface name
                        if((QString::fromUtf8(arguments[counter+2]).compare("-d") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-D") == 0)
			|| (QString::fromUtf8(arguments[counter+2]).compare("-h") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-l") == 0)
			|| (QString::fromUtf8(arguments[counter+2]).compare("-p") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-S") == 0)
			|| (QString::fromUtf8(arguments[counter+2]).compare("-U") == 0) || (QString::fromUtf8(arguments[counter+2]).compare("-v") == 0)
			||  QString::fromUtf8(arguments[counter+2]).isEmpty())
                                counter += 1;
                        else {

                                this->M_Arguments["Ip"]["Interface"] = arguments[counter+2];
                                counter += 2;

                        }
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-l") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-l requires an argument!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom log level file
                        this->M_Arguments["Log"]["Level"] = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-p") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-p requires an argument!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom pid file name
                        this->M_Arguments["Process"]["File"] = arguments[counter+1];
                        counter += 1;
                        continue;

		 } else if(QString::fromUtf8(arguments[counter]).compare("-R") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-R requires an argument!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom directory name
                        this->M_Arguments["Process"]["Root"] = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-S") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-S requires an argument!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Argument respects the regex uri database
			QRegExp r_databaseUri("(.*):(.*)@(.*)");
                        if(!r_databaseUri.exactMatch(arguments[counter+1])) {

                                qInfo() << "\n-S requires an argument with the format <database:username@password>!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Define the regex for gettinf all database configurations
			QRegExp r_databaseName(":(.*)"), r_databaseUsername("^(.*):|@(.*)$"), r_databasePassword("^(.*)@");
                        //Get the database name
                        this->M_Arguments["Database"]["Name"] = QString::fromUtf8(arguments[counter+1]).replace(r_databaseName, "");
                        //Get the database username
                        this->M_Arguments["Database"]["User"] = QString::fromUtf8(arguments[counter+1]).replace(r_databaseUsername, "");
                        //Get the database password
                        this->M_Arguments["Database"]["Password"] = QString::fromUtf8(arguments[counter+1]).replace(r_databasePassword, "");

                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-U") == 0) {

                        //Argument incompleted
                        if((counter == total-1) || QString::fromUtf8(arguments[counter+1]).isEmpty()) {

                                qInfo() << "\n-U requires an argument!\n";
                                this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                                exit(EXIT_FAILURE);

                        }

                        //Save the custom user name
                        this->M_Arguments["Process"]["User"] = arguments[counter+1];
                        counter += 1;
                        continue;

                } else if(QString::fromUtf8(arguments[counter]).compare("-v") == 0) {

                        this->showingVersion(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                        exit(EXIT_SUCCESS);

                } else {

                        qInfo() << "\nIllegal command line argument: " << arguments[counter] << "\n";
                        this->showingHelp(QString::fromUtf8(arguments[0]).replace(r_executable, ""));
                        exit(EXIT_FAILURE);

                }

        }

}

QHash<QString, QHash<QString, QString>> CArgument::getting(void) const {

	return this->M_Arguments;

}

void CArgument::showingHelp(QString executable) const {

	qInfo() << "Usage: " << executable << " [-d] [-h] [-I <mode> | <interface>] [-l <level>] [-p <file>]";
	qInfo() << "		[-R <directory>] [-S <database:user@password>] [-U <user>] [-v]\n";
	qInfo() << "-d			      Run " << executable << " not like a daemon.";
	qInfo() << "-R <directory>		      Define the specified directory as the root.";
	qInfo() << "-h			      Print this help page.";
	qInfo() << "-I <mode> <interface>         Specify the ip mode and the network interface name";
	qInfo() << "			      ipv4 / ipv6 / ipv4-ipv6 (mode by default).";
	qInfo() << "-l <level>		      Record traces depending on the level";
	qInfo() << "			      error / warning / info (level by default).";
	qInfo() << "-p <file>		      Use the specified name for the pid file";
	qInfo() << "	                      instead of the default one.";
	qInfo() << "-S <database:user@password>   Specify the database connection configuration.";
	qInfo() << "-U <user>		      Run as the specified user instead of " << executable << ".";
	qInfo() << "-v			      Print the version info of " << executable << ".\n";
	qInfo() << "For bug reporting, please go: <https://github.com/j3suscri3/Protsion/issues>.";

}

void CArgument::showingVersion(QString executable) const {

	qInfo() << executable + " " + C_EXECUTABLEVERSION + " " + C_EXECUTABLEARCHITECTURE + " (GIT hash: " + C_EXECUTABLEHASH + ")";
	qInfo() << "Compiled on" << C_HOSTNAME << C_HOSTCATEGORY << C_HOSTVERSION << "(" << C_HOSTCODE << ") with" << C_COMPILATORNAME << C_COMPILATORVERSION << ".";
	qInfo() << "The embedded database used is" << C_DATABASENAME << C_DATABASEVERSION << ".\n";
	qInfo() << "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.";
	qInfo() << "This is free software: you are free to change and redistribute it.";
	qInfo() << "There is NO WARRANTY, to the extent permitted by law.\n";

}
