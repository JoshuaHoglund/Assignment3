#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "file_operations.h"
#include "graphics.h"

void updateParticles(double delta_t, double* values, int N) {
   //Set constants
   double *forcex=(double*)calloc(N,sizeof(double));
   double *forcey=(double*)calloc(N,sizeof(double));
   const double G = 100.0/N;
   const double eps = 0.001;
   double abs_r;
   double r_x, r_y;
   double x;
   double y;
   double m_j;
   double m_i;
   double k;
   
   for(int i=0; i<N; i++){
      x = values[5*i];
      y = values[5*i+1];
      m_i = values[5*i+2];
      
      // For each particle i, calculate the sum of the forces acting on it
      // two for loops!!!
      for(int j=i+1; j<N; j++){
         
            m_j = values[5*j+2];
            
            // Calculate the distance betweem particles i and j.
            // USE SQRTF????
            abs_r = sqrt((x-values[5*j])*(x-values[5*j])+(y-values[5*j+1])*(y-values[5*j+1]));
            r_x = x-values[5*j];
            r_y = y-values[5*j+1];
            // Plumber spheres
            // use dummy variable???
            k = -G*m_i*m_j/((abs_r+eps)*(abs_r+eps)*(abs_r+eps));
            forcex[i] += k*r_x;
            forcey[i] += k*r_y;
            forcex[j] += -k*r_x;
            forcey[j] += -k*r_y;
         
      }    
        
   }
   // Using the force, update the velocity and position.
   for(int i=0;i<N;i++){
      m_i = 1/values[5*i+2];
      values[5*i+3]+=delta_t*forcex[i]*m_i;
      values[5*i+4]+=delta_t*forcex[i]*m_i;
      values[5*i]+=delta_t*values[5*i+3];
      values[5*i+1]+=delta_t*values[5*i+4];
   }
   free(forcex);
  free(forcey);
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
   
 	int nsteps = atoi(argv[3]);
 	double delta_t = atof(argv[4]);
 	
   int graphics = atoi(argv[5]);
 double *values =(double*)malloc(5*N*sizeof(double));
 read_doubles_from_file(N*5, values, ptr_file);
 
 //Allocate memory for particles  
   
   if(!graphics) {
      for(int t=0;t<nsteps;t++) {
         // dont use function?
         updateParticles(delta_t, values, N);
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
              x = values[5*i];
              //printf("%lf\n", x);
              y = values[5*i+1];
              //printf("%lf\n", y);
              circleRadius = 0.005;
              DrawCircle(x, y, L, W, circleRadius, 0.1);          
           }
           Refresh();
           //usleep(800);
           updateParticles(delta_t, values, N);
         }
    
     FlushDisplay();
     CloseDisplay();
   }
   
   write_doubles_to_file(5*N,values,"result.gal");
   
  return 0;
 
}
