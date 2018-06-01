//Edmond Chew
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
#elif __linux__
  #include <GL/gl.h>
  #include <GL/glut.h> 
#endif


#define X_RESOLUTION 800 
#define Y_RESOLUTION 600 
#define SQUARE_LENGTH 10 
#define UNDEFINED -1
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define TRUE 1
#define FALSE 0



typedef struct ball_color
{
  int r;
  int g;
  int b;
} ball_color;

typedef struct vec2
{
  int x;
  int y;
} vec2;

typedef struct vec3
{
  int x;
  int y;
  int z;
  int direction;
} vec3;


typedef struct ball_type
{
  vec3 position;
  int radius;
  ball_color color;
} ball_type;

typedef struct square
{
    float top_left;
    float top_right;
    float bot_left;
    float bot_right;
    int cases;
    vec2 position;
} square;

ball_type ball1;
ball_type ball2;
ball_type ball3;
ball_type ball4;
ball_type ball5;
ball_type ball6;
ball_type ball7;
ball_type ball8;

square grid[X_RESOLUTION/SQUARE_LENGTH][Y_RESOLUTION/SQUARE_LENGTH];

int deciding_case (square);
void draw_square (square);
int ball_hit_wall (ball_type);
int select_ball_direction (ball_type);
ball_type move_ball (ball_type);
void display (void);
void reshape (int, int);
void keyboard (unsigned char, int, int);



int deciding_case (square sq)
{
    float square_x, square_y, square_r, limit = 0.7;
    int i = 0, total = 0;
    ball_type tempball;
    int topLeft = 0, topRight = 0, botLeft = 0, botRight = 0;    
    while( i < 8 ){
        switch(i){
            case 0:
            tempball = ball1;
            break;
            case 1:
            tempball = ball2;
            break;
            case 2:
            tempball = ball3;
            break;
            case 3:
            tempball = ball4;
            break;
            case 4:
            tempball = ball5;
            break;
            case 5:
            tempball = ball6;
            break;
            case 6:
            tempball = ball7;
            break;
            case 7:
            tempball = ball8;
            break;
        }

        // r^2/ (x-x0)^2 + (y-y0)^2 <= 1
        square_r = tempball.radius * tempball.radius;

        //top left
        square_x = sq.position.x - tempball.position.x;        
        square_x *= square_x;
        square_y = sq.position.y - tempball.position.y;        
        square_y *= square_y;
        if(square_r/(square_x + square_y) >= limit)  
            topLeft = 1;        
        if(sq.top_left < square_r/(square_x + square_y)) 
            sq.top_left = square_r/(square_x + square_y);

        //top right
        square_x = sq.position.x + SQUARE_LENGTH - tempball.position.x;        
        square_x *= square_x;
        square_y = sq.position.y - tempball.position.y;        
        square_y *= square_y;
        if(square_r/(square_x + square_y) >= limit)  
            topRight = 1;            
        if(sq.top_right < square_r/(square_x + square_y)) 
            sq.top_right = square_r/(square_x + square_y);

        //bot right
        square_x = sq.position.x + SQUARE_LENGTH - tempball.position.x;        
        square_x *= square_x;
        square_y = sq.position.y + SQUARE_LENGTH - tempball.position.y;        
        square_y *= square_y;
        if(square_r/(square_x + square_y) >= limit)  
            botRight = 1;        
        if(sq.bot_right < square_r/(square_x + square_y)) 
            sq.bot_right = square_r/(square_x + square_y);

        //bot left
        square_x = sq.position.x - tempball.position.x;        
        square_x *= square_x;
        square_y = sq.position.y + SQUARE_LENGTH- tempball.position.y;        
        square_y *= square_y;
        if(square_r/(square_x + square_y) >= limit)  
            botLeft = 1;            
        if(sq.bot_left < square_r/(square_x + square_y)) 
            sq.bot_left = square_r/(square_x + square_y);

        i++;
    }
    
    return topLeft * 8 + topRight * 4 + botRight * 2 + botLeft * 1;

}

void draw_square (square sq)
{
    //interpolation Middle - Above/ Bottom - Above
    switch(sq.cases){
        case 0:
        case 15:       
        break;
        case 1:
        case 14:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y + SQUARE_LENGTH, 0.0f);
            glEnd();
        break;
        case 2:
        case 13:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x + SQUARE_LENGTH, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y + SQUARE_LENGTH, 0.0f);
            glEnd();
        break;
        case 3:
        case 12:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glEnd();
        break;
        case 4:
        case 11:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glEnd();
        break;

        case 5:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x + SQUARE_LENGTH, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y + SQUARE_LENGTH, 0.0f);
            glEnd();
            glBegin (GL_LINES);
            glVertex3f (sq.position.x, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y, 0.0f);
            glEnd();
        break;
        case 10:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y + SQUARE_LENGTH, 0.0f);
            glEnd();
            glBegin (GL_LINES);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glEnd();
        break;

        case 6:
        case 9:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y + SQUARE_LENGTH, 0.0f);
            glEnd();
        break;

        case 7:
        case 8:
            glBegin (GL_LINES);
            glVertex3f (sq.position.x, sq.position.y + SQUARE_LENGTH/2, 0.0f);
            glVertex3f (sq.position.x + SQUARE_LENGTH/2, sq.position.y, 0.0f);
            glEnd();
        break;
    }

}

int ball_hit_wall (ball_type ball)
{
  int ball_hit_wall;

  ball_hit_wall = FALSE;
  switch (ball.position.direction)
  {
    case NORTH:
      if ((ball.position.y - ball.radius) <= 0)
      {
        ball_hit_wall = TRUE;
      }
    break;
    case NORTHEAST:
      if (((ball.position.y - ball.radius) <= 0) || ((ball.position.x + ball.radius) >= X_RESOLUTION))
      {
        ball_hit_wall = TRUE;
      }
    break;
    case EAST:
      if ((ball.position.x + ball.radius) >= X_RESOLUTION)
      {
        ball_hit_wall = TRUE;
      }
    break;
    case SOUTHEAST:
      if (((ball.position.y + ball.radius) >= Y_RESOLUTION) || ((ball.position.x + ball.radius) >= X_RESOLUTION))
      {
        ball_hit_wall = TRUE;
      }
    break;
    case SOUTH:
      if ((ball.position.y + ball.radius) >= Y_RESOLUTION)
      {
        ball_hit_wall = TRUE;
      }
    break;
    case SOUTHWEST:
      if (((ball.position.y + ball.radius) >= Y_RESOLUTION) || ((ball.position.x - ball.radius) <= 0))
      {
        ball_hit_wall = TRUE;
      }
    break;
    case WEST:
      if ((ball.position.x - ball.radius) <= 0)
      {
        ball_hit_wall = TRUE;
      }
    break;
    case NORTHWEST:
      if (((ball.position.y - ball.radius) <= 0) || ((ball.position.x - ball.radius) <= 0))
      {
        ball_hit_wall = TRUE;
      }
    break;
    default:
    break;
  }
  return ball_hit_wall;
}





int select_ball_direction (ball_type ball)
{
  int new_ball_direction, random_direction;
  float new_move_north, new_move_northeast, new_move_east, new_move_southeast, new_move_south, new_move_southwest, new_move_west, new_move_northwest;
  float random_number, lower_bound, upper_bound;

  switch (ball.position.direction)
  {
    case NORTH:
      if (((ball.position.x - ball.radius) <= 0) && ((ball.position.y - ball.radius) <= 0))
      {
        new_ball_direction = SOUTHEAST;
      }
      else
      {
        if (((ball.position.x + ball.radius) >= X_RESOLUTION) && ((ball.position.y - ball.radius) <= 0))
        {
          new_ball_direction = SOUTHWEST;
        }
        else
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = SOUTHEAST;
            break;
            case 1:
              new_ball_direction = SOUTHWEST;
            break;
            default:
              new_ball_direction = SOUTHEAST;
            break;
          }
        }
      }     
    break;
    case NORTHEAST:
      if ((ball.position.x + ball.radius) >= X_RESOLUTION)
      {
        if ((ball.position.y - ball.radius) <= 0)
        {
          new_ball_direction = SOUTHWEST;
        }
        else
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = NORTHWEST;
            break;
            case 1:
              new_ball_direction = WEST;
            break;
            default:
              new_ball_direction = NORTHWEST;
            break;
          }
        }
      }
      else
      {
        random_number = (float) random() / (float) RAND_MAX * 2.0f;
        random_direction = (int) random_number;
        switch (random_direction)
        {
          case 0:
            new_ball_direction = SOUTHEAST;
          break;
          case 1:
            new_ball_direction = EAST;
          break;
          default:
            new_ball_direction = SOUTHEAST;
          break;
        }
      }
    break;
    case EAST:
      if ((ball.position.y - ball.radius) <= 0)
      {
        new_ball_direction = SOUTHWEST;
      }
      else
      {
        if ((ball.position.y + ball.radius) >= Y_RESOLUTION)
        {
          new_ball_direction = NORTHWEST; 
        }
        else
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = NORTHWEST; 
            break;
            case 1:
              new_ball_direction = SOUTHWEST;
            break;
            default:
              new_ball_direction = NORTHWEST;
            break;
          }
        }
      }
    break;
    case SOUTHEAST:
      if ((ball.position.y + ball.radius) >= Y_RESOLUTION)
      {
        if ((ball.position.x + ball.radius) >= X_RESOLUTION)
        {
          new_ball_direction = NORTHWEST;
        }
        else
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = NORTH;
            break;
            case 1:
              new_ball_direction = NORTHEAST;
            break;
            case 2:
              new_ball_direction = EAST;
            break;
            default:
              new_ball_direction = NORTH;
            break;
          }
        }
      }
      else
      {
        random_number = (float) random() / (float) RAND_MAX * 2.0f;
        random_direction = (int) random_number;
        switch (random_direction)
        {
          case 0:
            new_ball_direction = SOUTH;
          break;
          case 1:
            new_ball_direction = SOUTHWEST;
          break;
          case 2:
            new_ball_direction = WEST;
          break;
          default:
            new_ball_direction = SOUTH;
          break;
        }
      }
    break;
    case SOUTH:
      if ((ball.position.x + ball.radius) >= X_RESOLUTION)
      {
        new_ball_direction = NORTHWEST;
      }
      else
      {
        new_ball_direction = NORTHEAST;
      }
    break;
    case SOUTHWEST:
      if ((ball.position.x - ball.radius) <= 0)
      { 
        if ((ball.position.y + ball.radius) >= Y_RESOLUTION)
        {
          new_ball_direction = NORTHEAST;
        }
        else
        {
          new_ball_direction = SOUTHEAST;
        }
      }
      else
      {
        random_number = (float) random() / (float) RAND_MAX * 2.0f;
        random_direction = (int) random_number;
        switch (random_direction)
        {
          case 0:
            new_ball_direction = NORTH;
          break;
          case 1:
            new_ball_direction = NORTHWEST;
          break;
          case 2:
            new_ball_direction = WEST;
          break;
          default:
            new_ball_direction = NORTH;
          break;
        }
      }
    break;
    case WEST:
      if ((ball.position.y - ball.radius) <= 0)
      {
        new_ball_direction = SOUTHEAST;
      }
      else
      {
        if ((ball.position.y + ball.radius) >= Y_RESOLUTION)
        {
          new_ball_direction = NORTHEAST;
        }
        else
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = NORTHEAST;
            break;
            case 1:
              new_ball_direction = SOUTHEAST;
            break;
            default:
              new_ball_direction = NORTHEAST;
            break;
          }
        }
      }
    break;
    case NORTHWEST:
      if ((ball.position.x - ball.radius) <= 0)
      { 
        if ((ball.position.y - ball.radius) <= 0)
        {
          new_ball_direction = SOUTHEAST;
        }
        else
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = NORTHEAST;
            break;
            case 1:
              new_ball_direction = EAST;
            break;
            default:
              new_ball_direction = NORTHEAST;
            break;
          }
        }
      }
      else
      {
        if ((ball.position.y - ball.radius) <= 0)
        {
          random_number = (float) random() / (float) RAND_MAX * 2.0f;
          random_direction = (int) random_number;
          switch (random_direction)
          {
            case 0:
              new_ball_direction = SOUTHWEST;
            break;
            case 1:
              new_ball_direction = SOUTH;
            break;
            case 2:
              new_ball_direction = WEST;
            break;
            default:
              new_ball_direction = SOUTHWEST;
            break;
          }
        }
      }
    break;
    default:
    break;
  }
  ball.position.direction = new_ball_direction;
  return ball.position.direction;
}





ball_type move_ball (ball_type ball)
{
  ball_type new_ball;

  new_ball = ball;
  switch (ball.position.direction)
  {
    case NORTH:
      new_ball.position.y--;
    break;
    case NORTHEAST:
      new_ball.position.x++;
      new_ball.position.y--;
    break;
    case EAST:
      new_ball.position.x++;
    break;
    case SOUTHEAST:
      new_ball.position.x++;
      new_ball.position.y++;
    break;
    case SOUTH:
      new_ball.position.y++;
    break;
    case SOUTHWEST:
      new_ball.position.x--;
      new_ball.position.y++;
    break;
    case WEST:
      new_ball.position.x--;
    break;
    case NORTHWEST:
      new_ball.position.x--;
      new_ball.position.y--;
    break;
    default:
    break;
  }
  return new_ball;
}




void draw_ball (ball_type ball)
{
  float theta, circle_iterations = 24.0;

  glColor3ub (ball.color.r, ball.color.g, ball.color.b);
  glPolygonMode (GL_FRONT, GL_FILL);
  glBegin (GL_POLYGON);
  glVertex3f (ball.position.x + ball.radius, ball.position.y, 0.0f);
  for (theta = 0; theta < 2 * M_PI; theta += M_PI / circle_iterations)
  {
    glVertex3f (ball.position.x + cos(theta) * ball.radius, ball.position.y + sin(theta) * ball.radius, 0.0f);
  }
  glEnd();
}





void display (void)
{
// **********
// * Ball 1 *
// **********
  if (ball_hit_wall(ball1))
  {
    ball1.position.direction = select_ball_direction (ball1);
  }
  ball1 = move_ball (ball1);
// **********
// * Ball 2 *
// **********
  if (ball_hit_wall(ball2))
  {
    ball2.position.direction = select_ball_direction (ball2);
  }
  ball2 = move_ball (ball2);
// **********
// * Ball 3 *
// **********
  if (ball_hit_wall(ball3))
  {
    ball3.position.direction = select_ball_direction (ball3);
  }
  ball3 = move_ball (ball3);
// **********
// * Ball 4 *
// **********
  if (ball_hit_wall(ball4))
  {
    ball4.position.direction = select_ball_direction (ball4);
  }
  ball4 = move_ball (ball4);
// **********
// * Ball 5 *
// **********
  if (ball_hit_wall(ball5))
  {
    ball5.position.direction = select_ball_direction (ball5);
  }
  ball5 = move_ball (ball5);
// **********
// * Ball 6 *
// **********
  if (ball_hit_wall(ball6))
  {
    ball6.position.direction = select_ball_direction (ball6);
  }
  ball6 = move_ball (ball6);
// **********
// * Ball 7 *
// **********
  if (ball_hit_wall(ball7))
  {
    ball7.position.direction = select_ball_direction (ball7);
  }
  ball7 = move_ball (ball7);
// **********
// * Ball 8 *
// **********
  if (ball_hit_wall(ball8))
  {
    ball8.position.direction = select_ball_direction (ball8);
  }
  ball8 = move_ball (ball8);

//
// clear the screen to black
  glColor3ub (0, 0, 0);
  glPolygonMode (GL_FRONT, GL_FILL);
  glRecti (0, 0, X_RESOLUTION, Y_RESOLUTION);

int i = 0, j = 0;
glColor3ub (100, 100, 100);
/*
    //grid
    while(i <= Y_RESOLUTION/SQUARE_LENGTH){
        j = 0;
        while(j <= X_RESOLUTION/SQUARE_LENGTH){
            glBegin (GL_LINES);
            glVertex3f (j * SQUARE_LENGTH, 0, 0.0f);
            glVertex3f (j * SQUARE_LENGTH, Y_RESOLUTION, 0.0f);
            glEnd();
            glBegin (GL_LINES);
            glVertex3f (0, i * SQUARE_LENGTH, 0.0f);
            glVertex3f (X_RESOLUTION, i * SQUARE_LENGTH, 0.0f);
            glEnd();
            j++;
        }
        i++;
    }

*/ 
    // setting squares
    glColor3ub (0, 255, 255);
    i = 0;
    j = 0;
    while(i < Y_RESOLUTION/SQUARE_LENGTH){
        j = 0;
        while(j < X_RESOLUTION/SQUARE_LENGTH){
            grid[j][i].cases = deciding_case(grid[j][i]);
            //printf("%d", grid[j][i].cases); 
            draw_square(grid[j][i]);            
            j++;
        }
        i++;
    } 

// draw ball
/*
  draw_ball (ball1);
  draw_ball (ball2);
  draw_ball (ball3);
  draw_ball (ball4);
  draw_ball (ball5);
  draw_ball (ball6);
  draw_ball (ball7);
  draw_ball (ball8);
*/
  glutSwapBuffers();
  glutPostRedisplay();

}


void reshape (int w, int h)  
{
  glViewport (0, 0, w, h);
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();  
  if (h == 0)  
  {
    glOrtho (0.0, X_RESOLUTION, 1, 0, -1.0, 1.0);
  }
  else
  {
    glOrtho (0.0, X_RESOLUTION, Y_RESOLUTION, 0, -1.0, 1.0);
  }
  glMatrixMode(GL_MODELVIEW);  
  glLoadIdentity(); 
}


void keyboard (unsigned char key, int x, int y) 
{
  switch (key) 
  {
    case 27:    
      exit (0);
    break;  
    default: 
    break;
  }
}


int main (int argc, char *argv[]) 
{
// **************************
// * ball 1 characteristics *
// **************************
  ball1.position.x = X_RESOLUTION / 2;
  ball1.position.y = Y_RESOLUTION / 2,
  ball1.position.z = 0;
  ball1.position.direction = NORTH;
  ball1.radius = 30;
  ball1.color.r = 255;
  ball1.color.g = 255;
  ball1.color.b = 0;
// **************************
// * ball 2 characteristics *
// **************************
  ball2.position.x = X_RESOLUTION / 2;
  ball2.position.y = Y_RESOLUTION / 2;
  ball2.position.z = 0;
  ball2.position.direction = EAST;
  ball2.radius = 50;
  ball2.color.r = 255;
  ball2.color.g = 255;
  ball2.color.b = 0;
// **************************
// * ball 3 characteristics *
// **************************
  ball3.position.x = X_RESOLUTION / 2;
  ball3.position.y = Y_RESOLUTION / 2;
  ball3.position.z = 0;
  ball3.position.direction = SOUTH;
  ball3.radius = 50;
  ball3.color.r = 255;
  ball3.color.g = 255;
  ball3.color.b = 0;
// **************************
// * ball 4 characteristics *
// **************************
  ball4.position.x = X_RESOLUTION / 2;
  ball4.position.y = Y_RESOLUTION / 2;
  ball4.position.z = 0;
  ball4.position.direction = WEST;
  ball4.radius = 100;
  ball4.color.r = 255;
  ball4.color.g = 255;
  ball4.color.b = 0;
// **************************
// * ball 5 characteristics *
// **************************
  ball5.position.x = X_RESOLUTION / 2;
  ball5.position.y = Y_RESOLUTION / 2;
  ball5.position.z = 0;
  ball5.position.direction = NORTHEAST;
  ball5.radius = 50;
  ball5.color.r = 255;
  ball5.color.g = 255;
  ball5.color.b = 0;
// **************************
// * ball 6 characteristics *
// **************************
  ball6.position.x = X_RESOLUTION / 2;
  ball6.position.y = Y_RESOLUTION / 2;
  ball6.position.z = 0;
  ball6.position.direction = NORTHWEST;
  ball6.radius = 50;
  ball6.color.r = 255;
  ball6.color.g = 255;
  ball6.color.b = 0;
// **************************
// * ball 7 characteristics *
// **************************
  ball7.position.x = X_RESOLUTION / 2;
  ball7.position.y = Y_RESOLUTION / 2;
  ball7.position.z = 0;
  ball7.position.direction = SOUTHEAST;
  ball7.radius = 50;
  ball7.color.r = 255;
  ball7.color.g = 255;
  ball7.color.b = 0;
// **************************
// * ball 8 characteristics *
// **************************
  ball8.position.x = X_RESOLUTION / 2;
  ball8.position.y = Y_RESOLUTION / 2;
  ball8.position.z = 0;
  ball8.position.direction = SOUTHEAST;
  ball8.radius = 70;
  ball8.color.r = 255;
  ball8.color.g = 255;
  ball8.color.b = 0;
// **************************
// * initalize square origin *
// **************************
    int i = 0, j = 0;
    while(i < Y_RESOLUTION/SQUARE_LENGTH){
        j = 0;
        while(j < X_RESOLUTION/SQUARE_LENGTH){
            grid[j][i].position.x = j * SQUARE_LENGTH;
            grid[j][i].position.y = i * SQUARE_LENGTH;   
            j++;
        }
        i++;
    } 
// seed the random number generator
  srandom (time(0));
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
  glutInitWindowSize (X_RESOLUTION, Y_RESOLUTION);
  glutCreateWindow ("Marching Squares");
  glutDisplayFunc (display);  
  glutReshapeFunc (reshape);
  glutKeyboardFunc (keyboard);
  glutMainLoop();
}

