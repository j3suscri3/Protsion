#ifndef CCONFIGURATION_HPP
#define CCONFIGURATION_HPP

#include <QHash>
#include <QString>
#include <QThread>

#include "cdatabase.hpp"
#include "cprocess.hpp"

class CConfiguration {

	public:

		//Constructor
		CConfiguration(QHash<QString, QHash<QString, QString>> Arguments);
		//~Destructor
		~CConfiguration();
		//Get the specified configuration category
		QHash<QString, QString> getting(QString Category) const;
		//Load all configurations
		bool loading(CDatabase Database);
		//Unload all configurations
                bool unloading(CDatabase Database);

	private:

		CProcess M_CProcess;
		QHash<QString, QHash<QString, QString>> M_Configurations;
		QThread M_QThread;

};

#endif
