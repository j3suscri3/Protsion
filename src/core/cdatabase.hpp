#ifndef CDATABASE_HPP
#define CDATABASE_HPP

#include <string>

#include "../extra/sqlite3.h"

class CDatabase {

	public:

		//Constructor
		CDatabase(std::string database, std::string username, std::string password, std::string directory, std::string level);
		//Connection to the specific database
                bool open(void);
		//Close from the specific database
		bool close(void);
		//Write a new log entry ot the file
		bool log(std::string message) const;
		//Write a new log entry to the database
		bool writeLog(std::string level, std::string message) const;

	private:

		std::string M_CDatabase, M_CUsername, M_CPassword, M_CDirectory, M_CLevel;
		sqlite3 *M_CSqlite;

};

#endif
