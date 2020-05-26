#version 120

uniform vec4        Ambient_uniform;
uniform vec4        Specular_uniform;
uniform float       SpecularPower_uniform;
uniform sampler2D   Texture_uniform;

varying vec3    ViewDirection;
varying vec3    LightDirection;
varying vec3    Normal;
varying vec2    texCoord;
uniform vec3    Colour_uniform;

void main( void )
{
   vec3  fvLightDirection = normalize(LightDirection );
   vec3  fvNormal         = normalize( Normal );
   float fNDotL           = dot( fvNormal, fvLightDirection );

   vec3  fvReflection     = 0.6 *normalize( ( ( 2.0 * fvNormal ) * fNDotL ) - fvLightDirection );
   vec3  fvViewDirection  = normalize( ViewDirection );
   float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );

   vec4  fvTotalDiffuse   = fNDotL * vec4(Colour_uniform, 1.0) * texture2D(Texture_uniform, texCoord);
   vec4  fvTotalSpecular  = 10* Specular_uniform * ( pow( fRDotV, SpecularPower_uniform ) );

   gl_FragColor = vec4((Ambient_uniform.rgb + fvTotalDiffuse.rgb + fvTotalSpecular.rgb), 1.0);
}
