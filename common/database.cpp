#include <string>

#include "database.hpp"

#include "../extra/sqlite3.h"

using namespace std;

Database::Database(string host, string database) : m_host(""), m_database("protsion.db") {

	//Check if the database host isn't empty
	if (!host.empty())
		this->m_host = host;

	//Check the database name
	if (!database.empty())
		this->m_database = database;

}

bool Database::openDatabase(void) const {

	sqlite3 *database(0);
	string uri = "file:" + this->m_host + "/db/" + this->m_database + "?mode=memory&cache=private&nolock=false&vfs=unix-dotfile";

	//Open a connection to the specific database
	if(!sqlite3_open16(uri.c_str(), &database)) {

		sqlite3_close(database);
		return false;
	}

	return true;

}
