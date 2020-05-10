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
CXXDECLARES	=  	-DE_VERSION=\"$(E_VERSION)\" \
			-DE_HASH=\"$(E_HASH)\" \
			-DE_ARCHITECTURE=\"$(E_ARCHITECTURE)\" \
			-DH_NAME=\"$(H_NAME)\" \
			-DH_CATEGORY=\"$(H_CATEGORY)\" \
			-DH_VERSION=\"$(H_VERSION)\" \
			-DH_CODE=\"$(H_CODE)\" \
			-DK_VERSION=\"$(K_VERSION)\" \
			-DC_NAME=\"$(CXX)\" \
			-DC_VERSION=\"$(C_VERSION)\" \
			-DD_NAME=\"$(D_NAME)\" \
                        -DD_VERSION=\"$(D_VERSION)\"
CXXFLAGS	+= 	-std=c++17 \
			-g \
			-O2 \
			-s \
			-pedantic \
			-Wall \
			-I/usr/include/boost
DIRSOURCES	=  	../source/
CXXSOURCES	=  	$(DIRSOURCES)protsion.cpp \
			$(DIRSOURCES)common/argument.cpp \
			$(DIRSOURCES)common/configuration.cpp \
			$(DIRSOURCES)common/database.cpp
CXXRESOURCES	=  	$(wildcard $(DIRSOURCES)database/*.*)
DIREXECUTABLE	=  	../bin/
CXXEXECUTABLE	=  	$(DIREXECUTABLE)protsion
LDFLAGS		+= 	-L/home/mooninfo/sylverant/production/sqlite/lib/
CXXLINKS	+= 	-lboost_system \
			-lboost_filesystem \
			-lstdc++fs \
			-lsqlite3 \
			-lpthread

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

