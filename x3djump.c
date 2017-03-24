
#include <math.h>
#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define N_PLANES 5 // number of planes
#define SIZE_OF_PLANE 3.5
double planes [N_PLANES][3] = {{0,0,2},{4,0,4},{0,0,2},{5,0,7},{-5,0,10}}; // where planes are
double m = 0.1; // speed of turn
double a = 0.2; // speed to walk
double b = 0.1; // speed to fall
double g = 0.1; // gravity
double j = 3.0; // speed to jump
int up_key_status = 0;
int down_key_status = 0;
int left_key_status = 0;
int right_key_status = 0;
int space_key_status = 0;
SDL_Event e;

typedef struct {
  double x,y,z,dx,dy,dz, vx,vy,vz;
} camera;
camera c;


static void init_camera(){
  c.x = 10;
  c.y = 0;
  c.z = 1;
  c.dx = -1;
  c.dy = 0;
  c.dz = 0;
  c.vx = 0;
  c.vy = 0;
  c.vz = 0;
}

static void watch_from(double x, double y, double z, double dx, double dy, double dz){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 0.9, 50);
  gluLookAt(x,y,z, x + dx,y+dy,z+dz, 0,0,1);
}

static void watch_from_camera (){
  watch_from(c.x, c.y, c.z, c.dx, c.dy, c.dz);
}


#define A (glVertex3f(x,y,z));
#define B (glVertex3f(x+SIZE_OF_PLANE,y,z));
#define C (glVertex3f(x,y+SIZE_OF_PLANE,z));
#define D (glVertex3f(x+SIZE_OF_PLANE,y+SIZE_OF_PLANE,z));
static void draw_plane(double x, double y, double z){
        glBegin(GL_QUADS);
        A B C D;
        glEnd();
}
#undef A
#undef B
#undef C
#undef D


#define A (glVertex3f(x,y,z));
#define B (glVertex3f(x+0.5,y,z));
#define C (glVertex3f(x,y+0.5,z));
#define D (glVertex3f(x,y,z+0.5));
static void draw_cone(double x, double y, double z){
        glBegin(GL_TRIANGLES);
        A B C;
        A B D;
        B C D;
        A C D;
        glEnd();
}
#undef A
#undef B
#undef C
#undef D


static void draw_scene(){
  int i=0;
  glClear(GL_COLOR_BUFFER_BIT);
  watch_from_camera();
  draw_cone(0,0,0);
  draw_cone(3,1,0);
  draw_cone(3,4,3);
  draw_cone(5,-2,1);
  draw_cone(0,3,0);
  while (i < N_PLANES){
    draw_plane(planes[i][0], planes[i][1], planes[i][2]);
    ++i;
  }
}

static void key_up_pressed(){
 up_key_status=1;
}
static void key_down_pressed(){
 down_key_status=1;
}
static void key_left_pressed(){
 left_key_status=1;
}
static void key_right_pressed(){
 right_key_status=1;
}
static void key_space_pressed(){
 space_key_status=1;
}

static void key_up_not_pressed(){
 up_key_status=0;
}
static void key_down_not_pressed(){
 down_key_status=0;
}
static void key_left_not_pressed(){
 left_key_status=0;
}
static void key_right_not_pressed(){
 right_key_status=0;
}
static void key_space_not_pressed(){
 space_key_status=0;
}

static void cb_keydown(SDL_KeyboardEvent *e){

 if( e->keysym.sym == SDLK_UP) {
   key_up_pressed();
 } else if(e->keysym.sym == SDLK_DOWN) {
   key_down_pressed();
 } else if( e->keysym.sym == SDLK_LEFT) {
   key_left_pressed();
 } else if(e->keysym.sym == SDLK_RIGHT) {
   key_right_pressed();
 } else if(e->keysym.sym == SDLK_SPACE) {
   key_space_pressed();
 }
}

static void cb_keyup(SDL_KeyboardEvent *e){

 if( e->keysym.sym == SDLK_UP) {
   key_up_not_pressed();
 } else if(e->keysym.sym == SDLK_DOWN) {
   key_down_not_pressed();
 } else if( e->keysym.sym == SDLK_LEFT) {
   key_left_not_pressed();
 } else if(e->keysym.sym == SDLK_RIGHT) {
   key_right_not_pressed();
 } else if(e->keysym.sym == SDLK_SPACE) {
   key_space_not_pressed();
 }
}

static int up_is_pressed(){
 return up_key_status;
}
static int down_is_pressed(){
 return down_key_status;
}
static int left_is_pressed(){
 return left_key_status;
}
static int right_is_pressed(){
 return right_key_status;
}
static int space_is_pressed(){
 return space_key_status;
}


static void process_event(SDL_Event *e){
  switch(e->type){
    case(SDL_KEYDOWN):
      cb_keydown((SDL_KeyboardEvent *)e);
      break; 
    case(SDL_KEYUP):
      cb_keyup((SDL_KeyboardEvent *)e);
      break; 
    default:
      break; 
  }
}

static void normalize() {
 double length = sqrt(pow(c.dx, 2)+ pow (c.dy, 2)+pow(c.dz,2));
 c.dx = c.dx / length;
 c.dy = c.dy / length;
 c.dz = c.dz / length;
}

static void turn_left(){
 c.dx -= m*c.dy;
 c.dy += m*c.dx;
 normalize();
}
static void turn_right(){
 c.dx += m*c.dy;
 c.dy -= m*c.dx;
 normalize();
}

static void set_camera(){
  if (up_is_pressed()){
    c.x += a*c.dx;
    c.y += a*c.dy;
  } else if (down_is_pressed()){
    c.x -= a*c.dx;
    c.y -= a*c.dy;
  } else if (left_is_pressed()){
    turn_left();
  } else if (right_is_pressed()){
    turn_right();
  }
}

static int on_the_ground(){
  int i = 0;
  if (c.z <= 0.0){
   return 1;
  }
  while (i < N_PLANES){
      if (
         (c.x < planes[i][0] + SIZE_OF_PLANE) && (c.x > planes [i][0])
          &&(c.y < planes[i][1]+SIZE_OF_PLANE) && (c.y > planes [i][1])
          &&(c.z < planes [i][2]+1.0)&&(c.z > planes[i][2]-0.5)
      )
     {
         return 1;
     }
         ++i;
  }
  return 0;
}

static void init_gl(){
 SDL_SetVideoMode(200, 200, 0, SDL_OPENGL); 
}

static void swap_buffers(){
 SDL_GL_SwapBuffers();
}

static void check_fps(){
 SDL_Delay(60);
}

int main(){
init_gl();
init_camera();
while(1){
  set_camera();
  draw_scene();
  swap_buffers();
  while(SDL_PollEvent(&e)) {
    process_event(&e);
  }

  c.vz -= g;
  if (on_the_ground()){
   c.vz=0;
   if (space_is_pressed()){
    c.vz += j;
   }
  }
  c.z += b * c.vz;

  check_fps();
}
return 0;
}

