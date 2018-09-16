#include "Octree.h"

std::queue<Entity *> Octree::pendingInsertion = std::queue<Entity *>();
bool Octree::treeBuilt = false;
bool Octree::treeReady = false;


Octree::Octree(BoundingBox & boundingBox, std::vector<Entity *> & objList) :region{ boundingBox }, objects{objList} {
	
}

Octree::Octree(BoundingBox & boundingBox) : region{ boundingBox }, objects{} {
	
}

Octree::Octree(BoundingBox & boundingBox, Entity * entity) : region{ boundingBox }, objects{entity} {
	
}

void Octree::addEntityToQueue(Entity * entity) {
	pendingInsertion.push(entity);
}

void Octree::updateTree() {
	if (!treeBuilt) {
		// if tree is not built yet, then dump everything into objects of root node, then build it
		// the build method will scatter objects down the tree
		while (!pendingInsertion.empty()) {
			Entity * e = pendingInsertion.front();
			objects.push_back(e);
			pendingInsertion.pop();
		}
		buildTree();
	}
	else {
		while (!pendingInsertion.empty()) {
			Entity * e = pendingInsertion.front();
			pendingInsertion.pop();
			insertEntity(e);
		}
	}
	treeReady = true;
}

void Octree::buildTree() {
	if (objects.size() <= 1) {
		return;
	}
	
	glm::vec3 edgeSize = region.getEdgeSize();
	if (edgeSize.x <= MIN_SIZE && edgeSize.y <= MIN_SIZE && edgeSize.z <= MIN_SIZE) {
		return; 
	}
	glm::vec3 regionCenter = region.getCenter();
	BoundingBox topLeftFront{region.minDimension.x, regionCenter.x, regionCenter.y, region.maxDimension.y, regionCenter.z, region.maxDimension.z};
	BoundingBox topRightFront{regionCenter.x, region.maxDimension.x, regionCenter.y, region.maxDimension.y, regionCenter.z, region.maxDimension.z};
	BoundingBox bottomLeftFront{region.minDimension.x, regionCenter.x, region.minDimension.y, regionCenter.y, regionCenter.z, region.maxDimension.z};
	BoundingBox bottomRightFront{regionCenter.x, region.maxDimension.x, region.minDimension.y, regionCenter.y, regionCenter.z, region.maxDimension.z};
	BoundingBox topLeftBack{region.minDimension.x, regionCenter.x, regionCenter.y, region.maxDimension.y, region.minDimension.z, regionCenter.z};
	BoundingBox topRightBack{regionCenter.x, region.maxDimension.x, regionCenter.y, region.maxDimension.y, region.minDimension.z, regionCenter.z};
	BoundingBox bottomLeftBack{region.minDimension.x, regionCenter.x, region.minDimension.y, regionCenter.y, region.minDimension.z, regionCenter.z};
	BoundingBox bottomRightBack{regionCenter.x, region.maxDimension.x, region.minDimension.y, regionCenter.y, region.minDimension.z, regionCenter.z};
	
	BoundingBox octant[8];

	octant[0] = topLeftFront;
	octant[1] = topRightFront;
	octant[2] = bottomLeftFront;
	octant[3] = bottomRightFront;
	octant[4] = topLeftBack;
	octant[5] = topRightBack;
	octant[6] = bottomLeftBack;
	octant[7] = bottomRightBack;

	std::vector<Entity *> entitiesInEachOctant[8];
	std::vector<Entity *> deList;

	for (int i = 0; i < objects.size(); i++) {
		glm::mat4 modelMat = objects[i]->getModelMat();
		BoundingBox objBox = objects[i]->boundingBox.move(modelMat);
		for (int j = 0; j < 8; j++) {
			if (octant[j].contains(objBox)) {
				entitiesInEachOctant[j].push_back(objects[i]);
				deList.push_back(objects[i]);
				break;
			}
		}
	}
	
	for (int i = 0; i < deList.size(); i++) {
		Entity * enPtr = deList[i];
		objects.erase(std::remove(objects.begin(), objects.end(), enPtr), objects.end());
	}

	for (int i = 0; i < 8; i++) {
		if (entitiesInEachOctant[i].size() > 0) {
			Octree * childTree = new Octree{octant[i], entitiesInEachOctant[i]};
			childNodes[i] = childTree;
			activeNodes = activeNodes | (1 << i);
			childNodes[i]->buildTree();
			childNodes[i]->parent = this;
		}
	}
	treeBuilt = true;
	treeReady = true;

}

void Octree::insertEntity(Entity * entity) {
	if (objects.size() < 1 && activeNodes == 0) { // activeNodes=0 means no children and no object yet so just leave obj here
		objects.push_back(entity);
		return;
	}
	glm::vec3 edgeSize = region.getEdgeSize();
	if (edgeSize.x <= MIN_SIZE && edgeSize.y <= MIN_SIZE && edgeSize.z <= MIN_SIZE) {// if there is no more way of dividing smaller region (1x1x1)
		objects.push_back(entity);
		return;
	}
	glm::vec3 regionCenter = region.getCenter();

	BoundingBox octant[8];

	octant[0] = (childNodes[0] != nullptr)? childNodes[0]->region : BoundingBox{ region.minDimension.x, regionCenter.x, regionCenter.y, region.maxDimension.y, regionCenter.z, region.maxDimension.z };
	octant[1] = (childNodes[1] != nullptr)? childNodes[1]->region : BoundingBox{ regionCenter.x, region.maxDimension.x, regionCenter.y, region.maxDimension.y, regionCenter.z, region.maxDimension.z };
	octant[2] = (childNodes[2] != nullptr) ? childNodes[2]->region : BoundingBox{ region.minDimension.x, regionCenter.x, region.minDimension.y, regionCenter.y, regionCenter.z, region.maxDimension.z };
	octant[3] = (childNodes[3] != nullptr) ? childNodes[3]->region : BoundingBox{ regionCenter.x, region.maxDimension.x, region.minDimension.y, regionCenter.y, regionCenter.z, region.maxDimension.z };
	octant[4] = (childNodes[4] != nullptr) ? childNodes[4]->region : BoundingBox{ region.minDimension.x, regionCenter.x, regionCenter.y, region.maxDimension.y, region.minDimension.z, regionCenter.z };
	octant[5] = (childNodes[5] != nullptr) ? childNodes[5]->region : BoundingBox{ regionCenter.x, region.maxDimension.x, regionCenter.y, region.maxDimension.y, region.minDimension.z, regionCenter.z };
	octant[6] = (childNodes[6] != nullptr) ? childNodes[6]->region : BoundingBox{ region.minDimension.x, regionCenter.x, region.minDimension.y, regionCenter.y, region.minDimension.z, regionCenter.z };
	octant[7] = (childNodes[7] != nullptr) ? childNodes[7]->region : BoundingBox{ regionCenter.x, region.maxDimension.x, region.minDimension.y, regionCenter.y, region.minDimension.z, regionCenter.z };

	glm::mat4 modelMat = entity->getModelMat();
	BoundingBox b = entity->boundingBox.move(modelMat);
	bool found = false;
	for (int i = 0; i < 8; i++) {
		if (octant[i].contains(b)) {// if found a region that can contain this obj
			if (childNodes[i] != nullptr) { // if the node associated with that region is already created
				childNodes[i]->insertEntity(entity); // add as deep as possible from that node 
			}
			else { // if the node associated with that region is not created
				childNodes[i] = new Octree{octant[i], entity}; // create a new node. the object will be the only object there since the node is new
				activeNodes = activeNodes | (1 << i); // mark the node as active
			}
			found = true;
			break; // this could be wrong
		}
	}
	if (!found) { // if the object is not inside any quadrant 
		objects.push_back(entity);
	}

}

Octree::~Octree() {
	for (int i = 0; i < 8; i++) {
		if ((activeNodes & (1 << i)) != 0) {
			delete childNodes[i];
		}
	}
}

void Octree::update(float elapsedSeconds, EntityRenderer & entityRenderer, Frustum & viewFrustum) {
	if (objects.size() == 0 && activeNodes == 0) { // if no child or object
		if (curLife == -1) { // if death timer hasnt started, start death timer
			curLife = maxLifeSpan;
		}
		else if (curLife > 0) { // if death timer has started, run it
			curLife -= 1;
		}
	}
	else { // if there is object
		if (curLife != -1) { // if death timer is already started
			if (maxLifeSpan <= 64) { // double max life span for each time unit it is alive, but not let maxLifeSpan surpass 64
				maxLifeSpan *= 2;
			}
			curLife = -1; // shut death timer
		}
		// if death timer isn't started, no need to do anything
	}
	std::vector<Entity *> movedObjects;
	for (Entity * object : objects) {
		bool moved = object->update(elapsedSeconds);
		if (moved) {
			movedObjects.push_back(object);
		}
		glm::mat4 modelMat = object->getModelMat();
		BoundingBox b = object->boundingBox.move(modelMat);
		if (viewFrustum.boxInFrustum(b)) { // only draw object inside frustum
			entityRenderer.addEntity(object);
		}
		//else { // for testing purposes
		//	entityRenderer.addEntity(object);
		//}
	}
	for (int i = 0; i < 8; i++) {
		if ((activeNodes & (1<<i)) != 0) {
			childNodes[i]->update(elapsedSeconds, entityRenderer, viewFrustum);
		}
	}
	for (Entity * movedObj : movedObjects) { // for each object that moves
		Octree * currentNode = this;
		glm::mat4 modelMat = movedObj->getModelMat();
		BoundingBox b = movedObj->boundingBox.move(modelMat);
		while (!currentNode->region.contains(b)) { // moves the moved object up until a node's able to contain it
			currentNode = currentNode->parent;
		}
		if (currentNode != this) { // if this new node's different from the old node (sometimes, moving object stills lie within the old node. in that case, do nothing)
			remove(movedObj); // remove the object in the old node
			currentNode->insertEntity(movedObj); // insert as deep as possible starting from new node
		}
	}

	for (int i = 0; i < 8; i++) {
		if ((activeNodes & (1 << i)) != 0 && childNodes[i]->curLife == 0) {
			childNodes[i] = nullptr;
			activeNodes = activeNodes ^ (1 << i);
		}
	}

}

void Octree::remove(Entity * entity) {
	objects.erase(std::remove(objects.begin(), objects.end(), entity), objects.end());
}

void Octree::hitRecursive(Ray & ray, IntersectionRecord * record) { // this is recursive method
	if (objects.size() == 0 && activeNodes == 0) {
		return;
	}
	float hitDepth;
	for (int i = 0; i < objects.size(); i++) {
		glm::mat4 modelMat = objects[i]->getModelMat();
		BoundingBox b = objects[i]->boundingBox.move(modelMat);
		if (intersect(ray, &hitDepth, b)) {
			if (hitDepth < record->hitDepth) {
				record->e_ptr = objects[i];
				record->hitDepth = hitDepth;
			}
		}
	}
	for (int k = 0; k < 8; k++) {
		if ((activeNodes & (1 << k)) != 0 && intersect(ray, &hitDepth, childNodes[k]->region)) {
			childNodes[k]->hitRecursive(ray, record);
		}
	}
}

bool Octree::intersect(Ray & r, float * hitDepth, BoundingBox & box) {
	
	glm::vec3 & lb = box.minDimension;
	glm::vec3 & rt = box.maxDimension;
	float t1 = (lb.x - r.origin.x)*r.invDir.x;
	float t2 = (rt.x - r.origin.x)*r.invDir.x;
	float t3 = (lb.y - r.origin.y)*r.invDir.y;
	float t4 = (rt.y - r.origin.y)*r.invDir.y;
	float t5 = (lb.z - r.origin.z)*r.invDir.z;
	float t6 = (rt.z - r.origin.z)*r.invDir.z;

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		*hitDepth = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		*hitDepth = tmax;
		return false;
	}

	*hitDepth = tmin;
	return true;
}

void Octree::dragSelectRecursive(std::vector<Entity *> & chosens, float minX, float minY, float maxX, float maxY) {
	if (objects.size() == 0 && activeNodes == 0) {
		return;
	}
	if (region.overlap(minX, minY, maxX, maxY)) {
		for (int i = 0; i < objects.size(); i++) {
			glm::mat4 modelMat = objects[i]->getModelMat();
			BoundingBox b = objects[i]->boundingBox.move(modelMat);
			if (b.within(minX, minY, maxX, maxY)) {
				chosens.push_back(objects[i]);
			}
		}
		for (int k = 0; k < 8; k++) {
			if ((activeNodes & (1 << k)) != 0) {
				childNodes[k]->dragSelectRecursive(chosens, minX, minY, maxX, maxY);
			}
		}
	}
}