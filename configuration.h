#ifndef DEF_CONFIGURATION
#define DEF_CONFIGURATION

#include <string>

class Configuration
{

	public:

		//Constructor
		Configuration(std::string directory, std::string fileConf, std::string filePid, std::string level, bool status, std::string user);
		//Get the root directory
	        std::string getDirectory() const;
		//Get the configuration filename
		std::string getFileConf() const;
		//Get the pid filename
		std::string getFilePid() const;
		//Get the log level
		std::string getLevel() const;
		//Get the daemonize status
		bool getStatus() const;
		//Set the username
		std::string getUser() const;

	private:

		std::string m_directory;
		std::string m_fileConf;
		std::string m_filePid;
		std::string m_level;
		bool m_status;
		std::string m_user;

};

#endif
