CC=g++
CFLAGS=-Wall -std=c++11
img=Image
rgb=RGB
main=toneMappers
clamp=clamp
equ=equalize
equclamp=equalizeAndClamp
gamma=gamma
cgamma=clampAndGamma

##link
$(clamp): $(rgb).o $(img).o $(clamp).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o  $(clamp).o -o $(clamp)

$(equ): $(rgb).o $(img).o $(equ).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o  $(equ).o -o $(equ)

$(equclamp): $(rgb).o $(img).o $(equclamp).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o  $(equclamp).o -o $(equclamp)

$(gamma): $(rgb).o $(img).o $(gamma).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o  $(gamma).o -o $(gamma)

$(cgamma): $(rgb).o $(img).o $(cgamma).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o  $(cgamma).o -o $(cgamma)

##compile
$(rgb).o: $(rgb).cpp
	$(CC) $(rgb).cpp -c

$(img).o: $(img).cpp
	$(CC) $(img).cpp -c

all: $(clamp) $(equ) $(equclamp) $(gamma) $(cgamma)

$(clamp).o: $(main).cpp
	$(CC) $(main).cpp -DCLAMP -c -o $(clamp).o

$(equ).o: $(main).cpp
	$(CC) $(main).cpp -DEQUALIZE -c -o $(equ).o

$(equclamp).o: $(main).cpp
	$(CC) $(main).cpp -DEQUALIZECLAMP -c -o $(equclamp).o

$(gamma).o: $(main).cpp
	$(CC) $(main).cpp -DGAMMA -c -o $(gamma).o

$(cgamma).o: $(main).cpp
	$(CC) $(main).cpp -DCLAMPGAMMA -c -o $(cgamma).o


##clear
clean:
	rm -rf *.o $(clamp) $(equ) $(equclamp) $(gamma) $(cgamma)
