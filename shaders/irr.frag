uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D stencilMap;

varying vec3 ViewDirection;
varying vec3 LightDirection1;
varying float LightDistMultiplier;

void main( void )
{
   
   vec3  fvLightDirection1 = normalize( LightDirection1 );

   
   vec3  fvNormal         = texture2D( normalMap, gl_TexCoord[0].xy ).yxz;
   fvNormal.xy*=2.0;
   fvNormal.xy-=1.0;
   
   vec2 tc = gl_TexCoord[0].xy;
   tc.y *= 0.1;
   float stencil = texture2D(stencilMap, tc).r;
   
   fvNormal=(vec3(0.0,0.0,1.0)-fvNormal)*2.1+fvNormal;
   
   fvNormal=normalize(fvNormal);
   
   float fNDotL1           = max(dot(fvNormal, fvLightDirection1),0.0)-0.1;
   vec3  fvReflection1     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection1 ); 
   vec3  fvViewDirection  = normalize( ViewDirection );
   float fRDotV1          = max( 0.0, dot( fvReflection1, fvViewDirection ) );

   
   vec4  fvBaseColor      = texture2D( colorMap, gl_TexCoord[0].xy );
   
   vec4  fvTotalAmbient   = vec4(0.3,0.3,0.3,1) * fvBaseColor;
   
   vec4  fvTotalDiffuse   = vec4(1,1,1,1) * fNDotL1* fvBaseColor; 
   vec4  fvTotalSpecular  = fNDotL1*vec4(1,0.8,0.5,0) * ( pow( fRDotV1, 2.0 ) ) * stencil;
   //vec4  fvTotalSpecular  = fNDotL1*vec4(0.8,0.8,1,0) * ( pow( fRDotV1, 2.0 ) ) * stencil;
   
   vec4 color = fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular;
   gl_FragColor = color;

}