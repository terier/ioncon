#include "roadGenerator.h"
#include "utils.h"

CSpline* generateRoad(int numControlPoints, float radius, float radiusJitter, float directionJitterRad)
{
	CSpline* spline = new CSpline();

	int n = numControlPoints; // tired of long attribute names?
	for (int i=0; i<n; i++)
	{
		float phi = (i / (float)n) * 2 * PI;
		float r = radius + frand() * radiusJitter;
		vec3 pos(cos(phi) * r, frand() * radius * 0.2f, sin(phi) * r);
		float dirphi = phi + PI2 + frandm() * directionJitterRad;
		float zdirphi = frand() * PI * 0.15f;
		vec3 dir(cos(dirphi) * cos(zdirphi), sin(zdirphi), sin(dirphi) * cos(zdirphi));
		dir *= radius;
		float g1, g2; // two random angles for random curvature vector
		g1 = frand() * 2 * PI;
		g2 = frand() * 2 * PI;
		vec3 cur(cos(g1) * cos(g2), sin(g2), sin(g1) * cos(g2));
		float rup = frand();
		float upphi = frand() * 2 * PI;
		vec3 up(rup * cos(phi), 3, rup * sin(phi));
		spline->addControlPoint(SControlPoint(pos, dir, cur, up));
	}
	return spline;
}