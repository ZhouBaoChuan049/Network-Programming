ALL : Server Client
Server :Server.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp
Client : TcpClient.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp
.PHONY:
clean:
	rm -f Server Client 