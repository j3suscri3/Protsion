#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <grp.h>
	#include <iostream>
#include <pwd.h>
#include <regex>
#include <sys/signal.h>
#include <sys/stat.h>
#include <string>
#include <unistd.h>

#include <QString>

#include "cconfiguration.hpp"

using namespace boost::asio::ip;
using namespace boost::filesystem;
using namespace boost::system;
using namespace std;

CConfiguration::CConfiguration(bool processStatus, QString databaseName, QString databaseUsername, QString databasePassword, QString directory, QString processFilename, QString logLevel, QString processUsername, QString ipMode, QString interface) : M_CCDatabase(M_CDatabaseName, M_CDatabaseUsername, M_CDatabasePassword, M_CDirectory, M_CLogLevel), M_CProcessStatus(true), M_CDatabaseName("protsion"), M_CDatabaseUsername("protsion"), M_CDatabasePassword("protsion"), M_CDirectory(""), M_CProcessFilename("_protsion.pid"), M_CLogLevel("info"), M_CProcessUsername("protsion"), M_CHostname(""), M_CIpMode("ipv4-ipv6"), M_CInterface("") {
/*
	this->M_CCDatabase = CDatabase(this->M_CDatabaseName, this->M_CDatabaseUsername, this->M_CDatabasePassword, this->M_CDirectory, this->M_CLevel);
	//If the database connection has failed
	if(!this->M_CCDatabase.open())
		exit(EXIT_FAILURE);
*/
	mode_t mode = 0660;
	if(opendir(this->M_CDirectory.c_str()) == NULL) {

		mkdir(this->M_CDirectory.c_str(), mode);
		cout << "directory not exists" << endl;

	} else {

		cout << "directory exists" << endl;

	}


        path p_directory;
	boost::system::error_code error;
	/*
	//Check the root directory
        if (!directory.empty()) {

                this->M_CDirectory = directory;
                 //Move to the specified directory
                p_directory = this->M_CDirectory;
                current_path(this->M_CDirectory, error);

                if (!exists(p_directory)) {

                        if (!create_directories(this->M_CDirectory)) {

                                this->M_CCDatabase.writeLog("error", "Can't create the root directory " + this->M_CDirectory);
                                exit(EXIT_FAILURE);

                        }

                }

		current_path(this->M_CDirectory, error);

        } else {

                this->M_CDirectory = canonical(current_path()).string() + "/..";
		current_path(this->M_CDirectory, error);
		this->M_CDirectory = canonical(current_path()).string();

	}
	*/
        this->M_CCDatabase.writeLog("info", "[Configuration] Root (directory) : " + this->M_CDirectory);

	cout << this->M_CDirectory << endl;

	//Check the process launching mode
        if (!processStatus)
                this->M_CProcessStatus = processStatus ? "daemonize" : "not daemonize";
        this->M_CCDatabase.writeLog("info", "[Configuration] Process (mode) : " + this->M_CProcessStatus);

        //Check the process launching username
        if (!processUsername.empty())
                this->M_CProcessUsername = processUsername;
        this->M_CCDatabase.writeLog("info", "[Configuration] Launcher (user) : " + this->M_CProcessUsername);

	//Check the database sub-directory
        p_directory = "database";
        if (!exists(p_directory))
                create_directory(p_directory);
        this->M_CCDatabase.writeLog("info", "[Configuration] Database (path) : " + this->M_CDirectory + "/" + p_directory.c_str() + "/");

	this->M_CCDatabase.writeLog("info", "[Configuration] Database (file) : " + this->M_CDatabaseName);
	this->M_CCDatabase.writeLog("info", "[Configuration] Database (user) : " + this->M_CDatabaseUsername);
	this->M_CCDatabase.writeLog("info", "[Configuration] Database (password) : " + this->M_CDatabasePassword);

	//Rename the database file
      	directory = canonical(current_path()).string();
        for (const auto & i_file : std::filesystem::directory_iterator(directory)) {

		//If the database name isn't already exists
                if (!boost::algorithm::ends_with(canonical(i_file.path()).string(), this->M_CDatabaseName))
	        	//When the current database file has found
                	if (boost::algorithm::ends_with(canonical(i_file.path()).string(), ".db"))
        	        	//Rename it with the new name
                        	std::filesystem::rename(i_file.path(), directory + this->M_CDatabaseName);

        }

	//Check the pid sub-directory
        p_directory = "run";
        if (!exists(p_directory))
		create_directory(p_directory);
	this->M_CCDatabase.writeLog("info", "[Configuration] PID (path) : " + this->M_CDirectory + "/" + p_directory.c_str()+ "/");

	//Check the pid file name
	if (!processFilename.empty()) {

		if (processFilename.substr(processFilename.find_last_of(".") + 1) == "pid")
                	this->M_CProcessFilename = processFilename;
		else
                        this->M_CProcessFilename = "_" + processFilename + ".pid";

        }
	this->M_CCDatabase.writeLog("info", "[Configuration] PID (file) : " + this->M_CProcessFilename);

	//Check the log level
	if (!logLevel.empty())
                this->M_CLogLevel = logLevel;
	this->M_CCDatabase.writeLog("info", "[Configuration] Log (level) : " + this->M_CLogLevel);

	//Get the host name
	this->M_CHostname = host_name();
	this->M_CCDatabase.writeLog("info", "[Configuration] Host (name) : " + this->M_CHostname);

	//Get the ip mode(s) choosen
	if (!ipMode.empty())
        	this->M_CIpMode = ipMode;
	this->M_CCDatabase.writeLog("info", "[Configuration] Ip (mode) : " + this->M_CIpMode);

	//Get the specified network interface ipv4 and/or ipv6 with its name
	if (!interface.empty()) {

		//Create the main service
		boost::asio::io_service io_service;
		//Create the resolver
        	tcp::resolver resolver(io_service);
		//Configure the resolver on localhost
		tcp::resolver::query query("localhost", "");
		//Get the iterator list
		tcp::resolver::iterator iterator = resolver.resolve(query);

		//For each network interface
		while (iterator != tcp::resolver::iterator()) {

			address ip = (iterator++)->endpoint().address();

			//Get the ipv4 address
			if (this->M_CIpMode != "ipv6" && !ip.is_v6()) {

				this->M_CIpv4 = ip.to_string();
				this->M_CCDatabase.writeLog("info", "[Configuration] Ip (v4) : " + this->M_CIpv4);

			}

			//Get the ipv6 address
                        if (this->M_CIpMode != "ipv4" && ip.is_v6()) {

				this->M_CIpv6 = ip.to_string();
				this->M_CCDatabase.writeLog("info", "[Configuration] Ip (v6) : " + this->M_CIpv6);

                        }

		}

	}

}

bool CConfiguration::createProcessFile(void) const {

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

	return true;

}

bool CConfiguration::deleteProcessFile(void) const {

	path p_filePid = this->M_CDirectory + "/run/"+ this->M_CProcessFilename;

	cout << "delete" << endl;

	//Delete the PID file
	if (!remove(p_filePid)) {

		this->M_CCDatabase.writeLog("error", "Can't delete the pid file " + this->M_CProcessFilename);
		return false;

	}

	return true;

}

bool CConfiguration::dropUserPrivileges(void) const {

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
	return true;

}

bool CConfiguration::load(void) const {

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

	return true;

}

bool CConfiguration::unload(void) const {

	if (this->M_CProcessStatus) {

		//Delete the PID file
		if (!this->deleteProcessFile())
			return false;

	}

	return true;

}
