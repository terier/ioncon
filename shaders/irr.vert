varying vec3 ViewDirection;
varying vec3 LightDirection1;
varying float LightDistMultiplier;
   
float getLengthSQR (vec3 vec)
{
return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
} 
   
void main( void )
{     
   gl_Position = ftransform();
   gl_TexCoord[0] = gl_MultiTexCoord0;
   
   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;
   vec3 fvLightPos1 = gl_LightSource[0].position.xyz;

   
   vec3 fvViewDirection  =  - fvObjectPosition.xyz;
   
   vec3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz);

   LightDistMultiplier = 1.0;///(getLengthSQR (fvLightDirection1)/1000.0);
     
   vec3 fvNormal       = gl_NormalMatrix * gl_Normal;
   
   vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);
   vec3 fvBinormal =cross(fvTangent,gl_Normal);  
   fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);
   fvBinormal=gl_NormalMatrix*fvBinormal;

     
   ViewDirection.x  = dot( fvTangent, fvViewDirection );
   ViewDirection.y  = dot( fvBinormal, fvViewDirection );
   ViewDirection.z  = dot( fvNormal, fvViewDirection );
   
   LightDirection1.x  = dot( fvTangent, fvLightDirection1.xyz );
   LightDirection1.y  = dot( fvBinormal, fvLightDirection1.xyz );
   LightDirection1.z  = dot( fvNormal, fvLightDirection1.xyz );   
}