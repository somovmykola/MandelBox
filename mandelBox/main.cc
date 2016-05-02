/*
   This file is part of the Mandelbox program developed for the course
    CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
    Winter of 2015-2016 at McMaster University.

    Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include "vector3d.h"
#include <time.h>
#include <omp.h>

//#define thresh 2.0

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, double &probeDist);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);

MandelBoxParams mandelBox_params;

int main(int argc, char** argv)
{
  int i = atoi(argv[2]);
  int j;
  bool knowWhere = true;
  //bool go = true;
  bool turn = false;
  double start = omp_get_wtime();

  double thresh = 1.0;
  int stuck = 0;

  if (argc != 3){
    printf("you need 3 inputs!");
    return 1;
  } 

  vec3 temp = {0.0, 0.0, 0.0};
  double mag;

  

  bool threshold = true;
  //bool last = false;
  double xpos = 1.0;
  double ypos = 1.0;
  double zpos = 1.0;

  char buf[1024];

  CameraParams    camera_params;
  RenderParams    renderer_params;
  
  getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);

  srand ( time(NULL) );

  for (j = 1; j<i +1; j++)
    {  
    sprintf(buf, "images/%d.bmp", j);
    strcpy((&renderer_params)->file_name, buf);

    double *d;
    d = (&camera_params)->camPos;
    vec3 from = {*d, *(d+1), *(d+2)};
    double probeDist;
    if (knowWhere) plus(from, temp, from);

    *d = from.x;
    *(d+1) = from.y;
    *(d+2) = from.z;

    //*(d+2) = *(d+2) + (double) j; 

    //printf("EGOMANIAC BUTTNERD %f %f %f\n", from.x, from.y, from.z);
    
    d = (&camera_params)->camTarget;
    vec3 to = {*d, *(d+1), *(d+2)}; 

    if(fabs(from.x) > 7 || fabs(from.y) > 7 || fabs(from.z) > 7){
	*d = 0;
	*(d+1) =0;
	*(d+2)=0;
	threshold = true;
    }

    if (stuck >= 50) thresh /= 2.0;

    if((j% 800) > 790){
	switch(rand()%3){
	    case 0:
		//*d = (float)(rand() %7)* xpos;
		*d = *d + 1*(xpos);
	    case 1:
		//*(d+1)= (float)(rand() %7) * ypos;
		*(d+1) = *(d+1) + 1*(ypos);
	    case 2:
    		//*(d+2)= (float)(rand() %7) * zpos;	
		*(d+2) = *(d+2) + 1*(zpos);
	}
	//turn = false;
	threshold = true;
    }

    //printf("thing ring do your thing %f %f %f\n", to.x, to.y, to.z);

    int image_size = renderer_params.width * renderer_params.height;
    unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

    init3D(&camera_params, &renderer_params);

    renderFractal(camera_params, renderer_params, image, probeDist);
    
    //printf("the dist is %f\n", &probeDist);

    if (threshold){
	minus(to, from, temp);	
    	magnitude(mag, temp);
    	linDivide(temp, mag, temp);
	linMult(temp, 0.01, temp);
    	threshold = false;
	//if (fabs(probeDist) <= 1.0 && fabs(probeDist) > 0.0) linMult(temp, fabs(probeDist)/1.2, temp);
	
	//linMult(temp, abs(probeDist), temp);
    }

    if (probeDist <= thresh){
	stuck++;
	threshold = true;
	//last = true;
	d = (&camera_params)->camTarget;
	//float r = (float)(rand()-RAND_MAX)/((float)RAND_MAX/2.0);
	switch(rand()%3){
	    case 0:
		*d = *d +1*(xpos);
	    case 1:
		*(d+1) = *(d+1) + 1*(ypos);
	    case 2:
    		*(d+2) = *(d+2) + 1*(zpos);	
	}

	//if (abs(probeDist) < 0.000001) go = false;

	knowWhere = false;

	vec3 to = {*d, *(d+1), *(d+2)}; 
	turn = true;
		
    }
    else if (turn){
	switch(rand()%3){
	    case 0:
		xpos = xpos*(-1.0);
	    case 1:
		ypos = ypos*(-1.0);
	    case 2:
    		zpos = zpos*(-1.0);	
	}
	knowWhere = true;
	turn = false;
	//go = true;
	stuck =0;
    }
    else {
    	turn = false;
	knowWhere = true;
	stuck=0;
    }

	
    //linMult(temp,0.90, temp);
  
    saveBMP(renderer_params.file_name, image, renderer_params.width, renderer_params.height);

    //printf("dist is %f\n", dist);
  
    free(image);
    printf("%d\n", j);		
  }

  double end = omp_get_wtime();

  printf("final time is: %f\n", end-start);

  return 0;
}
