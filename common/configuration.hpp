#ifndef DEF_CONFIGURATION
#define DEF_CONFIGURATION

#include <string>

class Configuration {

	public:

		//Constructor
		Configuration(std::string directory, std::string filePid, std::string level, bool status, std::string user);
		//Get the root directory
	        std::string getDirectory(void) const;
		//Get the pid filename
		std::string getFilePid(void) const;
		//Get the log level
		std::string getLevel(void) const;
		//Get the daemonize status
		bool getStatus(void) const;
		//Set the username
		std::string getUser(void) const;
		//Load all configurations
		void loadConfiguration(void) const;

	private:

		std::string m_directory;
		std::string m_filePid;
		std::string m_level;
		bool m_status;
		std::string m_user;

		//Drop the user privileges
		bool dropUserPrivileges(void) const;
		//Open the pid file
		bool createFilePid(void) const;

};

#endif
