CC=g++
CFLAGS=-Wall -ggdb -std=c++11
img=Image
imgDir=imaging
colorDir=color
rgb=RGB
main=test
shapeDir=shapes
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
pathTr=pathTracer
ray=Ray
material=Material
matDir=materials
rng=rng
clamp=Clamp
equClamp=EqualizeAndClamp
cgamma=GammaAndClamp
iw=ImageWriter
lp=LightPoint
al=AreaLight

##link
$(main):  $(dir).o $(point).o $(coord).o $(matrix).o $(img).o $(plane).o $(sphere).o $(scene).o $(shape).o $(camera).o $(ray).o $(rgb).o $(oper).o $(material).o $(pathTr).o $(lp).o $(al).o $(iw).o $(clamp).o $(equClamp).o $(cgamma).o $(main).o
	$(CC) $(CFLAGS) $(dir).o $(point).o $(coord).o $(matrix).o $(img).o $(plane).o $(shape).o $(sphere).o $(scene).o $(camera).o $(ray).o $(rgb).o $(oper).o $(material).o $(pathTr).o $(lp).o $(al).o $(iw).o $(clamp).o $(equClamp).o $(cgamma).o $(main).o -o $(main)

##compile
$(iw).o: $(colorDir)/$(iw).cpp
	$(CC) $(CFLAGS) $(colorDir)/$(iw).cpp -c 

$(clamp).o: $(colorDir)/$(clamp).cpp
	$(CC) $(CFLAGS) $(colorDir)/$(clamp).cpp -c 

$(equClamp).o: $(colorDir)/$(equClamp).cpp
	$(CC) $(CFLAGS) $(colorDir)/$(equClamp).cpp -c 

$(cgamma).o: $(colorDir)/$(cgamma).cpp
	$(CC) $(CFLAGS) $(colorDir)/$(cgamma).cpp -c 

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

$(material).o: $(matDir)/$(material).cpp
	$(CC) $(CFLAGS) $(matDir)/$(material).cpp -c 

$(scene).o: $(scene).cpp
	$(CC) $(CFLAGS) $(scene).cpp -c 

$(camera).o: $(camera).cpp
	$(CC) $(CFLAGS) $(camera).cpp -c 

$(ray).o: $(ray).cpp
	$(CC) $(CFLAGS) $(ray).cpp -c 

$(lp).o: $(lp).cpp
	$(CC) $(CFLAGS) $(lp).cpp -c 

$(al).o: $(al).cpp
	$(CC) $(CFLAGS) $(al).cpp -c 

$(coord).o: $(vfieldDir)/$(coord).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(coord).cpp -c 

$(dir).o: $(vfieldDir)/$(dir).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(dir).cpp -c 

$(point).o: $(vfieldDir)/$(point).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(point).cpp -c 

$(matrix).o: $(vfieldDir)/$(matrix).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(matrix).cpp -c 

$(pathTr).o: $(pathTr).cpp
	$(CC) $(CFLAGS) $(pathTr).cpp -c 

$(oper).o: $(vfieldDir)/$(oper).cpp
	$(CC) $(CFLAGS) $(vfieldDir)/$(oper).cpp -c 

$(main).o: $(main).cpp
	$(CC) $(CFLAGS) $(main).cpp -c 


##clear
clean:
	rm -rf *.o $(plane) $(shape) $(sphere) $(gamma) $(cgamma)
