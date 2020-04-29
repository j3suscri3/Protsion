#include "configuration.h"

#include <string>
#include <limits.h>
#include <unistd.h>

using namespace std;

Configuration::Configuration(std::string directory, std::string fileConf, std::string filePid, std::string level, bool status, std::string user) : m_directory(""), m_fileConf("protsion.conf"), m_filePid("protsion.pid"),  m_level("i"), m_status(true), m_user("protsion") {

	if (!directory.empty()) {
                this->m_directory = directory;
        }
	else {
		//Get the executable full path name
		char 	result[PATH_MAX];
        	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        	this->m_directory = string(result, (count > 0) ? count : 0);
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

bool Configuration::createPid() const {
	/* Attempt to open and lock the pid file */
        if (!this->m_filePid) {
        	this->m_filePid = this->m_directory + "run/" + this->m_filePid;
		cout << this->m_filePid << endl;
	}
/*
	pf = pidfile_open(pidfile_name, 0660, &op);

        if(!pf) {
            if(errno == EEXIST) {
                debug(DBG_ERROR, "Shipgate already running? (pid: %ld)\n",
                      (long)op);
                exit(EXIT_FAILURE);
            }

            debug(DBG_WARN, "Cannot create pidfile: %s!\n", strerror(errno));
        }

        if(daemon(1, 0)) {
            debug(DBG_ERROR, "Cannot daemonize\n");
            perror("daemon");
            exit(EXIT_FAILURE);
        }

        if(drop_privs())
            exit(EXIT_FAILURE);

        open_log();

        // Write the pid file.
        pidfile_write(pf);
*/
}

bool Configuration::deletePid() const {
/*
	struct passwd *pw;
    uid_t uid;
    gid_t gid;
    int gid_count = 0;
    gid_t *groups;

    // Make sure we're actually root, otherwise some of this will fail.
    if(getuid() && geteuid())
        return 0;

    // Look for users. We're looking for the user "sylverant", generally.
    if((pw = getpwnam(runas_user))) {
        uid = pw->pw_uid;
        gid = pw->pw_gid;
    }
    else {
        debug(DBG_ERROR, "Cannot find user \"%s\". Bailing out!\n", runas_user);
        return -1;
    }

    // Change the pidfile's uid/gid now, before we drop privileges...
    if(pf) {
        if(fchown(pidfile_fileno(pf), uid, gid)) {
            debug(DBG_WARN, "Cannot change pidfile owner: %s\n",
                  strerror(errno));
        }
    }

#ifdef HAVE_GETGROUPLIST
    // Figure out what other groups the user is in...
    getgrouplist(runas_user, gid, NULL, &gid_count);
    if(!(groups = malloc(gid_count * sizeof(gid_t)))) {
        perror("malloc");
        return -1;
    }

    if(getgrouplist(runas_user, gid, groups, &gid_count)) {
        perror("getgrouplist");
        free(groups);
        return -1;
    }

    if(setgroups(gid_count, groups)) {
        perror("setgroups");
        free(groups);
        return -1;
    }

    //We're done with most of these, so clear this out now...
    free(groups);
#else
    if(setgroups(1, &gid)) {
        perror("setgroups");
        return -1;
    }
#endif

    if(setgid(gid)) {
        perror("setgid");
        return -1;
    }

    if(setuid(uid)) {
        perror("setuid");
        return -1;
    }

    // Make sure the privileges stick.
    if(!getuid() || !geteuid()) {
        debug(DBG_ERROR, "Cannot set non-root privileges. Bailing out!\n");
        return -1;
    }

    return 0;
}
*/
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

void Configuration::loadConfiguration() const [

	//Check if we need to deamonize the executable
	if(this->m_status) {
		if(!this->createPid())
			exit(EXIT_FAILURE);
	}
	else {
		if(!this->deletePid())
			exit(EXIT_FAILURE);
	}
}
