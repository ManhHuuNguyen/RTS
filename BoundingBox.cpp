#include "BoundingBox.h"



// class entity uses this constructor to initialize bounding box, thats why only this constructor have addBBtoVao method
BoundingBox::BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
	minDimension = glm::vec3(minX, minY, minZ);
	maxDimension = glm::vec3(maxX, maxY, maxZ);
	//std::cout << "Bounding box non-default constructor is called" << std::endl;
}

BoundingBox::BoundingBox() {
	//std::cout << "Bounding box default constructor is called, whatever" << std::endl;
}

BoundingBox::BoundingBox(glm::vec3 & minDimension, glm::vec3 & maxDimension) {
	this->minDimension = minDimension;
	this->maxDimension = maxDimension;
	//std::cout << "Bounding box non-default 2 constructor is called" << std::endl;
}

BoundingBox::BoundingBox(const BoundingBox & obj) {
	minDimension = obj.minDimension;
	maxDimension = obj.maxDimension;
	//std::cout << "Bounding box copy constructor is called by Octree" << std::endl;
}


bool BoundingBox::contains(BoundingBox & b) {
	glm::vec3 maxDifference = maxDimension - b.maxDimension;
	if (maxDifference.x < 0.0f || maxDifference.y < 0.0f || maxDifference.z < 0.0f) {
		return false;
	}
	glm::vec3 minDifference = b.minDimension - minDimension;
	if (minDifference.x < 0.0f || minDifference.y < 0.0f || minDifference.z < 0.0f) {
		return false;
	}
	return true;
}

bool BoundingBox::within(float xStart, float zStart, float xEnd, float zEnd) {
	float xMax, xMin, zMax, zMin;
	if (xStart < xEnd) {
		xMax = xEnd;
		xMin = xStart;
	}
	else {
		xMax = xStart;
		xMin = xEnd;
	}
	if (zStart < zEnd) {
		zMax = zEnd;
		zMin = zStart;
	}
	else {
		zMax = zStart;
		zMin = zEnd;
	}
	if (minDimension.x >= xMin && maxDimension.x <= xMax && minDimension.z >= zMin && maxDimension.z <= zMax) {
		return true;
	}
	return false;
}

glm::vec3 BoundingBox::getEdgeSize() {
	return maxDimension - minDimension;
}

glm::vec3 BoundingBox::getCenter() {
	return (maxDimension + minDimension) / 2.0f;
}

std::vector<glm::vec3> BoundingBox::getCorners() {
	std::vector<glm::vec3> corners;
	// front
	glm::vec3 A = glm::vec3(minDimension.x, minDimension.y, maxDimension.z);
	glm::vec3 B = glm::vec3(maxDimension.x, minDimension.y, maxDimension.z);
	glm::vec3 C = glm::vec3(maxDimension.x, maxDimension.y, maxDimension.z);
	glm::vec3 D = glm::vec3(minDimension.x, maxDimension.y, maxDimension.z);
	// back
	glm::vec3 E = glm::vec3(minDimension.x, minDimension.y, minDimension.z);
	glm::vec3 F = glm::vec3(maxDimension.x, minDimension.y, minDimension.z);
	glm::vec3 G = glm::vec3(maxDimension.x, maxDimension.y, minDimension.z);
	glm::vec3 H = glm::vec3(minDimension.x, maxDimension.y, minDimension.z);
	corners.push_back(A);
	corners.push_back(B);
	corners.push_back(C);
	corners.push_back(D);
	corners.push_back(E);
	corners.push_back(F);
	corners.push_back(G);
	corners.push_back(H);
	return corners;
}

BoundingBox BoundingBox::move(glm::mat4 & modelMatrix) {
	std::vector<glm::vec3> corners;

	glm::vec4 A = modelMatrix * glm::vec4(minDimension.x, minDimension.y, minDimension.z, 1.0f);
	glm::vec4 B = modelMatrix * glm::vec4(maxDimension.x, minDimension.y, minDimension.z, 1.0f);
	glm::vec4 C = modelMatrix * glm::vec4(minDimension.x, maxDimension.y, minDimension.z, 1.0f);
	glm::vec4 D = modelMatrix * glm::vec4(maxDimension.x, maxDimension.y, minDimension.z, 1.0f);
	glm::vec4 E = modelMatrix * glm::vec4(minDimension.x, minDimension.y, maxDimension.z, 1.0f);
	glm::vec4 F = modelMatrix * glm::vec4(maxDimension.x, minDimension.y, maxDimension.z, 1.0f);
	glm::vec4 G = modelMatrix * glm::vec4(minDimension.x, maxDimension.y, maxDimension.z, 1.0f);
	glm::vec4 H = modelMatrix * glm::vec4(maxDimension.x, maxDimension.y, maxDimension.z, 1.0f);

	float minx = std::min(std::min(std::min(std::min(std::min(std::min(std::min(A.x, B.x), C.x), D.x), E.x), F.x), G.x), H.x);
	float miny = std::min(std::min(std::min(std::min(std::min(std::min(std::min(A.y, B.y), C.y), D.y), E.y), F.y), G.y), H.y);
	float minz = std::min(std::min(std::min(std::min(std::min(std::min(std::min(A.z, B.z), C.z), D.z), E.z), F.z), G.z), H.z);
	float maxx = std::max(std::max(std::max(std::max(std::max(std::max(std::max(A.x, B.x), C.x), D.x), E.x), F.x), G.x), H.x);
	float maxy = std::max(std::max(std::max(std::max(std::max(std::max(std::max(A.y, B.y), C.y), D.y), E.y), F.y), G.y), H.y);
	float maxz = std::max(std::max(std::max(std::max(std::max(std::max(std::max(A.z, B.z), C.z), D.z), E.z), F.z), G.z), H.z);

	return BoundingBox{minx, maxx, miny, maxy, minz, maxz};
}

bool BoundingBox::collide(BoundingBox & b) {
	return (minDimension.x <= b.maxDimension.x && maxDimension.x >= b.minDimension.x) && 
		   (minDimension.y <= b.maxDimension.y && maxDimension.y >= b.minDimension.y) && 
		   (minDimension.z <= b.maxDimension.z && maxDimension.z >= b.minDimension.z);
}

bool BoundingBox::overlap(float xStart, float zStart, float xEnd, float zEnd) {
	if (xStart > maxDimension.x || xEnd < minDimension.x) {
		return false;
	}
	if (zStart > maxDimension.z || zEnd < minDimension.z) {
		return false;
	}
	return true;
}