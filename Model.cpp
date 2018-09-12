#include "Model.h"


glm::vec2 ModelData::calculateBaseCenter() {
	return glm::vec2((maxX + minX) / 2.0f, (maxZ + minZ) / 2.0f);
}

float ModelData::calculateHeight() {
	return maxY - minY;
}

float ModelData::calculateRadius() {
	return std::max(maxX - minX, maxZ - minZ) / 2.0f;
}

glm::vec3 ModelData::calculateModelCenter() {
	return glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
}


Model::Model(const char * file, Assimp::Importer & importer, bool leftHanded) {
	this->leftHanded = leftHanded;
	const aiScene * scene = importer.ReadFile(file,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		std::cout << "Failed importing model " << file << std::endl;
		return;
	}
	std::map<std::string, Texture *> loadedTextures;
	std::map<unsigned int, Material *> materialMap;
	std::map<std::string, int> jointIndexMap;

	for (int s = 0; s < scene->mNumMeshes; s++) {
		aiMesh * AssimpMesh = scene->mMeshes[s];
		unsigned int materialIndex = AssimpMesh->mMaterialIndex;
		if (materialMap.count(materialIndex) == 0) { // if this material has not been created
			aiMaterial * material = scene->mMaterials[materialIndex];

			aiColor3D KDco; // diffuse coefficient
			material->Get(AI_MATKEY_COLOR_DIFFUSE, KDco);
			glm::vec3 KD = Util::aiToGLM(KDco);

			aiColor3D KAco; // ambient coefficent
			material->Get(AI_MATKEY_COLOR_AMBIENT, KAco);
			glm::vec3 KA = Util::aiToGLM(KAco);

			aiColor3D KSco; // specular coefficent
			material->Get(AI_MATKEY_COLOR_SPECULAR, KSco);
			glm::vec3 KS = Util::aiToGLM(KSco);

			Material * m = new Material{ KD, KA, KS };

			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString textureFile;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFile);
				std::string fullPath = Util::getCurrentFolderPath(file) + "/" + Util::getFileName(textureFile.C_Str());
				if (!isTextureLoaded(fullPath, loadedTextures)) {
					Texture * texture = new Texture{ fullPath, 0, GL_CLAMP_TO_EDGE};
					m->diffuseTexture = texture;
					loadedTextures.insert(std::pair<std::string, Texture *>(fullPath, texture));
				}
				else {
					m->diffuseTexture = loadedTextures[fullPath];
				}
			}
			materialMap.insert(std::pair<unsigned int, Material *>(materialIndex, m));
			materials.push_back(m);
		}

		aiBone ** assimpBoneArray = AssimpMesh->mBones;
		for (int i = 0; i < AssimpMesh->mNumBones; i++) {
			aiBone * bone = assimpBoneArray[i];
			animatedJoints.insert(std::string{ bone->mName.C_Str() });
		}
	}
	
	aiNode * rootNode = scene->mRootNode;

	fillJointHierarchy(rootNode, CONSTANT::IDENTITY_MATRIX, jointIndexMap);
	createMesh(scene, rootNode, materialMap, loadedTextures, CONSTANT::IDENTITY_MATRIX, jointIndexMap);	
	calculateAnimation(scene);
	for (int i = 0; i < materials.size(); i++) {
		normalizeVertexWeight(materials[i]->mesh);
	}
	loadModelToVAO();
}

void Model::loadModelToVAO() {
	std::vector<glm::vec4> vertices = getMeshesPositions();
	std::vector<glm::vec4> normals = getMeshesNormals();
	std::vector<glm::vec2> textures = getMeshesTextures();
	std::vector<glm::ivec4> jointIDs = getMeshesJoints();
	std::vector<glm::vec4> weights = getMeshesWeights();


	glGenVertexArrays(1, &vao);
	glGenBuffers(5, vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec4), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, jointIDs.size() * sizeof(glm::ivec4), &jointIDs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, 0, 0); // because passing bone index, which is int

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::vec4), &weights[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, false, 0, 0);
}


void Model::normalizeVertexWeight(Mesh & mesh) {
	for (int i = 0; i < mesh.vertices.size(); i++) {
		float totalWeight = mesh.vertices[i].weights.x + mesh.vertices[i].weights.y + mesh.vertices[i].weights.z + mesh.vertices[i].weights.w;
		mesh.vertices[i].weights.x /= totalWeight;
		mesh.vertices[i].weights.y /= totalWeight;
		mesh.vertices[i].weights.z /= totalWeight;
		mesh.vertices[i].weights.w /= totalWeight;
	}
}

void Model::calculateAnimation(const aiScene * scene) {
	int untitledAnimationCount = 0;
	for (int i = 0; i < scene->mNumAnimations; i++) {
		Animation a;
		aiAnimation * animationData = scene->mAnimations[i];
		std::string animationName = animationData->mName.C_Str();
		if (animationName.size() == 0) {
			animationName = "untitled" + std::to_string(untitledAnimationCount);
			untitledAnimationCount++;
		}
		float tickPerSecond = animationData->mTicksPerSecond;
		if (tickPerSecond <= CONSTANT::MARGIN_OF_ERROR) { // if tickPerSecond = 0, then set it to 1, bc it wont change a thing
			tickPerSecond = 1.0f;
		}
		a.animationTimeLength = animationData->mDuration / tickPerSecond;
		for (int z = 0; z < animationData->mNumChannels; z++) {
			aiNodeAnim * boneChannel = animationData->mChannels[z];

			JointAnimation jointAnimation;
			for (int w = 0; w < boneChannel->mNumPositionKeys; w++) {
				PositionKeyFrame position;
				position.timeStamp = boneChannel->mPositionKeys[w].mTime / tickPerSecond;
				position.position = glm::vec4(Util::aiToGLM(boneChannel->mPositionKeys[w].mValue), 1.0f);
				jointAnimation.posKeyframes.push_back(position);
			}
			for (int w = 0; w < boneChannel->mNumRotationKeys; w++) {
				RotationKeyFrame rotation;
				rotation.timeStamp = boneChannel->mRotationKeys[w].mTime / tickPerSecond;
				rotation.rotation = Util::aiToGLM(boneChannel->mRotationKeys[w].mValue);
				jointAnimation.rotKeyframes.push_back(rotation);
			}
			for (int w = 0; w < boneChannel->mNumScalingKeys; w++) {
				ScaleKeyFrame scale;
				scale.timeStamp = boneChannel->mScalingKeys[w].mTime / tickPerSecond;
				scale.scale = Util::aiToGLM(boneChannel->mScalingKeys[w].mValue);
				jointAnimation.scaleKeyframes.push_back(scale);
			}
			a.jointAnimations[std::string{ boneChannel->mNodeName.C_Str() }] = jointAnimation;

		}
		animationMap[animationName] = a;
	}

}

void Model::fillJointHierarchy(aiNode * rootNode, glm::mat4 & parentMatrix, std::map<std::string, int> & jointIndexMap) {
	Joint j;
	j.localTransform = Util::aiToGLM(rootNode->mTransformation);
	j.animatedTransform = parentMatrix * j.localTransform;
	rootJoint = j;
	std::string name = std::string{ rootNode->mName.C_Str() };
	j.name = name;
	
	if (animatedJoints.find(name) != animatedJoints.end()) {
		// if it is an animated joint, calculate its inverseBindTransform
		j.isAnimated = true;
		j.inverseBindTransform = glm::inverse(j.animatedTransform);
		jointIndexMap[name] = jointIndexMap.size();
		animated = 1u;
	}	
	for (int i = 0; i < rootNode->mNumChildren; i++) {
		fillJointHierarchyRecursive(rootNode->mChildren[i], rootJoint, jointIndexMap);
	}
}

void Model::fillJointHierarchyRecursive(aiNode * currentNode, Joint & parentJoint, std::map<std::string, int> & jointIndexMap) {
	Joint j;
	j.localTransform = Util::aiToGLM(currentNode->mTransformation);
	j.animatedTransform = parentJoint.animatedTransform * j.localTransform;
	std::string name = std::string{ currentNode->mName.C_Str() };
	j.name = name;
	if (animatedJoints.find(name) != animatedJoints.end()) {
		// if it is an animated joint, calculate its inverseBindTransform
		j.isAnimated = true;
		j.inverseBindTransform = glm::inverse(j.animatedTransform);
		jointIndexMap[name] = jointIndexMap.size();
		animated = 1u;
	}
	for (int i = 0; i < currentNode->mNumChildren; i++) {
		fillJointHierarchyRecursive(currentNode->mChildren[i], j, jointIndexMap);
	}
	parentJoint.children.push_back(j);

}

Model::~Model() {
	for (int i = 0; i < materials.size(); i++) {
		delete materials[i];
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(5, vbo);
}

void Model::createMesh(const aiScene * scene,
		aiNode * rootNode,
		std::map<unsigned int, Material *> & materialMap,
		std::map<std::string, Texture *> & loadedTextures,
		glm::mat4 & parentMatrix,
		std::map<std::string, int> & jointIndexMap
) {
	int numMesh = rootNode->mNumMeshes;
	glm::mat4 localMatrix = parentMatrix * Util::aiToGLM(rootNode->mTransformation);
	for (int i = 0; i < numMesh; i++) {
		aiMesh * AssimpMesh = scene->mMeshes[rootNode->mMeshes[i]];
		Material * material = materialMap[AssimpMesh->mMaterialIndex];
		aiBone ** assimpBoneArray = AssimpMesh->mBones;
		std::map<int, std::vector<std::pair<int, float>>> verticesAffectedByBoneMap;
		for (int e = 0; e < AssimpMesh->mNumBones; e++) {
			aiBone * bone = assimpBoneArray[e];
			std::string jointName = std::string{ bone->mName.C_Str() };
			for (int r = 0; r < bone->mNumWeights; r++) {
				verticesAffectedByBoneMap[bone->mWeights[r].mVertexId].push_back(std::pair<int, float>(jointIndexMap[jointName], bone->mWeights[r].mWeight));
			}
		}
		aiVector3D * assimpVertexArray = AssimpMesh->mVertices;
		aiVector3D * assimpNormalArray = AssimpMesh->mNormals;
		aiVector3D * assimpTextureArray = nullptr;
		int numberOfTexture = AssimpMesh->GetNumUVChannels();
		bool hasTexture = false;
		if (numberOfTexture > 0) {
			assimpTextureArray = AssimpMesh->mTextureCoords[0]; // has to be fixed later for multi-texturing
			hasTexture = true;
		}

		unsigned int materialIndex = AssimpMesh->mMaterialIndex;
		Material * thisMaterial = materialMap[materialIndex];
			
		aiFace * triangles = AssimpMesh->mFaces;
		for (int i = 0; i < AssimpMesh->mNumFaces; i++) {
			aiFace & triangle = triangles[i];
			unsigned int * indices = triangle.mIndices;

			unsigned int indice1 = indices[0];
			Vertex v1;
			v1.position = localMatrix * glm::vec4(assimpVertexArray[indice1].x, assimpVertexArray[indice1].y, assimpVertexArray[indice1].z, 1.0f);
			v1.normal = localMatrix * glm::vec4(assimpNormalArray[indice1].x, assimpNormalArray[indice1].y, assimpNormalArray[indice1].z, 0.0f);
			if (hasTexture) {
				if (leftHanded) {
					v1.textureCoord = glm::vec2(assimpTextureArray[indice1].x, 1.0f - assimpTextureArray[indice1].y);
				}
				else {
					v1.textureCoord = glm::vec2(assimpTextureArray[indice1].x, assimpTextureArray[indice1].y);
				}
			}
			modelData.maxX = std::max(modelData.maxX, v1.position.x);
			modelData.maxY = std::max(modelData.maxY, v1.position.y);
			modelData.maxZ = std::max(modelData.maxZ, v1.position.z);
			modelData.minX = std::min(modelData.minX, v1.position.x);
			modelData.minY = std::min(modelData.minY, v1.position.y);
			modelData.minZ = std::min(modelData.minZ, v1.position.z);

			std::vector<std::pair<int, float>> & vertex1Weights = verticesAffectedByBoneMap[indice1];
			for (int z = 0; z < vertex1Weights.size(); z++) {
				v1.addNewJoint(vertex1Weights[z].first, vertex1Weights[z].second);
			}
			material->mesh.vertices.push_back(v1);

			unsigned int indice2 = indices[1];
			Vertex v2;
			v2.position = localMatrix * glm::vec4(assimpVertexArray[indice2].x, assimpVertexArray[indice2].y, assimpVertexArray[indice2].z, 1.0f);
			v2.normal = localMatrix * glm::vec4(glm::vec4(assimpNormalArray[indice2].x, assimpNormalArray[indice2].y, assimpNormalArray[indice2].z, 0.0f));
			if (hasTexture) {
				if (leftHanded) {
					v2.textureCoord = glm::vec2(assimpTextureArray[indice2].x, 1.0f - assimpTextureArray[indice2].y);
				}
				else {
					v2.textureCoord = glm::vec2(assimpTextureArray[indice2].x, assimpTextureArray[indice2].y);
				}
			}
			
			modelData.maxX = std::max(modelData.maxX, v2.position.x);
			modelData.maxY = std::max(modelData.maxY, v2.position.y);
			modelData.maxZ = std::max(modelData.maxZ, v2.position.z);
			modelData.minX = std::min(modelData.minX, v2.position.x);
			modelData.minY = std::min(modelData.minY, v2.position.y);
			modelData.minZ = std::min(modelData.minZ, v2.position.z);

			std::vector<std::pair<int, float>> & vertex2Weights = verticesAffectedByBoneMap[indice2];
			for (int z = 0; z < vertex2Weights.size(); z++) {
				v2.addNewJoint(vertex2Weights[z].first, vertex2Weights[z].second);
			}
			material->mesh.vertices.push_back(v2);

			unsigned int indice3 = indices[2];
			Vertex v3;
			v3.position = localMatrix * glm::vec4(assimpVertexArray[indice3].x, assimpVertexArray[indice3].y, assimpVertexArray[indice3].z, 1.0f);
			v3.normal = localMatrix * glm::vec4(assimpNormalArray[indice3].x, assimpNormalArray[indice3].y, assimpNormalArray[indice3].z, 0.0f);
			if (hasTexture) {
				if (leftHanded) {
					v3.textureCoord = glm::vec2(assimpTextureArray[indice3].x, 1.0f - assimpTextureArray[indice3].y);
				}
				else {
					v3.textureCoord = glm::vec2(assimpTextureArray[indice3].x, assimpTextureArray[indice3].y);
				}
			}
			
			modelData.maxX = std::max(modelData.maxX, v3.position.x);
			modelData.maxY = std::max(modelData.maxY, v3.position.y);
			modelData.maxZ = std::max(modelData.maxZ, v3.position.z);
			modelData.minX = std::min(modelData.minX, v3.position.x);
			modelData.minY = std::min(modelData.minY, v3.position.y);
			modelData.minZ = std::min(modelData.minZ, v3.position.z);

			std::vector<std::pair<int, float>> & vertex3Weights = verticesAffectedByBoneMap[indice3];
			for (int z = 0; z < vertex3Weights.size(); z++) {
				v3.addNewJoint(vertex3Weights[z].first, vertex3Weights[z].second);
			}
			material->mesh.vertices.push_back(v3);
		}
	}
	for (int s = 0; s < rootNode->mNumChildren; s++) {
		aiNode * childNode = rootNode->mChildren[s];
		createMesh(scene, childNode, materialMap, loadedTextures, localMatrix, jointIndexMap);
	}
}

bool Model::isTextureLoaded(std::string path, std::map<std::string, Texture *> & loadedTextures) {
	if (loadedTextures.count(path) > 0) {
		return true;
	}
	return false;
}

std::vector<glm::vec4> Model::getMeshesPositions() {
	std::vector<glm::vec4> positions;
	for (int i = 0; i < materials.size(); i++) {
		Material * material = materials[i];
		std::vector<glm::vec4> meshPos = material->mesh.getPositions();
		positions.insert(positions.end(), meshPos.begin(), meshPos.end());
	}
	return positions;
}

std::vector<glm::vec4> Model::getMeshesNormals() {
	std::vector<glm::vec4> normals;
	for (int i = 0; i < materials.size(); i++) {
		Material * material = materials[i];
		std::vector<glm::vec4> meshNorm = material->mesh.getNormals();
		normals.insert(normals.end(), meshNorm.begin(), meshNorm.end());
	}
	return normals;
}
std::vector<glm::vec2> Model::getMeshesTextures() {
	std::vector<glm::vec2> textureCoords;
	for (int i = 0; i < materials.size(); i++) {
		Material * material = materials[i];
		std::vector<glm::vec2> meshTCoord = material->mesh.getTextures();
		textureCoords.insert(textureCoords.end(), meshTCoord.begin(), meshTCoord.end());
	}
	return textureCoords;

}
std::vector<glm::ivec4> Model::getMeshesJoints() {
	std::vector<glm::ivec4> joints;
	for (int i = 0; i < materials.size(); i++) {
		Material * material = materials[i];
		std::vector<glm::ivec4> meshJoints = material->mesh.getJoints();
		joints.insert(joints.end(), meshJoints.begin(), meshJoints.end());
	}
	return joints;
}
std::vector<glm::vec4> Model::getMeshesWeights() {
	std::vector<glm::vec4> weights;
	for (int i = 0; i < materials.size(); i++) {
		Material * material = materials[i];
		std::vector<glm::vec4> meshWeights = material->mesh.getWeights();
		weights.insert(weights.end(), meshWeights.begin(), meshWeights.end());
	}
	return weights;
}