CC=g++
CFLAGS=-Wall -std=c++11 -ggdb
img=Image
rgb=RGB
main=main
clamp=Clamp
equ=Equalize
equclamp=EqualizeAndClamp
gamma=Gamma
cgamma=GammaAndClamp
mainClamp=clamp_main
mainEqu=equalize_main
mainEquC=equalizeAndClam_main
mainGam=gamma_main
mainGamC=gammaAndClamp_main
toneMap=ToneMapper

##link
$(clamp): $(rgb).o $(img).o $(clamp).o $(mainClamp).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o $(clamp).o $(mainClamp).o -o $(clamp)

$(equ): $(rgb).o $(img).o $(equ).o $(mainEqu).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o $(equ).o $(mainEqu).o -o $(equ)

$(equclamp): $(rgb).o $(img).o $(equclamp).o $(equ).o $(clamp).o $(mainEquC).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o $(equclamp).o $(equ).o $(clamp).o $(mainEquC).o -o $(equclamp)

$(gamma): $(rgb).o $(img).o $(gamma).o $(mainGam).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o $(gamma).o $(mainGam).o -o $(gamma)

$(cgamma): $(rgb).o $(img).o $(cgamma).o $(equclamp).o $(equ).o $(clamp).o $(gamma).o $(mainGamC).o
	$(CC) $(CFLAGS) $(rgb).o $(img).o $(cgamma).o $(equclamp).o $(equ).o $(clamp).o $(gamma).o $(mainGamC).o -o $(cgamma)

##compile
$(rgb).o: $(rgb).cpp $(rgb).hpp
	$(CC) $(CFLAGS) $(rgb).cpp -c

$(img).o: $(img).cpp $(img).hpp
	$(CC) $(CFLAGS) $(img).cpp -c

all: $(clamp) $(equ) $(equclamp) $(gamma) $(cgamma)

$(mainClamp).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -DCLAMP -c -o $(mainClamp).o

$(mainEqu).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -DEQUALIZE -c -o $(mainEqu).o

$(mainEquC).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -DEQUALIZECLAMP -c -o $(mainEquC).o

$(mainGam).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -DGAMMA -c -o $(mainGam).o

$(mainGamC).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -DCLAMPGAMMA -c -o $(mainGamC).o

$(clamp).o: $(clamp).cpp $(clamp).hpp $(toneMap).hpp
	$(CC) $(CFLAGS) $(clamp).cpp -DCLAMP -c -o $(clamp).o

$(equ).o: $(equ).cpp $(equ).hpp $(toneMap).hpp
	$(CC) $(CFLAGS) $(equ).cpp -DEQUALIZE -c -o $(equ).o

$(equclamp).o: $(equclamp).cpp $(equclamp).hpp $(toneMap).hpp
	$(CC) $(CFLAGS) $(equclamp).cpp -DEQUALIZECLAMP -c -o $(equclamp).o

$(gamma).o: $(gamma).cpp $(gamma).hpp $(toneMap).hpp
	$(CC) $(CFLAGS) $(gamma).cpp -DGAMMA -c -o $(gamma).o

$(cgamma).o: $(cgamma).cpp $(cgamma).hpp $(toneMap).hpp
	$(CC) $(CFLAGS) $(cgamma).cpp -DCLAMPGAMMA -c -o $(cgamma).o


##clear
clean:
	rm -rf *.o $(clamp) $(equ) $(equclamp) $(gamma) $(cgamma)
