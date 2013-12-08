#define MAX_LIGHTS 8
#define NUM_LIGHTS 3

varying vec3 normal, lightDir[MAX_LIGHTS], eyeVec;

void main()
{	
  gl_Position = ftransform();		
  normal = gl_NormalMatrix * gl_Normal;
  vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
  eyeVec = -vVertex.xyz;
  int i;
  for (i=0; i<NUM_LIGHTS; ++i)
    lightDir[i] = vec3(gl_LightSource[i].position.xyz - vVertex.xyz);
}