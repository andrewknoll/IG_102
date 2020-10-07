CC=g++
CFLAGS=-Wall -std=c++11
coord=Coordinates
dir=Direction
point=Point
matrix=Matrix4
oper=Operations
main=main

##link
$(main): $(coord).o $(dir).o $(point).o $(matrix).o $(oper).o $(main).o
	$(CC) $(CFLAGS) $(coord).o $(dir).o $(point).o $(matrix).o $(oper).o $(main).o -o $(main)

##compile
$(coord).o: $(coord).cpp
	$(CC) $(coord).cpp -c

$(dir).o: $(dir).cpp
	$(CC) $(dir).cpp -c

$(point).o: $(point).cpp
	$(CC) $(point).cpp -c

$(matrix).o: $(matrix).cpp
	$(CC) $(matrix).cpp -c

$(oper).o: $(oper).cpp
	$(CC) $(oper).cpp -c

$(main).o: $(main).cpp
	$(CC) $(main).cpp -c


##clear
clean:
	rm -rf *.o $(main)
