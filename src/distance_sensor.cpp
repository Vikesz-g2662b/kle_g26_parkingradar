// Szimulalt hatso ultrahangos tavolsagszenzor.
// Az auto tolat egy fal fele: a tavolsag 2.0 m-rol 0.1 m-ig csokken,
// utana ujraindul (uj parkolas). A mert ertekhez kis zajt adunk.

#include <chrono>
#include <functional>
#include <memory>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/range.hpp"

using namespace std::chrono_literals;

class DistanceSensor : public rclcpp::Node
{
public:
  DistanceSensor()
  : Node("distance_sensor"), distance_(2.0), noise_(0.0, 0.01)
  {
    pub_ = this->create_publisher<sensor_msgs::msg::Range>("parking/distance", 10);
    timer_ = this->create_wall_timer(100ms, std::bind(&DistanceSensor::timer_callback, this));
    RCLCPP_INFO(this->get_logger(), "Tavolsagszenzor elindult (10 Hz)");
  }

private:
  void timer_callback()
  {
    // 10 Hz-en 0.02 m-es lepes = 0.2 m/s tolatasi sebesseg
    distance_ -= 0.02;
    if (distance_ < 0.1) {
      distance_ = 2.0;
      RCLCPP_INFO(this->get_logger(), "Uj parkolas indul 2.0 m-rol");
    }

    auto msg = sensor_msgs::msg::Range();
    msg.header.stamp = this->now();
    msg.header.frame_id = "rear_ultrasonic";
    msg.radiation_type = sensor_msgs::msg::Range::ULTRASOUND;
    msg.field_of_view = 0.5;
    msg.min_range = 0.02;
    msg.max_range = 4.0;
    msg.range = std::max(msg.min_range, static_cast<float>(distance_ + noise_(gen_)));
    pub_->publish(msg);
  }

  rclcpp::Publisher<sensor_msgs::msg::Range>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  double distance_;
  std::mt19937 gen_{std::random_device{}()};
  std::normal_distribution<double> noise_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DistanceSensor>());
  rclcpp::shutdown();
  return 0;
}
