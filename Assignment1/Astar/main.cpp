#include"Astar.hpp"
using namespace std;
int main() {
	string ori, des;
	Astar test("Data/graph.txt", "Data/hDistance.txt", "Data/names.txt");
	cout << "Please enter the origin and the destination:\n";
	cin >> ori >> des;
	test.getPath(ori, des);
	
	return 0;
}