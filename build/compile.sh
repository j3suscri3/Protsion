#!/bin/sh

#BINARY 	   = Protsion
#LINKS	   = -lboost_system -lboost_filesystem -lstdc++fs -lsqlite3
#OBJECTS	   = protsion.cpp common/argument.cpp common/configuration.cpp common/database.cpp
#COMPILATOR = g++ -std=c++17 -g -O2 -s -pedantic -Wall
#FLAGS	   = -I/usr/include/boost -L/home/mooninfo/sylverant/production/sqlite/lib/

#build: $(OBJECTS)
#	$(COMPILATOR) $(FLAGS) $(OBJECTS) -o $(BINARY) $(LINKS)

#clean:
#	rm -rf *.o $(BINARY)

clear &&
g++ -std=c++17 -g -O2 -s -pedantic -Wall -Wextra -I/usr/include/boost -L/home/mooninfo/sylverant/production/sqlite/lib/ protsion.cpp common/argument.cpp common/configuration.cpp common/database.cpp -o Protsion -lboost_system -lboost_filesystem -lstdc++fs -lsqlite3

