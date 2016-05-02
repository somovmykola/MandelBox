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
#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"


//#pragma acc routine seq
//extern double getTime();

//#pragma acc routine seq
//extern void   printProgress( double perc, double time );

#pragma acc routine seq
extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, double eps, pixelData &pix_data, const MandelBoxParams &mandelBox_params, SuperVec &vecHolder);

#pragma acc routine seq
extern void getColour(vec3 &returnVal, const pixelData &pixData, const RenderParams &render_params,
		      const vec3 &from, const vec3  &direction, SuperVec &vecHolder);

#pragma acc routine seq
extern void UnProject(double winX, double winY, const CameraParams &camP, vec3 &obj);



void renderFractal(const CameraParams camera_params, const RenderParams renderer_params, unsigned char* image, const MandelBoxParams mandelBox_params)
{

  
  const double eps = pow(10.0, renderer_params.detail); 
  //double farPoint[3];
  vec3 from;
  
  SetDoublePoint(from, camera_params.camPos);
  
  const int height = renderer_params.height;
  const int width  = renderer_params.width;

  int n = height * width;

  
  vec3 *farPoint = (vec3*)malloc((n)*sizeof(vec3));

  pixelData *pix_data = (pixelData*)malloc((n)*sizeof(pixelData));

  vec3 *to = (vec3*)malloc((n)*sizeof(vec3));

  SuperVec *vecHolder = (SuperVec*)malloc((n)*sizeof(SuperVec));
  
 // double time = getTime();
  vec3 *col = (vec3*)malloc((n)*sizeof(vec3));

  
  int i,j,k;
  
  #pragma acc data copyout(image[0:3*n]) create(pix_data[0:n],to[0:n],farPoint[0:n],col[0:n], vecHolder[0:n]) copyin(eps,from,renderer_params,camera_params, mandelBox_params) copy()
  {

  #pragma acc parallel loop 
  for(j = 0; j < height; j++)
    {
      //for each column pixel in the row
      #pragma acc loop
      for(i = 0; i < width; i++)
	{
	  // get point on the 'far' plane
	  // since we render one frame only, we can use the more specialized method

	  int jWidthi = j * width + i;
	  
	  UnProject(i, j, camera_params, farPoint[jWidthi]);
	  
          pSubStar(to[jWidthi], farPoint[jWidthi], camera_params.camPos);

	  NORMALIZE(to[jWidthi]);
	  //render the pixel
	  rayMarch(renderer_params, from, to[jWidthi], eps, pix_data[jWidthi],mandelBox_params, vecHolder[jWidthi]);
		 

	  //get the colour at this pixel
	  getColour(col[jWidthi], pix_data[jWidthi], renderer_params, from, to[jWidthi], vecHolder[jWidthi]);
      
	  //save colour into texture
	  k = (j * width + i)*3;
	  image[k+2] = (unsigned char)(col[jWidthi].x * 255);
	  image[k+1] = (unsigned char)(col[jWidthi].y * 255);
	  image[k]   = (unsigned char)(col[jWidthi].z * 255);
	}
      //printProgress((j+1)/(double)height,getTime()-time);
    }



  }

  int r;
  for(r=0; n > r; r++){
	printf("for generation %d value is %f %f %f \n",r,vecHolder[r].CamLight.x,vecHolder[r].CamLight.y,vecHolder[r].CamLight.z);
  }

  printf("\n rendering done:\n");
}
