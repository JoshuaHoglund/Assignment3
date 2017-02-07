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
printf("The program runs." );
 
 FILE *ptr_file;
	char buf[1000];
	
	

	ptr_file = fopen(argv[1], "r");

	if(!ptr_file){
		printf("File does not exist." );
		return 1;}
	
	while(fgets(buf, 1000, ptr_file)!=NULL)
			printf("%s", buf);

			
	fclose(ptr_file);
	
	

  return 0;
 
}
