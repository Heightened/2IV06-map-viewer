#version 130

in vec3 fPosition;
in vec3 fNormal;
in vec3 fColor;

in vec3 eyeVector;
in vec3 lightVector;
in vec3 normalVector;

out vec3 color;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	vec3 light = vec3(50,50,50);

	// Color noise
	color = fColor;// * ((3 + rand(fPosition.xz))/3.5f);

	// Light emission properties
	vec3 LightColor = vec3(1,0.8f,0.8f);
	float LightPower = 1500.0f;
	
	// Material properties
	vec3 MaterialDiffuseColor =	color;
	vec3 MaterialAmbientColor = 0.1f * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Light angles
	float distance = length( light - fPosition );
	vec3 n = normalize(normalVector);
	vec3 l = normalize(lightVector);
	float cosTheta = clamp( dot( n,l ), 0,1 );
	vec3 E = normalize(eyeVector);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	// Combine to calculate color
	color = /*vec3(0.2f,0.2f,0.2f) +*/ LightPower / (distance*distance) * (MaterialDiffuseColor * cosTheta + MaterialSpecularColor * cosAlpha);
}
