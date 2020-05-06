#ifndef DEF_DATABASE
#define DEF_DATABASE

#include <string>

#include "../extra/sqlite3.h"

class Database {

	public:

		//Constructor
		Database(std::string database, std::string directory, std::string level);
		//Connection to the specific database
                bool openDatabase(void);
		//Close from the specific database
		bool closeDatabase(void);
		//Write a new log entry ot the file
		bool logDatabase(std::string message) const;
		//Write a new log entry to the database
		bool writeDatabase_Log(std::string level, std::string message) const;

	private:

		std::string m_database, m_user, m_password, m_directory, m_level;
		sqlite3 *m_sqlite;

};

#endif
