#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QHash>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QString>
#include <QThread>
#include <QtWidgets/QApplication>

#include "cconfiguration.hpp"

CConfiguration::CConfiguration(QHash<QString, QHash<QString, QString>> Arguments) : M_CProcess(Arguments[""]) {

	//Loop into the first level of the arguments hash
	QHashIterator<QString, QHash<QString, QString>> ArgumentsIterator1(Arguments);
	while(ArgumentsIterator1.hasNext()) {

		ArgumentsIterator1.next();

		//Loop into the second level of the arguments hash
		QHashIterator<QString, QString> ArgumentsIterator2(ArgumentsIterator1.value());
        	while(ArgumentsIterator2.hasNext()) {

                	ArgumentsIterator2.next();

			//If the default vale is empty
			if(ArgumentsIterator2.value().isEmpty()) {

				//Getting the current root directory
				if((ArgumentsIterator1.key().compare("Process") == 0) && (ArgumentsIterator2.key().compare("Root") == 0)) {

					this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = QDir::rootPath();
					continue;

				}

			} else {

				//Activate the user privileges changements
				if((ArgumentsIterator1.key().compare("Process") == 0) && (ArgumentsIterator2.key().compare("User") == 0)
				&& !ArgumentsIterator2.value().isEmpty())
					QApplication::setSetuidAllowed(true);

				//Check the main directory format
				if((ArgumentsIterator1.key().compare("Process") == 0) && (ArgumentsIterator2.key().compare("Root") == 0)
				&& (!ArgumentsIterator2.value().endsWith("/"))) {

					this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = QString(ArgumentsIterator2.value()).append("/");
					continue;

				}

				//Check the database name format
                                if((ArgumentsIterator1.key().compare("Database") == 0) && (ArgumentsIterator2.key().compare("Name") == 0)
                                && (!ArgumentsIterator2.value().endsWith(".db"))) {

                                        this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = QString(ArgumentsIterator2.value()).append(".db");
                                        continue;

                                }

				//Check the process filename format
				if((ArgumentsIterator1.key().compare("Process") == 0) && (ArgumentsIterator2.key().compare("File") == 0)
				&& (!ArgumentsIterator2.value().startsWith("_") || !ArgumentsIterator2.value().endsWith(".pid"))) {

					QString Argument = ArgumentsIterator2.value();

					if(!ArgumentsIterator2.value().startsWith("_"))
						Argument.insert(0, "_");

					if(!ArgumentsIterator2.value().endsWith(".pid"))
                                         	Argument += QString(".pid");

					this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = Argument;
					continue;

				}

				//Getting the ip of specified network interface
                                if((ArgumentsIterator1.key().compare("Ip") == 0) && (ArgumentsIterator2.key().compare("Interface") == 0)) {

                                        QNetworkInterface Interface = QNetworkInterface::interfaceFromName(ArgumentsIterator2.value());
					QList<QNetworkAddressEntry> Ips = Interface.addressEntries();

					for(int AddressesIterator = 0; AddressesIterator < Ips.size(); AddressesIterator++) {

						QHostAddress Ip = Ips.value(AddressesIterator).ip();

						//Getting ipv4
						if(((Arguments[ArgumentsIterator1.key()]["Mode"].compare("ipv4") == 0) || (Arguments[ArgumentsIterator1.key()]["Mode"].compare("ipv4-ipv6") == 0))
						&& (Ip.protocol() == 0))
							this->M_Configurations["Ip"]["v4"] = Ip.toString();

						//Getting ipv6
						if(((Arguments[ArgumentsIterator1.key()]["Mode"].compare("ipv6") == 0) || (Arguments[ArgumentsIterator1.key()]["Mode"].compare("ipv4-ipv6") == 0))
						&& (Ip.protocol() == 1) && !Ip.toString().contains(ArgumentsIterator2.value()))
                                                         this->M_Configurations["Ip"]["v6"] = Ip.toString();

					}

                                }

				this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = ArgumentsIterator2.value();

			}

        	}

	}

	this->M_Configurations["Process"]["Path"] = QDir::currentPath().append("/run/");
	this->M_Configurations["Database"]["Path"] = QDir::currentPath().append("/database/");
	this->M_Configurations["Ip"]["Host"] = QHostInfo::localHostName();

	//Check the process path
        if(!QDir(this->M_Configurations["Process"]["Path"]).exists())
                QDir().mkdir(this->M_Configurations["Process"]["Path"]);

	//Check the database path
        if(!QDir(this->M_Configurations["Database"]["Path"]).exists())
                QDir().mkdir(this->M_Configurations["Database"]["Path"]);

	//Check the database name
        QDirIterator databaseIterator(this->M_Configurations["Database"]["Path"], QDirIterator::NoIteratorFlags);
        while(databaseIterator.hasNext()) {

                QString file = databaseIterator.next();

                if(file.endsWith(".db"))
                        QDir().rename(file, this->M_Configurations["Database"]["Path"] + this->M_Configurations["Database"]["Name"]);

        }

}

CConfiguration::~CConfiguration() {

}

QHash<QString, QString> CConfiguration::getting(QString Category) const {

	return this->M_Configurations[Category];

}

bool CConfiguration::loading(CDatabase Database) {

	//Memorize the process root pathname
	Database.writingLog("info", "[Configuration] Process (Root) : " + this->M_Configurations["Process"]["Root"]);
	//Memorize the process launch mode
	if(this->M_Configurations["Process"]["Mode"].compare("true") == 0)
		Database.writingLog("info", "[Configuration] Process (Mode) : daemonized");
	else
		Database.writingLog("info", "[Configuration] Process (Mode) : not daemonized");
	//Memorize the process pathname
	Database.writingLog("info", "[Configuration] Process (Pathname) : " + this->M_Configurations["Process"]["Path"]);
	//Memorize the process filename
	if(this->M_Configurations["Process"]["Mode"].compare("true") == 0)
		Database.writingLog("info", "[Configuration] Process (Filename) : " + this->M_Configurations["Process"]["File"]);
	//Memorize the process launch username
	Database.writingLog("info", "[Configuration] Process (Username) : " + this->M_Configurations["Process"]["User"]);
	//Memorize the database pathname
	Database.writingLog("info", "[Configuration] Database (Pathname) : " + this->M_Configurations["Database"]["Path"]);
	//Memorize the database name
	Database.writingLog("info", "[Configuration] Database (Name) : " + this->M_Configurations["Database"]["Name"]);
	//Memorize the database username
        Database.writingLog("info", "[Configuration] Database (Username) : " + this->M_Configurations["Database"]["User"]);
	//Memorize the database password
        Database.writingLog("info", "[Configuration] Database (Password) : " + this->M_Configurations["Database"]["Password"]);
	//Memorize the log level
        Database.writingLog("info", "[Configuration] Database (Log) : " + this->M_Configurations["Database"]["Log"]);
	//Memorize the hostname
	Database.writingLog("info", "[Configuration] Ip (Hostname) : " + this->M_Configurations["Ip"]["Host"]);
	//Memorize the ip version(s) authorized
	Database.writingLog("info", "[Configuration] Ip (Mode) : " + this->M_Configurations["Ip"]["Mode"]);
	//Memorize the ip address(es) for the net interface specified
	if(!this->M_Configurations["Ip"]["Interface"].isEmpty()) {

		Database.writingLog("info", "[Configuration] Ip (Interface) : " + this->M_Configurations["Ip"]["Interface"]);

		if(!this->M_Configurations["Ip"]["v4"].isEmpty())
			Database.writingLog("info", "[Configuration] Ip (v4) : " + this->M_Configurations["Ip"]["v4"]);

		if(!this->M_Configurations["Ip"]["v6"].isEmpty())
                        Database.writingLog("info", "[Configuration] Ip (v6) : " + this->M_Configurations["Ip"]["v6"]);

	}

	this->M_CProcess = CProcess(this->getting("Process"));

	this->M_QThread.start(QThread::TimeCriticalPriority);
        qInfo() << QThread::currentThreadId();

	//Check if the process file already exists
	if(!this->M_CProcess.checking(Database))
		return false;

	return true;

}

bool CConfiguration::unloading(CDatabase Database) {

	if(this->M_Configurations["Process"]["Mode"].compare("true") == 0) {

		//Delete the process file
		if(!this->M_CProcess.deleting(Database))
			return false;

	}

	this->M_QThread.quit();

	return true;

}
