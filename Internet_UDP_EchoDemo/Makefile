OBJECT1 = Server 
OBJECT2 = Client
SOURCE1 = Server.cc
SOURCE2 = Client.cc
ALL : $(OBJECT1) $(OBJECT2)
$(OBJECT1) : $(SOURCE1)
	g++ -o $@ $^ -std=c++17
$(OBJECT2) : $(SOURCE2)
	g++ -o $@ $^ -std=c++17
.PHONY :
clean :
	rm -f $(OBJECT1) $(OBJECT2)