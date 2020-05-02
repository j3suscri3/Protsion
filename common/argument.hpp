#ifndef DEF_ARGUMENT
#define DEF_ARGUMENT

#include <string>

class Argument {

	public:

		//Constructor
		Argument();
		//Get the directory
		std::string getDirectory(void) const;
		//Get the configuration filename
		std::string getFileConf(void) const;
		//Get the pid filename
		std::string getFilePid(void) const;
		//Get the log level
                std::string getLevel(void) const;
		//Get the status
		bool getStatus(void) const;
		//Get the username
		std::string getUser(void) const;
		//Parse all argument passed
		void parse(int total, char *table[]);
		//Show the helper
		void showHelp(std::string sh) const;
		//Show the version
		void showVersion(std::string sh) const;

	private:

		std::string m_directory;
		std::string m_fileConf;
		std::string m_filePid;
		std::string m_level;
		bool m_status;
		std::string m_user;

};

#endif
