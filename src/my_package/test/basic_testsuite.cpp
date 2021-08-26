#include "ros/ros.h"
#include <ros/console.h>
#include "std_msgs/String.h"

#include <gtest/gtest.h>

#include <sstream>

class MyTestFixture : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  std::string desired_topic;

  MyTestFixture() {
     // You can do set-up work for each test here.
  }

  ~MyTestFixture() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     desired_topic = "/helloWorldSpam";
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(MyTestFixture, topicPresent){
    ros::master::V_TopicInfo master_topics;
    ros::master::getTopics(master_topics);

    //std::string desired_topic = "/helloWorldSpam";
    bool found = false;
    for (ros::master::V_TopicInfo::iterator it = master_topics.begin() ; it != master_topics.end(); it++) {
        ROS_INFO_STREAM(it->name);
        if (desired_topic == it->name){
            found = true;
            break;
        }
    }
    ASSERT_TRUE(found);
}

// TEST(TestSuite, receiveSpam){

// }

// TEST(TestSuite, spamFrequency){

// }

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    ros::init(argc, argv, "tester");
    ros::NodeHandle nh;
    return RUN_ALL_TESTS();
}