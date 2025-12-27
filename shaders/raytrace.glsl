#version 460
layout(local_size_x = 16, local_size_y = 20) in;

layout(rgba32f, binding = 0) uniform image2D img_output;

layout(std430, binding = 1) buffer Pos {
	vec4 Positions[];
};

layout(std430, binding = 2) buffer Index {
	unsigned int Indices[];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

float camDist = 0.1;
float width = 2 * camDist * tan(radians(22.5));
float height = 0.75 * width;

struct Triangle {
    vec3 v1;
    vec3 v2;
    vec3 v3;
    vec3 normal;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

float getColourFactor(Triangle triangle, Ray ray) {
    
    float factor = max(0.0, 5 * dot(triangle.normal, -ray.direction));
    return factor;

}

bool rayIntersectedTriangle(Ray ray, Triangle triangle) {
    vec3 side1 = triangle.v2 - triangle.v1;
    vec3 side2 = triangle.v3 - triangle.v1;
    vec3 negDir = - ray.direction;

    mat3 checkIntersect = mat3(negDir, side1, side2);

    float det = determinant(checkIntersect);

    if (abs(det) > 0.0000001){
        mat3 inverseIntersect = inverse(checkIntersect);
        vec3 intPoint = inverseIntersect * (ray.origin - triangle.v1);
        float sum = intPoint.y + intPoint.z;
        return sum >= 0 && sum <= 1 && intPoint.y >= 0 && intPoint.z >= 0;
    }
    else {
        return false;
    }
    
};

void main() {

  ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
  float x = float(pix.x);
  float y = float(pix.y);
  vec3 origin = vec3(0.0, 10.0, 0.0);
  float xPrime = (x/800.0 * width) - width/2;
  float yPrime = (y/600.0 * height) - height/2;
  vec3 direction = vec3(xPrime, -camDist, -yPrime);

  struct Ray ray = {origin, direction};

  for (int i = 0; i < Indices.length(); i += 3) {
    vec3 v1 = Positions[Indices[i]].xyz;
    vec3 v2 = Positions[Indices[i + 1]].xyz;
    vec3 v3 = Positions[Indices[i + 2]].xyz;

    vec3 s1 = v2 - v1;
    vec3 s2 = v3 - v1;
    vec3 normal = cross(s1, s2);

    struct Triangle triangle = {v1, v2, v3, normal};

    bool didHit = rayIntersectedTriangle(ray, triangle);

    
    if (didHit){
        float colFactor = getColourFactor(triangle, ray);
        vec4 colour = vec4(1.0, 0.0, 0.0, 1.0);
        imageStore(img_output, pix, colour);
        break;
    }

    
  }

}