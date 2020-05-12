#ifndef CCONFIGURATION_HPP
#define CCONFIGURATION_HPP

#include <QString>

#include "cdatabase.hpp"

class CConfiguration {

	public:

		//Constructor
		CConfiguration(bool processStatus, QString databaseName, QString databaseUsername, QString databasePassword, QString directory, QString processFilename, QString logLevel, QString ProcessUsername, QString ipMode, QString interface);
		//Load all configurations
		bool load(void) const;
		//Unload all configurations
                bool unload(void) const;

	private:

		CDatabase M_CCDatabase;
		bool M_CProcessStatus;
		QString M_CDatabaseName, M_CDatabaseUsername, M_CDatabasePassword, M_CDirectory, M_CProcessFilename, M_CLogLevel, M_CProcessUsername, M_CHostname, M_CIpMode, M_CInterface, M_CIpv4, M_CIpv6;

		//Open the process file
                bool createProcessFile(void) const;
		//Delete the process file
                bool deleteProcessFile(void) const;
		//Drop the user privileges
		bool dropUserPrivileges(void) const;

};

#endif
