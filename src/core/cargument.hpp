#ifndef CARGUMENT_HPP
#define CARGUMENT_HPP C_EXECUTABLEVERSION C_EXECUTABLEHASH C_HOSTNAME C_HOSTCATEGORY C_HOSTVERSION C_HOSTCODE C_COMPILATORNAME C_COMPILATORVERSION C_DATABASENAME, C_DATABASEVERSION

#include <QString>

class CArgument {

	public:

		//Constructor
		CArgument(int total, char *arguments[]);
		//Get the database name
		QString getDatabaseName(void) const;
		//Get the database username
                QString getDatabaseUsername(void) const;
		//Get the database password
                QString getDatabasePassword(void) const;
		//Get the directory
		QString getDirectory(void) const;
		//Get the pid filename
		QString getProcessFilename(void) const;
		//Get the status
                bool getProcessStatus(void) const;
		//Get the username
                QString getProcessUsername(void) const;
		//Get the network interface name
                QString getInterface(void) const;
		//Get the ip mode
                QString getIpMode(void) const;
		//Get the log level
                QString getLogLevel(void) const;
		//Show the helper
		void showHelp(QString executable) const;
		//Show the version
		void showVersion(QString executable) const;

	private:

		QString M_CDatabaseName, M_CDatabaseUsername, M_CDatabasePassword, M_CDirectory, M_CProcessFilename, M_CLogLevel, M_CIpMode, M_CInterface, M_CProcessUsername;
		bool M_CProcessStatus;

};

#endif
