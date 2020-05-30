#version 400 core

in vec3 vColour;			// Interpolated colour using colour calculated in the vertex shader
in vec2 vTexCoord;			// Interpolated texture coordinate using texture coordinate from the vertex shader
out vec4 vOutputColour;		// The output colour

uniform sampler2D sampler0, sampler1;  // The texture sampler
uniform samplerCube CubeMapTex;
uniform bool bUseTexture;    // A flag indicating if texture-mapping should be applied
uniform bool renderSkybox;
in vec3 worldPosition, n;
in vec4 vEyePosition;
uniform float fMinHeight , fMaxHeight;

in vec4 p;

uniform float t;
uniform float fogpass;

//float rho = 0.01f;
float rho = 0.0095f;

vec3 fogColour = vec3(0.55f);
vec4 fogColour1 = vec4(0.55f);

in vec3 vColour2;
out vec4 outputColour2;

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

		//Pulse fog
		if(fogpass == 0.0){
		rho=0.0095;
		rho = rho * (0.5 + 0.5*sin(0.001*t));	//using sin and time to alternate between 0-1
		fogColour1 = fogColour1 * (1 + 0.5*sin(0.001*t));	//using sin and time to alternate between 0-1
		}

	//render skybox + fog
	if (renderSkybox) {
		vOutputColour = texture(CubeMapTex, worldPosition) + fogColour1;
		//Dense fog
		if (fogpass==1.0){
		rho = 0.2;
		vOutputColour = fogColour1; }
		
	} else {

		// Get the texel colour from the texture sampler
		vec4 vTexColour = texture(sampler0, vTexCoord);	

		if (bUseTexture)
			vOutputColour = vTexColour*vec4(vColour, 1.0f);	// Combine object colour and texture 
		else
		//	vOutputColour = vec4(vColour, 1.0f);	// Just use the colour instead
	
		vOutputColour = vTexColour*vec4(vColour , 1.0f);
		float d = length(p.xyz);
		float w = exp(-rho*d);

		vOutputColour.rgb = mix(fogColour , vOutputColour.rgb ,w);	//add the fog
		
		//vOutputColour = vec4(vColour2, 1.0);
		}
	
}
