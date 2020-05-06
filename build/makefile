CXX		=  g++
CXXFLAGS	+= -std=c++17 -g -O2 -s -pedantic -Wall -lboost_system -lboost_filesystem -lstdc++fs -lsqlite3 -lpthread -I/usr/include/boost
DIRSOURCES	=  ../source/
CXXSOURCES	=  $(DIRSOURCES)protsion.cpp $(DIRSOURCES)common/argument.cpp $(DIRSOURCES)common/configuration.cpp $(DIRSOURCES)common/database.cpp
CXXRESOURCES	= $(wildcard $(DIRSOURCES)database/*.*)
DIREXECUTABLE	=  ../bin/
CXXEXECUTABLE	=  $(DIREXECUTABLE)protsion
LDFLAGS		+= -L/home/mooninfo/sylverant/production/sqlite/lib/


all: protsion

release:
	mkdir -p $(DIREXECUTABLE)database/; \
	for CXXRESOURCE in $(CXXRESOURCES); do \
		cp -f $$CXXRESOURCE $(DIREXECUTABLE)database/; \
	done

protsion: release
	$(CXX) $(CXXFLAG) $(LDFLAGS) $(CXXSOURCES) -o $(CXXEXECUTABLE)

.PHONY: clean
clean:
	rm -rf $(DIREXECUTABLE)*.o; \
	rm -rf $(DIREXECUTABLE)protsion;

distclean:
	rm -rf $(DIREXECUTABLE)
