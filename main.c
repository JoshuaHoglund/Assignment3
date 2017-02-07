#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "file_operations.h"
#include "graphics.h"

typedef struct particle
{
   float             x_pos;
   float             y_pos;
   float 	     mass;
} particle_t;
 
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
  	char buf[1000];
 

  	ptr_file = fopen(argv[2], "r");
 
  	if(!ptr_file){
  		printf("File does not exist." );
  		return 1;}
 	
  	while(fgets(buf, 1000, ptr_file)!=NULL)
  			printf("%s", buf);
 		
  	fclose(ptr_file);
  // read in number of steps
 	int nsteps = atoi(argv[3]);
 	printf ("nsteps = %d\n", nsteps);
 	
 	
 	
  // read in time delta t 
 	float delta_t = atoi(argv[4]);
 	printf ("delta_t = %f\n", delta_t);
 	
 	
 // read in graphics turned on 1 or turned off 0

  return 0;
 
}
