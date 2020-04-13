#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;    
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int left = img.step / 3; 
    int rigth = 2 * img.step / 3;

    // Loop through each pixel in the image and check if there's a bright white one
    for (int i = 0; i < img.height * img.step; i++) {
        if (img.data[i] == white_pixel) {
            // Identify if falls in the left side of the image  
            if (img.data[i] < left) {
            drive_robot (0.0 , 0.5); // Call the drive_bot function and pass velocities to it
            break;
            }
            // Identify if falls in the right side of the image  
            else if (img.data[i] > rigth) {
            drive_robot (0.0 , -0.5); // Call the drive_bot function and pass velocities to it
            break;
            }
            // Else falls in the mid side of the image  
            else if (img.data[i] < left) { // Call the drive_bot function and pass velocities to it
            drive_robot (0.5 , 0.0);
            break;
            }
            break;
        }
    }
    // Request a stop when there's no white ball seen by the camera
    drive_robot (0.0 , 0.0); 

}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
