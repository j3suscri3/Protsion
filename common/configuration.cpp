#include <boost/filesystem.hpp>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <grp.h>
	#include <iostream>
#include <limits.h>
#include <pwd.h>
#include <regex>
#include <sys/signal.h>
#include <sys/stat.h>
#include <string>
#include <unistd.h>

#include "configuration.hpp"

using namespace boost::filesystem;
using namespace boost::system;
using namespace std;

Configuration::Configuration(std::string directory, std::string filePid, std::string level, bool status, std::string user) : m_directory(""), m_filePid("_protsion.pid"),  m_level("i"), m_status(true), m_user("protsion") {

	path p_directory;

	if (!directory.empty()) {

		this->m_directory = directory;
		 //Move to the specified directory
		p_directory = this->m_directory;
                boost::system::error_code error;
                current_path(this->m_directory, error);

                if (!exists(p_directory)) {

                        if (!create_directories(this->m_directory))
                                exit(EXIT_FAILURE);
                        current_path(this->m_directory, error);

                }

        } else {

		//Get the executable full path name (filename included)
		char  result[PATH_MAX];
        	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);

		this->m_directory = string(result, (count > 0) ? count : 0);
		//Get the executable path name only
		regex regex("(\\w|\\d|-|[.]){1,}$");
		regex_replace(this->m_directory, regex, "$0");

	}

	//Checks some sub-directories
        p_directory = "run";
        if (!exists(p_directory))
		create_directory(p_directory);
	p_directory = "db";
        if (!exists(p_directory))
                create_directory(p_directory);

	//Check the pid file name
	if (!filePid.empty()) {

		if (filePid.substr(filePid.find_last_of(".") + 1) == "pid")
                	this->m_filePid = filePid;
		else
                        this->m_filePid = "_" + filePid + ".pid";

        }

	//Check the log level
	if (!level.empty())
                this->m_level = level;

	//Check the launch mode
	if (!status)
                this->m_status = status;

	//Check the user name
	if (!user.empty())
                this->m_user = user;

}

bool Configuration::createFilePid(void) const {

        //Change the current path
        current_path("run");
        std::fstream stream;

        //Open the pid file
        stream.open(this->m_filePid, std::fstream::in|std::fstream::out|std::fstream::binary|std::fstream::trunc);
        if (stream.fail())
                return false;

        //Write the pid into the file
        stream << getpid();
        if (!stream.fail()) {

                stream.close();
                //INFO LOG get pid
                return true;

        } else
                return false;

}

bool Configuration::dropUserPrivileges(void) const {

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

      		return false;
		//ERROR LOG

    	} else {

      		//Change the group privileges
		if (setresgid(groupId, groupId, groupId) == -1) {

        		return false;
			//ERROR LOG

		}

		//Change the user privileges
      		if (setresuid(userId, userId, userId) == -1) {

        		return false;
			//ERROR LOG

		}
      		//INFO LOG << "privileges dropped to " << group_name << "[" << gid << "] " << user_name << "[" << uid << "]";

    	}
	return true;

}

string Configuration::getDirectory(void) const {

        return this->m_directory;

}

string Configuration::getFilePid(void) const {

        return this->m_filePid;

}

string Configuration::getLevel(void) const {

	return this->m_level;

}

bool Configuration::getStatus(void) const {
	return this->m_status;
}

string Configuration::getUser(void) const {

	return this->m_user;

}

void Configuration::loadConfiguration(void) const {

	//Check if we need to deamonize the executable
	if(this->m_status) {

  		pid_t processId = 0, sessionId = 0;

		//Get the parent process id
  		processId = fork();

  		if (processId < 0) {

    			exit(EXIT_FAILURE);

  		}

  		// If we got a good PID, then we can exit the parent process.
  		if (processId > 0) {

    			exit(EXIT_SUCCESS);

  		}

		cout << "processId: " << processId << " sessionId: " << sessionId << endl;

		//Ignore signal sent from child to parent process
		signal(SIGCHLD, SIG_IGN);

		//Create a new session id for the child process
  		sessionId = setpgid(0, 0);
  		if (sessionId < 0) {

    			//LOG(FATAL) << "setsid() failed";

  		}

  		// Change the file mode mask
  		umask(0);

		cout << "processId: " << processId << " sessionId: " << sessionId << endl;

        	//Open the garbage for the next returns
		std::fstream stream;
        	stream.open("/dev/null", std::fstream::in|std::fstream::out|std::fstream::trunc);
		if (stream.fail()) {

                	//LOG(FATAL) << "open(/dev/null) failed";

		}

		cin.rdbuf(stream.rdbuf());
		if (cin.fail()) {

			//LOF FATAL

		}

		cout.rdbuf(stream.rdbuf());
                if (cout.fail()) {

                        //LOF FATAL

                }

		cerr.rdbuf(stream.rdbuf());
                if (cerr.fail()) {

                        //LOF FATAL

                }

  		stream.close();

		path p_filePid = this->m_filePid;

		//If the pid file doesn't exist
        	if (!exists(p_filePid))
                	//Create the new pid file
                	if(!this->createFilePid())
                        	unlink(this->m_filePid.c_str());


	}

	//Change the privileges to the new user
	this->dropUserPrivileges();

}
