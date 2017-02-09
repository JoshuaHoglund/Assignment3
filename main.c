#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "file_operations.h"
#include "graphics.h"

typedef struct particle
{
   double             x_pos;
   double             y_pos;
   double 	     mass;
   double      vel_x;
   double       vel_y;
} particle_t;

void keep_within_box(float* xA, float* yA) {
  if(*xA > 1)
    *xA = 0;
  if(*yA > 1)
    *yA = 0;
}

void updateParticles(double delta_t, particle_t *particles, int N) {
   //Set constants
   double *forcex=(double*)malloc(N*sizeof(double));
   double *forcey=(double*)malloc(N*sizeof(double));
   double G = 100.0/N;
   double eps = 0.001;
   double abs_r;
   double r_x, r_y;
   double x;
   double y;
   double forceSum_x, forceSum_y;
   double m_j;
   double m_i;
   
   for(int i=0; i<N; i++){
      forceSum_x = 0;
      forceSum_y = 0;
      x = particles[i].x_pos;
      y = particles[i].y_pos;
      m_i = particles[i].mass;
      
      // For each particle i, calculate the sum of the forces acting on it
      for(int j=0; j<N; j++){
         if(j!=i) {
            m_j = particles[j].mass;
            
            // Calculate the distance betweem particles i and j.
            abs_r = sqrt(pow(x-particles[j].x_pos,2)+pow(y-particles[j].y_pos,2));
            r_x = x-particles[j].x_pos;
            r_y = y-particles[j].y_pos;
            
            
               // Plumber spheres
               forceSum_x += m_j*r_x/(pow(abs_r+eps,3));
               forceSum_y += m_j*r_y/(pow(abs_r+eps,3));
              
            
            
         }
      }
      forceSum_x *= -G*m_i;
      forceSum_y *= -G*m_i;
      forcex[i] = forceSum_x; 
      forcey[i] = forceSum_y;
      // Using the force, update the velocity and position.
      
   }
   for(int i=0;i<N;i++){
      m_i = particles[i].mass;
      particles[i].vel_x += delta_t*forcex[i]/m_i;
      particles[i].vel_y += delta_t*forcey[i]/m_i;
      particles[i].x_pos += delta_t*particles[i].vel_x;
      particles[i].y_pos += delta_t*particles[i].vel_y;
   }
}

 
int main(int argc, const char* argv[]) { 
 // read in N filename nsteps delta_t graphics
 // N number of stars/particles to simulate 
 // filename is the filename of the file to read the initial configuration from 
 // nsteps is the number of timesteps
 // graphics is 1 or 0 meaning graphics on/off

// check if the parameters in the command line are correct, otherwise error message with instructions.	
  	if(argc != 6) {
      printf("Please give in: N filename nsteps delta_t graphics.\n");
      return -1;
    }
 
// read in N, check if N is 1 or larger otherwise error message.
  	int N = atoi(argv[1]);
    printf("N = %d\n", N);
    if(N < 1) {
      printf("Error: (N < 1).\n");
      return -1;
    }
  	
 // read in filename and open filename. 	
   FILE *ptr_file;
 

  	ptr_file = fopen(argv[2], "r");
 
  	if(!ptr_file){
  		printf("File does not exist." );
  		return 1;}
 	  // store data of opened file 
   
 		
  	fclose(ptr_file);
   
  // read in number of steps
 	int nsteps = atoi(argv[3]);
 	printf ("nsteps = %d\n", nsteps);
 	
 	
  // read in time delta_t
 	double delta_t = atof(argv[4]);
 	printf ("delta_t = %f\n", delta_t);
 	
 	// read graphics on or off
   int graphics = atoi(argv[5]);
   
     //Read the particle data from the file
 double *values =(double*)malloc(5*N*sizeof(double));
 read_doubles_from_file(atoi(argv[1])*5, values, argv[2]);
 
 //Allocate memory for particles  
 particle_t *particles = (particle_t*)malloc(N*sizeof(particle_t));
 
 //Set the particle data  
 int i = 0;
 int j = 0;  
 while(j<N){
    particles[j].x_pos = values[i];
    particles[j].y_pos = values[i+1];
    particles[j].mass = values[i+2];
    particles[j].vel_x = values[i+3];
    particles[j].vel_y = values[i+4];
    j++;
    i=j*5;
 }
   
   if(!graphics) {
      for(int t=0;t<nsteps;t++) {
         updateParticles(delta_t, particles, N);
      }
   }
   else {
      int L = 1;
      int W = 1;
      int windowWidth = 600;
      int windowHeight = 600;
      SetCAxes(0,1);


      InitializeGraphics("",windowWidth,windowHeight);
      double x, y, circleRadius;

         
        for(int t=0;t<nsteps;t++) {
            
           ClearScreen();
           
           for(int i=0;i<N;i++) {
              x = particles[i].x_pos;
              //printf("%lf\n", x);
              y = particles[i].y_pos;
              //printf("%lf\n", y);
              circleRadius = 0.005;
              //keep_within_box((float*) &x, (float*) &y);
              
              DrawCircle(x, y, L, W, circleRadius, 0.1);
              
              
           }
           Refresh();
           usleep(800);
           updateParticles(delta_t, particles, N);
         }
      usleep(8000000);
           

     
     FlushDisplay();
     CloseDisplay();
   }

   
 double * simulationData = (double *) malloc(5*N*sizeof(double));
 i = 0;
 j = 0;  
 while(j<N){
    simulationData[i] = particles[j].x_pos;
    simulationData[i+1] = particles[j].y_pos;
    simulationData[i+2] = particles[j].mass;
    simulationData[i+3] = particles[j].vel_x;
    simulationData[i+4] = particles[j].vel_y;
    printf("%lf\n", particles[j].vel_x);
    printf("%lf\n", particles[j].vel_y);
    j++;
    i=j*5;
 }
   write_doubles_to_file(5*N,simulationData,"result.gal");
   
  return 0;
 
}
