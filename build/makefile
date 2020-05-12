E_VERSION	=	$(shell git log -1 | tr -d '\n' | sed -Ee "s/^(.*) //g")
E_HASH		= 	$(shell git log -1 | tr -d '\n' | sed -Ee "s/^\w{1,} |Author(.*)//g")
E_ARCHITECTURE 	=       $(shell g++ -print-multiarch)
H_NAME		= 	$(shell lsb_release -i | sed -Ee "s/^(.*)\t//g")
H_CATEGORY	= 	$(shell uname -o | sed -e "s/\//-/")
H_VERSION	= 	$(shell cat /etc/debian_version)
H_CODE		=	$(shell lsb_release -c | sed -Ee "s/^(.*)\t//g")
K_VERSION	=	$(shell uname -r)
C_VERSION	=	$(shell g++ --version | sed -Ee "s/(.*) //gm" | tr -d '\n' | sed -Ee "s/[^0-9.]{1,}\w{1,}[.]//g")
D_NAME		=	sqlite
D_VERSION	= 	$(shell sed -ne '/define SQLITE_VERSION / s/^.* "//p' $(DIRSOURCES)extra/sqlite3.h | tr -d '"')

CXX		=  	g++
CXXDECLARES	=  	-DC_EXECUTABLEVERSION=\"$(E_VERSION)\" \
			-DC_EXECUTABLEHASH=\"$(E_HASH)\" \
			-DC_EXECUTABLEARCHITECTURE=\"$(E_ARCHITECTURE)\" \
			-DC_HOSTNAME=\"$(H_NAME)\" \
			-DC_HOSTCATEGORY=\"$(H_CATEGORY)\" \
			-DC_HOSTVERSION=\"$(H_VERSION)\" \
			-DC_HOSTCODE=\"$(H_CODE)\" \
			-DC_COMPILATORNAME=\"$(CXX)\" \
			-DC_COMPILATORVERSION=\"$(C_VERSION)\" \
			-DC_DATABASENAME=\"$(D_NAME)\" \
                        -DC_DATABASEVERSION=\"$(D_VERSION)\"
CXXFLAGS	+= 	-std=c++17 \
			-g \
			-O2 \
			-s \
			-pedantic \
			-Wall \
			-fPIC \
			-I/usr/include/boost \
			-I/usr/include/x86_64-linux-gnu/qt5/ \
			-I/usr/include/x86_64-linux-gnu/qt5/QtCore/
DIRSOURCES	=  	../src/
CXXSOURCES	=  	$(DIRSOURCES)protsion.cpp \
			$(DIRSOURCES)core/cargument.cpp 
#\
#			$(DIRSOURCES)core/cconfiguration.cpp \
#			$(DIRSOURCES)core/cdatabase.cpp
DIRRESOURCES	=	$(DIRSOURCES)res/
CXXRESOURCES	=  	$(wildcard $(DIRRESOURCES)*.*)
DIREXECUTABLE	=  	../bin/
CXXEXECUTABLE	=  	$(DIREXECUTABLE)protsion
LDFLAGS		+= 	-L/home/mooninfo/sylverant/production/sqlite/lib/
CXXLINKS	+= 	-lboost_system \
			-lboost_filesystem \
			-lstdc++fs \
			-lsqlite3 \
			-lpthread \
			-lQt5Core \

all: protsion

release:
	mkdir -p $(DIREXECUTABLE)database/; \
	for CXXRESOURCE in $(CXXRESOURCES); do cp -f $$CXXRESOURCE $(DIREXECUTABLE)database/; done

protsion: release
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CXXDECLARES) $(CXXSOURCES) -o $(CXXEXECUTABLE) $(CXXLINKS)

.PHONY: distclean protsion
clean:
	rm -rf $(DIREXECUTABLE)*.o; \
	rm -rf $(DIREXECUTABLE)protsion;

distclean:
	rm -rf $(DIREXECUTABLE)

