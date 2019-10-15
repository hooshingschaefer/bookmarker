objs = serversimple.o protocol.o respond.o debug.o users.o db.o cookies.o 
../dist/serverbin: $(objs)
	g++ -o ../dist/serverbin -pthread $(objs) -L/usr/lib/x86_64-linux-gnu -lpq -lcurl 
serversimple.o: protocol.h debug.h respond.h users.h serversimple.cpp
	g++ -c -I/usr/include/postgresql serversimple.cpp -Wall
protocol.o: debug.h 
respond.o: protocol.h debug.h
debug.o: debug.h
users.o: users.h
db.o: debug.h	
	g++ -c -I/usr/include/postgresql db.cpp -Wall
cookies.o: protocol.h
clean:
	rm $(objs)	
