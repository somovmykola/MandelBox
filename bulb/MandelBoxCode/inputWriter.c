#include <stdio.h>

#define DEBUG_MODE 1

int main(int argc, char** argv){
	
		if (argc!=3){
			printf("sorry, incorrect number of inputs, need 2\n");
			return 1;
		}

		FILE *ptr_file_out;
		FILE *ptr_file_in;

		int iter = atoi(argv[2]);
		char buffer [50];
		char line [80];
		size_t len = 0;
		char * file = argv[1];
		
		int x, y, z;

		int i;

		//if (DEBUG_MODE==1) getchar();
		for (i= 0; i< iter; i++){
			//if (DEBUG_MODE==1) getchar();

			int j = 1;
			//if (DEBUG_MODE==1) getchar();

			sprintf(buffer,"%s%d.txt", argv[1], i);
			//if (DEBUG_MODE==1) getchar();

			ptr_file_out =fopen(buffer, "w");
			//if (DEBUG_MODE==1) getchar();

			ptr_file_in=fopen(file, "r");
			//if (DEBUG_MODE==1) getchar();

			if (!ptr_file_in) return 1;
			//if (DEBUG_MODE==1) getchar();


			while (fgets(line, 80, ptr_file_in)!= NULL) {
				if (DEBUG_MODE==1) printf("made it\n");

				if (j==3){
					if (DEBUG_MODE==1) printf("made it\n");
					sscanf(line, "%d %d %d", &x, &y, &z);
					fprintf(ptr_file_out,"%d %d %d\n", x, y, z+1);
				}
				else if (j==5){
					sscanf(line, "%d %d %d", &x, &y, &z);
					fprintf(ptr_file_out,"%d %d %d\n", x, y, z);
				}
				else if (j==29){

					fprintf(ptr_file_out,"image%d.bmp\n", i);

				}				
				else fprintf(ptr_file_out,"%s", line);
				j++;

			}		

			printf("%i\n", feof(ptr_file_in));



			clearerr(ptr_file_out);
			clearerr(ptr_file_in);

			fclose(ptr_file_out);
			fclose(ptr_file_in);

			printf("%i\n", feof(ptr_file_in));

			file = buffer;


		}


	return 0;
}