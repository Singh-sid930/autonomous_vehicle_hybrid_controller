#pragma once

#include <iostream>
#include <vector>

using namespace std;

struct node
{
	float x;
	float y;
	int parent_node;
	float cost;
};

struct obst
{
	float x_c;
	float y_c;
	float rad;
};

