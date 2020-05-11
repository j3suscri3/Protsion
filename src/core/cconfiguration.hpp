#ifndef CCONFIGURATION_HPP
#define CCONFIGURATION_HPP

#include <string>

#include "cdatabase.hpp"

class CConfiguration {

	public:

		//Constructor
		CConfiguration(bool processStatus, std::string databaseName, std::string databaseUsername, std::string databasePassword, std::string directory, std::string processFilename, std::string logLevel, std::string ProcessUsername, std::string ipMode, std::string interface);
		//Load all configurations
		bool load(void) const;
		//Unload all configurations
                bool unload(void) const;

	private:

		CDatabase M_CCDatabase;
		bool M_CProcessStatus;
		std::string M_CDatabaseName, M_CDatabaseUsername, M_CDatabasePassword, M_CDirectory, M_CProcessFilename, M_CLogLevel, M_CProcessUsername, M_CHostname, M_CIpMode, M_CInterface, M_CIpv4, M_CIpv6;

		//Open the process file
                bool createProcessFile(void) const;
		//Delete the process file
                bool deleteProcessFile(void) const;
		//Drop the user privileges
		bool dropUserPrivileges(void) const;

};

#endif
