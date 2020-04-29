#ifndef DEF_ARGUMENT
#define DEF_ARGUMENT

class Argument {

	public:

	//Parse all argument passed
	void parsing(int total, char *table[]) const;
	//Show the helper
	void showingHelp(const char *sh) const;
	//Show the version
	void showingVersion(const char *sh) const;

};

#endif
