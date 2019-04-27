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

	 // cout<<"in the dist function \n";

	return sqrt(pow((new_node.y-old_node.y),2)+pow((new_node.x-old_node.x),2));
}





void RRT::rewire(struct node node_new, int count, int cost)
{
	 // cout<<"in the rewire function\n";

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

//*************************************************** Check if the path to the new node from the parent node is collission free *********///

bool RRT::collision_free(struct node p1, struct node p2)
{

	if (fabs(p2.y-p1.y)<0.0001 && fabs(p2.x-p1.x)<0.0001) {
		// ROS_INFO("1 false");
		return false;
	}

	else if (fabs(p2.y-p1.y)<0.0001){
		float x_small = min(p1.x,p2.x);
		float x_large = max(p1.x,p2.x);
		float step = (x_large-x_small)/20.0;
		int count = 0;
		for (float i=x_small; i<=x_large; i+=step){
			if (count>30) return false;
			// ROS_INFO("2 %f %f", p1.y , i);
			int y_cal = p1.y;
			int x_cal = i;
			if (occu_grid[y_cal][x_cal]==1) return false;
			count++;
		}
		return true;
	}

	else if (fabs(p2.x-p1.x)<0.0001){
		float y_small = min(p2.y,p1.y);
		float y_large = max(p2.y,p1.y);
		float step = (y_large-y_small)/20.0;
		int count = 0;
		for (float i=y_small; i<=y_large; i+=step){
			if (count>30) return false;
			// ROS_INFO("3 %f %f", i , p1.x);
			int y_cal = i;
			int x_cal = p1.x;
			if (occu_grid[y_cal][x_cal]==1) return false;
			count++;
		}
		return true;
	}

	else {
		float slope = (p2.y - p1.y)/(p2.x - p1.x);
		float c = p1.y - p1.x * slope;
		float x_small = min(p1.x,p2.x);
		float x_large = max(p1.x,p2.x);
		double step = (x_large-x_small)/20.0;
		int count = 0;
		for (double i=x_small; i<=x_large;i+=step){
			if (count>30) return false;
			int y_cal = slope*i+c;
			int x_cal = i;
			if (occu_grid[y_cal][x_cal]==1) return false;
			count++;
		}
		return true;			
	}
	

}


//**************************************************** Find the nearest node to the new node **********************************///

node RRT:: Nearest_node(struct node node_new, int count)
{
	 // cout<<"in the nearest node function\n";


	int min_dist 	=	10000;
	int check_dist	= 	0;

	for(int i =0; i< count ; i++ )
	{
		// cout<<"going in Nearest_node"<<"\n";

		int cur_dist 	= 	dist(node_list[i],node_new);



		if(cur_dist<min_dist)
		{
			node_new.parent_node 	=	 i;
			float parent_cost 		=   node_list[i].cost;
			node_new.cost 			= 	parent_cost + cur_dist;
			node_list[count] 		= 	node_new;
			check_dist				= 	cur_dist;
			min_dist 				= 	cur_dist;
		}


		
	}

	if(check_dist>MAX_DIST)
	{
		 // cout<<"going distance check"<<"\n";

		float y_s = node_new.y-node_list[node_new.parent_node].y;
		float x_s = node_new.x-node_list[node_new.parent_node].x;

		int node_parent = node_list[count].parent_node;
		float parent_cost = node_list[node_parent].cost;

		float theta 			= 	atan2(y_s,x_s);
		node_list[count].x 		= 	round(node_list[node_new.parent_node].x + MAX_DIST*cos(theta));
		node_list[count].y 		= 	round(node_list[node_new.parent_node].y + MAX_DIST*sin(theta));
		node_list[count].cost 	= 	parent_cost + MAX_DIST;
		// cout<<node_list[count].x;
		// cout<<node_list[count].y;

	}

	return node_list[node_new.parent_node];

}

/// **************************************** Check if the given node is in the free space or not (completed) ****************************************///


bool RRT::In_free_space(struct node a){

	 // cout<<"in the free space function\n";


	if (occu_grid[(a.x)][(a.y)]==1)
	{
		return false;

	}
	else
	{
	
		return true;
		
	}

}

///******************************************** fill the occupancy grid with the obstacles (complete) ***********************************////

void RRT::fill_obstacles(struct obst obstacle){

	float x_c = obstacle.x_c;
	float y_c = obstacle.y_c;
	float rad = obstacle.rad;
	float x_st = x_c - rad;
	float y_st = y_c - rad;

	while(x_st<=x_c+rad){
		while(y_st<=y_c+rad){
			occu_grid[x_st][y_st] = 1;
			y_st++;
		}
		y_st = y_c - rad;
		x_st++;
	}


	

}

//******************************************* Public function or generating the grid and finding the path ***********************///

vector<float> RRT::create_grid(struct node start,struct node goal, struct obst obstacle)

{
	 // cout<<"going in grid function"<<"\n";


	fill_obstacles(obstacle);

	srand (time(NULL));


	start.cost 		= 	0;
	start.parent_node = -1;
	node_list[0]	=	start;
	

	int count 		= 	0;
	

	if(!In_free_space(goal)){
		goal.x = goal.x-obstacle.rad-1;
	}


	// for(int r = 0; r<occu_grid.size();r++){

	// 	for (int c = 0; c<occu_grid[0].size();c++){

	// 		cout<<occu_grid[r][c]<< " ";
	// 	}
	// 	cout<<"\n";
	// }

	if(!In_free_space(start)){

		ROS_INFO("Houston, We have a problem. ");
		return vector<float>{0,0};
	}


	struct node nnew;



	 while (true){

	 	count++;

	 	int x_new 	=	rand() % 19 + 1;
	 	int y_new 	=  	rand() % 19 + 1;


	 	nnew.x 		= 	x_new;
	 	nnew.y 		= 	y_new;

	 	// cout<<"x_new="<<x_new<<"\n";
	 	// cout<<"y_new="<<y_new<<"\n";

	 	if (!In_free_space(nnew))
	 	{
	 		// cout<<"new node not in free space";
	 		count--;
	 		continue;
	 	}


		struct node parent_node 	= 	Nearest_node(nnew, count);


		if (!collision_free(node_list[count],parent_node))
		{
			count--;
		 	continue;
	 	}

	 	// rewire(nnew, count, parent_node.cost);


		if (count>399)
		{
			count = 1;
		}



		if(fabs(node_list[count].x-goal.x)<=1 && fabs(node_list[count].y-goal.y)<=1){
			break;
		}



	 }

	 int i = count;

	 vector<int> x_list;
	 vector<int> y_list;

	 while (i != -1){

	 	// cout<<"x="<<node_list[i].x<<" , ";
	 	// cout<<"y="<<node_list[i].y<<" , ";
	 	// cout<<"cost="<<  node_list[i].cost<<" , ";
	 	// cout<<"parent node="<<node_list[i].parent_node<<"\n";

	 	x_list.push_back(node_list[i].x);
	 	y_list.push_back(node_list[i].y);
	 	
	 	i = node_list[i].parent_node; 	

	 }

	 int len = x_list.size();
	 int mid = round(len/2);

	 vector<float> coordinate;
	 coordinate.push_back(x_list[mid]);
	 coordinate.push_back(y_list[mid]);

	 return coordinate;





	
}

void RRT::animate_grid()
{
	cout<<"animate function"<<"\n";
	
}

