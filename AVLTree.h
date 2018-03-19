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
	}//��������

	void preOrder() const;//ǰ�����
	void inOrder() const;//�������
	void postOrder() const;//��������
	void layerOrder() const;//�������

	node_pointer search_recursion(value_type key) const;//�ݹ����
	node_pointer search_iterator(value_type key) const;//��������

	value_type search_min() const;
	value_type search_max() const;
	node_pointer successor(node_pointer pnode) const;//����ָ���ڵ�ĺ�̽ڵ�
	node_pointer predecessor(node_pointer pnode) const;//����ָ���ڵ��ǰ���ڵ�

	void insert(value_type key);
	void remove(value_type key);

	void destory();
	void print() const;

	int height();//�������ĸ߶�

protected:

	node_pointer root;

	int height(node_pointer pnode);//���ؽڵ�ĸ߶�
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

	int isNotBalance(node_pointer pnode);//����ڵ��Ƿ�ƽ��
	//void blance(node_pointer pnode);//��pnode����ά��ƽ��
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

/*����ת����*/
/*pnodeΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
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
/*����ת����*/
/*pnodeΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
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
/*������������*/
/*����pnodeΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
template<typename T>
typename AVLTree<T>::node_pointer AVLTree<T>::leftRightRotation(node_pointer pnode)
{
	pnode->lchild = leftRotation(pnode->lchild);
	return rightRotation(pnode);
}
/*������������*/
/*����prootΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
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
		return 0;//ƽ��
	else if(BF>=2)
	{
		return 1;//�����С
	}
	else if(BF<-2)
	{
		return -1;//��С�Ҵ�
	}
}

template<typename T>
void AVLTree<T>::insert(value_type key)
{
	node_pointer pparent = nullptr;
	node_pointer pnode = root;

	bool isLeftChild;//������Ƿ�������
	bool isLeftChildTree;//������Ƿ��ڲ�ƽ��Ľڵ��������


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

	//ά��ƽ��

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
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
	while (key != pnode->value)//�ҵ�ɾ����
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
	else if (pdel->parent->lchild == pdel)//�������ͷ�ڵ㲢������ڵ�
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

	pnode = pdel->parent;//ʹpnodeָ��ɾ����ĸ���

	delete pdel;

	//�ҵ���ƽ��ĵ�
	while (!isNotBalance(pnode) && pnode->parent != nullptr)
	{
		pnode = pnode->parent;
	}

	node_pointer pparent = pnode->parent;//��ƽ��ڵ�ĸ��ڵ�
	int pnodeBlance = isNotBalance(pnode);
	if (pnodeBlance>0)//��ƽ��ڵ�����ҵ�
	{
		if (height(pnode->lchild->lchild) > height(pnode->lchild->rchild))//��ƽ��ڵ����������������������������������������������
		{

			if (pparent == nullptr)//pnodeΪ���ڵ�
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
		else//��ƽ��ڵ�ĵ���������������С����������������������Ҫ������������
		{
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
	else if (pnodeBlance < 0)//��ƽ��ڵ�����Ҹ�
	{
		if (height(pnode->rchild->lchild) > height(pnode->rchild->rchild))//������������������
		{
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
		else//����������
		{
			if (pparent == nullptr)//pnodeΪ���ڵ�
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
	//���pnode��������
	if (pnode->lchild != nullptr)
	{
		pnode = pnode->lchild;
		while (pnode->rchild != nullptr)
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
	//����ڵ����������������̽ڵ�Ϊ������������ڵ�
	if (pnode->rchild != nullptr)
	{
		pnode = pnode->rchild;
		while (pnode->lchild != nullptr)//Ѱ��������������ڵ�
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
//�������
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
//�������
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