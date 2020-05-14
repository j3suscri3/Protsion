#include <QHash>
#include <QString>

#include "cprocess.hpp"

CProcess::CProcess(QHash<QString, QHash<QString, QString>> M_Configurations) {



}

bool CProcess::checking(void) const {

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

}

bool CProcess::creating(void) const {
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

bool CProcess::deleting(void) const {
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

bool CProcess::Changing(void) const {
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
