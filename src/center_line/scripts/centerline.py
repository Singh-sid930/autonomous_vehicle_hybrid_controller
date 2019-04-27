#!/usr/bin/env python

# obstacle position in body(car) frame, goal position in body(car) frame publish at 50Hz

import numpy as np
import math

import rospy
from std_msgs.msg import String
from gazebo_msgs.msg import ModelState
from gazebo_msgs.msg import ModelStates
import geometry_msgs.msg
import tf


egovehicle_statemsg = ModelState()
obstaclevehicle_statemsg = ModelState()
egovehicle = "catvehicle"
obstaclevehicle = "prius_hybrid"

egovehicle_pose_inworld = geometry_msgs.msg.Pose()
obstaclevehicle_pose_inworld = geometry_msgs.msg.Pose()

obstaclevehicle_position_inbody = geometry_msgs.msg.Point()
goal_position_inbody = geometry_msgs.msg.Point()
goal_position_inworld = geometry_msgs.msg.Point()

world_initial_position = geometry_msgs.msg.Pose()


waypoints = np.array([[0,0],[50,0],[100,0],[150,0]], dtype = np.float64) #straight
# waypoints = [[],[],[],[]] #curved

def find_nearest(array, value):
    idx = (np.sum(np.abs(np.subtract(interp_waypoints,ego)),axis=1)).argmin()
    return array[idx,:],idx


def positioncallback(position_msg):
	# print("callback")
	# print(position_msg.pose[3])
	# print("callback")
	global egovehicle_pose_inworld,obstaclevehicle_pose_inworld
	egovehicle_pose_inworld  = position_msg.pose[2]
	obstaclevehicle_pose_inworld = position_msg.pose[3]
	# print(egovehicle_pose_inworld.position, obstaclevehicle_pose_inworld.position)

def calculate_trajectory(waypoints):
	# discretize to about 1m points

	x = np.linspace(0,waypoints.shape[0]-1,waypoints.shape[0])
	y = waypoints[:,0]

	x_new = np.linspace(0,waypoints.shape[0]-1,50*(waypoints.shape[0]-1)+1)

	interp_waypoints =  np.interp(x_new, x, y).reshape(x_new.size,1)
	interp_waypoints = np.concatenate((interp_waypoints,np.zeros(interp_waypoints.shape)), axis=1)
	return interp_waypoints

def transform_to_car_frame(target_position,source_pose):
	transformed_position_msg = geometry_msgs.msg.Point()

	R = tf.transformations.quaternion_matrix(np.array([source_pose.orientation.w,source_pose.orientation.x,source_pose.orientation.y,source_pose.orientation.z]))
	t = source_pose.position
	# print(R,t)
	transformed_position = np.array([[target_position.x,target_position.y,target_position.z]]) - np.array([[t.x,t.y,t.z]])
	transformed_position = np.matmul(R[0:3,0:3],transformed_position.T)
	# print(R,t)
	transformed_position_msg.x,transformed_position_msg.y,transformed_position_msg.z = transformed_position[:,0]

	return transformed_position_msg

def find_nearest(array, value):
    idx = (np.sum(np.abs(np.subtract(array,value)),axis=1)).argmin()
    return array[idx,:],idx

def calculate_goal_position(traj_points, egovehicle_pose_inworld):
	egovehicle_position = np.array([[egovehicle_pose_inworld.position.x, egovehicle_pose_inworld.position.y]])

	closest_pt, closest_pt_index = find_nearest(traj_points,egovehicle_position)

	goal_position_inworld.x,goal_position_inworld.y = traj_points[closest_pt_index+10] #look ahead 10 points~10m

	goal_position_incar = transform_to_car_frame(goal_position_inworld,egovehicle_pose_inworld)

	return goal_position_incar
	

if __name__ == '__main__':
	try:
		traj_points = calculate_trajectory(waypoints)

		rospy.init_node('center_line', anonymous=True)
		obstaclevehicle_position_pub = rospy.Publisher('obstaclevehicle_position', geometry_msgs.msg.Point, queue_size=10)
		goal_position_pub = rospy.Publisher('goal_position', geometry_msgs.msg.Point, queue_size=10)
		
		rospy.Subscriber("/gazebo/model_states", ModelStates, positioncallback)
		print("here we go!!")
		
		rate = rospy.Rate(100) # 10hz

		while not rospy.is_shutdown():
			# print(obstaclevehicle_pose_inworld.position,egovehicle_pose_inworld.position)

			goal_position_inbody = calculate_goal_position(traj_points, egovehicle_pose_inworld)
			
			obstaclevehicle_position_inbody = transform_to_car_frame(obstaclevehicle_pose_inworld.position,egovehicle_pose_inworld)

			# print(goal_position_inbody)
			# print(obstaclevehicle_position_inbody)

			obstaclevehicle_position_pub.publish(obstaclevehicle_position_inbody)
			goal_position_pub.publish(goal_position_inbody)	
			rate.sleep()


	except rospy.ROSInterruptException:
		pass