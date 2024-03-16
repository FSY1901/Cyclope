#type vertex

#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 TexCoords;

out vec2 texCoords;

void main(){
	gl_Position = vec4(inPos, 1.0);
	texCoords = TexCoords;
}








#type fragment

#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform float iTime;

float sdSphere(vec3 p, float r){
	return length(p) - r;
}

float sdCube(vec3 p, vec3 b){
	vec3 q = abs(p) - b;
	return length(max(q, 0.0) + min(max(q.x, max(q.y, q.z)), 0.0));
}

float smin(float a, float b, float k){
	float h = max(k - abs(a-b), 0.0)/k;
	return min(a, b) - h*h*h*k*(1.0/6.0);
}

vec4 sminV(vec4 a, vec4 b, float k){
	float h = clamp(0.5 + 0.5*(b.w-a.w)/k, 0.0, 1.0);
	vec3 color = mix(b.xyz, a.xyz, h);
	float dist = mix(b.w, a.w, h) - k*h*(1.0-h);
	return vec4(color, dist);
}

vec4 map(vec3 p){
	vec3 spherePos = vec3(sin(iTime)*3, 0.0, 0.0);
	vec3 sphereColor = vec3(1.0, 0.0, 0.0);
	vec4 sphere = vec4(sphereColor.xyz, sdSphere(p - spherePos, 1));

	vec3 boxColor = vec3(0.0, 0.0, 1.0);
	//vec4 box = vec4(boxColor.xyz, sdCube(p, vec3(.75)));
	vec4 box = vec4(boxColor.xyz, sdSphere(p, 1));

	vec4 res = vec4(sminV(sphere, box, 1.3));
	//float distance = smin(sphere.w, box.w, 1.3);
	//vec4 res = vec4(1.0, 0.0, 0.0, distance);
	return res;
}

vec3 calculate_normal(in vec3 p)
{
    const vec3 small_step = vec3(0.001, 0.0, 0.0);

    float gradient_x = map(p + small_step.xyy).w - map(p - small_step.xyy).w;
    float gradient_y = map(p + small_step.yxy).w - map(p - small_step.yxy).w;
    float gradient_z = map(p + small_step.yyx).w - map(p - small_step.yyx).w;

    vec3 normal = vec3(gradient_x, gradient_y, gradient_z);

    return normalize(normal);
}

void main(){

	vec2 uv = (texCoords * 2) - 1;

	vec3 ro = vec3(0, 0, -3);
	vec3 rd = normalize(vec3(uv, 1));

	vec3 col = vec3(0);

	float t = 0.;
	vec4 res;
	for(int i = 0; i < 80; i++){
		vec3 p = ro + rd * t;

		res = map(p);

		vec3 normal = calculate_normal(p);

		vec3 light_position = vec3(2.0, -2.0, 3.0);

		vec3 direction_to_light = normalize(p - light_position);

		float diffuse_intensity = max(0.0, dot(normal, direction_to_light));

		col = res.xyz * diffuse_intensity;

		t += res.w;

		if(res.w < .001 || res.w > 100) break;
	}
	if(t > 4 ){
		col = vec3(1.0, 1.0, 1.0);
	}
	color = vec4(col, 1.0);
}