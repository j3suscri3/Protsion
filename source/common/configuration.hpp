#ifndef DEF_CONFIGURATION
#define DEF_CONFIGURATION

#include <string>

#include "database.hpp"

class Configuration {

	public:

		//Constructor
		Configuration(Database Database, std::string database, std::string directory, std::string filePid, std::string level, bool status, std::string user, std::string interface);
		//Load all configurations
		bool loadConfiguration(Database Database) const;

	private:

		std::string m_database, m_directory, m_filePid, m_level;
		bool m_status;
		std::string m_user, m_host, m_interface, m_ipv4, m_ipv6;

		//Drop the user privileges
		bool dropUserPrivileges(Database Database) const;
		//Open the pid file
		bool createFilePid(Database Database) const;

};

#endif
