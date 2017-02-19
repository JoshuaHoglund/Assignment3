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
   int fiveI;
   int fiveJ;
   
   for(int i=0; i<N; i++){
      fiveI=5*i;
      x = values[fiveI];
      y = values[fiveI+1];
      m_i = values[fiveI+2];
      
      // For each particle i, calculate the sum of the forces acting on it. We
      // make use of the fact that two particles affect each other with
      // the same but opposite forces.
      for(int j=i+1; j<N; j++){
            fiveJ=5*j;
            m_j = values[fiveJ+2];
            
            // Calculate the distance betweem particles i and j.
            abs_r = sqrtf((x-values[fiveJ])*(x-values[fiveJ])+(y-values[fiveJ+1])*(y-values[fiveJ+1]));
            r_x = x-values[fiveJ];
            printf("r_x: %lf \n", r_x);
            r_y = y-values[fiveJ+1];
            printf("r_y: %lf \n", r_y);
            // Plumber spheres
            k = -G*m_i*m_j/((abs_r+eps)*(abs_r+eps)*(abs_r+eps));
            forcex[i] += k*r_x;
            forcey[i] += k*r_y;
            forcex[j] += -k*r_x;
            forcey[j] += -k*r_y;         
      }
       printf("forceX: %lf \n", forcex[i]);
   printf("forceY: %lf \n", forcey[i]);
   }
   // Using the force, update the velocity and position.
   for(int i=0;i<N;i++){
      fiveI=5*i;
      m_i = 1/values[fiveI+2];
      values[fiveI+3]+=delta_t*forcex[i]*m_i;
      values[fiveI+4]+=delta_t*forcey[i]*m_i;
      values[fiveI]+=delta_t*values[fiveI+3];
      values[fiveI+1]+=delta_t*values[fiveI+4];
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
   
  
  	fclose(ptr_file);
   
 	int nsteps = atoi(argv[3]);
 	double delta_t = atof(argv[4]);
   int graphics = atoi(argv[5]);
   
   // store data of opened file 
   double *values =(double*)malloc(5*N*sizeof(double));
   read_doubles_from_file(N*5, values, argv[2]); 
   
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
              y = values[5*i+1];
              circleRadius = 0.005;
              DrawCircle(x, y, L, W, circleRadius, 0.1);          
           }
           Refresh();
           // We tried using a sleep function but we got an error message on linux
           usleep(80000000);
           updateParticles(delta_t, values, N);
         }    
     FlushDisplay();
     CloseDisplay();
   }
   
  write_doubles_to_file(5*N,values,"result.gal");   
  return 0;
}
