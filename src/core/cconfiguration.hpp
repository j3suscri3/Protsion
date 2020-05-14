#ifndef CCONFIGURATION_HPP
#define CCONFIGURATION_HPP

#include <QHash>
#include <QString>

//#include "cdatabase.hpp"
#include "cprocess.hpp"

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
		CProcess M_CProcess;
		QHash<QString, QHash<QString, QString>> M_Configurations;

};

#endif
