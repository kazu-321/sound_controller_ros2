// Copyright 2025 Kazusa Hashimoto
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __BUTTON_SOUND_HPP__
#define __BUTTON_SOUND_HPP__

#include "rclcpp/rclcpp.hpp"
#include "sound_play/msg/sound_request.hpp"

#include "sensor_msgs/msg/joy.hpp"

namespace button_sound {
class button_sound : public rclcpp::Node {
   public:
    button_sound (const rclcpp::NodeOptions &node_options);

   private:
    size_t                   num_button_;
    std::vector<std::string> sound_packages_;
    std::vector<std::string> sound_directories_;
    std::vector<std::string> sound_files_;
    std::vector<double>      volumes_;

    sensor_msgs::msg::Joy::SharedPtr last_joy_msg_;

    void joy_callback (const sensor_msgs::msg::Joy::SharedPtr msg);

    rclcpp::Publisher<sound_play::msg::SoundRequest>::SharedPtr publisher_;
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr      subscriber_;
};
}  // namespace button_sound
#endif  // __BUTTON_SOUND_HPP__
