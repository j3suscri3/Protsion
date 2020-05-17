#ifndef CPROCESS_HPP
#define CPROCESS_HPP

#include <QHash>
#include <QString>

#include "cdatabase.hpp"

class CProcess {

        public:

                //Constructor
                CProcess(QHash<QString, QString> Configurations);
		//Destructor
		~CProcess();
		//Change the user privileges
                bool changing(CDatabase Database) const;
		//Check the process fileexistance
		bool checking(CDatabase Database);
		//Open the process file
                bool creating(CDatabase Database) const;
                //Delete the process file
                bool deleting(CDatabase Database) const;

        private:

                QHash<QString, QString> M_Process;

};

#endif

