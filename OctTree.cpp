#include "OctTree.h"
#include "BallObject.h"
#include "BallInGame.h"
#include <iostream>



OctTree::OctTree(unsigned int ballArraySize, unsigned int maxDepth, const Vector3f & centerPosition, const Vector3f & halfRange):
	currentIndex(0),
	maxDepth(maxDepth),
	boxs(500*maxDepth),
	colliderArray(ballArraySize),
	center(centerPosition),
	halfSize(halfRange),
	root(0)
{

	for (auto& box : boxs)
	{
		box.indexList.reserve(ballArraySize / 8);
	}
	boxs[0].set(-1, 0, 0, ballArraySize, TREE_INDEX::UNKNOWN, center, halfSize);
	boxs[0].indexList.reserve(colliderArray.size());
	for (int i = 0; i <colliderArray.size(); ++i)
	{
		boxs[0].indexList.emplace_back(i);
	}
	pairList.reserve(ballArraySize*10);


}

OctTree::~OctTree()
{
}

void OctTree::createColliderList(const std::vector < shared_ptr<BallInGame >> &ballArray)
{
	if (ballArray.size() != colliderArray.size())
	{
		//errorMsg
		cerr << "can't create colliderArray" << endl;
		return;
	}
	for (int i = 0; i < colliderArray.size(); ++i)
	{
		float r = ballArray[i]->getRadius();
		colliderArray[i].setHalfSize(move(Vector3f(r, r, r)));
		colliderArray[i].setPosition(ballArray[i]->getPosition());
		//colliderArray[i].getPosition().debugWrite("position");
		//colliderArray[i].getHalfSize().debugWrite("half");
	}
	//cout << "creation complete" << endl;
}

void OctTree::constructTree()
{
	createChildren(boxs[0]);
}
void OctTree::reset()
{
	//cout << "ratio" << (float)pairList.size()/ (float)pairList.capacity() << endl;
	for (int i = 1; i < currentIndex; ++i) {
		boxs[i].reset();
	}
	currentIndex = 0;
	//boxs[0].set(-1, 0, 0, colliderArray.size(), TREE_INDEX::UNKNOWN, center, halfSize);
	boxs[0].children  = { -1,-1,-1,-1,-1,-1,-1,-1 };
	pairList.clear();
}

const std::vector<array<int, 2>>& OctTree::getPairList() const
{
	return pairList;
}

void OctTree::createChildren(Box &parent)
{
	//cout << "boxNo." << parent.index << endl;
	//cout << "indexSize" << parent.indexList.size() << endl;
	//cout << "colliderSize" << colliderArray.size() << endl;

	if (parent.indexList.size()  < 2)
	{
		//cout << "good bye size 1or 0" << endl;
		return;
	}
	else {
		for (int index : parent.indexList) {
			//cout << "index kun" << index << ",";
		}
	}
	if (parent.depth >= maxDepth)
	{
		//cout << "good bye maxDepth" << endl;
		for (int i = 0; i < parent.indexList.size(); ++i)
		{
			for (int j = i+1; j < parent.indexList.size(); ++j)
			{
				array<int, 2> pair = { parent.indexList[i],parent.indexList[j] };
				pairList.emplace_back(pair);
			}
		}
		return;
	}

	//cout << "boxNo" << currentIndex << endl;

	//cout << endl;
	Vector3f childrenSize = parent.collider.getHalfSize();
	//childrenSize.debugWrite("chil");
	Vector3f parentPositon = parent.collider.getPosition();
	//parentPositon.debugWrite("parent");
	childrenSize *= 0.5;
	
	for (unsigned int i = 0; i < 8; ++i)
	{

		if (currentIndex >= boxs.size() - 1)
		{
			boxs.resize(boxs.size() + 1000);
			cout << "brocken" << endl;
		}
		currentIndex++;
		Vector3f temp = childrenSize;
		//ビット演算で場所を求める
		//上下
		if (( i &unsigned int(4) )== 0) {
			temp.y *= -1.0;
		}
		//左右
		if ((i &unsigned int(2) )== 0) {
			temp.x *= -1.0;
		}
		//前後
		if ((i & unsigned int(1)) == 0) {
			temp.z *= -1.0;
		}	

		//parent.children[i] = currentIndex;

		boxs[currentIndex].set(parent.index, currentIndex, parent.depth + 1, colliderArray.size(),static_cast<TREE_INDEX>(i), parentPositon + temp, childrenSize);
		bool hasIndex = false;
		for (int index : parent.indexList) {
		
			if (boxs[currentIndex].collider.intersect(colliderArray[index]))
			{

				//cout << "insert box" << currentIndex << " and ball" << index << endl;
				hasIndex = true;
				boxs[currentIndex].indexList.emplace_back(index);
			}
		}

		if(hasIndex )
		createChildren(boxs[currentIndex]);
		
	}
}



OctTree::Box::Box() :
	parent(-1),
	depth(-1),
	index(-1),
	position(TREE_INDEX::UNKNOWN),
	children({ -1,-1,-1,-1,-1,-1,-1,-1 })
{
}

OctTree::Box::Box(int parent,int index, int depth, int colliderSize,TREE_INDEX positionIndex, const Vector3f &center, const Vector3f & halfSize):
	parent(parent),
	index(index),
	depth(depth),
	position(positionIndex),
	children({ -1,-1,-1,-1,-1,-1,-1,-1 })
{
	collider.setPosition(center);
	collider.setHalfSize(halfSize);
}

void OctTree::Box::set(int parent,int index, int depth,int colliderSize, TREE_INDEX positionIndex, const Vector3f & center, const Vector3f & halfSize)
{
	this->parent = parent,
	this->index = index;
	this->depth=depth;
	this->position = positionIndex;
	collider.setPosition(center);
	collider.setHalfSize(halfSize);
	children = { -1,-1,-1,-1,-1,-1,-1,-1 };
	if (depth != 0)
	{
		indexList.clear();
		//indexList.shrink_to_fit();
		indexList.reserve(colliderSize/(depth+1));
	}
	else
	{
		//indexList.clear();
		//indexList.shrink_to_fit();

	}
}

void OctTree::Box::reset()
{
	parent = -1;
	depth = -1;
	index = -1;
	position = TREE_INDEX::UNKNOWN;
	children = { -1,-1,-1,-1,-1,-1,-1,-1 };

}
