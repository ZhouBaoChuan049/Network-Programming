OBJ1 = Server
OBJ2 = Client
SRC1 = UdpServer.cc
SRC2 = UdpClient.cc
ALL : $(OBJ1) $(OBJ2)
$(OBJ1) : $(SRC1)
	g++ -o $@ $^ -std=c++17 -lpthread
$(OBJ2) : $(SRC2)
	g++ -o $@ $^ -std=c++17 -lpthread
.PHONY:
clean :
	rm -f $(OBJ1) $(OBJ2) ./log/log.txt