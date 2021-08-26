#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char** argv){
    ros::init(argc, argv, "talker");
    ros::NodeHandle myNode;
    ros::Publisher chatter_pub = myNode.advertise<std_msgs::String>("helloWorldSpam", 100);
    ros::Rate loop_rate(10);

    int count = 0;

    while(ros::ok()){
        std_msgs::String myMsg;
        std::stringstream ss;
        ss << "hello world " << count;
        myMsg.data = ss.str();

        ROS_INFO("%s", myMsg.data.c_str());

        chatter_pub.publish(myMsg);ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }

    return 0;
}