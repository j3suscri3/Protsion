#ifndef CCONFIGURATION_HPP
#define CCONFIGURATION_HPP

#include <QHash>
#include <QString>

//#include "cdatabase.hpp"

class CConfiguration {

	public:

		//Constructor
		CConfiguration(QHash<QString, QHash<QString, QString>> Arguments);
		//Load all configurations
		bool loading(void) const;
		//Unload all configurations
                bool unloading(void) const;

	private:

		//CDatabase M_CDatabase;
		QHash<QString, QHash<QString, QString>> M_Configurations;

		//Open the process file
                bool createProcessFile(void) const;
		//Delete the process file
                bool deleteProcessFile(void) const;
		//Drop the user privileges
		bool dropUserPrivileges(void) const;

};

#endif
