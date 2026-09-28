// Tolatoradar: a tavolsag alapjan figyelmeztetest es sipolasi frekvenciat hirdet.
//   > 1.5 m      -> BIZTONSAGOS (nincs sipolas)
//   0.8 - 1.5 m  -> FIGYELEM    (2 Hz)
//   0.3 - 0.8 m  -> VESZELY     (5 Hz)
//   < 0.3 m      -> STOP!       (20 Hz, folyamatos hang)

#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/range.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class ParkingRadar : public rclcpp::Node
{
public:
  ParkingRadar()
  : Node("parking_radar")
  {
    sub_ = this->create_subscription<sensor_msgs::msg::Range>(
      "parking/distance", 10, std::bind(&ParkingRadar::range_callback, this, _1));
    warning_pub_ = this->create_publisher<std_msgs::msg::String>("parking/warning", 10);
    beep_pub_ = this->create_publisher<std_msgs::msg::Float32>("parking/beep_rate", 10);
    RCLCPP_INFO(this->get_logger(), "Tolatoradar elindult");
  }

private:
  void range_callback(const sensor_msgs::msg::Range & msg)
  {
    auto warning = std_msgs::msg::String();
    auto beep = std_msgs::msg::Float32();

    if (msg.range > 1.5) {
      warning.data = "BIZTONSAGOS";
      beep.data = 0.0;
    } else if (msg.range > 0.8) {
      warning.data = "FIGYELEM";
      beep.data = 2.0;
    } else if (msg.range > 0.3) {
      warning.data = "VESZELY";
      beep.data = 5.0;
    } else {
      warning.data = "STOP!";
      beep.data = 20.0;
    }

    warning_pub_->publish(warning);
    beep_pub_->publish(beep);

    // Csak szintvaltaskor irunk a terminalba, hogy ne legyen tul sok sor
    if (warning.data != last_warning_) {
      RCLCPP_INFO(
        this->get_logger(), "Tavolsag: %.2f m -> %s (sipolas: %.0f Hz)",
        msg.range, warning.data.c_str(), beep.data);
      last_warning_ = warning.data;
    }
  }

  rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr sub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr warning_pub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr beep_pub_;
  std::string last_warning_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParkingRadar>());
  rclcpp::shutdown();
  return 0;
}
