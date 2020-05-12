#ifndef CARGUMENT_HPP
#define CARGUMENT_HPP C_EXECUTABLEVERSION C_EXECUTABLEHASH C_HOSTNAME C_HOSTCATEGORY C_HOSTVERSION C_HOSTCODE C_COMPILATORNAME C_COMPILATORVERSION C_DATABASENAME, C_DATABASEVERSION

#include <QHash>
#include <QString>

class CArgument {

	public:

		//Constructor
		CArgument(int total, char *arguments[]);
		//Get the arguments table
		QHash<QString, QHash<QString, QString>> getting(void) const;
		//Show the helper
		void showingHelp(QString executable) const;
		//Show the version
		void showingVersion(QString executable) const;

	private:

		QHash<QString, QHash<QString, QString>> M_Arguments;
};

#endif
