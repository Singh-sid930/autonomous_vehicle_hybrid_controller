#include "RRT_star_p.h"
#include "node.h"
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include "ros/ros.h"
#include <geometry_msgs/Point.h>

using namespace std;

float obst_x = 0;
float obst_y = 0;
float obst_z = 0;


float goal_x = 0;
float goal_y = 0;
float goal_w = 0;


void Callback1(geometry_msgs::Point msg)
{
   goal_x = msg.x;
   goal_y = msg.y;
   goal_w = msg.z;
  
}

void Callback(geometry_msgs::Point msg)
{

   obst_x = msg.x;
   obst_y = msg.y;
   obst_z = msg.z;


}

// void transform_position(x_goal,y_goal,w_goal){




// }

int main(int argc, char **argv)
{

   ros::init(argc, argv, "rrt_goal_node");

  ros::NodeHandle n;
  // ros::NodeHandle ns1;
  // ros::NodeHandle np;

  //Create a subscriber object and subscribe to any topic to be subscribed and send the data subscribed to the Callback function
  ros::Subscriber sub = n.subscribe("/obstaclevehicle_position", 10, Callback);
  ros::Subscriber sub1 = n.subscribe("/goal_position", 10, Callback1);

  //Create a publisher object and publish to any publlished topic 
  ros::Publisher pub = n.advertise<geometry_msgs::Point>("get_goal", 1000);

  // Publish at 5Hz
  ros::Rate loop_rate(5);


	RRT rrt;

	struct node node_s;
	struct node node_g;
	struct obst obstacle;

	// obstacle.x_c = 10; 
	// obstacle.y_c = 10; 
	// obstacle.rad = 2;
	vector<float> coordinates;

	node_s.x = 10;

	node_s.y = 0;


	float mpc_goal_x;
	float mpc_goal_y;

	 while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it. In my case I have used a geometry message type.
     */


  	node_g.x = goal_y + 10;
  	node_g.y = goal_x ;


  	// ROS_INFO("obstacle position is = %f", x_obst);
   //  ROS_INFO("obstacle position is = %f", y_obst);
  

  	obstacle.x_c = obst_y + 10; 
	obstacle.y_c = obst_x; 
	obstacle.rad = 2;


  	coordinates = rrt.create_grid(node_s,node_g, obstacle);

  	// break;

	mpc_goal_x = coordinates[0];
	mpc_goal_y = coordinates[1];


    geometry_msgs::Point msg;

    msg.x = mpc_goal_x; 
    msg.y = mpc_goal_y;
    msg.z = 0.0;

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */

    // break;


    pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }


	// std::cout<<"connections made"<<"\n";
	// RRT rrt;
	// rrt.create_grid();
	// rrt.animate_grid();
	return 0;
}