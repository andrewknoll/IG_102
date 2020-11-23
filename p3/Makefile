CC=g++
CFLAGS=-Wall -std=c++11 -ggdb
img=Image
imgDir=Images
colorDir=Color
rgb=RGB
main=test
shapeDir=Shapes
plane=Plane
shape=Shape
sphere=Sphere
vfieldDir=vfield
coord=Coordinates
dir=Direction
point=Point
matrix=Matrix4
oper=Operations
scene=Scene
camera=Camera
ray=rayTracer
equClamp=EqualizeAndClamp

##link
$(main):  $(dir).o $(point).o $(coord).o $(matrix).o $(img).o $(plane).o $(sphere).o $(scene).o $(shape).o $(camera).o $(ray).o $(rgb).o $(oper).o $(main).o
	$(CC) $(CFLAGS) $(dir).o $(point).o $(coord).o $(matrix).o $(img).o $(plane).o $(shape).o $(sphere).o $(scene).o $(camera).o $(ray).o $(rgb).o $(oper).o $(main).o -o $(main)

##compile

$(rgb).o: $(colorDir)/$(rgb).cpp $(colorDir)/$(rgb).hpp
	$(CC) $(CFLAGS) $(colorDir)/$(rgb).cpp -c

$(img).o: $(imgDir)/$(img).cpp $(imgDir)/$(img).hpp
	$(CC) $(CFLAGS) $(imgDir)/$(img).cpp -c

$(plane).o: $(shapeDir)/$(plane).cpp
	$(CC) $(CFLAGS) $(shapeDir)/$(plane).cpp -c

$(shape).o: $(shapeDir)/$(shape).cpp
	$(CC) $(CFLAGS) $(shapeDir)/$(shape).cpp -c

$(sphere).o: $(shapeDir)/$(sphere).cpp
	$(CC) $(CFLAGS) $(shapeDir)/$(sphere).cpp -c

$(scene).o: $(scene).cpp
	$(CC) $(CFLAGS) $(scene).cpp -c

$(camera).o: $(camera).cpp
	$(CC) $(CFLAGS) $(camera).cpp -c

$(ray).o: $(ray).cpp
	$(CC) $(CFLAGS) $(ray).cpp -c

$(coord).o: $(vfieldDir)/$(coord).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(coord).cpp -c

$(dir).o: $(vfieldDir)/$(dir).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(dir).cpp -c

$(point).o: $(vfieldDir)/$(point).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(point).cpp -c

$(matrix).o: $(vfieldDir)/$(matrix).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(matrix).cpp -c

$(oper).o: $(vfieldDir)/$(oper).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(oper).cpp -c

$(main).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -c


##clear
clean:
	rm -rf *.o $(plane) $(shape) $(sphere) $(gamma) $(cgamma)
