#version 460
layout(local_size_x = 16, local_size_y = 20) in;

layout(rgba32f, binding = 0) uniform image2D img_output;

layout(std430, binding = 1) buffer Pos {
	vec4 Positions[];
};

layout(std430, binding = 2) buffer Index {
	unsigned int Indices[];
};

layout(std430, binding = 3) buffer Normal {
	vec4 Normals[];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 camPos = vec3(0.0, 10.0, 0.0);
float camDist = 0.1;
float width = 2 * camDist * tan(radians(45));
float height = 0.75 * width;
vec3 lightPos = vec3(-25.0, 0.0, 0.0);
float lightRadius = 5.0;

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

float getColourFactor(Triangle triangle, vec3 rayDirection) {
    
    float factor = max(0.0, dot(triangle.normal, rayDirection));
    return factor;
}



vec3 rayIntersectedTriangle(Ray ray, Triangle triangle) {
    vec3 side1 = triangle.v2 - triangle.v1;
    vec3 side2 = triangle.v3 - triangle.v1;
    vec3 negDir = - ray.direction;

    mat3 checkIntersect = mat3(negDir, side1, side2);

    float det = determinant(checkIntersect);

    if (abs(det) > 0.0000001){
        mat3 inverseIntersect = inverse(checkIntersect);
        return inverseIntersect * (ray.origin - triangle.v1);
    }
    else {
        return vec3(-1, -1, -1);
    }
};

bool checkIfHit(vec3 barycentricPoint) {
    float sum = barycentricPoint.y + barycentricPoint.z;
    return sum >= 0 && sum <= 1 && barycentricPoint.y >= 0 && barycentricPoint.z >= 0;
}

Ray getReflectedRay(vec3 hitPoint, vec3 incidentDir, vec3 normal) {

    vec3 reflectedDir = normalize(incidentDir - 2*dot(incidentDir, normal)*normal);

    struct Ray reflectedRay = {hitPoint, reflectedDir};

    return reflectedRay;

}

bool reachesLightSource(Ray ray) {
    
    float a = 1;
    float b = 2*dot(ray.direction, ray.origin) - 2*dot(ray.direction, lightPos);
    float c = -pow(lightRadius, 2.0) + pow(length(ray.origin), 2.0) - 2*dot(ray.origin, lightPos) + pow(length(lightPos), 2.0);

    float discriminant = pow(dot(ray.direction, ray.origin - lightPos) , 2.0) - (pow(length(ray.origin - lightPos) ,2.0) - pow(lightRadius,2.0));
    //pow(b, 2.0) - 4 * c;

    float root1 = (-b + discriminant)/2;
    float root2 = (-b - discriminant)/2;

    return discriminant >= 0 && (root1 > 0 || root2 > 0);

}


struct Intersection {
    vec3 hitPoint;
    Triangle hitTriangle;
    bool isHit;
    
};

Intersection findClosestTriangle (Ray ray) {
    float curMinT = 1000000000.0;
    struct Triangle curMaxTriangle = {vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0)};
    vec3 maxHitPoint = vec3(0.0, 0.0, 0.0);
    bool hitAnything = false;
    struct Intersection inter;


    for (int i = 0; i < Indices.length(); i += 3) {
    vec3 v1 = Positions[Indices[i]].xyz;
    vec3 v2 = Positions[Indices[i + 1]].xyz;
    vec3 v3 = Positions[Indices[i + 2]].xyz;

    vec3 n1 = Normals[Indices[i]].xyz;
    vec3 n2 = Normals[Indices[i + 1]]. xyz;
    vec3 n3 = Normals[Indices[i + 2]].xyz;

    vec3 s1 = v2 - v1;
    vec3 s2 = v3 - v1;
    vec3 normal = cross(s1, s2);
    normal = normalize(normal);
    if (dot(normal, -ray.direction) < 0) {
       normal = -normal;
    }
    // vec3 normal = vec3(0, 0, 0);

    struct Triangle triangle = {v1, v2, v3, normal};
    vec3 hitPointBary = rayIntersectedTriangle(ray, triangle);
    // float u = hitPointBary.y;
    // float v = hitPointBary.z;
    // float w = 1 - u - v;

    // triangle.normal = normalize(w * n1 + u * n2 + v * n3);

    vec3 hitPoint = camPos + hitPointBary.x * ray.direction;
    //float depth = hitPoint.y;

    bool didHit = checkIfHit(hitPointBary);
    
    if (didHit && hitPointBary.x > 0.0) {
        if (hitPointBary.x <= curMinT) {            
            curMaxTriangle = triangle;
            maxHitPoint = hitPoint;
            curMinT = hitPointBary.x;
            hitAnything = true;
      }
    } 
  }

  inter.hitPoint = maxHitPoint;
  inter.hitTriangle = curMaxTriangle;
  inter.isHit = hitAnything;

  return inter;
}

bool getPixColour(Ray ray, int depth) {
    //Ray rayStack[5];
    //int idx = 0;
    for (unsigned int i = 0; i < depth; i ++) {
        if (reachesLightSource(ray)) {
            return true;
        }
        //rayStack[idx] = ray;
        //idx += 1;
        Intersection closestInter = findClosestTriangle(ray);
        ray = getReflectedRay(closestInter.hitPoint, ray.direction, closestInter.hitTriangle.normal);
    }

    return false;
    
}

void main() {

  ivec2 pix = ivec2(gl_GlobalInvocationID.xy);
  float x = float(pix.x);
  float y = float(pix.y);
//   vec3 origin = vec3(0.0, 10.0, 0.0);
  float xPrime = (x/800.0 * width) - width/2;
  float yPrime = (y/600.0 * height) - height/2;
  vec3 direction = normalize(vec3(xPrime, -camDist, -yPrime));
  struct Ray ray = {camPos, direction};

  Intersection firstInter = findClosestTriangle(ray);
  Ray reflection = getReflectedRay(firstInter.hitPoint, ray.direction, firstInter.hitTriangle.normal);

  if (firstInter.isHit) {
      bool reachedLightSource = getPixColour(reflection, 10);
      float colFactor = getColourFactor(firstInter.hitTriangle, reflection.direction);
      vec4 colour = vec4(colFactor * float(reachedLightSource) * 1.0, 0.0, 0.0, 1.0);//vec4(colFactor * 1.0, 0.0, 0.0, 1.0);
      imageStore(img_output, pix, colour);
  }
//   float curMinT = 1000000000.0;
//   struct Triangle curMaxTriangle = {vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0)};
//   vec3 maxHitPoint = vec3(0.0, 0.0, 0.0);
//   bool hitAnything = false;


//   for (int i = 0; i < Indices.length(); i += 3) {
//     vec3 v1 = Positions[Indices[i]].xyz;
//     vec3 v2 = Positions[Indices[i + 1]].xyz;
//     vec3 v3 = Positions[Indices[i + 2]].xyz;

//     vec3 s1 = v2 - v1;
//     vec3 s2 = v3 - v1;
//     vec3 normal = cross(s1, s2);
//     normal = normalize(normal);
//     if (dot(normal, -ray.direction) < 0) {
//         normal = -normal;
//     }

//     //vec3 normal = normalize((Normals[i].xyz + Normals[i + 1].xyz + Normals[i + 2].xyz));

//     struct Triangle triangle = {v1, v2, v3, normal};
//     vec3 hitPointBary = rayIntersectedTriangle(ray, triangle);
//     vec3 hitPoint = camPos + hitPointBary.x * direction;
//     //float depth = hitPoint.y;

//     bool didHit = checkIfHit(hitPointBary);
    
//     if (didHit && hitPointBary.x > 0.0) {
//         if (hitPointBary.x <= curMinT) {            
//             curMaxTriangle = triangle;
//             maxHitPoint = hitPoint;
//             curMinT = hitPointBary.x;
//             hitAnything = true;
//       }
//     } 
//   }
//     if (hitAnything) {
//         struct Ray reflection = getReflectedRay(camPos, maxHitPoint, curMaxTriangle.normal);
//         bool hitLightSource = reachesLightSource(reflection);
//         float colFactor = getColourFactor(curMaxTriangle, reflection.direction);
//         vec4 colour = vec4(colFactor * float(hitLightSource) * 1.0, 0.0, 0.0, 1.0);//vec4(colFactor * 1.0, 0.0, 0.0, 1.0);
//         imageStore(img_output, pix, colour);
//     }
    // else {
    //     vec4 colour = vec4(0.0, 0.0, 1.0, 1.0);//vec4(colFactor * 1.0, 0.0, 0.0, 1.0);
    //     imageStore(img_output, pix, colour);
    // }
}