	#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QProcess>
#include <QString>
#include <QTextStream>
#include <QtGlobal>

#include "cprocess.hpp"

CProcess::CProcess(QHash<QString, QString> Configurations) {

	//Loop into the second level of the arguments hash
       	QHashIterator<QString, QString> ConfigurationsIterator(Configurations);
	while(ConfigurationsIterator.hasNext()) {

		this->M_Process[ConfigurationsIterator.key()] = ConfigurationsIterator.value();

	}

}

bool CProcess::changing(void) const {

#ifdef Q_OS_UNIX

	QProcess::execute("id -u" + this->));
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
#endif

        return true;

}

bool CProcess::checking(void) {

	//Check if the process file doesn't exist yet
	if(!QDir(this->M_Process["Path"] + this->M_Process["File"]).exists()) {

		//Get the process id
		QProcess process;
		process.start();
		this->M_Process["Id"] = QString::number(process.processId());

		//Check if the current process hasn't an id already
		if(this->M_Process["Id"] == 0) {

			//Create the process file
			if(!this->creating())
				return false;

			//this->M_CCDatabase.writeLog("info", "[Configuration] Process (id) : " + this->M_Process["Id"]);

		} else {

			//this->M_CCDatabase.writeLog("error", "The server is already running!");
                	return false;

		}

	} else {

		//this->M_CCDatabase.writeLog("error", "The server is already running!");
		return false;

	}

	return true;

}

bool CProcess::creating(void) const {

	QFile file(this->M_Process["Path"] + this->M_Process["File"]);

	//Open the new process file
	if(!file.open(stderr, QIODevice::WriteOnly)) {

		//this->M_CCDatabase.writeLog("error", "Can't open the process file <" + this->M_Process["File"] + ">");
		return false;

	}

	//Write the process id into the file
	QTextStream stream(&file);
    	stream << this->M_Process["Id"] << endl;

	//Apply specific permissions to the process file
	if(!file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner|
				QFileDevice::ReadGroup|QFileDevice::WriteGroup|QFileDevice::ExeGroup)) {

		//this->M_CCDatabase.writeLog("error", "Can't change the permissions about the process file <" + this->M_Process["File"] + ">";
		return false;

	}

	return true;

}

bool CProcess::deleting(void) const {

	QFile file(this->M_Process["Path"] + this->M_Process["File"]);

	//Delete the process file
	if(!file.remove()) {

		//this->M_CCDatabase.writeLog("error", "Can't delete the process file <" + this->M_Process["File"] + ">");
		return false;

	}

	return true;

}
