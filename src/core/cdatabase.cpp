#include <QHash>
#include <QString>
#include <QtSql>

#include "cdatabase.hpp"

CDatabase::CDatabase(QHash<QString, QString> Configurations) {

        //Loop into the second level of the arguments
	QHashIterator<QString, QString> ConfigurationsIterator(Configurations);
        while(ConfigurationsIterator.hasNext()) {

               	ConfigurationsIterator.next();

		this->M_Database[ConfigurationsIterator.key()] = ConfigurationsIterator.value();

	}

}

CDatabase::~CDatabase() {

}

bool CDatabase::opening(void) {

	this->M_Sql = QSqlDatabase::addDatabase("QSQLITE");
	QString uri = this->M_Database["Path"] + this->M_Database["Name"]; //+ QString("?nolock=false&vfs=unix-dotfile");
	this->M_Sql.setDatabaseName(uri);
	this->M_Sql.setUserName(this->M_Database["User"]);
	this->M_Sql.setPassword(this->M_Database["Password"]);
	this->M_Sql.setConnectOptions("QSQLITE_OPEN_URI");

	if(!this->M_Sql.open()) {

		QString Message = "Can't open the database (" + this->M_Sql.lastError().text() + ")";
		this->logging(Message);

		return false;

	}

	QString Message = "Connection to the database " + this->M_Database["Name"] +  " with success!";
	this->writingLog(this->M_Database["Log"], Message);

	return true;

}

bool CDatabase::closing(void) {

	QString Message = "Deconnection to the database " + this->M_Database["Name"] +  "!";
	this->writingLog(this->M_Database["Log"], Message);

	this->M_Sql.close();

	if(this->M_Sql.isOpen()) {

		QString Message = "Can't close the database (" + this->M_Sql.lastError().text() + ").";
                this->logging(Message);

		return false;

	}

	return true;

}

bool CDatabase::logging(QString Message) const {
/*
	//Define the current date
	time_t t_datetime = time(nullptr);
	char s_datetime[100];
	strftime(s_datetime, sizeof(s_datetime), "%Y%m%d", localtime(&t_datetime));

	std::ofstream log;
	//Open the offline log file
	log.open(this->M_CDirectory + "protsion_" + s_datetime + ".log", std::ofstream::out|std::ofstream::app);
	if(log.fail()) {

		cout << "Can't open the offline log file!" << endl;
		return false;

	}

	//Write the error into the offline log file
	strftime(s_datetime, sizeof(s_datetime), "%Y-%m-%d %H:%M:%S", localtime(&t_datetime));
	log << "[" << s_datetime << "] " << message << "\n";

	//Close the offline log file
	log.close();
	if(log.fail()) {

                cout << "Can't close the offline log file!" << endl;
                return false;

        }
*/
	return true;

}

bool CDatabase::writingLog(QString Level, QString Message) const {

	//Check if the log level for saving
        if((Level == "warning" && this->M_Database["Log"] == "error") || (Level == "info" && this->M_Database["Log"] != "info"))
        	return true;

	QSqlQuery query;
    	if(!query.exec("INSERT INTO log VALUES (DATETIME('now', 'localtime'), '" + Level + "', '" + Message + "')")) {

		QString Message = "Can't write datas into the database (" + this->M_Sql.lastError().text() + ").";
		this->logging(Message);

                return false;

	}

	return true;

}
