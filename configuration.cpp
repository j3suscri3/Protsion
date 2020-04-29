#include "configuration.h"

#include <string>

using namespace std;

Configuration::Configuration(std::string directory, std::string fileConf, std::string filePid, std::string level, bool status, std::string user) : m_directory(""), m_fileConf("protsion.conf"), m_filePid("protsion.pid"),  m_level("i"), m_status(true), m_user("protsion") {

	if (!directory.empty()) {
                this->m_directory = directory;
        }

	if (!fileConf.empty()) {
		if (fileConf.substr(fileConf.find_last_of(".") + 1) == "conf") {
			this->m_fileConf = fileConf;
		}
		else {
			this->m_fileConf = fileConf + ".conf";
		}
	}

	if (!filePid.empty()) {
		if (filePid.substr(filePid.find_last_of(".") + 1) == "pid") {
                	this->m_filePid = filePid;
                }
                else {
                        this->m_filePid = filePid + ".pid";
                }
        }

	if (!level.empty()) {
                this->m_level = level;
	}

	if (!status) {
                this->m_status = status;
        }

	if (!user.empty()) {
                this->m_user = user;
        }

}

string Configuration::getDirectory() const {
        return this->m_directory;
}

string Configuration::getFileConf() const {
	return this->m_fileConf;
}

string Configuration::getFilePid() const {
        return this->m_filePid;
}

string Configuration::getLevel() const {
	return this->m_level;
}

bool Configuration::getStatus() const {
	return this->m_status;
}

string Configuration::getUser() const {
	return this->m_user;
}
