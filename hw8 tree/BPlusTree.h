/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/

#include <vector>
#include <fstream>
#include <algorithm>
#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE
using namespace std;
///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode{
public:
	BPlusTreeNode() : parent(NULL) {};
	bool is_leaf();
	bool contains(const T& key);

	//For grading only, this is actually bad to have because
	//it exposes a private member variable.
	BPlusTreeNode* get_parent() { return parent; } 

	//We need this to let BPlusTree access private members
	friend class BPlusTree<T>; 
private:
	bool contains(const T& key,std::size_t low,std::size_t high);
	std::vector<T> keys;
	std::vector<BPlusTreeNode*> children;
	BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
	for(unsigned int i=0; i<children.size(); i++){
		if(children[i]){
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
	if(low>high){
		return false;
	}
	if(low==high){
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]){
		return contains(key,0,mid);
	}
	else{
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////

template <class T>
class BPlusTree {
public:
	BPlusTree(int Max_degree);
	BPlusTree(const BPlusTree<T>& tree);
	~BPlusTree() { this->clear(root); }
	void insert(const T& key);
	BPlusTreeNode<T>* find(const T& key);
	BPlusTree<T>& operator=(const BPlusTree<T>& tree);
	void clear(BPlusTreeNode<T>* root);
	void print_sideways(ofstream& output);
	void print_BFS(ofstream& output);
	void print_BFS_pretty(ofstream& output);
private:
	BPlusTreeNode<T>* copy(BPlusTreeNode<T>* node);
	BPlusTreeNode<T>* find(BPlusTreeNode<T>* node ,const T& key);
	void print_sideways(ofstream& output, BPlusTreeNode<T>* node,int count);
	void print_BFS(ofstream& output, BPlusTreeNode<T>* node);
	void split(BPlusTreeNode<T>* node,const T& key);
	int max_degree;
	BPlusTreeNode<T>* root;
};

//constructor 1
template <class T>
BPlusTree<T>::BPlusTree(int Max_degree) {
	max_degree = Max_degree;
	root = new BPlusTreeNode<T>;
}
//constructor 2
template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& tree) {
	max_degree = tree.max_degree;
	root = this->copy(tree.root);
}
//clear
template <class T>
void BPlusTree<T>::clear(BPlusTreeNode<T>* root) {
	if (root != NULL) {
		int size = root->children.size();
		for (int i = 0; i < size; i++) {
			clear(root->children[i]);
		}
		delete root;
	}
}
//= operator
template <class T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& tree) {
	clear(root);
	max_degree = tree.max_degree;
	root = this->copy(tree.root);
	return *this;
}
//= operator helper copy()
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::copy(BPlusTreeNode<T>* node) {
	BPlusTreeNode<T>* new_node = new BPlusTreeNode<T>;
	if (node == NULL) return new_node;
	else {
		new_node->parent = node->parent;
		new_node->keys = node->keys;
		new_node->children = node->children;
		for (int i = 0; i < node->children.size(); i++)
			new_node->children[i] = copy(node->children[i]);
		return new_node;
	}
}
//insert
template <class T>
void BPlusTree<T>::insert(const T& key) {
	BPlusTreeNode<T>* node = find(key);
	if (node == NULL) root->keys.push_back(key);
	else {
		node->keys.push_back(key);
		sort(node->keys.begin(), node->keys.end());
		if (node->keys.size() == max_degree) split(node,key);
	}

}
//split
template <class T>
void BPlusTree<T>::split(BPlusTreeNode<T>* node,const T& key) {
	//split in leaf
	if (node->is_leaf()) {
		int left_size = max_degree / 2;
		//new left branch
		BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
		for (int i = 0; i < left_size; i++)
			left->keys.push_back(node->keys[i]);
		//new right branch
		BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
		for (int i = left_size; i < max_degree; i++) 
			right->keys.push_back(node->keys[i]);
		//if it has no parent(it's the root)
		if (node->parent==NULL) {
			BPlusTreeNode<T>* new_root = new BPlusTreeNode<T>;
			new_root->children.push_back(left);
			new_root->children.push_back(right);
			new_root->keys.push_back(right->keys[0]);
			left->parent = new_root; right->parent = new_root; 
			delete node;
			root = new_root;
		}
		//if it has parent
		else {
			//find the corresponding position in parent's children vector
			int position = -1;
			for (int i = 0; i < node->parent->keys.size(); i++) {
				if (key < node->parent->keys[i]) break;
				position = i;
			}
			//erase and insert in parent's children vector
			node->parent->children.erase(node->parent->children.begin()+position+1);
			node->parent->children.insert
			(node->parent->children.begin() + position + 1, left);
			node->parent->children.insert
			(node->parent->children.begin() + position + 2, right);
			//link the new left and right branch to the parent
			left->parent = node->parent;
			right->parent = node->parent;
			//insert the key into parent's key
			node->parent->keys.insert
				(node->parent->keys.begin()+position+1,right->keys[0]);
			delete node;
			//if parent is broken
			if (right->parent->keys.size() == max_degree) {
					split(right->parent,key);
			}
		}
	}
	//split in branch
	else {
		int left_size = max_degree / 2;
		//new left branch
		BPlusTreeNode<T>* left = new BPlusTreeNode<T>;
		//put keys in it
		for (int i = 0; i < left_size; i++)
			left->keys.push_back(node->keys[i]);
		//put children in it
		//and link children to it
		for (int i = 0; i < left_size + 1; i++) {
			left->children.push_back(node->children[i]);
			node->children[i]->parent = left;
		}
		//new right branch
		BPlusTreeNode<T>* right = new BPlusTreeNode<T>;
		//put keys in it
		for (int i = left_size + 1; i < max_degree; i++)
			right->keys.push_back(node->keys[i]);
		//put children in it
		//and link children to it
		for (int i = left_size + 1; i < node->children.size(); i++) {
			right->children.push_back(node->children[i]);
			node->children[i]->parent = right;
		}
		//if it has no parent(it's the root)
		if (node->parent == NULL) {
			BPlusTreeNode<T>* new_root = new BPlusTreeNode<T>;
			new_root->keys.push_back(node->keys[left_size]);
			new_root->children.push_back(left);
			new_root->children.push_back(right);
			left->parent = new_root; right->parent = new_root;
			delete node;
			root = new_root;
		}
		//if it has parent
		else {
			int position = -1;
			for (int i = 0; i < node->parent->keys.size(); i++) {
				if (key < node->parent->keys[i]) break;
				position = i;
			}
			//erase and insert in parent's children vector
			node->parent->children.erase(node->parent->children.begin() + position + 1);
			node->parent->children.insert
				(node->parent->children.begin()+position+1,left);
			node->parent->children.insert
				(node->parent->children.begin() + position + 2, right);
			//link the new left and right branch to the parent
			left->parent = node->parent;
			right->parent = node->parent;
			//insert the key into parent's key
			node->parent->keys.insert
				(node->parent->keys.begin()+position+1,node->keys[left_size]);
			delete node;
			//if parent is broken
			if (right->parent->keys.size() == max_degree) split(right->parent,key);
		}
	}
}
//find driver
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& key) {
	if (root->keys.size() == 0) return NULL;
	return find(root, key);
}
//find helper
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(BPlusTreeNode<T>* node, const T& key) {
	if (node->is_leaf()) return node;
	int index=-1;
	for (int i = 0; i < node->keys.size(); i++) {
		if (node->keys[i] > key) break;
		index = i;
	}
	return find(node->children[index+1], key);
}
//print_BFS driver
// BFS double loop
template <class T>
void BPlusTree<T>::print_BFS(ofstream& output) {
	if (root->keys.size() == 0) output << "Tree is empty.\n";
	else {
		vector<BPlusTreeNode<T>*> row;
		vector<BPlusTreeNode<T>*> next_row;
		if (root) row.push_back(root);
		while (row.size() != 0) {
			while (row.size() != 0) {
				BPlusTreeNode<T>* temp = row.front();
				print_BFS(output, temp);
				if(row.size()!=1) output << "\t";
				for (int i = 0; i < temp->children.size(); i++)
					next_row.push_back(temp->children[i]);
				row.erase(row.begin());
			}
			output << "\n";
			row = next_row;
			next_row.clear();
		}
	}
}
//print_BFS helper
//print a single node
template <class T>
void BPlusTree<T>::print_BFS(ofstream& output, BPlusTreeNode<T>* node) {
	for (int i = 0; i < node->keys.size(); i++) {
		output << node->keys[i];
		if (i != node->keys.size() - 1) output << ",";
	}
}
//print_BFS_pretty
template <class T>
void BPlusTree<T>::print_BFS_pretty(ofstream& output) {
	if (root->keys.size() == 0) output << "Tree is empty.\n";
	else {
		vector<BPlusTreeNode<T>*> row;
		vector<BPlusTreeNode<T>*> next_row;
		if (root) row.push_back(root);
		while (row.size() != 0) {
			while (row.size() != 0) {
				BPlusTreeNode<T>* temp = row.front();
				for (int i = 0; i < temp->children.size(); i = i + 2)
					output << "\t";
				print_BFS(output, temp);
				if (row.size() != 1) output << "\t";
				for (int i = 0; i < temp->children.size(); i++)
					next_row.push_back(temp->children[i]);
				row.erase(row.begin());
			}
			output << "\n";
			row = next_row;
			next_row.clear();
		}
	}
}
//print_sideways driver
template <class T>
void BPlusTree<T>::print_sideways(ofstream& output) {
	if (root->keys.size() == 0)	output << "Tree is empty.\n";
	else print_sideways(output, root,0);
}
//print_sideways helper
template <class T>
void BPlusTree<T>::print_sideways(ofstream& output, BPlusTreeNode<T>* node, int count) {
	if (node) {
		for (int i = 0; i < node->children.size()/2; i++)
			print_sideways(output, node->children[i],count+1);
		for (int i = 0; i < count; i++) output << "\t";
		for (int i = 0; i < node->keys.size(); i++) {
			output << node->keys[i];
			if (i != node->keys.size() - 1) output << ",";
		}
		output << "\n";
		for (int i = node->children.size()/2; i < node->children.size(); i++)
			print_sideways(output, node->children[i], count + 1);
	}
}

#endif
