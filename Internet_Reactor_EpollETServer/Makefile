OBJ_1 = Server
OBJ_2 = Client
SRC_1 = ServerMain.cpp
SRC_2 = TcpClient.cc
TARGET = $(OBJ_1) $(OBJ_2)
ALL : $(TARGET)
$(OBJ_1) : $(SRC_1)
	g++ -o $@ $^ -std=c++17 -ljsoncpp
$(OBJ_2) : $(SRC_2)
	g++ -o $@ $^ -std=c++17 -ljsoncpp
.PHONY: clean
clean :
	rm -f $(TARGET)