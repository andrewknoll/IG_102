CC=g++
CFLAGS=-Wall -std=c++11
img=Image
rgb=RGB
main=main

##link
$(main): $(rgb).o $(img).o $(main).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o  $(main).o -o $(main)

##compile
$(rgb).o: $(rgb).cpp
	$(CC) $(rgb).cpp -c

$(img).o: $(img).cpp
	$(CC) $(img).cpp -c

$(main).o: $(main).cpp
	$(CC) $(main).cpp -c


##clear
clean:
	rm -rf *.o $(main)
