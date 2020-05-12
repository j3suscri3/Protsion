#include <boost/algorithm/string/predicate.hpp>
#include <codecvt>
#include <ctime>
#include <boost/filesystem.hpp>
#include <filesystem>
	#include <iostream>
#include <regex>
#include <string>

#include <QString>

#include "cdatabase.hpp"

#include "../extra/sqlite3.h"

using namespace boost::filesystem;
using namespace boost::system;
using namespace std;

CDatabase::CDatabase(QString database, QString username, QString password, QString directory, QString level) : M_CDatabase(database), M_CUsername(username), M_CPassword(password), M_CDirectory(directory), M_CLevel(level),  M_CSqlite(0) {
/*
	//Get the database name
	this->M_CDatabaseName = databaseName;
	//Get the database username
	this->M_CUsername = databaseUsername;
	//Get the database password
        this->M_CPassword = databasePassword;
*/
	//Rename the database file
        directory = canonical(current_path()).string() + "/database/";
        for (const auto & i_file : std::filesystem::directory_iterator(directory)) {

		//If the database name isn't already exists
		if (!boost::algorithm::ends_with(canonical(i_file.path()).string(), this->M_CDatabase))
                	//When the current database file has found
                	if (boost::algorithm::ends_with(canonical(i_file.path()).string(), ".db"))
                        	//Rename it with the new name
                        	std::filesystem::rename(i_file.path(), directory + this->M_CDatabase);

        }

	path p_directory;
	boost::system::error_code error;

	//Check the host name
	if (!directory.empty()) {

		this->M_CDirectory = directory;
                p_directory = this->M_CDirectory;
		//Move to the specified directory
                current_path(this->M_CDirectory, error);

                if (!exists(p_directory)) {

                        if (!create_directories(this->M_CDirectory))
                                exit(EXIT_FAILURE);
                        current_path(this->M_CDirectory, error);

                }

        } else
		this->M_CDirectory = canonical(current_path()).string();

	//Check the log level
        if (!level.empty())
                this->M_CLevel = level;

}

bool CDatabase::open(void) {

	string uri = "file:///" + this->M_CDirectory + this->M_CDatabase + "?nolock=false&vfs=unix-dotfile";

	//Open a connection to the specific database
	if(sqlite3_open_v2(uri.c_str(), &M_CSqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_PRIVATECACHE | SQLITE_OPEN_NOFOLLOW, NULL) != SQLITE_OK) {

		string message =  sqlite3_errmsg(this->M_CSqlite);
		this->log("Can't open the database (" + message + ")");
		this->close();

		return false;

	}

	this->writeLog(this->M_CLevel, "Connection to the database " + this->M_CDatabase +  " with success!");

	return true;

}

bool CDatabase::close(void) {

	if(sqlite3_close(this->M_CSqlite) != SQLITE_OK) {

		string message = sqlite3_errmsg(this->M_CSqlite);
		this->log("Can't close the database (" + message + ")");
		return false;

	} else
		return true;

}

bool CDatabase::log(QString message) const {

	//Define the current date
	time_t t_datetime = time(nullptr);
	char s_datetime[100];
	strftime(s_datetime, sizeof(s_datetime), "%Y%m%d", localtime(&t_datetime));

	std::ofstream log;
	//Open the offline log file
	log.open(this->M_CDirectory + "protsion_" + s_datetime + ".log", std::ofstream::out|std::ofstream::app);
	if(log.fail()) {

		cout << "Can't open the offline log file!" << endl;
		return false;

	}

	//Write the error into the offline log file
	strftime(s_datetime, sizeof(s_datetime), "%Y-%m-%d %H:%M:%S", localtime(&t_datetime));
	log << "[" << s_datetime << "] " << message << "\n";

	//Close the offline log file
	log.close();
	if(log.fail()) {

                cout << "Can't close the offline log file!" << endl;
                return false;

        }

	return true;

}

bool CDatabase::writeLog(QString level, QString message) const {

	char *error = nullptr;
	std::string request = "INSERT INTO log VALUES (DATETIME('now', 'localtime'), '" + level + "', '" + message + "');";
	int 	    result = 0;

	//If the sql request has an error
	result = sqlite3_exec(this->M_CSqlite, request.c_str(), NULL, 0, &error);

	//Check if the log level for saving
	if((level == "warning" && this->M_CLevel == "error") || (level == "info" && this->M_CLevel != "info"))
		return true;

	//If we are an error with then we need be the memory free
	if (result && error != NULL){

		string message = error;
		this->log("Can't write a log to the database (" + message + ")");

		sqlite3_free(error);
		error = NULL;
		return false;

	}

	return true;

}
