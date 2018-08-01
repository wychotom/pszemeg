server: main.o eNodeB.o Channel.o
	g++ -g -o server.out main.o eNodeB.o Channel.o

main.o: main.cpp eNodeB.h
	g++ -g -std=c++11 -c main.cpp

eNodeB.o: eNodeB.cpp eNodeB.h Channel.h
	g++ -g -std=c++11 -c eNodeB.cpp

Channel.o: Channel.cpp Channel.h
	g++ -g -std=c++11 -c Channel.cpp

clean:
	rm *.o server.out