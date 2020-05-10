#ifndef DEF_ARGUMENT
#define DEF_ARGUMENT

#include <string>

class Argument {

	public:

		//Constructor
		Argument();
		//Get the database connection configuration
		std::string getDatabase(void) const;
		//Get the directory
		std::string getDirectory(void) const;
		//Get the configuration filename
		std::string getFileConf(void) const;
		//Get the pid filename
		std::string getFilePid(void) const;
		//Get the network interface name
                std::string getInterface(void) const;
		//Get the ip mode
                std::string getIpMode(void) const;
		//Get the log level
                std::string getLevel(void) const;
		//Get the status
		bool getStatus(void) const;
		//Get the username
		std::string getUser(void) const;
		//Parse all argument passed
		void parse(int total, char *table[]);
		//Show the helper
		void showHelp(std::string sh) const;
		//Show the version
		void showVersion(std::string sh) const;

	private:

		std::string m_database, m_directory, m_fileConf, m_filePid, m_level, m_ipMode, m_interface;
		bool m_status;
		std::string m_user;

};

#endif
