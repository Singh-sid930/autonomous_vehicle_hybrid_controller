
#ifndef DATE_H
#define DATE_H

#include "node.h"
#include <iostream>
#include <vector>
#include "ros/ros.h"

using namespace std;

class RRT 
{
	private:
    	vector<vector<int>> occu_grid = vector<vector<int>> (20, vector<int> (20, 0)); 		
		// vector<vector<int>> occu_grid;
    	vector<node> node_list = vector<node>(400);	
		const int MAX_DIST 	= 	2; 
		const int RANGE 	=	1;

		float dist(struct node old_node, struct node new_node);
		node Nearest_node(struct node node_new, int count);
		bool In_free_space(struct node a);
		bool collision_free(struct node a,struct node b);
		void rewire(struct node node_new, int count, int cost);
		void fill_obstacles(struct obst);


	public:

		vector<float> create_grid(struct node start,struct node goal, struct obst obstacle);
		void animate_grid();


};

#endif