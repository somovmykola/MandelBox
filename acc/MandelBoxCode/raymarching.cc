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
#include <assert.h>
#include <algorithm>
#include <stdio.h>

#include "color.h"
#include "renderer.h"
#include "distance_est.cc"
#include "mandelboxde.cc"


extern void DE(double &returnVal, const vec3 &p, const MandelBoxParams &mandelBox_params, SuperVec &vecHolder);


inline void normal (const vec3 & p, vec3 & normal, const MandelBoxParams &mandelBox_params, SuperVec &vecHolder);

#pragma acc routine seq
void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3  &direction, double eps,
	      pixelData& pix_data, const MandelBoxParams &mandelBox_params, SuperVec &vecHolder)
{
  double dist = 0.0;
  double totalDist = 0.0;
  
  // We will adjust the minimum distance based on the current zoom

  double epsModified = 0.0;
  
  int steps=0;
  //vec3 p;
  //vec3 dirResult;
  do 
    {      
      linMult(direction, totalDist, vecHolder.dirResult);
      plus(from,vecHolder.dirResult,vecHolder.p); 
     // p = from + direction * totalDist;
     DE(dist, vecHolder.p, mandelBox_params, vecHolder);
      
      totalDist += .95*dist;
      
      epsModified = totalDist;
      epsModified*=eps;
      steps++;
    }
  while (dist > epsModified && totalDist <= render_params.maxDistance && steps < render_params.maxRaySteps);
  
  //vec3 hitNormal;
  if (dist < epsModified) 
    {
      //we didnt escape
      pix_data.escaped = false;
      
      // We hit something, or reached MaxRaySteps
      pix_data.hit = vecHolder.p;
      
      //figure out the normal of the surface at this point
      //vec3 normPos;// = p - direction * epsModified;

      linMult(direction,epsModified,vecHolder.normPos);
      minus(vecHolder.p,vecHolder.normPos,vecHolder.normPos);
      normal(vecHolder.normPos, pix_data.normal, mandelBox_params, vecHolder);
    }
  else 
    //we have the background colour
    pix_data.escaped = true;
}


inline void normal(const vec3 & p, vec3 & normal, const MandelBoxParams &mandelBox_params, SuperVec &vecHolder)
{
  // compute the normal at p
  const double sqrt_mach_eps = 1.4901e-08;

  double mag;
  //vec3 normResult = normal;
  //vec3 resultP1, resultP2, resultP3;
  //vec3 resultM1, resultM2, resultM3;

  vecHolder.normResult = normal;
  magnitude(mag,p);

  double eps = std::max( mag, 1.0 )*sqrt_mach_eps;


  //double eps = std::max( p.Magnitude(), 1.0 )*sqrt_mach_eps;

  //vec3 e1 = {eps, 0, 0};
  vecHolder.e1.x = eps;
  vecHolder.e1.y = 0;
  vecHolder.e1.z = 0;

  //vec3 e2 = {0  , eps, 0};
  vecHolder.e2.x = 0;
  vecHolder.e2.y = eps;
  vecHolder.e2.z = 0;

  //vec3 e3 = {0  , 0, eps};
  vecHolder.e3.x = 0;
  vecHolder.e3.y = 0;
  vecHolder.e3.z = eps;


  
//  normal = vec3(DE(p+e1,mandelBox_params)-DE(p-e1,mandelBox_params), DE(p+e2,mandelBox_params)-DE(p-e2,mandelBox_params), DE(p+e3,mandelBox_params)-DE(p-e3,mandelBox_params));
  
 // normal.Normalize();

  plus(p,vecHolder.e1,vecHolder.resultP1);
  plus(p,vecHolder.e2,vecHolder.resultP2);
  plus(p,vecHolder.e3,vecHolder.resultP3);
  minus(p,vecHolder.e1,vecHolder.resultM1);
  minus(p,vecHolder.e2,vecHolder.resultM2);
  minus(p,vecHolder.e3,vecHolder.resultM3);

  double x1,x2,y1,y2,z1,z2;

  DE(x1,vecHolder.resultP1, mandelBox_params, vecHolder);
  DE(x2,vecHolder.resultM1, mandelBox_params, vecHolder);

  DE(y1,vecHolder.resultP2, mandelBox_params, vecHolder);
  DE(y2,vecHolder.resultM2, mandelBox_params, vecHolder);

  DE(z1, vecHolder.resultP3, mandelBox_params, vecHolder);
  DE(z2, vecHolder.resultM3, mandelBox_params, vecHolder);

  double normalVec [3] = {x1-x2, y1-y2, z1-z2};

  //double normalVec [3] = {DE(resultP1, mandelBox_params)-DE(resultM1, mandelBox_params), DE(resultP2, mandelBox_params)-DE(resultM2, mandelBox_params), DE(resultP3, mandelBox_params)-DE(resultM3, mandelBox_params)};


  SetDoublePoint(vecHolder.normResult, normalVec);
  NORMALIZE(vecHolder.normResult);
  normal = vecHolder.normResult;
 
}
