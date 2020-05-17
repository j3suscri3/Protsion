#ifndef CDATABASE_HPP
#define CDATABASE_HPP

#include <QHash>
#include <QString>
#include <QtSql>


class CDatabase {

	public:

		//Constructor
		CDatabase(QHash<QString, QString> Configurations);
		//Desctructor
		~CDatabase();
		//Connection to the specific database
                bool opening(void);
		//Close from the specific database
		bool closing(void);
		//Write a new log entry ot the file
		bool logging(QString Message) const;
		//Write a new log entry to the database
		bool writingLog(QString Level, QString Message) const;

	private:

		QHash<QString, QString> M_Database;
		QSqlDatabase M_Sql;

};

#endif
