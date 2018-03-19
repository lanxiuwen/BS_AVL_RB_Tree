//BSTree.h
#pragma once

#include<queue>
#include<iostream>

template<typename T>
class BSNode
{
	template<typename C> friend class BSTree;
	typedef typename BSNode<T> node;
	typedef typename BSNode<T>* node_pointer;
public:
	BSNode(T t)
		:value(t), lchild(nullptr), rchild(nullptr){}
	BSNode() = default;

	T value;
	node_pointer lchild;
	node_pointer rchild;
	node_pointer parent;
};

template<typename T>
class BSTree
{
public:

	typedef typename BSNode<T> node;
	typedef typename BSNode<T>* node_pointer;
	typedef typename T value_type;

	BSTree()
		:root(nullptr){}
	BSTree(value_type val)
	{
		root = new node(val);
	}
	virtual ~BSTree()
	{
		destory(root);
	}

	void preOrder() const;//前序遍历
	void inOrder() const;//中序遍历
	void postOrder() const;//后续遍历
	void layerOrder() const;//层序遍历

	node_pointer search_recursion(value_type key) const;//递归查找
	node_pointer search_iterator(value_type key) const;//迭代查找

	value_type search_min() const;
	value_type search_max()const;
	node_pointer successor(node_pointer x) const;//查找指定节点的后继节点
	node_pointer predecessor(node_pointer x) const;//查找指定节点的前驱节点

	virtual void insert(value_type key);
	virtual void remove(value_type key);//删除节点
	void destory();
	void print() const;

protected:
	node_pointer root;

	node_pointer search(node_pointer p,value_type key) const;
	virtual void remove(node_pointer p, value_type key);
	void preOrder(node_pointer p) const;
	void inOrder(node_pointer p) const;
	void postOrder(node_pointer p) const;
	void layerOrder(node_pointer p) const;
	value_type search_minimun(node_pointer p) const;
	value_type search_maximum(node_pointer p) const;
	void destory(node_pointer &p);
};

template<typename T>
void BSTree<T>::insert(value_type key)
{
	node_pointer pparent=nullptr;
	node_pointer pnode = root;
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
			pparent->rchild = pnode;
		else
		{
			pparent->lchild = pnode;
		}
	}
	pnode->parent = pparent;
}
//前序遍历
template<typename T>
void BSTree<T>::preOrder() const
{
	preOrder(root);
}
template<typename T>
void BSTree<T>::preOrder(node_pointer p) const
{
	if (p != nullptr)
	{
		std::cout << p ->value <<" ";
		preOrder(p->lchild);
		preOrder(p->rchild);
	}

}
//中序遍历
template<typename T>
void BSTree<T>::inOrder() const
{
	inOrder(root);
}

template<typename T>
void BSTree<T>::inOrder(node_pointer p) const
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
void BSTree<T>::postOrder() const
{
	postOrder(root);
}
template<typename T>
void BSTree<T>::postOrder(node_pointer p) const
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
void BSTree<T>::layerOrder() const
{
	layerOrder(root);
}
template<typename T>
void BSTree<T>::layerOrder(node_pointer pnode) const
{
	std::queue<node_pointer> Q;
	Q.push(pnode);
	while (!Q.empty())
	{
		pnode = Q.front();
		std::cout << pnode->value <<" ";
		Q.pop();
		if (pnode->lchild != nullptr)
			Q.push(pnode->lchild);
		if (pnode->rchild != nullptr)
			Q.push(pnode->rchild);
	}
	std::cout << std::endl;
}
template<typename T>
void BSTree<T>::print() const
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

//寻找节点的前驱节点
template<typename T>
BSNode<T>* BSTree<T>::predecessor(node_pointer pnode) const
{
	//如果pnode有左子树
	if (pnode->lchild != nullptr)
	{
		pnode = pnode->lchild;
		while (pnode->rchild!=nullptr)
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
		while (pnode->parent!= nullptr&&pnode->parent->lchild == pnode)
		{
			pnode = pnode->parent;
		}
		return pnode->parent;
	}
}
template<typename T>
BSNode<T>* BSTree<T>::successor(node_pointer pnode) const
{
	//如果节点有右子树，则其后继节点为右子树的最左节点
	if (pnode->rchild != nullptr)
	{
		pnode = pnode->rchild;
		while (pnode->lchild!=nullptr)//寻找右子树的最左节点
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
void BSTree<T>::remove(value_type key)
{
	remove(root, key);
}
template<typename T>
void BSTree<T>::remove(node_pointer pnode, value_type key)
{
	if (pnode != nullptr)
	{
		if (pnode->value == key)//找到删除的对象
		{
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
			else if(pdel->parent->lchild==pdel)//如果不是头节点并且是左节点
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
			delete pdel;
		}
		else if(key>pnode->value)
		{
			remove(pnode->rchild, key);
		}
		else
		{
			remove(pnode->lchild, key);
		}
	}
}


template<typename T>
typename BSTree<T>::node_pointer BSTree<T>::search(node_pointer pnode, value_type key) const
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
typename BSTree<T>::node_pointer BSTree<T>::search_recursion(value_type key) const
{
	return search(root, key);
}

template<typename T>
typename BSTree<T>::node_pointer BSTree<T>::search_iterator(value_type key) const
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
typename BSTree<T>::value_type BSTree<T>::search_minimun(node_pointer pnode) const
{
	while(pnode->lchild != nullptr)
		pnode = pnode->lchild;
	return pnode->value;
}
template<typename T>
typename BSTree<T>::value_type BSTree<T>::search_min() const
{
	if (root == nullptr)
		return value_type(0);
	return search_minimun(root);
}
template<typename T>
typename BSTree<T>::value_type BSTree<T>::search_maximum(node_pointer pnode) const
{
	while (pnode->rchild != nullptr)
		pnode = pnode->rchild;
	return pnode->value;
}
template<typename T>
typename BSTree<T>::value_type BSTree<T>::search_max() const
{
	if (root == nullptr)
		return value_type(0);
	return search_maximum(root);
}
template<typename T>
void BSTree<T>::destory()
{
	destory(root);
}
template<typename T>
void BSTree<T>::destory(node_pointer& pnode)
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