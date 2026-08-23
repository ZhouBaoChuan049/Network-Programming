OBJ1 = Server
OBJ2 = Client
SRC1 = Server.cc
SRC2 = Client.cc
ALL : $(OBJ1) $(OBJ2)
$(OBJ1) : $(SRC1)
	g++ -o $@ $^ -std=c++17
$(OBJ2) : $(SRC2)
	g++ -o $@ $^ -std=c++17
.PHONY:
clean : 
	rm -f $(OBJ1) $(OBJ2)