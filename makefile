#
# PROGRAM: ASSIGNMENT #5
# AUTHOR:  Leonart Jaos
#

CXXFLAGS = -Wall -Werror -std=c++14
	
all: z1911688_project5 z1911688_project5_p2

z1911688_project5: z1911688_project5.cpp
	g++ $(CXXFLAGS) -o z1911688_project5 z1911688_project5.cpp -lpthread
	
z1911688_project5_p2: z1911688_project5_p2.cpp
	g++ $(CXXFLAGS) -o z1911688_project5_p2 z1911688_project5_p2.cpp -lpthread
clean:
	rm -f *.o z1911688_project5
	rm -f *.o z1911688_project5_p2