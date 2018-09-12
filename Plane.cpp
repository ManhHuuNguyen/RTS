#include "Plane.h"


Plane::Plane(glm::vec3 point, glm::vec3 normal) :point{ point }, normal{normal} {

}


Plane::Plane(): point {0.0, 0.0, 0.0}, normal(0.0, 1.0, 0.0){

}

float Plane::distanceFromPoint(glm::vec3 point) {//a.b = |a||b|cos(theta) but |b| = 1 because it is normal so a.b = |a|cos(theta) = distance
	return glm::dot((point - this->point), normal);
}