#ifndef CPROCESS_HPP
#define CPROCESS_HPP

#include <QHash>
#include <QString>

class CProcess {

        public:

                //Constructor
                CProcess(QHash<QString, QHash<QString, QString>> Configurations);
		//Check the process fileexistance
		bool checking(void) const;
		//Open the process file
                bool creating(void) const;
                //Delete the process file
                bool deleting(void) const;
                //Change the user privileges
                bool changing(void) const;

        private:

                QHash<QString, QString> M_Process;

};

#endif

