HOSTDECLARE		:= #$(shell lsb_release -i) $(shell cat /etc/debian_version)
CXXDECLARE		:= #$(shell g++ --version)
DBDECLARE		:= #
CXX			=  g++
CXXDECLARES		=  -DC_HOST="$(HOSTDECLARE)" -DC_COMPILATOR=\"$(COMPILATORDECLARE)\"
CXXFLAGS		+= -std=c++17 -g -O2 -s -pedantic -Wall -I/usr/include/boost
DIRSOURCES		=  ../source/
CXXSOURCES		=  $(DIRSOURCES)protsion.cpp $(DIRSOURCES)common/argument.cpp $(DIRSOURCES)common/configuration.cpp $(DIRSOURCES)common/database.cpp
CXXRESOURCES		= $(wildcard $(DIRSOURCES)database/*.*)
DIREXECUTABLE		=  ../bin/
CXXEXECUTABLE		=  $(DIREXECUTABLE)protsion
LDFLAGS			+= -L/home/mooninfo/sylverant/production/sqlite/lib/
CXXLINKS		+=  -lboost_system -lboost_filesystem -lstdc++fs -lsqlite3 -lpthread

all: protsion

release:
	mkdir -p $(DIREXECUTABLE)database/; \
	for CXXRESOURCE in $(CXXRESOURCES); do \
		cp -f $$CXXRESOURCE $(DIREXECUTABLE)database/; \
	done

protsion: release
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(CXXDECLARES) $(CXXSOURCES) -o $(CXXEXECUTABLE) $(CXXLINKS)

.PHONY: clean
clean:
	rm -rf $(DIREXECUTABLE)*.o; \
	rm -rf $(DIREXECUTABLE)protsion;

distclean:
	rm -rf $(DIREXECUTABLE)

