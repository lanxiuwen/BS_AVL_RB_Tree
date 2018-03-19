#pragma once
#include<stdlib.h>
#include<queue>
#include<iostream>
#define MAX(a,b) ((a)>(b)?(a):(b))


template<typename T>
class AVLNode
{
public:
	typedef typename AVLNode<T> node;
	typedef typename AVLNode<T>* node_pointer;

	AVLNode(T t)
		:value(t){}

	T value;
	node_pointer lchild;
	node_pointer rchild;
	node_pointer parent;
};



template<typename T>
class AVLTree 
{
public:
	typedef typename AVLNode<T> node;
	typedef typename AVLNode<T>* node_pointer;
	typedef typename T value_type;
	AVLTree()
		:root(nullptr) {}

	AVLTree(value_type val)
	{
		root = new node(val);
	}
	~AVLTree() 
	{
		destory(root);
	}//析构函数

	void preOrder() const;//前序遍历
	void inOrder() const;//中序遍历
	void postOrder() const;//后续遍历
	void layerOrder() const;//层序遍历

	node_pointer search_recursion(value_type key) const;//递归查找
	node_pointer search_iterator(value_type key) const;//迭代查找

	value_type search_min() const;
	value_type search_max() const;
	node_pointer successor(node_pointer pnode) const;//查找指定节点的后继节点
	node_pointer predecessor(node_pointer pnode) const;//查找指定节点的前驱节点

	void insert(value_type key);
	void remove(value_type key);

	void destory();
	void print() const;

	int height();//返回树的高度

protected:

	node_pointer root;

	int height(node_pointer pnode);//返回节点的高度
	node_pointer search(node_pointer p, value_type key) const;
	void remove(node_pointer p, value_type key);
	void preOrder(node_pointer p) const;
	void inOrder(node_pointer p) const;
	void postOrder(node_pointer p) const;
	void layerOrder(node_pointer p) const;
	value_type search_minimun(node_pointer p) const;
	value_type search_maximum(node_pointer p) const;
	void destory(node_pointer &p);

	node_pointer leftRotation(node_pointer pnode);
	node_pointer rightRotation(node_pointer pnode);
	node_pointer leftRightRotation(node_pointer pnode); 
	node_pointer rightLeftRotation(node_pointer pnode);

	int isNotBalance(node_pointer pnode);//计算节点是否平衡
	//void blance(node_pointer pnode);//自pnode回溯维护平衡
};

template<typename T>
int AVLTree<T>::height(node_pointer pnode)
{
	int u, v;
	if (pnode == nullptr) return -1;
	u = height(pnode->lchild); v = height(pnode->rchild);
	return u > v ? (u + 1) : (v + 1);
}
template<typename T>
int AVLTree<T>::height()
{
	return height(root);
}

/*左旋转操作*/
/*pnode为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::leftRotation(node_pointer pnode)
{
	node_pointer prchild = pnode->rchild;
	node_pointer pparent = pnode->parent;
	pnode->rchild = prchild->lchild;
	if(pnode->rchild!=nullptr)
		pnode->rchild->parent = pnode;
	prchild->lchild = pnode;
	pnode->parent = prchild;
	prchild->parent = pparent;
	//pnode->height = MAX(height(pnode->lchild), height(pnode->rchild)) + 1;
	//prchild->height = MAX(height(prchild->lchild), height(prchild->rchild)) + 1;

	return prchild;
}
/*右旋转操作*/
/*pnode为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::rightRotation(node_pointer pnode)
{
	node_pointer plchild = pnode->lchild;
	node_pointer pparent = pnode->parent;
	pnode->lchild = plchild->rchild;
	if(pnode->lchild!=nullptr)
		pnode->lchild->parent = pnode;
	plchild->rchild = pnode;
	pnode->parent = plchild;
	plchild->parent = pparent;
	
	//pnode->height = MAX(height(pnode->lchild), height(pnode->rchild)) + 1;
	//plchild->height = MAX(height(plchild->lchild), height(plchild->rchild)) + 1;
	return plchild;
}
/*先左旋再右旋*/
/*参数pnode为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::leftRightRotation(node_pointer pnode)
{
	pnode->lchild = leftRotation(pnode->lchild);
	return rightRotation(pnode);
}
/*先右旋再左旋*/
/*参数proot为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::rightLeftRotation(node_pointer pnode)
{
	pnode->rchild = rightRotation(pnode->rchild);
	return leftRotation(pnode);
}

template<typename T>
int AVLTree<T>::isNotBalance(node_pointer pnode)
{
	int hl = height(pnode->lchild);
	int hr = height(pnode->rchild);
	int BF = hl-hr;

	if (BF > (-2) && (BF < 2))
		return 0;//平衡
	else if(BF>=2)
	{
		return 1;//左大右小
	}
	else if(BF<-2)
	{
		return -1;//左小右大
	}
}

template<typename T>
void AVLTree<T>::insert(value_type key)
{
	node_pointer pparent = nullptr;
	node_pointer pnode = root;

	bool isLeftChild;//插入的是否是左孩子
	bool isLeftChildTree;//插入点是否在不平衡的节点的左子树


	//寻找插入点
	while (pnode != nullptr)
	{
		pparent = pnode;//这里实际就是实现了pparent=pnode->parent;
		if (key < pnode->value)
			pnode = pnode->lchild;
		else if (key > pnode->value)//这里用eles if很重要，else 就不行，因为在上一步已经改变了Pnode的值
			pnode = pnode->rchild;
		else
		{
			break;
		}
	}
	pnode = new node(key);
	if (pparent == nullptr)//为root节点
		root = pnode;
	else
	{
		if (key > pparent->value)
		{
			pparent->rchild = pnode;
			isLeftChild = false;
		}
		else
		{
			pparent->lchild = pnode;
			isLeftChild = true;
		}
	}
	pnode->parent = pparent;

	//维护平衡

	while (!isNotBalance(pnode)&&pnode->parent!=nullptr)
	{
		if (pnode == pnode->parent->lchild)
			isLeftChildTree = true;
		else
			isLeftChildTree = false;
		pnode = pnode->parent;
	}
	pparent = pnode->parent;
	if (isNotBalance(pnode))
	{
		if (isLeftChild&&isLeftChildTree)
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = rightRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = rightRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = rightRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
		else if (!isLeftChild && !isLeftChildTree)
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = leftRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = leftRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = leftRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
		else if (!isLeftChild && isLeftChildTree)
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = leftRightRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = leftRightRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = leftRightRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
		else if (isLeftChild && !isLeftChildTree)
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = rightLeftRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = rightLeftRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = rightLeftRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
	}
}
template<typename T>
void AVLTree<T>::remove(node_pointer pnode, value_type key)
{
	if (pnode == nullptr)
		return;
	while (key != pnode->value)//找到删除点
	{
		if (key > pnode->value)
		{
			pnode = pnode->rchild;
		}
		else if(key<pnode->value)
		{
			pnode = pnode->lchild;
		}
		else if(pnode==nullptr)
		{
			return;
		}
	}

	node_pointer pdel = nullptr;

	//pnode为只有左子树或者只有右子树，或者没有子树
	if (pnode->lchild == nullptr || pnode->rchild == nullptr)
		pdel = pnode;
	else//pnode有左右子树，使用前驱顶替
	{
		pdel = predecessor(pnode);
	}
	//此时要删除的节点只有一个孩子或者没有孩子，保存孩子指针
	node_pointer pchild = nullptr;
	if (pdel->lchild != nullptr)
		pchild = pdel->lchild;
	else
	{
		pchild = pdel->rchild;
	}
	//让孩子指向被删节点的父节点
	if (pchild != nullptr)
		pchild->parent = pdel->parent;
	//如果删的是头节点，就修改root的值
	if (pdel->parent == nullptr)
		root = pchild;
	else if (pdel->parent->lchild == pdel)//如果不是头节点并且是左节点
	{
		pdel->parent->lchild = pchild;
	}
	else//不是头节点且是右节点
	{
		pdel->parent->rchild = pchild;
	}
	//如果删除节点的值与目标删除节点值不一样，就是被删节点有左右子树的情况，pdel指向的是被删节点的前驱节点，所以删除之，其值给原意删除节点。
	if (pnode->value != pdel->value)
		pnode->value = pdel->value;

	pnode = pdel->parent;//使pnode指向删除点的父点

	delete pdel;

	//找到不平衡的点
	while (!isNotBalance(pnode) && pnode->parent != nullptr)
	{
		pnode = pnode->parent;
	}

	node_pointer pparent = pnode->parent;//不平衡节点的父节点
	int pnodeBlance = isNotBalance(pnode);
	if (pnodeBlance>0)//不平衡节点左高右低
	{
		if (height(pnode->lchild->lchild) > height(pnode->lchild->rchild))//不平衡节点的左子树的左子树高于其左子树的右子树，右旋即可
		{

			if (pparent == nullptr)//pnode为根节点
			{
				pnode = rightRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = rightRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = rightRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
		else//不平衡节点的的左子树的左子树小于其左子树的右子树，需要先左旋再右旋
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = leftRightRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = leftRightRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = leftRightRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
	}
	else if (pnodeBlance < 0)//不平衡节点左低右高
	{
		if (height(pnode->rchild->lchild) > height(pnode->rchild->rchild))//适用于先右旋再左旋
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = rightLeftRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = rightLeftRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = rightLeftRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
		else//适用于左旋
		{
			if (pparent == nullptr)//pnode为根节点
			{
				pnode = leftRotation(pnode);
				root = pnode;
			}
			else if (pnode == pparent->lchild)
			{
				pnode = leftRotation(pnode);
				pnode->parent = pparent;
				pparent->lchild = pnode;
			}
			else
			{
				pnode = leftRotation(pnode);
				pnode->parent = pparent;
				pparent->rchild = pnode;
			}
		}
	}
}
template<typename T>
void AVLTree<T>::remove(value_type key)
{
	remove(root, key);
}

template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::predecessor(node_pointer pnode) const
{
	//如果pnode有左子树
	if (pnode->lchild != nullptr)
	{
		pnode = pnode->lchild;
		while (pnode->rchild != nullptr)
		{
			pnode = pnode->rchild;
		}
		return pnode;
	}
	//如果没有左子树，两种情况，一，它本身为左子树，二，它为右子树。
	if (pnode->parent->rchild == pnode)//情况二
		return pnode->parent;
	else//情况一 寻找第一个为右子树的父节点的父节点
	{
		while (pnode->parent != nullptr&&pnode->parent->lchild == pnode)
		{
			pnode = pnode->parent;
		}
		return pnode->parent;
	}
}
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::successor(node_pointer pnode) const
{
	//如果节点有右子树，则其后继节点为右子树的最左节点
	if (pnode->rchild != nullptr)
	{
		pnode = pnode->rchild;
		while (pnode->lchild != nullptr)//寻找右子树的最左节点
		{
			pnode = pnode->lchild;
		}
		return pnode;
	}
	//////////////////////////////////////////////////////
	// 
	//
	//                |---->本身是左子树，后继节点为其父节点
	//			      |
	//如果没有右子树---|
	//				  |
	//				  |---->本身是右子树，后继节点为其“具有左子树的最近父节点”
	//
	////////////////////////////////////////////////////////////
	node_pointer pparent = pnode->parent;
	while (pparent != nullptr&&pparent->rchild == pnode)//如果pnode!=pparent->rchild,pnode是pparent的左子树，也就是找到了
	{
		pnode = pparent;
		pparent = pnode->parent;
	}
	return pparent;
}
template<typename T>
void AVLTree<T>::preOrder() const
{
	preOrder(root);
}

template<typename T>
void AVLTree<T>::preOrder(node_pointer p) const
{
	if (p != nullptr)
	{
		std::cout << p->value << " ";
		preOrder(p->lchild);
		preOrder(p->rchild);
	}
}
template<typename T>
void AVLTree<T>::inOrder() const
{
	inOrder(root);
}
template<typename T>
void AVLTree<T>::inOrder(node_pointer p) const
{
	if (p != nullptr)
	{
		inOrder(p->lchild);
		std::cout << p->value << " ";
		inOrder(p->rchild);
	}
}
//后序遍历
template<typename T>
void AVLTree<T>::postOrder() const
{
	postOrder(root);
}
template<typename T>
void AVLTree<T>::postOrder(node_pointer p) const
{
	if (p != nullptr)
	{
		postOrder(p->lchild);
		postOrder(p->rchild);
		std::cout << p->value << " ";
	}

}
//层序遍历
template<typename T>
void AVLTree<T>::layerOrder() const
{
	layerOrder(root);
}
template<typename T>
void AVLTree<T>::layerOrder(node_pointer pnode) const
{
	std::queue<node_pointer> Q;
	Q.push(pnode);
	while (!Q.empty())
	{
		pnode = Q.front();
		std::cout << pnode->value << " ";
		Q.pop();
		if (pnode->lchild != nullptr)
			Q.push(pnode->lchild);
		if (pnode->rchild != nullptr)
			Q.push(pnode->rchild);
	}
	std::cout << std::endl;
}
template<typename T>
void AVLTree<T>::print() const
{
	std::cout << "前序遍历： ";
	preOrder();
	std::cout << std::endl;
	std::cout << "中序遍历： ";
	inOrder();
	std::cout << std::endl;
	std::cout << "后序遍历： ";
	postOrder();
	std::cout << std::endl;
	std::cout << "层序遍历： ";
	layerOrder();
	std::cout << std::endl;
}
template<typename T>
void AVLTree<T>::destory(node_pointer & pnode)
{
	if (pnode != nullptr)
	{
		if (pnode->lchild != nullptr)
			destory(pnode->lchild);
		if (pnode->rchild != nullptr)
			destory(pnode->rchild);
		delete(pnode);
		pnode = nullptr;
	}
}
template<typename T>
void AVLTree<T>::destory()
{
	destory(root);
}
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::search(node_pointer pnode, value_type key) const
{
	if (pnode == nullptr)
		return nullptr;
	if (pnode->value == key)
		return pnode;
	if (key > pnode->value)
		return search(pnode->rchild, key);
	else
	{
		return search(pnode->lchild, key);
	}
}
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::search_recursion(value_type key) const
{
	return search(root, key);
}
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::search_iterator(value_type key) const
{
	node_pointer pnode = root;
	while (pnode != nullptr)
	{
		if (key == pnode->value)
			return pnode;
		if (key > pnode->value)
			pnode = pnode->rchild;
		else
		{
			pnode = pnode->lchild;
		}
	}
	return nullptr;
}
template<typename T>
typename AVLTree<T>::value_type AVLTree<T>::search_minimun(node_pointer pnode) const
{
	while (pnode->lchild != nullptr)
		pnode = pnode->lchild;
	return pnode->value;
}
template<typename T>
typename AVLTree<T>::value_type AVLTree<T>::search_min() const
{
	if (root == nullptr)
		return value_type(0);
	return search_minimun(root);
}
template<typename T>
typename AVLTree<T>::value_type AVLTree<T>::search_maximum(node_pointer pnode) const
{
	while (pnode->rchild != nullptr)
		pnode = pnode->rchild;
	return pnode->value;
}
template<typename T>
typename AVLTree<T>::value_type AVLTree<T>::search_max() const
{
	if (root == nullptr)
		return value_type(0);
	return search_maximum(root);
}