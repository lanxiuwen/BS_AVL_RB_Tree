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

	void preOrder() const;//ǰ�����
	void inOrder() const;//�������
	void postOrder() const;//��������
	void layerOrder() const;//�������

	node_pointer search_recursion(value_type key) const;//�ݹ����
	node_pointer search_iterator(value_type key) const;//��������

	value_type search_min() const;
	value_type search_max()const;
	node_pointer successor(node_pointer x) const;//����ָ���ڵ�ĺ�̽ڵ�
	node_pointer predecessor(node_pointer x) const;//����ָ���ڵ��ǰ���ڵ�

	virtual void insert(value_type key);
	virtual void remove(value_type key);//ɾ���ڵ�
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
	//Ѱ�Ҳ����
	while (pnode != nullptr)
	{
		pparent = pnode;//����ʵ�ʾ���ʵ����pparent=pnode->parent;
		if (key < pnode->value)
			pnode = pnode->lchild;
		else if (key > pnode->value)//������eles if����Ҫ��else �Ͳ��У���Ϊ����һ���Ѿ��ı���Pnode��ֵ
			pnode = pnode->rchild;
		else
		{
			break;
		}
	}
	pnode = new node(key);
	if (pparent == nullptr)//Ϊroot�ڵ�
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
//ǰ�����
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
//�������
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
//�������
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
//�������
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
	std::cout << "ǰ������� ";
	preOrder();
	std::cout << std::endl;
	std::cout << "��������� ";
	inOrder();
	std::cout << std::endl;
	std::cout << "��������� ";
	postOrder();
	std::cout << std::endl;
	std::cout << "��������� ";
	layerOrder();
	std::cout << std::endl;
}

//Ѱ�ҽڵ��ǰ���ڵ�
template<typename T>
BSNode<T>* BSTree<T>::predecessor(node_pointer pnode) const
{
	//���pnode��������
	if (pnode->lchild != nullptr)
	{
		pnode = pnode->lchild;
		while (pnode->rchild!=nullptr)
		{
			pnode = pnode->rchild;
		}
		return pnode;	
	}
	//���û�������������������һ��������Ϊ��������������Ϊ��������
	if (pnode->parent->rchild == pnode)//�����
		return pnode->parent;
	else//���һ Ѱ�ҵ�һ��Ϊ�������ĸ��ڵ�ĸ��ڵ�
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
	//����ڵ����������������̽ڵ�Ϊ������������ڵ�
	if (pnode->rchild != nullptr)
	{
		pnode = pnode->rchild;
		while (pnode->lchild!=nullptr)//Ѱ��������������ڵ�
		{
			pnode = pnode->lchild;
		}
		return pnode;
	}
	//////////////////////////////////////////////////////
	// 
	//
	//                |---->����������������̽ڵ�Ϊ�丸�ڵ�
	//			      |
	//���û��������---|
	//				  |
	//				  |---->����������������̽ڵ�Ϊ�䡰������������������ڵ㡱
	//
	////////////////////////////////////////////////////////////
	node_pointer pparent = pnode->parent;
	while (pparent != nullptr&&pparent->rchild == pnode)//���pnode!=pparent->rchild,pnode��pparent����������Ҳ�����ҵ���
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
		if (pnode->value == key)//�ҵ�ɾ���Ķ���
		{
			node_pointer pdel = nullptr;
			//pnodeΪֻ������������ֻ��������������û������
			if (pnode->lchild == nullptr || pnode->rchild == nullptr)
				pdel = pnode;
			else//pnode������������ʹ��ǰ������
			{
				pdel = predecessor(pnode);
			}
			//��ʱҪɾ���Ľڵ�ֻ��һ�����ӻ���û�к��ӣ����溢��ָ��
			node_pointer pchild = nullptr;
			if (pdel->lchild != nullptr)
				pchild = pdel->lchild;
			else
			{
				pchild = pdel->rchild;
			}
			//�ú���ָ��ɾ�ڵ�ĸ��ڵ�
			if (pchild != nullptr)
				pchild->parent = pdel->parent;
			//���ɾ����ͷ�ڵ㣬���޸�root��ֵ
			if (pdel->parent == nullptr)
				root = pchild;
			else if(pdel->parent->lchild==pdel)//�������ͷ�ڵ㲢������ڵ�
			{
				pdel->parent->lchild = pchild;
			}
			else//����ͷ�ڵ������ҽڵ�
			{
				pdel->parent->rchild = pchild;
			}
			//���ɾ���ڵ��ֵ��Ŀ��ɾ���ڵ�ֵ��һ�������Ǳ�ɾ�ڵ������������������pdelָ����Ǳ�ɾ�ڵ��ǰ���ڵ㣬����ɾ��֮����ֵ��ԭ��ɾ���ڵ㡣
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