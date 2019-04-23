#include "RRT_star_p.h"
#include "node.h"
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;

int main()
{

	 RRT rrt;

	 struct node node_s;
	 struct node node_g;

	cout<<"enter x coordinate of goal";
	int x_goal;
	cin>>x_goal;
	node_g.x = x_goal;


	cout<<"enter y coordinate of goal";
	int y_goal;
	cin>>y_goal;
	node_g.y = y_goal;

	cout<<"enter x coordinate of start";
	int x_start;
	cin>>x_start;
	node_s.x = x_start;

	cout<<"enter y coordinate of start";
	int y_start;
	cin>>y_start;
	node_s.y = x_start;

	cout<<"getting the function"<<"\n";

	rrt.create_grid(node_s,node_g);

	cout<<"completed"<<"\n";


	// std::cout<<"connections made"<<"\n";
	// RRT rrt;
	// rrt.create_grid();
	// rrt.animate_grid();
	return 0;
}