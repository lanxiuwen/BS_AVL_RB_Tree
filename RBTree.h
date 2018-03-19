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

	bool check();//检查红黑树的合法性
	node_pointer successor(node_pointer pnode) const;//查找指定节点的后继节点
	node_pointer predecessor(node_pointer pnode) const;//查找指定节点的前驱节点
	void preOrder() const;//前序遍历
	void inOrder() const;//中序遍历
	void postOrder() const;//后续遍历
	void layerOrder() const;//层序遍历

	node_pointer search_recursion(value_type key) const;//递归查找
	node_pointer search_iterator(value_type key) const;//迭代查找

	value_type search_min() const;
	value_type search_max() const;

	void destory();
	void print() const;

	int height();//返回树的高度

private:
	node_pointer root;
	bool adjust(node_pointer pdel,  bool recall = false);
	bool remove(node_pointer pnode, value_type key);
	bool _check(node_pointer root, int blackNum, int CBNum);//检查辅助函数
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
	//先统计最左边支路的黑色数量
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
		if (root->lchild == nullptr&&root->rchild == nullptr)//到达了叶节点
		{
			if (blackNum == CBNum)
				return true;
			else
			{
				std::cout << "叶节点为 " << root->value << " 路径黑色节点数与最左支路不一致" << std::endl;
				return false;
			}
		}
	}
	else if(root->parent&&root->parent->color==Red)//检查节点的父节点存在且是否为两个连续的红色节点
	{
		std::cout << root->value << " AND " << root->parent->color << " both are red" << std::endl;
		return false;
	}
	//递归检查左右支路。
	return _check(root->lchild, blackNum, CBNum) && _check(root->rchild, blackNum, CBNum);
}


template<typename T>
bool RBTree<T>::insert(value_type key)
{
	//插入到根节点
	if (root == nullptr)
	{
		root = new node(key);
		root->color = Black;
		return true;
	}
	//寻找插入点
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
	//插入
	pnode = new node(key);
	pnode->parent = pparent;
	if (pparent->value < key)
		pparent->rchild = pnode;
	else
		pparent->lchild = pnode;


	//检查颜色分配是否满足要求
	while (pparent&&pparent->color == Red&&pnode->color == Red)
	{
		node_pointer pgrand = pparent->parent;//祖父节点
		if (pparent == pgrand->lchild)//父节点在祖父节点的左子树
		{
			node_pointer puncle = pgrand->rchild;
			if (puncle&&(puncle->color == Red))
			{
				//case1 父红，祖黑，叔红，变色即可
				pparent->color = Black;
				puncle->color = Black;
				pgrand->color = Red;

				pnode = pgrand;//可能影响上面，回溯
				pparent = pgrand->parent;
			}
			else if ((puncle&&(puncle->color == Black)) || puncle == nullptr)
			{
				if (pnode == pparent->lchild)
				{
					//case2 父红，祖黑，叔黑，父与祖变色后右旋,插入点在父的左节点
					pparent->color = Black;
					pgrand->color = Red;
					if (pgrand->parent == nullptr)
						root=rightRotation(pgrand);
					else
					{
						rightRotation(pgrand);
					}
					pnode = pgrand;//祖父变色了，需要回溯
					pparent = pnode->parent;
				}
				else
				{
					//case3 父红，祖黑，叔黑，插入在父的右节点,祖与插入点变色后左右旋
					pgrand->color = Red;
					pnode->color = Black;
					if(pgrand->parent==nullptr)//要考虑祖父节点为root，要修改root
						root=leftRightRotation(pgrand);
					else 
						leftRightRotation(pgrand);
					pnode = pgrand;//祖父变色了，需要回溯
					pparent = pnode->parent;
				}
			}
		}
		else if (pparent == pgrand->rchild)//父节点在祖父节点的右子树
		{
			node_pointer puncle = pgrand->lchild;
			if (puncle&&(puncle->color == Red))
			{
				//case1 父红，祖黑，叔红，变色即可
				pgrand->color = Red;
				pparent->color = Black;
				puncle->color = Black;

				pnode = pgrand;//祖父变色了，需要回溯
				pparent = pnode->parent;
			}
			else if ((puncle&&(puncle->color == Black)) || puncle == nullptr)
			{
				if (pnode == pparent->lchild)
				{
					//case2 父红，祖黑，叔黑，插入在父的左节点,祖与插入点变色后右左旋
					pnode->color = Black;
					pgrand->color = Red;
					if (pgrand->parent == nullptr)
						root=rightLeftRotation(pgrand);
					else
					{
						rightLeftRotation(pgrand);
					}
					pnode = pgrand;//祖父变色了，需要回溯
					pparent = pnode->parent;
				}
				else if(pnode==pparent->rchild)
				{
					//case3 父红，祖黑，叔黑，父与祖变色后左旋
					pgrand->color = Red;
					pparent->color = Black;
					if (pgrand->parent == nullptr)
						root=leftRotation(pgrand);
					else
						leftRotation(pgrand);
					pnode = pgrand;//祖父变色了，需要回溯
					pparent = pnode->parent;
				}
			}
		}
	}
	root->color = Black;//根节点设为黑色
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
	while (key != pnode->value)//找到删除点
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

	//pnode为只有左子树或者只有右子树，或者没有子树
	if (pnode->lchild == nullptr || pnode->rchild == nullptr)
		pdel = pnode;
	else//pnode有左右子树，使用后继顶替
	{
		pdel = successor(pnode);
	}
	//此时要删除的节点只有一个孩子或者没有孩子，保存孩子指针
	node_pointer pchild = nullptr;
	if (pdel->lchild != nullptr)
		pchild = pdel->lchild;
	else
	{
		pchild = pdel->rchild;
	}
	////让孩子指向被删节点的父节点
	//if (pchild != nullptr)
	//	pchild->parent = pdel->parent;
	////如果删的是头节点，就修改root的值
	//if (pdel->parent == nullptr)
	//	root = pchild;
	//else if (pdel->parent->lchild == pdel)//如果不是头节点并且是左节点
	//{
	//	pdel->parent->lchild = pchild;
	//}
	//else//不是头节点且是右节点
	//{
	//	pdel->parent->rchild = pchild;
	//}

	node_pointer ppdel = pdel;//指向删除点的位置，代替pdel;

	//如果删除节点的值与目标删除节点值不一样，就是被删节点有左右子树的情况，pdel指向的是被删节点的前驱节点，所以删除之，其值给原意删除节点。
	if (pnode->value != pdel->value)
	{
		pnode->value = pdel->value;
	}

	if (adjust(ppdel))
	{
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
			prbro = pparent->rchild;//删除点的右兄弟
		else
			plbro = pparent->lchild;//删除点额左兄弟
	}

	//开始调整性质

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



/*左旋转操作*/
/*pnode为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
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
/*右旋转操作*/
/*pnode为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
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
/*先左旋再右旋*/
/*参数pnode为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::leftRightRotation(node_pointer pnode)
{
	pnode->lchild = leftRotation(pnode->lchild);
	return rightRotation(pnode);
}
/*先右旋再左旋*/
/*参数proot为最小失衡子树的根节点*/
/*返回旋转后的根节点*/
template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::rightLeftRotation(node_pointer pnode)
{
	pnode->rchild = rightRotation(pnode->rchild);
	return leftRotation(pnode);
}

template<typename T>
typename RBTree<T>::node_pointer RBTree<T>::predecessor(node_pointer pnode) const
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
typename RBTree<T>::node_pointer RBTree<T>::successor(node_pointer pnode) const
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
//后序遍历
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
//层序遍历
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