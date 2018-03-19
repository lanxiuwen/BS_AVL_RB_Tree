#pragma once
#include<iostream>

typedef enum RBTreeColor
{
	Black,
	Red
}RBColor;

template<typename T>
class RBNode
{
public:
	typedef typename T value_type;
	typedef typename RBNode<T> node;
	typedef typename RBNode<T>* node_pointer;
	//typedef RBColor rb_color;

	value_type value;
	node_pointer lchild;
	node_pointer rchild;
	node_pointer parent;
	RBColor color;

	RBNode(value_type v)
		:value(v), color(Red), lchild(nullptr), rchild(nullptr), parent(nullptr)
	{}
};

template<typename T>
class RBTree
{
public:
	typedef typename T value_type;
	typedef typename RBNode<T> node;
	typedef typename RBNode<T>* node_pointer;

	RBTree()
	:root(nullptr)
	{}
	RBTree(value_type t)
		:root(t)
	{}
	~RBTree() {}

	bool insert(value_type key);
	bool remove(value_type key);

	bool check();//��������ĺϷ���
	node_pointer successor(node_pointer pnode) const;//����ָ���ڵ�ĺ�̽ڵ�
	node_pointer predecessor(node_pointer pnode) const;//����ָ���ڵ��ǰ���ڵ�
	void preOrder() const;//ǰ�����
	void inOrder() const;//�������
	void postOrder() const;//��������
	void layerOrder() const;//�������

	node_pointer search_recursion(value_type key) const;//�ݹ����
	node_pointer search_iterator(value_type key) const;//��������

	value_type search_min() const;
	value_type search_max() const;

	void destory();
	void print() const;

	int height();//�������ĸ߶�

private:
	node_pointer root;
	bool adjust(node_pointer pdel,  bool recall = false);
	bool remove(node_pointer pnode, value_type key);
	bool _check(node_pointer root, int blackNum, int CBNum);//��鸨������
	node_pointer leftRotation(node_pointer pnode);
	node_pointer rightRotation(node_pointer pnode);
	node_pointer leftRightRotation(node_pointer pnode);
	node_pointer rightLeftRotation(node_pointer pnode);
	void preOrder(node_pointer p) const;
	void inOrder(node_pointer p) const;
	void postOrder(node_pointer p) const;
	void layerOrder(node_pointer p) const;
	node_pointer search(node_pointer p, value_type key) const;
	value_type search_minimun(node_pointer p) const;
	value_type search_maximum(node_pointer p) const;
	void destory(node_pointer &p);
	int height(node_pointer pnode);
};

template<typename T>
bool RBTree<T>::check()
{
	int BlackNum = 0;
	node_pointer cur = root;
	//��ͳ�������֧·�ĺ�ɫ����
	while (cur)
	{
		if (cur->color == Black)
			BlackNum++;
		cur = cur->lchild;
	}
	int CBNum = 0;
	return _check(root, BlackNum, CBNum);
	
}
template<typename T>
bool RBTree<T>::_check(node_pointer root, int blackNum, int CBNum)
{
	if (root == nullptr)
		return true;
	if (root->color == Black)
	{
		CBNum++;
		if (root->lchild == nullptr&&root->rchild == nullptr)//������Ҷ�ڵ�
		{
			if (blackNum == CBNum)
				return true;
			else
			{
				std::cout << "Ҷ�ڵ�Ϊ " << root->value << " ·����ɫ�ڵ���������֧·��һ��" << std::endl;
				return false;
			}
		}
	}
	else if(root->parent&&root->parent->color==Red)//���ڵ�ĸ��ڵ�������Ƿ�Ϊ���������ĺ�ɫ�ڵ�
	{
		std::cout << root->value << " AND " << root->parent->color << " both are red" << std::endl;
		return false;
	}
	//�ݹ�������֧·��
	return _check(root->lchild, blackNum, CBNum) && _check(root->rchild, blackNum, CBNum);
}


template<typename T>
bool RBTree<T>::insert(value_type key)
{
	//���뵽���ڵ�
	if (root == nullptr)
	{
		root = new node(key);
		root->color = Black;
		return true;
	}
	//Ѱ�Ҳ����
	node_pointer pnode = root;
	node_pointer pparent = nullptr;

	while (pnode)
	{
		if (key > pnode->value)
		{
			pparent = pnode;
			pnode = pnode->rchild;
		}
		else
		{
			pparent = pnode;
			pnode = pnode->lchild;
		}
	}
	//����
	pnode = new node(key);
	pnode->parent = pparent;
	if (pparent->value < key)
		pparent->rchild = pnode;
	else
		pparent->lchild = pnode;


	//�����ɫ�����Ƿ�����Ҫ��
	while (pparent&&pparent->color == Red&&pnode->color == Red)
	{
		node_pointer pgrand = pparent->parent;//�游�ڵ�
		if (pparent == pgrand->lchild)//���ڵ����游�ڵ��������
		{
			node_pointer puncle = pgrand->rchild;
			if (puncle&&(puncle->color == Red))
			{
				//case1 ���죬��ڣ���죬��ɫ����
				pparent->color = Black;
				puncle->color = Black;
				pgrand->color = Red;

				pnode = pgrand;//����Ӱ�����棬����
				pparent = pgrand->parent;
			}
			else if ((puncle&&(puncle->color == Black)) || puncle == nullptr)
			{
				if (pnode == pparent->lchild)
				{
					//case2 ���죬��ڣ���ڣ��������ɫ������,������ڸ�����ڵ�
					pparent->color = Black;
					pgrand->color = Red;
					if (pgrand->parent == nullptr)
						root=rightRotation(pgrand);
					else
					{
						rightRotation(pgrand);
					}
					pnode = pgrand;//�游��ɫ�ˣ���Ҫ����
					pparent = pnode->parent;
				}
				else
				{
					//case3 ���죬��ڣ���ڣ������ڸ����ҽڵ�,���������ɫ��������
					pgrand->color = Red;
					pnode->color = Black;
					if(pgrand->parent==nullptr)//Ҫ�����游�ڵ�Ϊroot��Ҫ�޸�root
						root=leftRightRotation(pgrand);
					else 
						leftRightRotation(pgrand);
					pnode = pgrand;//�游��ɫ�ˣ���Ҫ����
					pparent = pnode->parent;
				}
			}
		}
		else if (pparent == pgrand->rchild)//���ڵ����游�ڵ��������
		{
			node_pointer puncle = pgrand->lchild;
			if (puncle&&(puncle->color == Red))
			{
				//case1 ���죬��ڣ���죬��ɫ����
				pgrand->color = Red;
				pparent->color = Black;
				puncle->color = Black;

				pnode = pgrand;//�游��ɫ�ˣ���Ҫ����
				pparent = pnode->parent;
			}
			else if ((puncle&&(puncle->color == Black)) || puncle == nullptr)
			{
				if (pnode == pparent->lchild)
				{
					//case2 ���죬��ڣ���ڣ������ڸ�����ڵ�,���������ɫ��������
					pnode->color = Black;
					pgrand->color = Red;
					if (pgrand->parent == nullptr)
						root=rightLeftRotation(pgrand);
					else
					{
						rightLeftRotation(pgrand);
					}
					pnode = pgrand;//�游��ɫ�ˣ���Ҫ����
					pparent = pnode->parent;
				}
				else if(pnode==pparent->rchild)
				{
					//case3 ���죬��ڣ���ڣ��������ɫ������
					pgrand->color = Red;
					pparent->color = Black;
					if (pgrand->parent == nullptr)
						root=leftRotation(pgrand);
					else
						leftRotation(pgrand);
					pnode = pgrand;//�游��ɫ�ˣ���Ҫ����
					pparent = pnode->parent;
				}
			}
		}
	}
	root->color = Black;//���ڵ���Ϊ��ɫ
	return true;
}

template<typename T>
bool RBTree<T>::remove(value_type key)
{
	return remove(root, key);
}
template<typename T>
bool RBTree<T>::remove(node_pointer pnode, value_type key)
{
	if (pnode == nullptr)
		return false;
	while (key != pnode->value)//�ҵ�ɾ����
	{
		if (key > pnode->value)
		{
			pnode = pnode->rchild;
		}
		else if (key<pnode->value)
		{
			pnode = pnode->lchild;
		}
		else if (pnode == nullptr)
		{
			return false;
		}
	}
	node_pointer pdel = nullptr;

	//pnodeΪֻ������������ֻ��������������û������
	if (pnode->lchild == nullptr || pnode->rchild == nullptr)
		pdel = pnode;
	else//pnode������������ʹ�ú�̶���
	{
		pdel = successor(pnode);
	}
	//��ʱҪɾ���Ľڵ�ֻ��һ�����ӻ���û�к��ӣ����溢��ָ��
	node_pointer pchild = nullptr;
	if (pdel->lchild != nullptr)
		pchild = pdel->lchild;
	else
	{
		pchild = pdel->rchild;
	}
	////�ú���ָ��ɾ�ڵ�ĸ��ڵ�
	//if (pchild != nullptr)
	//	pchild->parent = pdel->parent;
	////���ɾ����ͷ�ڵ㣬���޸�root��ֵ
	//if (pdel->parent == nullptr)
	//	root = pchild;
	//else if (pdel->parent->lchild == pdel)//�������ͷ�ڵ㲢������ڵ�
	//{
	//	pdel->parent->lchild = pchild;
	//}
	//else//����ͷ�ڵ������ҽڵ�
	//{
	//	pdel->parent->rchild = pchild;
	//}

	node_pointer ppdel = pdel;//ָ��ɾ�����λ�ã�����pdel;

	//���ɾ���ڵ��ֵ��Ŀ��ɾ���ڵ�ֵ��һ�������Ǳ�ɾ�ڵ������������������pdelָ����Ǳ�ɾ�ڵ��ǰ���ڵ㣬����ɾ��֮����ֵ��ԭ��ɾ���ڵ㡣
	if (pnode->value != pdel->value)
	{
		pnode->value = pdel->value;
	}

	if (adjust(ppdel))
	{
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


		delete pdel;
		return true;
	}
	else
	{
		return false;
	}
}
template<typename T>
bool RBTree<T>::adjust(node_pointer pdel,bool recall=false)
{
	node_pointer pparent = pdel->parent;
	node_pointer prbro = nullptr;
	node_pointer plbro = nullptr;
	if (pparent)
	{
		if (pdel == pparent->lchild)
			prbro = pparent->rchild;//ɾ��������ֵ�
		else
			plbro = pparent->lchild;//ɾ��������ֵ�
	}

	//��ʼ��������

	if (pdel->color == Red)
	{

		return true;
	}
	if (pdel->color == Black)
	{
		if (pparent == nullptr)
		{

			root->color = Black;
			return true;
		}
		if (pdel->lchild && pdel->lchild->color == Red&&!recall)
		{
			pdel->lchild->color = Black;
			return true;
		}
		else if (pdel->rchild && pdel->rchild->color == Red&&!recall)
		{
			pdel->rchild->color = Black;
			return true;
		}

		if (!plbro && !prbro&&pparent->color == Red)
		{
			pparent->color = Black;
			return true;
		}
		if (plbro
			&&plbro->color == Black
			&& pparent->color == Red
			&& (plbro->lchild == nullptr||plbro->lchild->color == Black)
			&& (plbro->rchild == nullptr||plbro->rchild->color == Black))
		{
			plbro->color = Red;
			pparent->color = Black;
			return true;
		}
		if (prbro
			&& prbro->color == Black
			&& pparent->color == Red
			&& (prbro->lchild == nullptr||prbro->lchild->color == Black )
			&& (prbro->rchild == nullptr||prbro->rchild->color == Black ))
		{
			prbro->color = Red;
			pparent->color = Black;
			return true;
		}
		if ((plbro || prbro) && pparent->color == Red)
		{
			if (plbro)
			{
				if ((plbro->lchild) && plbro->lchild->color == Red)
				{
					pparent->color = Black;
					plbro->color = Red;
					plbro->lchild->color = Black;
					rightRotation(pparent);
					return true;
				}
				else if ((plbro->rchild) && plbro->rchild->color == Red)
				{
					pparent->color = Black;
					leftRightRotation(pparent);
					return true;
				}
			}
			if (prbro)
			{
				if ((prbro->lchild) && prbro->lchild->color == Red)
				{
					pparent->color = Black;
					rightLeftRotation(pparent);
					return true;
				}
				else if ((prbro->rchild) && prbro->rchild->color == Red)
				{
					pparent->color = Black;
					prbro->color = Red;
					prbro->rchild->color = Black;
					leftRotation(pparent);
					return true;
				}
			}
		}

		if ((plbro || prbro) && pparent->color == Black)
		{
			if (prbro)
			{
				if ((prbro->rchild) && prbro->rchild->color == Red)
				{
					prbro->rchild->color = Black;
					leftRotation(pparent);
					return true;
				}
				if ((prbro->lchild) && prbro->lchild->color == Red)
				{
					prbro->lchild->color = Black;
					rightLeftRotation(pparent);
					return true;
				}
			}
			if (plbro)
			{
				if ((plbro->rchild) && plbro->rchild->color == Red)
				{
					plbro->rchild->color = Black;
					leftRightRotation(pparent);
					return true;
				}
				if ((plbro->lchild) && plbro->lchild->color == Red)
				{
					plbro->lchild->color = Black;
					rightRotation(pparent);
					return true;
				}
			}
		}
		if (pparent->color == Black)
		{
			if (prbro
				&&prbro->color == Black
				&& (prbro->lchild == nullptr||prbro->lchild->color == Black )
				&& (prbro->rchild == nullptr||prbro->rchild->color == Black ))
			{
				prbro->color = Red;
				return adjust(pparent,true);
			}
			if (plbro
				&&plbro->color == Black
				&& (plbro->lchild == nullptr||plbro->lchild->color == Black)
				&& (plbro->rchild == nullptr||plbro->rchild->color == Black))
			{
				plbro->color = Red;
				return adjust(pparent,true);
			}
		}
		if (pparent->color == Black&&prbro&&prbro->color == Red)
		{
			prbro->color = Black;
			pparent->color = Red;
			node_pointer temp = leftRotation(pparent);
			temp->lchild->color = Black;
			if (temp->lchild->rchild)
				temp->lchild->rchild->color = Red;
			if (temp->parent == nullptr)
				root = temp;
			return true;
		}
		if (pparent->color == Black&&plbro&&plbro->color == Red)
		{
			plbro->color = Black;
			pparent->color = Red;
			node_pointer temp = rightRotation(pparent);
			temp->rchild->color = Black;
			if (temp->rchild->lchild)
				temp->rchild->lchild->color = Red;
			if (temp->parent == nullptr)
				root = temp;
			return true;
		}
	}
}



/*����ת����*/
/*pnodeΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::leftRotation(node_pointer pnode)
{
	node_pointer prchild = pnode->rchild;
	node_pointer pparent = pnode->parent;

	bool isLeftChild=false;
	bool isRightChild = false;
	if (pparent != nullptr)
	{
		if (pnode == pparent->lchild)
			isLeftChild = true;
		if (pnode == pparent->rchild)
			isRightChild = true;
	}

	pnode->rchild = prchild->lchild;
	if (pnode->rchild != nullptr)
		pnode->rchild->parent = pnode;
	prchild->lchild = pnode;
	pnode->parent = prchild;
	prchild->parent = pparent;
	if (isLeftChild && !isRightChild)
		pparent->lchild = prchild;
	else if(!isLeftChild&&isRightChild)
	{
		pparent->rchild = prchild;
	}

	//pnode->height = MAX(height(pnode->lchild), height(pnode->rchild)) + 1;
	//prchild->height = MAX(height(prchild->lchild), height(prchild->rchild)) + 1;

	return prchild;
}
/*����ת����*/
/*pnodeΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::rightRotation(node_pointer pnode)
{

	node_pointer plchild = pnode->lchild;
	node_pointer pparent = pnode->parent;

	bool isLeftChild = false;
	bool isRightChild = false;
	if (pparent != nullptr)
	{
		if (pnode == pparent->lchild)
			isLeftChild = true;
		if (pnode == pparent->rchild)
			isRightChild = true;
	}

	pnode->lchild = plchild->rchild;
	if (pnode->lchild != nullptr)
		pnode->lchild->parent = pnode;
	plchild->rchild = pnode;
	pnode->parent = plchild;
	plchild->parent = pparent;

	if (isLeftChild && !isRightChild)
		pparent->lchild = plchild;
	else if (!isLeftChild&&isRightChild)
	{
		pparent->rchild = plchild;
	}

	//pnode->height = MAX(height(pnode->lchild), height(pnode->rchild)) + 1;
	//plchild->height = MAX(height(plchild->lchild), height(plchild->rchild)) + 1;
	return plchild;
}
/*������������*/
/*����pnodeΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::leftRightRotation(node_pointer pnode)
{
	pnode->lchild = leftRotation(pnode->lchild);
	return rightRotation(pnode);
}
/*������������*/
/*����prootΪ��Сʧ�������ĸ��ڵ�*/
/*������ת��ĸ��ڵ�*/
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::rightLeftRotation(node_pointer pnode)
{
	pnode->rchild = rightRotation(pnode->rchild);
	return leftRotation(pnode);
}

template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::predecessor(node_pointer pnode) const
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
typename RBTree<T>::node_pointer RBTree<T>::successor(node_pointer pnode) const
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
void RBTree<T>::preOrder() const
{
	preOrder(root);
}

template<typename T>
void RBTree<T>::preOrder(node_pointer p) const
{
	if (p != nullptr)
	{
		std::cout << p->value << " ";
		preOrder(p->lchild);
		preOrder(p->rchild);
	}
}
template<typename T>
void RBTree<T>::inOrder() const
{
	inOrder(root);
}
template<typename T>
void RBTree<T>::inOrder(node_pointer p) const
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
void RBTree<T>::postOrder() const
{
	postOrder(root);
}
template<typename T>
void RBTree<T>::postOrder(node_pointer p) const
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
void RBTree<T>::layerOrder() const
{
	layerOrder(root);
}
template<typename T>
void RBTree<T>::layerOrder(node_pointer pnode) const
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
void RBTree<T>::print() const
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
void RBTree<T>::destory(node_pointer & pnode)
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
void RBTree<T>::destory()
{
	destory(root);
}
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::search(node_pointer pnode, value_type key) const
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
typename RBTree<T>::node_pointer RBTree<T>::search_recursion(value_type key) const
{
	return search(root, key);
}
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::search_iterator(value_type key) const
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
typename RBTree<T>::value_type RBTree<T>::search_minimun(node_pointer pnode) const
{
	while (pnode->lchild != nullptr)
		pnode = pnode->lchild;
	return pnode->value;
}
template<typename T>
typename RBTree<T>::value_type RBTree<T>::search_min() const
{
	if (root == nullptr)
		return value_type(0);
	return search_minimun(root);
}
template<typename T>
typename RBTree<T>::value_type RBTree<T>::search_maximum(node_pointer pnode) const
{
	while (pnode->rchild != nullptr)
		pnode = pnode->rchild;
	return pnode->value;
}
template<typename T>
typename RBTree<T>::value_type RBTree<T>::search_max() const
{
	if (root == nullptr)
		return value_type(0);
	return search_maximum(root);
}
template<typename T>
int RBTree<T>::height()
{
	return height(root);
}
template<typename T>
int RBTree<T>::height(node_pointer pnode)
{
	int u, v;
	if (pnode == nullptr) return -1;
	u = height(pnode->lchild); v = height(pnode->rchild);
	return u > v ? (u + 1) : (v + 1);
}