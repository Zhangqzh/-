#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stack>
#include<algorithm>
#define SIZE 20
using namespace std;

struct node {
	int id;
	int f;
	int g;
	int h;
	node* parent;
	node(int a, int b, int c, node* p) {
		id = a;
		g = b;
		h = c;
		f = g + h;
		parent = p;
	}
	
};
bool cmp(node* a, node* b) {
	return a->f < b->f;
}
class Astar {
private:
	int Graph[SIZE][SIZE];
	int hDistance[SIZE][SIZE];
	string names[SIZE];
	
public:
	Astar() {
	}
	Astar(const char* g, const char* h, const char* n) {
		ifstream gfile("Data/graph.txt", ios::in);
		if (gfile.fail()) cout << "open graph file fail!\n";
		while (!gfile.eof()) {
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					gfile >> Graph[i][j];
				}
			}
		}
		gfile.close();
		ifstream dfile("Data/hDistance.txt", ios::in);
		if (dfile.fail()) cout << "open distance file fail!\n";
		while (!dfile.eof()) {
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					dfile >> hDistance[i][j];
				}
			}
		}
		dfile.close();
		ifstream nfile("Data/names.txt", ios::in);
		if (nfile.fail()) cout << "open names file fail!\n";
		while (!nfile.eof()) {
			for (int i = 0; i < SIZE; i++) {
				nfile >> names[i];
			}
		}
		gfile.close();
	}
	
	int getID(string name) {
		for (int i = 0; i < SIZE; i++) {
			if (names[i] == name)
				return i;
		}
	}

	void getPath(string a, string b) {
		vector<node*> open_list;
		vector<node*> close_list;
		bool isOpen[SIZE] = { false };
		bool isClosed[SIZE] = { false };
		int start = this->getID(a);
		int goal = this->getID(b);
		node *begin=new node(start, 0, hDistance[start][goal], nullptr);
		
		open_list.push_back(begin);
		isOpen[start] = true;
	
		while (!open_list.empty()) {
			sort(open_list.begin(), open_list.end(),cmp);
			node *current = open_list[0];
			if (current->id == goal) break;
			open_list.erase(open_list.begin());
			isOpen[current->id] = false;
			
			close_list.push_back(current);
			isClosed[current->id] = true;
			//sort(open_list.begin(), open_list.end());

			for (int i = 0; i < SIZE; i++) {
				
				if (Graph[current->id][i] && !isClosed[i]) {
					if (isOpen[i]) {
						int lable = 0;
						for (int j = 0; j < open_list.size(); j++) {
							if (open_list[j]->id == i)//这里是因为经过sort之后list里的node不是按它们的id排序的了，所以要找到那个node
								lable = j;
						}
						if (open_list[lable]->g > current->g + Graph[current->id][i]) {
							open_list[lable]->g = current->g + Graph[current->id][i];
							open_list[lable]->f = open_list[lable]->g + open_list[lable]->h;
							open_list[lable]->parent = current;
						}
						
					}
					else {
						node* a = new node(i, Graph[i][current->id] + current->g, hDistance[i][goal], current);
						open_list.push_back(a);
						
						isOpen[i] = true;
						
					}
				}
				
			}
		}
		if (open_list.empty()) cout << "No path\n";
		else {
			string a = "The shortest path is: ";
			node* p = open_list[0]->parent;
			stack<node*> tmp;
			tmp.push(open_list[0]);
			while (p) {
				tmp.push(p);
				p = p->parent;
			}
			while (!tmp.empty()) {
				a += names[tmp.top()->id];
				a += " ,";
				tmp.pop();
			}
			cout << a << "total cost: " << open_list[0]->f << endl;
		}
	}
};