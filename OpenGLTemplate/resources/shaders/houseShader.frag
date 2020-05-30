#version 400 core

in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
uniform sampler2D sampler0, sampler1;  // The texture sampler
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
in vec3 worldPosition, n;
uniform float fMinHeight , fMaxHeight;
out vec4 vOutputColour;		// The output colour

in vec4 p;

float rho = 0.0095f;
vec3 fogColour = vec3(0.55f);
vec4 fogColour1 = vec4(0.55f);
uniform float t;
uniform float fogpass;

//apply phong model for fog
vec3 PhongModel(vec4 p, vec3 n)
{		
vec3 vColour = PhongModel (p, normalize (n));
return vColour;
}

void main()
{
	//Retrieve fog value from game.cpp
		//No fog
		if (fogpass==2.0){
		rho = 0.0;}

		//dense fog
		if (fogpass==1.0){
		rho = 0.2;}

		//Pulse fog
		if(fogpass == 0.0){
		rho=0.0095;
		rho = rho * (0.5 + 0.5*sin(0.001*t));	//using sin and time to alternate between 0-1
		fogColour1 = fogColour1 * (1 + 0.5*sin(0.001*t));	//using sin and time to alternate between 0-1
		}

//retrieve textures
vec4 vTexColour0 = texture(sampler0, vTexCoord);
vec4 vTexColour1 = texture(sampler1, vTexCoord);

//mix the textures
float f = clamp((worldPosition.y - fMinHeight ) / (worldPosition.x - fMaxHeight ), 0.0 , 1.0);

vec4 vTexColour;
vTexColour = mix (vTexColour0, vTexColour1,f);
vOutputColour = mix (vTexColour0, vTexColour1,f);

float d = length(p.xyz);
float w = exp(-rho*d);

vOutputColour.rgb = mix(fogColour , vOutputColour.rgb ,w);	//add the fog

}
