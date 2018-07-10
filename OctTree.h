#pragma once
#include <vector>
#include <array>
#include <memory>
#include "Vector3.h"
#include "ColliderCube.h"
class BallObject;
class BallInGame;
using namespace std;
class OctTree
{
public:
	OctTree(unsigned int ballArraySize, unsigned int maxDepth,const Vector3f& centerPositon,const Vector3f& halfRange);
	~OctTree();
	void createColliderList(const std::vector<shared_ptr<BallInGame>>& ballArray);
	void constructTree();
	void reset();
	const std::vector<array<int, 2>>& getPairList() const;
	
	
private:
	enum TREE_INDEX {
		UPLEFT_FRONT = 5,
		UPRIGHT_FRONT = 7,
		UPRIGHT_BACK = 6,
		UPLEFT_BACK = 4,
		DOWNLEFT_FRONT = 1,
		DOWNRIGHT_FRONT = 3,
		DOWNRIGHT_BACK = 2,
		DOWNLEFT_BACK = 0,
		UNKNOWN = -1,
	};

	

	class Box
	{
	public:
		Box();
		Box(int parent, int index, int depth, int colliderSize, TREE_INDEX positionIndex, const Vector3f & center, const Vector3f & halfSize);
		void set(int parent, int index, int depth, int colliderSize, TREE_INDEX positionIndex, const Vector3f & center, const Vector3f & halfSize);
		void reset();
		int parent;
		int index;
		int depth;

		
		TREE_INDEX position;
		ColliderCube collider;
		std::array<int, 8> children;
		std::vector<int> indexList;
		
	};
	int root;
	int maxDepth;
	int currentIndex;
	Vector3f center;
	Vector3f halfSize;
	std::vector<ColliderCube> colliderArray;
	std::vector<Box> boxs;
	std::vector<array<int,2>> pairList;
	void createChildren(Box& box);

};

