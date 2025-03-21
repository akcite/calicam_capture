
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////////////////

int raw_image_count = 0;

int main(int argc, char **argv) {
  ros::init(argc, argv, "calicam_capture");
  ros::NodeHandle nh("~");

  ROS_INFO("CaliCam image node initialized");

  int usb_port = -1;
  nh.getParam("usb_port", usb_port);

  int image_width = -1;
  int image_height = -1;
  nh.getParam("image_width", image_width);
  nh.getParam("image_height", image_height);

  ROS_INFO("CaliCam USB Port: %d", usb_port);
  ROS_INFO("CaliCam USB image_width: %d", image_width);
  ROS_INFO("CaliCam image_height Port: %d", image_height);

  std::string raw_image_topic = "/calicam/raw_image";
  ros::Publisher img_pub = nh.advertise<sensor_msgs::Image>(raw_image_topic, 1);
  ROS_INFO("CaliCam raw image topic: %s", raw_image_topic.c_str());

  cv::VideoCapture vcapture;
  vcapture.open(usb_port);

  if (!vcapture.isOpened()) {
    std::cout << "Camera doesn't work" << std::endl;
    exit(-1);
  }

  vcapture.set(CV_CAP_PROP_FRAME_WIDTH,  image_width);
  vcapture.set(CV_CAP_PROP_FRAME_HEIGHT, image_height);
  vcapture.set(CV_CAP_PROP_FPS, 30);

  while (ros::ok()) {
    cv::Mat raw_img;
    vcapture >> raw_img;

    sensor_msgs::Image image_msg;

    cv_bridge::CvImage(std_msgs::Header(), "bgr8", raw_img).toImageMsg(image_msg);
    image_msg.header.stamp = ros::Time::now();
    image_msg.header.frame_id = "CaliCam";
    img_pub.publish(image_msg);

    ros::spinOnce();
  }

  ros::spin();

  return 0;
}

////////////////////////////////////////////////////////////////////////////////