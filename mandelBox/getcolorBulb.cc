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
#include <algorithm>
#include <stdio.h>

using namespace std;

void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV)
{
  vec3 CamLight = {1.0, 1.0, 1.0};
  double CamLightW = 1.8;
  double CamLightMin = 0.3;
  vec3 nn;
  vec3 colorResult;
  double dotResult;
  linMinus(n,1.0,nn);
  dot(dotResult, nn, direction);
  double ambient = max( CamLightMin, dotResult )*CamLightW;
  linMult(color, ambient, colorResult);
  mult(CamLight, colorResult, outV);
}

vec3 getColour(const pixelData &pixData, const RenderParams &render_params,
	       const vec3 &from, const vec3  &direction)
{
  vec3 hitColor;
  vec3 baseColor = {1.0, 1.0, 1.0};
  vec3 backColor = {1.0, 1.0, 1.0};
  hitColor = baseColor;
  if (pixData.escaped == false) 
    { 
      lighting(pixData.normal, hitColor, pixData.hit, direction, hitColor);

      if(render_params.colourType == 0 || render_params.colourType == 1)
	{
	  mult(hitColor, pixData.normal, hitColor);

	  linPlus(hitColor, 1.0, hitColor);
	  linDivide(hitColor, 2.0, hitColor);

	  linMult(hitColor, render_params.brightness, hitColor);
	  
	  //gamma correction
	  superClamp(hitColor, 0.0, 1.0);
	  mult(hitColor, hitColor, hitColor);
	}
      if(render_params.colourType == 1)
	{
	  double x = hitColor.x;
	  hitColor.x = hitColor.z;
	  hitColor.z = x;
	}
    }
  else 
    hitColor =backColor;
  
  return hitColor;
}
