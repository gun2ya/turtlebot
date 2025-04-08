#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class GoScan : public rclcpp::Node{
    public:
        GoScan() : Node("go_scan"){
            pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 1);
            timer_ = this->create_wall_timer(100ms, std::bind(&GoScan::timer_cb, this));
            sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
		"scan", rclcpp::QoS(1).best_effort(),
		std::bind(&GoScan::scan_cb, this, _1));
        }
    private:
        void timer_cb() {
            auto cmd = geometry_msgs::msg::Twist();
            obstacle = false;
            obstacle_left = false;
            obstacle_right = false;
            
            for (int i = 0; i < 27; i++) {
                printf("obstacle: %d, left: %f, right: %f\n", i+2, range_left_array[i+2], range_right_array[i+2]);
                if (range_left_array[i+2] < 0.4 && range_left_array[i+2] > 0.001)  {
                    obstacle = true;
                    obstacle_left = true;
                    break;
                }
                if (range_right_array[i+2] < 0.4 && range_right_array[i+2] > 0.001) {
                    obstacle = true;
                    obstacle_right = true;
                    break;
                }
            }
            printf("obstacle: %d, obstacle_left: %d, obstacle_right: %d\n", obstacle, obstacle_left, obstacle_right);

            if (!obstacle) {
                rotation_count = 0;  // Reset counter when no obstacle
                cmd.linear.x = 0.15;
                cmd.angular.z = 0.0;
            }
            else if (obstacle && obstacle_left) {
                cmd.linear.x = 0.0;
                if (rotation_count < 3) {
                    cmd.angular.z = 0.2;
                    rotation_count++;
                } else {
                    cmd.angular.z = 0.0;
                }
            } 
            else if (obstacle && obstacle_right) {
                cmd.linear.x = 0.0;
                if (rotation_count < 3) {
                    cmd.angular.z = -0.2;
                    rotation_count++;
                } else {
                    cmd.angular.z = 0.0;
                }
            }   

            pub_->publish(cmd);
        }

        void scan_cb(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
            for(int i = 0; i < 30; i++) {
                range_right_array[i] = msg->ranges[i];
                range_left_array[i] = msg->ranges[360-i-1];
            }
            range_ahead_ = msg->ranges[0];
            range_left_ = msg->ranges[10];
            range_right_ = msg->ranges[350];
            
            //printf("range ahead: %f, range left: %f, range right: %f\n", range_ahead_, range_left_, range_right_);
        }

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_;
        float range_ahead_;
        float range_left_;
        float range_right_;
        bool obstacle_left = false;
        bool obstacle_right = false;
        bool obstacle = false;
	    float range_right_array[30];
	    float range_left_array[30];
        float range_ahead_array[360];
        int rotation_count = 0;

};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GoScan>());
    rclcpp::shutdown();
    return 0;
}
