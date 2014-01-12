#include "roadGenerator.h"
#include "utils.h"

CSpline* generateRoad(int numControlPoints, float radius, float radiusJitter, float directionJitterRad)
{
	CSpline* spline = new CSpline();

	int n = numControlPoints; // tired of long attribute names?
	for (int i=0; i<n; i++)
	{
		float phi = (i / (float)n) * 2 * PI;
		float r = radius + frandm() * radiusJitter;
		vec3 pos(cos(phi) * r, frandm() * radius * 0.1f, sin(phi) * r);
		//vec3 pos(cos(phi) * r, 0, sin(phi) * r);

		float dirphi = phi + PI2 + frandm() * directionJitterRad;
		float zdirphi = frand() * PI * 0.05f;
		vec3 dir(cos(dirphi) * cos(zdirphi), sin(zdirphi), sin(dirphi) * cos(zdirphi));
		//vec3 dir(cos(dirphi) * cos(zdirphi), 0, sin(dirphi) * cos(zdirphi));
		dir *= radius * 0.5f;

		float g1, g2; // two random angles for random curvature vector
		g1 = frand() * 2 * PI;
		g2 = frandm() * PI * 0.1f;
		vec3 cur(cos(g1) * cos(g2), sin(g2), sin(g1) * cos(g2));
		//vec3 cur(cos(g1) * cos(g2), 0, sin(g1) * cos(g2));
		cur *= radius * 2;

		float rup = frand();
		float upphi = frand() * 2 * PI;
		vec3 up(rup * cos(phi), 4, rup * sin(phi));

		spline->addControlPoint(SControlPoint(pos, dir, cur, up));
	}
	return spline;
}