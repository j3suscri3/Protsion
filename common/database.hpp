#ifndef DEF_DATABASE
#define DEF_DATABASE

#include <string>

class Database {

	public:
		//Constructor
		Database(std::string host, std::string database);

	private:

		std::string m_host;
		std::string m_database;
		//Connection to a specific database
		bool openDatabase(void) const;

};

#endif

