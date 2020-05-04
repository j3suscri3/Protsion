#include <boost/algorithm/string/predicate.hpp>
#include <codecvt>
#include <ctime>
#include <boost/filesystem.hpp>
#include <filesystem>
	#include <iostream>
#include <regex>
#include <string>

#include "database.hpp"

#include "../extra/sqlite3.h"

using namespace boost::filesystem;
using namespace boost::system;
using namespace std;

Database::Database(string database, string directory, string level) : m_database("protsion.db"), m_user("protsion"), m_password("protsion"), m_directory(""), m_level("info"),  m_sqlite(0) {

	//Check the database name
	regex r_database(":(.*)$");
	regex r_user("^(.*):|@(.*)$");
	regex r_password("^(.*)@");

	if (!database.empty()) {

		//Get the database name
		this->m_database = regex_replace(database, r_database, "$3") + ".db";

		//Get the database username
                this->m_user = regex_replace(database, r_user, "$3");

		//Get the database password
                this->m_password = regex_replace(database, r_password, "$3");

	}

	//Rename the database file
        directory = canonical(current_path()).string() + "/database/";
        for (const auto & i_file : std::filesystem::directory_iterator(directory)) {

		//If the database name isn't already exists
		if (!boost::algorithm::ends_with(canonical(i_file.path()).string(), this->m_database))
                	//When the current database file has found
                	if (boost::algorithm::ends_with(canonical(i_file.path()).string(), ".db"))
                        	//Rename it with the new name
                        	std::filesystem::rename(i_file.path(), directory + this->m_database);

        }

	path p_directory;
	boost::system::error_code error;

	//Check the host name
	if (!directory.empty()) {

		this->m_directory = directory;
                p_directory = this->m_directory;
		//Move to the specified directory
                current_path(this->m_directory, error);

                if (!exists(p_directory)) {

                        if (!create_directories(this->m_directory))
                                exit(EXIT_FAILURE);
                        current_path(this->m_directory, error);

                }

        } else
		this->m_directory = canonical(current_path()).string();

	//Check the log level
        if (!level.empty())
                this->m_level = level;

}

bool Database::openDatabase(void) {

	string uri = "file:///" + this->m_directory + this->m_database + "?nolock=false&vfs=unix-dotfile";

	//Open a connection to the specific database
	if(sqlite3_open_v2(uri.c_str(), &m_sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_PRIVATECACHE | SQLITE_OPEN_NOFOLLOW, NULL) != SQLITE_OK) {

		//printf("Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
		closeDatabase();

		return false;

	}

	writeDatabase_Log(this->m_level, "Connection to the database " + this->m_database +  " with success!");

	return true;

}

bool Database::closeDatabase(void) {

	if(sqlite3_close(this->m_sqlite) != SQLITE_OK)
		return false;
	else
		return true;

}

bool Database::logDatabase(string message) const {

	//Define the current date
	time_t t_datetime = time(nullptr);
	char s_datetime[100];
	strftime(s_datetime, sizeof(s_datetime), "%A %c", localtime(&t_datetime));

	cout << "TocToc" << endl;

	std::ofstream log;
	//Open the offline log file
	log.open(this->m_directory + "protsion_" + s_datetime + ".log", std::fstream::in|std::fstream::app);
	if(log.fail()) {

		cout << "Can't open the offline log file!" << endl;
		return false;

	}

	//Write the error into the offline log file
	log << message;

	//Close the offline log file
	log.close();
	if(log.fail()) {

                cout << "Can't close the offline log file!" << endl;
                return false;

        }

	return true;

}

bool Database::writeDatabase_Log(string level, string message) const {

	char *error = NULL;
	std::string request = "INSERT INTO log VALUES (DATETIME('now', 'localtime'), '" + level + "', '" + message + "');";
	int 	    result = 0;

	//If the sql request has an error
	result = sqlite3_exec(this->m_sqlite, request.c_str(), NULL, 0, &error);

	//cout << "result : " << result << endl;
	//cout << "error : " << sqlite3_errmsg(this->m_sqlite) << endl;

	//Check if the log level for saving
	if((level == "warning" && this->m_level == "error") || (level == "info" && this->m_level != "info"))
		return true;

	//If we are an error with then we need be the memory free
	if (result && error != NULL){

		cout << "result : " << result << endl;
        	cout << "error : " << sqlite3_errmsg(this->m_sqlite) << endl;

		sqlite3_free(error);
		error = NULL;
		return false;

	}

	return true;

}
