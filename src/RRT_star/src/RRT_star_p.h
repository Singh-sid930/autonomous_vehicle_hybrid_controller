
#ifndef DATE_H
#define DATE_H

#include "node.h"
#include <iostream>
#include <vector>

using namespace std;

class RRT 
{
	private:
		vector<node> node_list = vector<node>(10000);
		const int MAX_DIST 	= 	2; 
		const int RANGE 	=	1;

		float dist(struct node old_node, struct node new_node);
		node Nearest_node(struct node node_new, int count);
		bool In_free_space(struct node a);
		bool collision_free(struct node a,struct node b);
		void rewire(struct node node_new, int count, int cost);


	public:

		void create_grid(struct node start,struct node goal);
		void animate_grid();


};

#endif