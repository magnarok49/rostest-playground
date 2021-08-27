#include <gtest/gtest.h>
#include <ros/console.h>

#include <sstream>

#include "ros/ros.h"
#include "std_msgs/String.h"

static int myGlobalCounter = 0;

class MyTestFixture : public ::testing::Test
{
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  std::string desired_topic;
  std::string desired_message_type;
  ros::NodeHandle n;
  int messagesReceived;

  MyTestFixture()
  {
    // You can do set-up work for each test here.
  }

  ~MyTestFixture() override
  {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override
  {
    // TODO these should be read from a paramserver
    desired_topic = "/helloWorldSpam";
    desired_message_type = "std_msgs/String";
  }

  void TearDown() override
  {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  static void myBlankCallBack(const std_msgs::String::ConstPtr& msg)
  {
    // ROS_ERROR("Running callback!");
  }

  static void myCallBack(const std_msgs::String::ConstPtr& msg)
  {
    // ROS_INFO("%s", msg->data);
    // ROS_ERROR("Running callback!");
    myGlobalCounter += 1;
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// TEST_F(MyTestFixture, topicPresent){
//     ros::master::V_TopicInfo master_topics;
//     ros::master::getTopics(master_topics);

//     //std::string desired_topic = "/helloWorldSpam";
//     bool found = false;
//     for (ros::master::V_TopicInfo::iterator it = master_topics.begin() ; it != master_topics.end(); it++) {
//         ROS_INFO_STREAM(it->name);
//         if (desired_topic == it->name){
//             found = true;
//             break;
//         }
//     }
//     ASSERT_TRUE(found);
// }

TEST_F(MyTestFixture, topicPresentAndCorrectType)
{
  ros::master::V_TopicInfo master_topics;
  ros::master::getTopics(master_topics);

  bool found = false;
  for (ros::master::V_TopicInfo::iterator it = master_topics.begin(); it != master_topics.end(); it++)
  {
    ROS_INFO_STREAM(it->datatype);
    if (desired_topic == it->name && desired_message_type == it->datatype)
    {
      found = true;
      break;
    }
  }
  ASSERT_TRUE(found);
}

TEST_F(MyTestFixture, ableToSubscribe)
{
  ros::Subscriber sub = n.subscribe(desired_topic, 100, myBlankCallBack);
  ros::Duration(0.2).sleep();  // give it some time to connect to any publishers
  ros::spinOnce();
  ASSERT_EQ(sub.getNumPublishers(), 1);
}

TEST_F(MyTestFixture, receiveSpamAtCorrectFrequency)
{
  ros::Subscriber sub = n.subscribe(desired_topic, 100, myCallBack);
  ros::Duration(0.5).sleep();  // give it some time to connect to any publishers
  ros::spinOnce();
  myGlobalCounter = 0;
  ros::Duration(0.5).sleep();
  ros::spinOnce();
  sub.shutdown();
  // should get around 5 messages in this time, allow 4 to 6
  ROS_ERROR("Received %d messages in 0.5 seconds", myGlobalCounter);
  ASSERT_LE(myGlobalCounter, 6);
  ASSERT_GE(myGlobalCounter, 4);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  ros::init(argc, argv, "tester");
  ros::NodeHandle nh;
  return RUN_ALL_TESTS();
}