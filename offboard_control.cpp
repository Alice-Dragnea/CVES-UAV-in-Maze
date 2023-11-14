// c++ example 
//send a desired point for drone to reach
//note that autopilot is used in offboard mode
//once UAV reaches the point it lands via land_serv (land service)
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "mavros_msgs/State.h"
#include "mavros_msgs/CommandBool.h"
#include "mavros_msgs/SetMode.h"
#include "mavros_msgs/CommandTOL.h"


mavros_msgs::State mav_state;
void mavros_state_cb( mavros_msgs::State mstate) {
    mav_state = mstate;
}

int main(int argc, char** argv ) {
    ros::init(argc, argv, "offboard_control");
    ros::NodeHandle nh;

    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    ros::ServiceClient land_client = nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/land");

    ros::Subscriber mavros_state_sub = nh.subscribe( "/mavros/state", 1, mavros_state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 1);

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";
    if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent){
    ROS_INFO("Manual mode enabled");
    }

    while(!mav_state.armed ) {
        usleep(0.1*1e6);
    ros::spinOnce();
    }
    ROS_INFO("Vehicle armed");

    geometry_msgs::PoseStamped pose;
    pose.pose.position.x = 1;
    pose.pose.position.y = 0;
    pose.pose.position.z = 2;

    ros::Rate r(10);
    float t = 0.0;
    while( ros::ok() && (t < 20.0) ) {
        local_pos_pub.publish(pose);
        t += (1.0/10.0);
        r.sleep();
        ros::spinOnce();
    }

    mavros_msgs::CommandTOL land_srv;
    land_client.call( land_srv );
    return 0;
    }
