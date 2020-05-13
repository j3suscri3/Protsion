#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QHash>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QString>

#include "cconfiguration.hpp"

CConfiguration::CConfiguration(QHash<QString, QHash<QString, QString>> Arguments) {

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
				if((ArgumentsIterator1.key() == "Process") && (ArgumentsIterator2.key() == "Root"))
					this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = QDir::currentPath() + QString("/");

			} else {

				//Check the main directory format
				if((ArgumentsIterator1.key() == "Process") && (ArgumentsIterator2.key() == "Root")
				&& (!ArgumentsIterator2.value().endsWith("/"))) {

					this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = ArgumentsIterator2.value() + QString("/");
					continue;

				}

				//Check the process filename format
				if((ArgumentsIterator1.key() == "Process") && (ArgumentsIterator2.key() == "File")
				&& (!ArgumentsIterator2.value().startsWith("_") || !ArgumentsIterator2.value().endsWith(".pid"))) {

					QString Argument = ArgumentsIterator2.value();

					if(!ArgumentsIterator2.value().startsWith("_"))
						Argument.insert(0, "_");

					if(!ArgumentsIterator2.value().endsWith(".pid"))
                                         	Argument += ".pid";

					this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = Argument;
					continue;

				}

				//Getting the ip of specified network interface
                                if((ArgumentsIterator1.key() == "Ip") && (ArgumentsIterator2.key() == "Interface")) {

                                        QNetworkInterface Interface = QNetworkInterface::interfaceFromName(ArgumentsIterator2.value());
					QList<QNetworkAddressEntry> Ips = Interface.addressEntries();

					for(int AddressesIterator = 0; AddressesIterator < Ips.size(); AddressesIterator++) {

						QHostAddress Ip = Ips.value(AddressesIterator).ip();

						//Getting ipv4
						if(((Arguments["Ip"]["Mode"] == "ipv4") || (Arguments["Ip"]["Mode"] == "ipv4-ipv6")) && (Ip.protocol()== 0))
							this->M_Configurations["Ip"]["v4"] = Ip.toString();

						//Getting ipv6
						if(((Arguments["Ip"]["Mode"] == "ipv6") || (Arguments["Ip"]["Mode"] == "ipv4-ipv6"))
						&& (Ip.protocol()== 1) && !Ip.toString().contains(ArgumentsIterator2.value()))
                                                         this->M_Configurations["Ip"]["v6"] = Ip.toString();

					}

					continue;

                                }

				this->M_Configurations[ArgumentsIterator1.key()][ArgumentsIterator2.key()] = ArgumentsIterator2.value();

			}

        	}

	}

	this->M_Configurations["Process"]["Path"] = this->M_Configurations["Process"]["Root"] + QString("run/");
	this->M_Configurations["Database"]["Path"] = this->M_Configurations["Process"]["Root"] + QString("database/");
	this->M_Configurations["Ip"]["Host"] = QHostInfo::localHostName();

	//Loop into the first level of the arguments hash
        QHashIterator<QString, QHash<QString, QString>> ConfigurationsIterator1(this->M_Configurations);
        while(ConfigurationsIterator1.hasNext()) {

                ConfigurationsIterator1.next();

                //Loop into the second level of the arguments hash
                QHashIterator<QString, QString> ConfigurationsIterator2(ConfigurationsIterator1.value());
                while(ConfigurationsIterator2.hasNext()) {

                        ConfigurationsIterator2.next();

			qInfo() << "key1: " << ConfigurationsIterator1.key() << " key2: " << ConfigurationsIterator2.key() << " value2: " << ConfigurationsIterator2.value();

                }

        }

}

bool CConfiguration::createProcessFile(void) const {
/*
        //Change the current path
	string filePid = this->M_CDirectory + "/run/" + this->M_CProcessFilename;

        //current_path("run");
        std::fstream stream;

        //Open the pid file
        stream.open(filePid, std::fstream::in|std::fstream::out|std::fstream::binary|std::fstream::trunc);
        if (stream.fail()) {

		this->M_CCDatabase.writeLog("error", "Can't open the pid file " + this->M_CProcessFilename);
                return false;

	}

        //Write the pid into the file
        stream << getpid();
	if (stream.fail()) {

		this->M_CCDatabase.writeLog("error", "Can't write into pid file " + this->M_CProcessFilename);
                return false;

	}

	stream.close();
        this->M_CCDatabase.writeLog("info", "[Configuration] Process (id) : " + to_string(getpid()));

	//Change the permissions
	path p_filePid = filePid;
	boost::system::error_code error;
        permissions(p_filePid, perms::owner_all|perms::group_all, error);
	if (error) {

		this->M_CCDatabase.writeLog("error", "Can't change the permissions about the pid file " + this->M_CProcessFilename);
		return false;

	}
*/
	return true;

}

bool CConfiguration::deleteProcessFile(void) const {
/*
	path p_filePid = this->M_CDirectory + "/run/"+ this->M_CProcessFilename;

	cout << "delete" << endl;

	//Delete the PID file
	if (!remove(p_filePid)) {

		this->M_CCDatabase.writeLog("error", "Can't delete the pid file " + this->M_CProcessFilename);
		return false;

	}
*/
	return true;

}

bool CConfiguration::dropUserPrivileges(void) const {
/*
	auto size = sysconf(_SC_GETPW_R_SIZE_MAX);
	//Manage the FreeBSD special case when defining the buffer size
        if (size == -1)
                size = 1024;
        char *buffer = static_cast<char *>(malloc(size));

        //Get the user id with the user name
	struct passwd s_password, *ps_password(0);
        getpwnam_r(this->M_CProcessUsername.c_str(), &s_password, buffer, size, &ps_password);
	uid_t userId = ps_password ? ps_password->pw_uid : -1;

	//Get the group id with the group name
	struct group s_group, *ps_group(0);
	getgrnam_r(this->M_CProcessUsername.c_str(), &s_group, buffer, size, &ps_group);
	gid_t groupId = ps_group ? ps_group->gr_gid : -1;
	free(buffer);

	//If we can't get the current user id and group id
	if (!userId || !groupId) {

		if (!userId)
			this->M_CCDatabase.writeLog("error", "Can't get the specified user id : '" + this->M_CProcessUsername + "'");

		if (!groupId)
			this->M_CCDatabase.writeLog("error", "Can't get the group id of the specified user '" + this->M_CProcessUsername + "'");

		return false;

    	} else {

      		//Change the group privileges
		if (setresgid(groupId, groupId, groupId) == -1) {

			this->M_CCDatabase.writeLog("error", "Can't change privileges to the group id of the specified user '" + this->M_CProcessUsername + "'");
        		return false;

		}

		//Change the user privileges
      		if (setresuid(userId, userId, userId) == -1) {

			this->M_CCDatabase.writeLog("error", "Can't change privileges to the specified user id '" + this->M_CProcessUsername + "'");
        		return false;

		}

		this->M_CCDatabase.writeLog("info", "[Configuration] Specified User Id : " + to_string(userId));
		this->M_CCDatabase.writeLog("info", "[Configuration] Specified User Group Id : " + to_string(groupId));

    	}
*/
	return true;

}

bool CConfiguration::loading(void) const {

	//Check the process root
	if(!QDir(this->M_Configurations["Process"]["Root"]).exists())
		QDir().mkdir(this->M_Configurations["Process"]["Root"]);
	//this->M_CCDatabase.writeLog("info", "[Configuration] Process (Root) : " + this->M_Configurations["Process"]["Root"]);

	//Check the process launching mode
	//this->M_CCDatabase.writeLog("info", "[Configuration] Process (Mode) : " + this->M_Configurations["Process"]["Mode"]);

	//Check the process path
	if(!QDir(this->M_Configurations["Process"]["Path"]).exists())
		QDir().mkdir(this->M_Configurations["Process"]["Path"]);
	//this->M_CCDatabase.writeLog("info", "[Configuration] Process (Pathname) : " + this->M_Configurations["Process"]["Path"]);

	//Check the process file
	if(this->M_Configurations["Process"]["Mode"] == "true")
		//this->M_CCDatabase.writeLog("info", "[Configuration] Process (Filename) : " + this->M_Configurations["Process"]["File"]);

	//Check the process launching username
	//this->M_CCDatabase.writeLog("info", "[Configuration] Process (Username) : " + this->M_Configurations["Process"]["Username"]);

	//Check the log level
	//this->M_CCDatabase.writeLog("info", "[Configuration] Log (level) : " + this->M_Configurations["Log"]["level"]);

	//Check the database path
	if(!QDir(this->M_Configurations["Database"]["Path"]).exists())
                QDir().mkdir(this->M_Configurations["Database"]["Path"]);
	//this->M_CCDatabase.writeLog("info", "[Configuration] Database (Pathname) : " + this->M_Configurations["Database"]["Path"]);

	//Check the database name
	QDirIterator databaseIterator(this->M_Configurations["Database"]["Path"], QDirIterator::NoIteratorFlags);
	while(databaseIterator.hasNext()) {

		QString file = databaseIterator.next();

		if(file.endsWith(".db"))
			QDir().rename(file, this->M_Configurations["Database"]["Path"] + this->M_Configurations["Database"]["Name"] + ".db");

	}
	//this->M_CCDatabase.writeLog("info", "[Configuration] Database (Name) : " + this->M_Configurations["Database"]["Name"]);

	//Check the database username
        //this->M_CCDatabase.writeLog("info", "[Configuration] Database (Username) : " + this->M_Configurations["Database"]["Username"]);

	//Check the database password
        //this->M_CCDatabase.writeLog("info", "[Configuration] Database (Password) : " + this->M_Configurations["Database"]["Password"]);

	//Check the hostname
	//this->M_CCDatabase.writeLog("info", "[Configuration] Ip (Hostname) : " + this->M_Configurations["Ip"]["Hostname"]);

	//Check the ip mode
	//this->M_CCDatabase.writeLog("info", "[Configuration] Ip (Mode) : " + this->M_Configurations["Ip"]["Mode"]);

	//Check the ipv4/ipv6
	if(!this->M_Configurations["Ip"]["Interface"].isEmpty()) {

		if(!this->M_Configurations["Ip"]["v4"].isEmpty())
			int a=2;
			//this->M_CCDatabase.writeLog("info", "[Configuration] Ip (v4) : " + this->M_Configurations["Ip"]["v4"]);

		if(!this->M_Configurations["Ip"]["v6"].isEmpty())
			int b=3;
                        //this->M_CCDatabase.writeLog("info", "[Configuration] Ip (v6) : " + this->M_Configurations["Ip"]["v6"]);

	}

/*
	//Check if we need to deamonize the executable
	if(this->M_CProcessStatus) {

		path p_filePid = this->M_CProcessFilename;

		//If the pid file doesn't exist
        	if (!exists(p_filePid)) {

			//Get the process id

                	pid_t processId = fork();

			//If the process id generation has failed!
			if (processId < 0) {

                        	this->M_CCDatabase.writeLog("error", "Can't arrive to daemonize the server!");
                        	return false;

                	}

			//Create the new pid file
                	if (!this->createProcessFile()) {

				//Delete the PID file
				this->deleteProcessFile();

				return false;

			}

		} else {

			this->M_CCDatabase.writeLog("error", "The server is already running!");
			return false;

		}

	}

	//Change the privileges to the new user
	if(!this->dropUserPrivileges())
		return false;
*/
	return true;

}

bool CConfiguration::unloading(void) const {
/*
	if (this->M_CProcessStatus) {

		//Delete the PID file
		if (!this->deleteProcessFile())
			return false;

	}
*/
	return true;

}
