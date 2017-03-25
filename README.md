# x3djump
Sample 3D program

up key: to go forward  
down key: to go backward  
left key: to turn left  
right key: to turn right  
space key: jump  


## build
(work on centos7)  
sudo yum install SDL-devel freeglut  
cd /var/tmp  
git clone git@github.com:tnaganawa/x3djump.git  
cd x3djump  
gcc $(sdl-config --cflags --libs) -lGL -lGLU -lm x3djump.c  
./a.out  
