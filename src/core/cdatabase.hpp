#ifndef CDATABASE_HPP
#define CDATABASE_HPP

#include <QString>

#include "../extra/sqlite3.h"

class CDatabase {

	public:

		//Constructor
		CDatabase(QString database, QString username, QString password, QString directory, QString level);
		//Connection to the specific database
                bool open(void);
		//Close from the specific database
		bool close(void);
		//Write a new log entry ot the file
		bool log(QString message) const;
		//Write a new log entry to the database
		bool writeLog(QString level, QString message) const;

	private:

		QString M_CDatabase, M_CUsername, M_CPassword, M_CDirectory, M_CLevel;
		sqlite3 *M_CSqlite;

};

#endif
