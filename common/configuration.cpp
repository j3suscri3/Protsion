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

Configuration::Configuration(Database Data, string database, string directory, string filePid, string level, bool status, string user, string interface) : m_database("protsion:protsion@protsion"), m_directory(""), m_filePid("_protsion.pid"), m_level("info"), m_status(true), m_user("protsion"), m_host(""), m_interface("") {

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

                                Data.writeDatabase_Log("error", "Can't create the root directory " + this->m_directory);
                                exit(EXIT_FAILURE);

                        }

                }

		current_path(this->m_directory, error);

        } else {

                this->m_directory = canonical(current_path()).string() + "/..";
		current_path(this->m_directory, error);
		this->m_directory = canonical(current_path()).string();

	}

        Data.writeDatabase_Log("info", "[Configuration] Root (directory) : " + this->m_directory);

	//Check the launch mode
        if (!status)
                this->m_status = status;
        string s_status = to_string(this->m_status) == "1" ? "daemonize" : "not daemonize";
        Data.writeDatabase_Log("info", "[Configuration] Launcher (mode) : " + s_status);

        //Check the user name
        if (!user.empty())
                this->m_user = user;
        Data.writeDatabase_Log("info", "[Configuration] Launcher (user) : " + this->m_user);

	//Check the database sub-directory
        p_directory = "database";
        if (!exists(p_directory))
                create_directory(p_directory);
        Data.writeDatabase_Log("info", "[Configuration] Database (path) : " + this->m_directory + "/" + p_directory.c_str() + "/");

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
	Data.writeDatabase_Log("info", "[Configuration] Database (file) : " + s_database);
	Data.writeDatabase_Log("info", "[Configuration] Database (user) : " + s_user);
	Data.writeDatabase_Log("info", "[Configuration] Database (password) : " + s_password);

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
	Data.writeDatabase_Log("info", "[Configuration] PID (path) : " + this->m_directory + "/" + p_directory.c_str()+ "/");

	//Check the pid file name
	if (!filePid.empty()) {

		if (filePid.substr(filePid.find_last_of(".") + 1) == "pid")
                	this->m_filePid = filePid;
		else
                        this->m_filePid = "_" + filePid + ".pid";

        }
	Data.writeDatabase_Log("info", "[Configuration] PID (file) : " + this->m_filePid);

	//Check the log level
	if (!level.empty())
                this->m_level = level;
	Data.writeDatabase_Log("info", "[Configuration] Log (level) : " + this->m_level);

	//Get the host name
	this->m_host = host_name();
	Data.writeDatabase_Log("info", "[Configuration] Host (name) : " + this->m_host);

	//Get the interface network ipv4 and ipv6
	boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
	tcp::resolver::query query(this->m_host, "");
	tcp::resolver::iterator iterator = resolver.resolve(query);

	while (iterator != tcp::resolver::iterator()) {

    		address addr = (iterator++)->endpoint().address();
		std::cout << addr.to_string() << std::endl;

	}

	//std::for_each(r_interface.resolve({this->m_host, ""}), {}, [](const auto& re) {

	//	std::cout << re.endpoint().address() << '\n';

    	//});

	if (!interface.empty()) {


	} else {


	}

}

bool Configuration::createFilePid(Database Data) const {

        //Change the current path
        current_path("run");
        std::fstream stream;

        //Open the pid file
        stream.open(this->m_filePid, std::fstream::in|std::fstream::out|std::fstream::binary|std::fstream::trunc);
        if (stream.fail()) {

		Data.writeDatabase_Log("error", "Can't open the pid file " + this->m_filePid);
                return false;

	}

        //Write the pid into the file
        stream << getpid();
        if (!stream.fail()) {

                stream.close();
                Data.writeDatabase_Log("info", "[Configuration] Pid Number : " + to_string(getpid()));
                return true;

        } else {

		Data.writeDatabase_Log("error", "Can't write into pid file " + this->m_filePid);
                return false;

	}

}

bool Configuration::dropUserPrivileges(Database Data) const {

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
			Data.writeDatabase_Log("error", "Can't get the specified user id : '" + this->m_user + "'");

		if (!groupId)
			Data.writeDatabase_Log("error", "Can't get the group id of the specified user '" + this->m_user + "'");

		return false;

    	} else {

      		//Change the group privileges
		if (setresgid(groupId, groupId, groupId) == -1) {

			Data.writeDatabase_Log("error", "Can't change privileges to the group id of the specified user '" + this->m_user + "'");
        		return false;

		}

		//Change the user privileges
      		if (setresuid(userId, userId, userId) == -1) {

			Data.writeDatabase_Log("error", "Can't change privileges to the specified user id '" + this->m_user + "'");
        		return false;

		}

		Data.writeDatabase_Log("info", "[Configuration] Specified User Id : " + to_string(userId));
		Data.writeDatabase_Log("info", "[Configuration] Specified User Group Id : " + to_string(groupId));

    	}
	return true;

}

bool Configuration::loadConfiguration(Database Data) const {

	//Check if we need to deamonize the executable
	if(this->m_status) {

  		pid_t processId = 0, sessionId = 0;

		//Get the parent process id
  		processId = fork();

  		if (processId < 0) {

			Data.writeDatabase_Log("error", "Can't get parent process id (" + to_string(processId) + ")");
    			return false;

  		}

  		// If we got a good PID, then we can exit the parent process.
  		if (processId > 0)
			Data.writeDatabase_Log("warning", "Can get parent process id (" + to_string(processId) + ") and quit it");

		Data.writeDatabase_Log("info", "[Configuration] Process Id : " + to_string(processId));

		//Ignore signal sent from child to parent process
		signal(SIGCHLD, SIG_IGN);

		//Create a new session id for the child process
  		sessionId = setpgid(0, 0);
  		if (sessionId < 0) {

			Data.writeDatabase_Log("error", "Can\'t get child process group id (" + to_string(sessionId) + ")");
    			return false;

  		}

  		// Change the file mode mask
  		umask(0);

		Data.writeDatabase_Log("info", "[Configuration] Session Id : " + to_string(sessionId));

        	//Open the garbage for the next returns
		std::fstream stream;
        	stream.open("/dev/null", std::fstream::in|std::fstream::out|std::fstream::trunc);
		if (stream.fail()) {

                	//LOG(FATAL) << "open(/dev/null) failed";
			return false;

		}

		cin.rdbuf(stream.rdbuf());
		if (cin.fail()) {

			//LOF FATAL
			return false;

		}

		cout.rdbuf(stream.rdbuf());
                if (cout.fail()) {

                        //LOF FATAL
			return false;

                }

		cerr.rdbuf(stream.rdbuf());
                if (cerr.fail()) {

                        //LOF FATAL
			return false;

                }

  		stream.close();

		path p_filePid = this->m_filePid;

		//If the pid file doesn't exist
        	if (!exists(p_filePid))
                	//Create the new pid file
                	if(!this->createFilePid(Data))
                        	unlink(this->m_filePid.c_str());


	}

	//Change the privileges to the new user
	this->dropUserPrivileges(Data);

	return true;

}
