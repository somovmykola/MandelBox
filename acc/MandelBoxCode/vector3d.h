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


typedef struct {
	//rayMarch()
	vec3 p;
	vec3 dirResult;
	vec3 normPos;

	//normal()
	vec3 normResult;
	vec3 resultP1, resultP2, resultP3;
	vec3 resultM1, resultM2, resultM3;
        vec3 e1,e2,e3;

	//lighting()
	vec3 nn;
	vec3 colorResult;
	vec3 CamLight;
	
	//getColor()
	vec3 base;
	vec3 back;
	vec3 hitColor;

	//MandleBoxDE()
	vec3 temp;
	

}	SuperVec;


#define SetDoublePoint(p,v){ \
	p.x=v[0]; \
	p.y=v[1]; \
	p.z=v[2]; \
}

#define compare(p,v,b){\
	b = (p.x == v.x && p.y == v.y && p.z == v.z);\
}

#define plus(p,v,b){\
	b.x=v.x+p.x; \
	b.y=v.y+p.y; \
	b.z=v.z+p.z;\
}

#define minus(p,v,b){\
	b.x=p.x-v.x; \
	b.y=p.y-v.y; \
	b.z=p.z-v.z;\
}

#define SubP(p,v,b){\
	b.x=p.x-v[0]; \
	b.y=p.y-v[1]; \
	b.z=p.z-v[2];\
}

#define negative(p,b){ \
	b.x=p.x*-1;	\
	b.y=p.y*-1; \
	b.z=p.z*-1; \
}

#define linDivide(p,S,b){ \
	double thing = 1.0/S; \
	b.x=p.x*thing; \
	b.y=p.y*thing; \
	b.z=p.z*thing; \
}

#define divide(p,v,b){ \
	b.x=p.x/v.x; \
	b.y=p.y/v.y; \
	b.z=p.z/v.z; \
}

#define mult(p,v,b){ \
	b.x=p.x*v.x; \
	b.y=p.y*v.y; \
	b.z=p.z*v.z; \
}

#define linMult(p,S,b){ \
	b.x=p.x*S; \
	b.y=p.y*S; \
	b.z=p.z*S; \
}

#define linPlus(p,S,b){ \
	b.x=p.x+S; \
	b.y=p.y+S; \
	b.z=p.z+S; \
}

#define linMinus(p,S,b){ \
	b.x=p.x-S; \
	b.y=p.y-S; \
	b.z=p.z-S; \
}

#define plusCurrent(p,v){ \
	p.x+=v.x; \
	p.y+=v.y; \
	p.z+=v.z; \
}

#define minusCurrent(p,v){ \
	p.x-=v.x; \
	p.y-=v.y; \
	p.z-=v.z; \
}

#define dot(d,p,v){ \
	d=( p.x*v.x + p.y*v.y + p.z*v.z ); \
}

#define magnitude(m,p){ \
	m = sqrt( p.x*p.x + p.y*p.y + p.z*p.z ); \
}

#define NORMALIZE(p) {					\
    double fMag = ( p.x*p.x + p.y*p.y + p.z*p.z );	\
    if (fMag != 0)					\
      {							\
		double fMult = 1.0/sqrt(fMag);			\
		p.x *= fMult;					\
		p.y *= fMult;					\
		p.z *= fMult;					\
      }							\
  }

#define pSub(p, a, b){ \
  	p.x = a[0] - b[0]; \
  	p.y = a[1] - b[1]; \
  	p.z = a[2] - b[2]; \
}

#define pSubStar(p, a, b){ \
  	p.x = a.x - b[0]; \
  	p.y = a.y - b[1]; \
  	p.z = a.z - b[2]; \
}	

#define clamp(d, min, max){	\
  	const double t = d < min ? min : d; \
  	d= (t > max ? max : t); \
}

#define superClamp(p, min, max){ \
	const double t = p.x < min ? min : p.x; \
	const double s = p.y < min ? min : p.y; \
	const double u = p.z < min ? min : p.z; \
	p.x = (t > max ? max : t); \
	p.y = (s > max ? max : s); \
	p.z = (u > max ? max : u); \
}

#endif
