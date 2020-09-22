CC=g++
CFLAGS=-Wall
coord=Coordinates
dir=Direction
point=Point
main=main

##link
$(main): $(coord).o $(dir).o $(point).o $(main).o
	$(CC) $(CFLAGS) $(coord).o $(dir).o $(point).o $(main).o -o $(main)

##compile
$(coord).o: $(coord).cpp
	$(CC) $(coord).cpp -c

$(dir).o: $(dir).cpp
	$(CC) $(dir).cpp -c

$(point).o: $(point).cpp
	$(CC) $(point).cpp -c

$(main).o: $(main).cpp
	$(CC) $(main).cpp -c


##clear
clean:
	rm -rf *.o $(main)
