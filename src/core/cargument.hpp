#ifndef CARGUMENT_HPP
#define CARGUMENT_HPP C_EXECUTABLEVERSION C_EXECUTABLEHASH C_HOSTNAME C_HOSTCATEGORY C_HOSTVERSION C_HOSTCODE C_COMPILATORNAME C_COMPILATORVERSION C_DATABASENAME, C_DATABASEVERSION

#include <string>

class CArgument {

	public:

		//Constructor
		CArgument(int total, char *arguments[]);
		//Get the database name
		std::string getDatabaseName(void) const;
		//Get the database username
                std::string getDatabaseUsername(void) const;
		//Get the database password
                std::string getDatabasePassword(void) const;
		//Get the directory
		std::string getDirectory(void) const;
		//Get the pid filename
		std::string getProcessFilename(void) const;
		//Get the status
                bool getProcessStatus(void) const;
		//Get the username
                std::string getProcessUsername(void) const;
		//Get the network interface name
                std::string getInterface(void) const;
		//Get the ip mode
                std::string getIpMode(void) const;
		//Get the log level
                std::string getLogLevel(void) const;
		//Show the helper
		void showHelp(std::string executable) const;
		//Show the version
		void showVersion(std::string executable) const;

	private:

		std::string M_CDatabaseName, M_CDatabaseUsername, M_CDatabasePassword, M_CDirectory, M_CProcessFilename, M_CLogLevel, M_CIpMode, M_CInterface, M_CProcessUsername;
		bool M_CProcessStatus;

};

#endif
