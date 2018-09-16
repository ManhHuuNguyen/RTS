#pragma once
#include "BoundingBox.h"
#include <vector>
#include "EntityRenderer.h"
#include <queue>
#include "Frustum.h"
#include "IntersectionRecord.h"
#include "Ray.h"
#include "IntersectionRecord.h"

class Octree {

	public:
		Octree * parent;
		BoundingBox region;
		const int MIN_SIZE = 1;	// minimum size for enclosing region 1x1x1
		Octree * childNodes[8]{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
		std::vector<Entity *> objects;
		char activeNodes = 0;
		
		int maxLifeSpan = 8;// this is how many frames we'll wait before deleting an empty tree branch. Note that this is not a constant. The maximum lifespan doubles
						    //every time a node is reused, until it hits a hard coded constant of 64
		int curLife = -1; //this is a countdown time showing how much time we have left to live

		static std::queue<Entity *> pendingInsertion;
		static bool treeReady; //false = the tree has a few objects which need to be inserted before it is complete
		static bool treeBuilt; //false = there is no pre-existing tree yet.

		Octree(BoundingBox & boundingBox, Entity * entity);
		Octree(BoundingBox & boundingBox, std::vector<Entity *> & objList);
		Octree(BoundingBox & boundingBox);
		void hitRecursive(Ray & ray, IntersectionRecord * record);
		void addEntityToQueue(Entity * entity); // only used before tree is built
		void updateTree(); // update the tree, flushing pending queue
		void update(float elapsedSeconds, EntityRenderer & entityRenderer, Frustum & viewFrustum);
		void buildTree();
		void insertEntity(Entity * entity);
		void remove(Entity * entity);
		void dragSelectRecursive(std::vector<Entity *> & chosens, float minX, float minY, float maxX, float maxY);
		~Octree();

		static bool intersect(Ray & ray, float * hitDepth, BoundingBox & box);
		
};

