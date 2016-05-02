#ifndef vec3_h
#define vec3_h

#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif

#include <cmath>

typedef struct {
	double x, y, z;
}	vec3;


#define SetDoublePoint(a,v){	\
	a.x=v[0]; 		\
	a.y=v[1]; 		\		     
	a.z=v[2]; 		\		
}											         

#define compare(a,v,b){		\
	b = (a.x == v.x && a.y == v.y && a.z == v.z);		}

#define plus(a,v,b){		\
	b.x=v.x+a.x;		\
	b.y=v.y+a.y;		\
	b.z=v.z+a.z;		\
}

#define minus(a,v,b){		\
	b.x=a.x-v.x;		\
	b.y=a.y-v.y;		\
	b.z=a.z-v.z;		\
}

#define SubP(a,v,b){		\
	b.x=a.x-v[0];		\
	b.y=a.y-v[1];		\
	b.z=a.z-v[2];		\
}

#define negative(a,b){		\	
	b.x=a.x*-1;		\
	b.y=a.y*-1;		\	
	b.z=a.z*-1;		\
}

#define linDivide(a,S,b){	\
	double thing = 1.0/S;	\
	b.x=a.x*thing;		\
	b.y=a.y*thing;		\
	b.z=a.z*thing;		\	
}

#define divide(a,v,b){		\
	b.x=a.x/v.x;		\
	b.y=a.y/v.y;		\
	b.z=a.z/v.z;		\
}

#define mult(a,v,b){		\
	b.x=a.x*v.x;		\
	b.y=a.y*v.y;		\
	b.z=a.z*v.z;		\
}		

#define linMult(a,S,b){		\
	b.x=a.x*S;		\
	b.y=a.y*S;		\
	b.z=a.z*S;		\
}

#define linPlus(a,S,b){		\
	b.x=a.x+S;		\
	b.y=a.y+S;		\
	b.z=a.z+S;		\
}

#define linMinus(a,S,b){	\
	b.x=a.x-S;		\
	b.y=a.y-S;		\
	b.z=a.z-S;		\
}			

#define plusCurrent(a,v){	\
	a.x+=v.x;		\
	a.y+=v.y;		\
	a.z+=v.z;		\
}

#define minusCurrent(a,v){	\
	a.x-=v.x;		\
	a.y-=v.y;		\
	a.z-=v.z;		\
}

#define dot(d,a,v){		\
	d=( a.x*v.x + a.y*v.y + a.z*v.z );	\	
}

#define magnitude(m,a){					\
	m = sqrt( a.x*a.x + a.y*a.y + a.z*a.z );	\
}

#define NORMALIZE(a) {					\
    double fMag = ( a.x*a.x + a.y*a.y + a.z*a.z );	\
    if (fMag != 0)					\
      {							\
		double fMult = 1.0/sqrt(fMag);		\
		a.x *= fMult;				\
		a.y *= fMult;				\
		a.z *= fMult;				\
      }							\
  }

#define pSub(a, c, b){					\
  	a.x = c[0] - b[0];				\
  	a.y = c[1] - b[1];				\
  	a.z = c[2] - b[2];				\
}									

#define clamp(d, min, max){				\
  	const double fcomp = d < min ? min : d;		\
  	d= (fcomp > max ? max : fcomp);				\
}				

#define superClamp(a, min, max){			\
	const double tcomp = a.x < min ? min : a.x;		\
	const double scomp = a.y < min ? min : a.y;		\
	const double ucomp = a.z < min ? min : a.z;		\
	a.x = (tcomp > max ? max : tcomp);			\
	a.y = (scomp > max ? max : scomp);			\
	a.z = (ucomp > max ? max : ucomp);			\
}				

#endif
