print("Landing now")
#request land command 
#mavros is ROS1 communication driver for autopilots: https://wiki.ros.org/mavros#mavros.2BAC8-Plugins.sys_status
rospy.wait_for_service('/mavros/cmd/land')
    try:
        land_client = rospy.ServiceProxy('/mavros/cmd/land', CommandTOL)
        response = land_client(altitude = abs(self.state.z), latitude=0, longitude=0, min_pitch=0, yaw=0)
        rospy.loginfo(response)
        
    except rospy.ServiceException as e:
        print("Landing failed: %s" %e) #print error


#modified code from: https://github.com/Oliver-ss/DroneLanding/blob/master/catkin_ws/src/control/src/new_land_kf.py

#Will use ROS1 to ROS2 bridge since this is implemented in ROS1 https://index.ros.org/p/ros1_bridge/