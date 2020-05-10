#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
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

#include "configuration.hpp"
#include "database.hpp"

using namespace boost::asio::ip;
using namespace boost::filesystem;
using namespace boost::system;
using namespace std;

Configuration::Configuration(Database Database, string database, string directory, string filePid, string level, bool status, string user, string ipMode, string interface) : m_database("protsion:protsion@protsion"), m_directory(""), m_filePid("_protsion.pid"), m_level("info"), m_status(true), m_user("protsion"), m_host(""), m_ipMode("ipv4-ipv6"), m_interface("") {

        path p_directory;
	boost::system::error_code error;
	string s_database, s_password, s_user;

	//Check the root directory
        if (!directory.empty()) {

                this->m_directory = directory;
                 //Move to the specified directory
                p_directory = this->m_directory;
                current_path(this->m_directory, error);

                if (!exists(p_directory)) {

                        if (!create_directories(this->m_directory)) {

                                Database.writeDatabase_Log("error", "Can't create the root directory " + this->m_directory);
                                exit(EXIT_FAILURE);

                        }

                }

		current_path(this->m_directory, error);

        } else {

                this->m_directory = canonical(current_path()).string() + "/..";
		current_path(this->m_directory, error);
		this->m_directory = canonical(current_path()).string();

	}

        Database.writeDatabase_Log("info", "[Configuration] Root (directory) : " + this->m_directory);

	//Check the launch mode
        if (!status)
                this->m_status = status;
        string s_status = to_string(this->m_status) == "1" ? "daemonize" : "not daemonize";
        Database.writeDatabase_Log("info", "[Configuration] Launcher (mode) : " + s_status);

        //Check the user name
        if (!user.empty())
                this->m_user = user;
        Database.writeDatabase_Log("info", "[Configuration] Launcher (user) : " + this->m_user);

	//Check the database sub-directory
        p_directory = "database";
        if (!exists(p_directory))
                create_directory(p_directory);
        Database.writeDatabase_Log("info", "[Configuration] Database (path) : " + this->m_directory + "/" + p_directory.c_str() + "/");

	//Check the database connection configuration
	regex r_database(":(.*)$");
	regex r_user("^(.*):|@(.*)$");
	regex r_password("^(.*)@");

        if (!database.empty()) {

                //Get the database name
                s_database = regex_replace(database, r_database, "$3") + ".db";

                //Get the database username
                s_user = regex_replace(database, r_user, "$3");

                //Get the database password
                s_password = regex_replace(database, r_password, "$3");

	} else {

		//Get the database name
                s_database = regex_replace(this->m_database, r_database, "$3") + ".db";

                //Get the database username
                s_user = regex_replace(this->m_database, r_user, "$3");

                //Get the database password
                s_password = regex_replace(this->m_database, r_password, "$3");

	}
	Database.writeDatabase_Log("info", "[Configuration] Database (file) : " + s_database);
	Database.writeDatabase_Log("info", "[Configuration] Database (user) : " + s_user);
	Database.writeDatabase_Log("info", "[Configuration] Database (password) : " + s_password);

	//Rename the database file
      	directory = canonical(current_path()).string();
        for (const auto & i_file : std::filesystem::directory_iterator(directory)) {

		//If the database name isn't already exists
                if (!boost::algorithm::ends_with(canonical(i_file.path()).string(), s_database))
	        	//When the current database file has found
                	if (boost::algorithm::ends_with(canonical(i_file.path()).string(), ".db"))
        	        	//Rename it with the new name
                        	std::filesystem::rename(i_file.path(), directory + s_database);

        }

	//Check the pid sub-directory
        p_directory = "run";
        if (!exists(p_directory))
		create_directory(p_directory);
	Database.writeDatabase_Log("info", "[Configuration] PID (path) : " + this->m_directory + "/" + p_directory.c_str()+ "/");

	//Check the pid file name
	if (!filePid.empty()) {

		if (filePid.substr(filePid.find_last_of(".") + 1) == "pid")
                	this->m_filePid = filePid;
		else
                        this->m_filePid = "_" + filePid + ".pid";

        }
	Database.writeDatabase_Log("info", "[Configuration] PID (file) : " + this->m_filePid);

	//Check the log level
	if (!level.empty())
                this->m_level = level;
	Database.writeDatabase_Log("info", "[Configuration] Log (level) : " + this->m_level);

	//Get the host name
	this->m_host = host_name();
	Database.writeDatabase_Log("info", "[Configuration] Host (name) : " + this->m_host);

	//Get the ip mode(s) choosen
	if (!ipMode.empty())
        	this->m_ipMode = ipMode;
	Database.writeDatabase_Log("info", "[Configuration] Ip (mode) : " + this->m_ipMode);

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
			if (this->m_ipMode != "ipv6" && !ip.is_v6()) {

				this->m_ipv4 = ip.to_string();
				Database.writeDatabase_Log("info", "[Configuration] Ip (v4) : " + this->m_ipv4);

			}

			//Get the ipv6 address
                        if (this->m_ipMode != "ipv4" && ip.is_v6()) {

				this->m_ipv6 = ip.to_string();
				Database.writeDatabase_Log("info", "[Configuration] Ip (v6) : " + this->m_ipv6);

                        }

		}

	}

}

bool Configuration::createFilePid(Database Database) const {

        //Change the current path
	string filePid = this->m_directory + "/run/" + this->m_filePid;

        //current_path("run");
        std::fstream stream;

        //Open the pid file
        stream.open(filePid, std::fstream::in|std::fstream::out|std::fstream::binary|std::fstream::trunc);
        if (stream.fail()) {

		Database.writeDatabase_Log("error", "Can't open the pid file " + this->m_filePid);
                return false;

	}

        //Write the pid into the file
        stream << getpid();
	if (stream.fail()) {

		Database.writeDatabase_Log("error", "Can't write into pid file " + this->m_filePid);
                return false;

	}

	stream.close();
        Database.writeDatabase_Log("info", "[Configuration] Process (id) : " + to_string(getpid()));

	//Change the permissions
	path p_filePid = filePid;
	boost::system::error_code error;
        permissions(p_filePid, perms::owner_all|perms::group_all, error);
	if (error) {

		Database.writeDatabase_Log("error", "Can't change the permissions about the pid file " + this->m_filePid);
		return false;

	}

	return true;

}

bool Configuration::deleteFilePid(Database Database) const {

	path p_filePid = this->m_directory + "/run/"+ this->m_filePid;

	cout << "delete" << endl;

	//Delete the PID file
	if (!remove(p_filePid)) {

		Database.writeDatabase_Log("error", "Can't delete the pid file " + this->m_filePid);
		return false;

	}

	return true;

}

bool Configuration::dropUserPrivileges(Database Database) const {

	auto size = sysconf(_SC_GETPW_R_SIZE_MAX);
	//Manage the FreeBSD special case when defining the buffer size
        if (size == -1)
                size = 1024;
        char *buffer = static_cast<char *>(malloc(size));

        //Get the user id with the user name
	struct passwd s_password, *ps_password(0);
        getpwnam_r(this->m_user.c_str(), &s_password, buffer, size, &ps_password);
	uid_t userId = ps_password ? ps_password->pw_uid : -1;

	//Get the group id with the group name
	struct group s_group, *ps_group(0);
	getgrnam_r(this->m_user.c_str(), &s_group, buffer, size, &ps_group);
	gid_t groupId = ps_group ? ps_group->gr_gid : -1;
	free(buffer);

	//If we can't get the current user id and group id
	if (!userId || !groupId) {

		if (!userId)
			Database.writeDatabase_Log("error", "Can't get the specified user id : '" + this->m_user + "'");

		if (!groupId)
			Database.writeDatabase_Log("error", "Can't get the group id of the specified user '" + this->m_user + "'");

		return false;

    	} else {

      		//Change the group privileges
		if (setresgid(groupId, groupId, groupId) == -1) {

			Database.writeDatabase_Log("error", "Can't change privileges to the group id of the specified user '" + this->m_user + "'");
        		return false;

		}

		//Change the user privileges
      		if (setresuid(userId, userId, userId) == -1) {

			Database.writeDatabase_Log("error", "Can't change privileges to the specified user id '" + this->m_user + "'");
        		return false;

		}

		Database.writeDatabase_Log("info", "[Configuration] Specified User Id : " + to_string(userId));
		Database.writeDatabase_Log("info", "[Configuration] Specified User Group Id : " + to_string(groupId));

    	}
	return true;

}

bool Configuration::loadConfiguration(Database Database) const {

	//Check if we need to deamonize the executable
	if(this->m_status) {

		path p_filePid = this->m_filePid;

		//If the pid file doesn't exist
        	if (!exists(p_filePid)) {

			//Get the process id

                	pid_t processId = fork();

			//If the process id generation has failed!
			if (processId < 0) {

                        	Database.writeDatabase_Log("error", "Can't arrive to daemonize the server!");
                        	return false;

                	}

			//Create the new pid file
                	if (!this->createFilePid(Database)) {

				//Delete the PID file
				this->deleteFilePid(Database);

				return false;

			}

		} else {

			Database.writeDatabase_Log("error", "The server is already running!");
			return false;

		}

	}

	//Change the privileges to the new user
	if(!this->dropUserPrivileges(Database))
		return false;

	return true;

}

bool Configuration::unloadConfiguration(Database Database) const {

	if (this->m_status) {

		//Delete the PID file
		if (!this->deleteFilePid(Database))
			return false;

	}

	return true;

}
