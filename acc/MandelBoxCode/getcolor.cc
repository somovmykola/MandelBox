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
#include "color.h"
#include "renderer.h"
#include "vector3d.h"
#include <cmath>
//#include <algorithm>

using namespace std;

//---lightning and colouring---------

// 1.27536;
// 0.48193;
//-----------------------------------



inline void Max(double &d, double x, double y){
	if(x > y)
	  d = x;
        else
          d = y;
}

inline void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV, SuperVec &vecHolder)
{

  linMinus(n,1.0,vecHolder.nn);
  double CamLightMin = 0.3;
  double CamLightW = 1.8;

  vecHolder.CamLight.x = 1.0;
  vecHolder.CamLight.y = 1.0;
  vecHolder.CamLight.z = 1.0;

  double dotResult;
  dot(dotResult, vecHolder.nn, direction);

  double ambient;
  Max(ambient, CamLightMin, dotResult);
  ambient*= CamLightW;

  linMult(color, ambient, vecHolder.colorResult);
  mult(vecHolder.CamLight, vecHolder.colorResult, outV);

}

#pragma acc routine seq
void getColour(vec3 &returnVal, const pixelData &pixData, const RenderParams &render_params,
	       const vec3 &from, const vec3  &direction, SuperVec &vecHolder)
{
  vecHolder.base.x = 1.0;
  vecHolder.base.y = 1.0;
  vecHolder.base.z = 1.0;

  vecHolder.back.x = 0.4;
  vecHolder.back.y = 0.4;
  vecHolder.back.z = 0.4;

  //colouring and lightning
  vecHolder.hitColor = vecHolder.base;
  
  if (pixData.escaped == false) 
    {
      //apply lighting
      lighting(pixData.normal, vecHolder.hitColor, pixData.hit, direction, vecHolder.hitColor, vecHolder);
      
      //add normal based colouring
      if(render_params.colourType == 0 || render_params.colourType == 1)
	{

          mult(vecHolder.hitColor, pixData.normal, vecHolder.hitColor);

          linPlus(vecHolder.hitColor, 1.0, vecHolder.hitColor);
          linDivide(vecHolder.hitColor, 2.0, vecHolder.hitColor);

          linMult(vecHolder.hitColor, render_params.brightness, vecHolder.hitColor);

          superClamp(vecHolder.hitColor, 0.0, 1.0);
          mult(vecHolder.hitColor, vecHolder.hitColor, vecHolder.hitColor);


	}
      if(render_params.colourType == 1)
	{
	  //"swap" colors
	  double t = vecHolder.hitColor.x;
	  vecHolder.hitColor.x = vecHolder.hitColor.z;
	  vecHolder.hitColor.z = t;
	}
    }
  else 
    //we have the background colour
    vecHolder.hitColor = vecHolder.back;
  
  returnVal = vecHolder.hitColor;
}
