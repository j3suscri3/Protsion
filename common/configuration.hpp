#ifndef DEF_CONFIGURATION
#define DEF_CONFIGURATION

#include <string>

#include "database.hpp"

class Configuration {

	public:

		//Constructor
		Configuration(Database Database, std::string database, std::string directory, std::string filePid, std::string level, bool status, std::string user);
		//Load all configurations
		bool loadConfiguration(Database Database) const;

	private:

		std::string m_database;
		std::string m_directory;
		std::string m_filePid;
		std::string m_level;
		bool m_status;
		std::string m_user;

		//Drop the user privileges
		bool dropUserPrivileges(Database Database) const;
		//Open the pid file
		bool createFilePid(Database Database) const;

};

#endif
