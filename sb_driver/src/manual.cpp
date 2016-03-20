/*
*  driver node to communicate and control arduino
*  sorts incoming sensor information and outputs correct ros messages 
*/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include "sb_msgs/RobotState.h"
#include "sb_msgs/IMU.h"
#include "SerialCommunication.h"
#include "elsa_driver.h"
#include <termios.h>

#include <tf/transform_broadcaster.h>//odom stuff
#include <nav_msgs/Odometry.h>//http://wiki.ros.org/navigation/Tutorials/RobotSetup/Odom reference


using namespace std;
using namespace ros;

//global constants
static const string ROS_NODE_NAME = "driver";
static const int ROS_LOOP_RATE = 20; //hz

static const int BAUD_RATE = 115200;
//static const string PORT_NAME = "/dev/ttyUSB";
static const string UNO_PORT_NAME = "/dev/ttyACM";
static const string BLUETOOTH_PORT_NAME = "/dev/rfcomm";

static const string CAR_COMMAND_TOPIC = "lidar_nav";
static const string TURRET_COMMAND_TOPIC = "turret_command";
static const string ESTOP_TOPIC = "eStop";
static const string ROBOT_STATE_TOPIC = "robot_state";
static const string GPS_STATE_TOPIC = "gps_state";
static const string COMPASS_STATE_TOPIC = "compass_state";

static const string INIT_STRING = "BG";
static const char IDENTIFIER_BYTE = 'B';

static const int SECOND = 1000000;

//global variables
//ServoControl servo;
MechControl mech;
char twist_y[3]={'1','2','5'};
char twist_z[3]={'1','2','5'};

bool eStop = false;

int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering      
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}

int main(int argc, char** argv)
{
    //initialize ros
    init(argc, argv, ROS_NODE_NAME);
	  NodeHandle n;
	  Rate loop_rate(ROS_LOOP_RATE);

	  ros::Time current_time, last_time;
	  current_time = ros::Time::now();//odom stuff
	  last_time = ros::Time::now();
	
	  double x = 0.0;
	  double y = 0.0;
  	double th = 0.0;
  
  	double vx = 0;
  	double vy = 0;
  	double vth = 0;



	//initialize serial communication
	SerialCommunication link;
	for (int i = 0; ; i++)
	{
	    stringstream ss;
	    ss << i;	    
	    if (link.connect(BAUD_RATE,(UNO_PORT_NAME + ss.str())))
	    {
	        cout << "connected on port " << UNO_PORT_NAME << i << endl;
	        break;
		} else if (link.connect(BAUD_RATE,(BLUETOOTH_PORT_NAME + ss.str()))) {
			cout << "connected on bluetooth port " << BLUETOOTH_PORT_NAME << i << endl;
			break;
	    } else if (i > 15) {
	        cout << "unable to find a device" << endl;
	        return 0;
	    }
	}
	
	
	ROS_INFO("arduino_driver ready");
	mech.twist_y=125;
	mech.twist_z=125;

	//while ros is good
	ros::Time begin = ros::Time::now();
	int counter = 0;
	link.clearBuffer();
	while(ok())
	{
	    stringstream ss;
	    int c = getch();
	   	if (c == 'w'){cout << "w" << endl;ss << (char)IDENTIFIER_BYTE << "255128128";} 
	   	else if (c == 'a') {cout << "a" << endl;ss << (char)IDENTIFIER_BYTE << "128128255";}
	  	else if (c == 's'){cout << "s" << endl;ss << (char)IDENTIFIER_BYTE << "000128128";}
		else if (c == 'd') {cout << "d" << endl;ss << (char)IDENTIFIER_BYTE << "128128000";}	
 		else {cout << "invalid" << endl;ss << (char)IDENTIFIER_BYTE << "128128128";}
	cout << ss.str() << endl;
	    link.writeData(ss.str(), 10);
	//ss << (char)IDENTIFIER_BYTE << "128128128";
	//	cout << ss.str() << endl;
	    
}
}




