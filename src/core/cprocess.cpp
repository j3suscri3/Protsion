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

	if(!Configurations.isEmpty()) {

		//Loop into the second level of the arguments
        	QHashIterator<QString, QString> ConfigurationsIterator(Configurations);
        	while(ConfigurationsIterator.hasNext()) {

                	ConfigurationsIterator.next();

                	this->M_Process[ConfigurationsIterator.key()] = ConfigurationsIterator.value();

        	}

	}

}

CProcess::~CProcess() {

}

bool CProcess::changing(CDatabase Database) const {

#ifdef Q_OS_UNIX

	QProcess process;
        process.start();

	//Get the user id
	process.execute("id -u " + this->M_Process["User"]);
	QString userId = process.readAllStandardOutput().data();

	 if(userId.isEmpty()) {

                Database.writingLog("error", "Can't get the specified user id!");
                return false;

        }

	//Get the group id
	process.execute("id -g " + this->M_Process["User"]);
	QString groupId = process.readAllStandardOutput().data();

	if(groupId.isEmpty()) {

                Database.writingLog("error", "Can't get the specified group id!");
                return false;

        }
	/*
	//Change the group privileges
	if(setresgid(groupId, groupId, groupId) == -1) {

		//this->M_CCDatabase.writeLog("error", "Can't change privileges to the group id of the specified user '");
		return false;

	}

	//Change the user privileges
	if(setresuid(userId, userId, userId) == -1) {

		//this->M_CCDatabase.writeLog("error", "Can't change privileges to the specified user id '" + this->M_CProc$
		return false;

	}
	*/
#endif

	return true;

}

bool CProcess::checking(CDatabase Database) {

	//Check if we need to deamonize the executable
        if(this->M_Process["Mode"].compare("true") == 0) {

		//Check if the process file doesn't exist yet
		if(!QDir(this->M_Process["Path"] + this->M_Process["File"]).exists()) {

			//Get the process id
			QProcess process;
			process.start();
			this->M_Process["Id"] = QString::number(process.processId());

			//Check if the current process hasn't an id already
			if(this->M_Process["Id"].compare("0") == 0) {

				//Create the process file
				if(!this->creating(Database))
					return false;

				QString Message = QString("[Configuration] Process (Id) : ") + this->M_Process["Id"];
				Database.writingLog("info", Message);

			} else {

				Database.writingLog("error", "The server is already running!");
                		return false;

			}

		} else {

			Database.writingLog("error", "The server is already running!");
			return false;

		}

	}

	//Change the user privileges for the process
        if(!this->changing(Database))
                return false;

	return true;

}

bool CProcess::creating(CDatabase Database) const {

	qInfo() << this->M_Process["Path"] << this->M_Process["File"];

	QFile file(this->M_Process["Path"] + this->M_Process["File"]);

	//Open the new process file
	if(!file.open(QIODevice::WriteOnly|QIODevice::Text)) {

		QString Message = QString("Can't open the process file <") + this->M_Process["File"] + QString(">");
		Database.writingLog("error", Message);
		return false;

	}

	//Write the process id into the file
	QTextStream stream(&file);
	stream << this->M_Process["Id"] << endl;

	//Apply specific permissions to the process file
	if(!file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ExeOwner|
				QFileDevice::ReadGroup|QFileDevice::WriteGroup|QFileDevice::ExeGroup)) {

		QString Message = QString("Can't change the permissions about the process file <") + this->M_Process["File"] + QString(">");
		Database.writingLog("error", Message);
		return false;

	}

	return true;

}

bool CProcess::deleting(CDatabase Database) const {

	QFile file(this->M_Process["Path"] + this->M_Process["File"]);

	//Delete the process file
	if(!file.remove()) {

		QString Message = QString("Can't delete the process file <") + this->M_Process["File"] + QString(">");
		Database.writingLog("error", Message);
		return false;

	}

	return true;

}
