#include"BStree.h"
#include"AVLTree.h"
#include"RBTree.h"
#include<iostream>

int main(void)
{
	RBTree<int> t;
	t.insert(12);
	t.insert(1);
	t.insert(9);
	t.insert(2);
	t.insert(0);
	t.insert(11);
	t.insert(7);
	t.insert(19);
	t.insert(4);
	t.insert(15);
	t.insert(18);
	t.insert(5);
	t.insert(14);
	t.insert(13);
	t.insert(10);
	t.insert(16);
	t.insert(6);
	t.insert(3);
	t.insert(8);
	t.insert(17);

	int height = t.height();
	t.print();

	std::cout << t.search_max()<<std::endl;
	std::cout << t.search_min()<<std::endl;

	bool ok=t.check();
	t.remove(12);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(1);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(9);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(2);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(0);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(11);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(7);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(19);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(4);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(15);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(18);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(5);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(14);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(13);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(10);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(16);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(6);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(3);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(8);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;
	t.remove(17);
	ok = t.check();
	if (ok)
		std::cout << "OK" << std::endl;
	else
		std::cout << "not ok" << std::endl;

	double lalala = 1 / 3;
	
	return 0;
}

