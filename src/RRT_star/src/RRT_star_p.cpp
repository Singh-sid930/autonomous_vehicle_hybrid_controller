#include "RRT_star_p.h"
#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <math.h>

using namespace std;


float RRT::dist(struct node old_node, struct node new_node) 
{

	return sqrt(pow((new_node.y-old_node.y),2)+pow((new_node.x-old_node.x),2));
}

node RRT:: Nearest_node(struct node node_new, int count)
{


	int old_dist 	=	INT32_MAX;
	int check_dist	= 	0;

	for(int i =0; i< count ; i++ )
	{
		// cout<<"going in Nearest_node"<<"\n";

		int cur_dist 	= 	dist(node_list[i],node_new);



		if(cur_dist<old_dist)
		{
			node_new.parent_node 	=	 i;
			node_new.cost 			= 	cur_dist;
			node_list[count] 		= 	node_new;
			check_dist				= 	cur_dist;
		}


		old_dist 	= 	cur_dist;
	}

	if(check_dist>MAX_DIST)
	{
		// cout<<"going distance check"<<"\n";

		float y_s = node_new.y-node_list[node_new.parent_node].y;
		float x_s = node_new.x-node_list[node_new.parent_node].x;

		float theta 			= 	atan2(y_s,x_s);
		node_list[count].x 		= 	node_list[node_new.parent_node].x + MAX_DIST*cos(theta);
		node_list[count].y 		= 	node_list[node_new.parent_node].y + MAX_DIST*sin(theta);
		node_list[count].cost 	= 	MAX_DIST;

	}

	return node_list[node_new.parent_node];

}

bool RRT::In_free_space(struct node a )
{


	if (a.x>45 && a.x<40 && a.y<40 && a.y>45)
	{
		return false;
	}
	else
	{
		return true; 
	}

}

bool RRT::collision_free(struct node a, struct node b)
{

	int len 		= 	(a.x-b.x>a.y-b.y)?(a.x-b.x):(a.y-b.y);
	float slope 	= 	(a.y-b.y)/(a.x-b.x);

	for (int i = 0 ; i<len ; i++)
	{
		int x 		= 	a.x+i;
		int y 		= 	slope *x + a.y;

		if (x>45 && x<40 && y<40 && y>45)
		{
			return false;
		}
		else
		{
			return true; 
		}
	}

}

void RRT::rewire(struct node node_new, int count, int cost)
{

	for(int i =0; i< count ; i++ )
	{
		int cur_dist 	= 	dist(node_list[i],node_new);

		if(cur_dist<RANGE)
		{
			if (node_list[i].cost<cost)
			{
				node_new.parent_node 	= 	i;
			}
		}
	}

}

void RRT::create_grid(struct node start,struct node goal)

{
	// cout<<"going in grid function"<<"\n";


	srand (time(NULL));


	start.cost 		= 	0;
	node_list[0]	=	start;
	

	int count 		= 	0;


	struct node nnew;


	 while (true)
	 {
	 	count++;

	 	int x_new 	=	rand() % 100 + 1;
	 	int y_new 	=  	rand() % 100 + 1;


	 	nnew.x 		= 	x_new;
	 	nnew.y 		= 	y_new;


	 	if (!In_free_space(nnew))
	 	{
	 		continue;
	 	}


		struct node parent_node 	= 	Nearest_node(nnew, count);


		 // if (collision_free(node_list[count],parent_node))
		 // {
		 // 	count--;
		 // 	continue;
	 	//  }

	 	rewire(nnew, count, parent_node.cost);

	 	cout<<"x="<<node_list[count].x<<" , ";
	 	cout<<"y="<<node_list[count].y<<" , ";
	 	cout<<"cost="<<node_list[count].cost<<" , ";
	 	cout<<"parent node="<<node_list[count].parent_node<<"\n";

	 	if (fabs(node_list[count].x-goal.x)<1 && fabs(node_list[count].y-goal.y)<1)
		{
		break;
		}

		if (count>9999)
		{
			count = 1;
		}



	 }
	
}

void RRT::animate_grid()
{
	cout<<"animate function"<<"\n";
	
}

