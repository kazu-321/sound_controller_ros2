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

#include "sound_controller_ros2/button_sound.hpp"

namespace button_sound {

button_sound::button_sound (const rclcpp::NodeOptions &node_options) : Node ("button_sound", node_options) {
    publisher_  = this->create_publisher<sound_play::msg::SoundRequest> ("robotsound", 10);
    subscriber_ = this->create_subscription<sensor_msgs::msg::Joy> ("joy", 10, std::bind (&button_sound::joy_callback, this, std::placeholders::_1));

    num_button_ = static_cast<size_t> (this->declare_parameter<int> ("num_button", 0));
    for (size_t i = 0; i < num_button_; i++) {
        std::string package   = this->declare_parameter<std::string> ("button_" + std::to_string (i) + ".sound_package", "");
        std::string directory = this->declare_parameter<std::string> ("button_" + std::to_string (i) + ".sound_directory", "");
        std::string file      = this->declare_parameter<std::string> ("button_" + std::to_string (i) + ".sound_file", "");
        double      volume    = this->declare_parameter<double> ("button_" + std::to_string (i) + ".volume", 1.0);
        sound_packages_.push_back (package);
        sound_directories_.push_back (directory);
        sound_files_.push_back (file);
        volumes_.push_back (volume);
    }
    last_joy_msg_ = nullptr;
}

void button_sound::joy_callback (const sensor_msgs::msg::Joy::SharedPtr msg) {
    if (num_button_ > msg->buttons.size ()) {
        RCLCPP_WARN (this->get_logger (), "Button number %zu is out of range.", num_button_);
        return;
    }
    if (last_joy_msg_ == nullptr) {
        last_joy_msg_ = msg;
        return;
    }
    size_t button_index = 0;
    for (size_t i = 0; i < num_button_; ++i) {
        if (msg->buttons[i] == 0) {
            continue;
        }
        if (last_joy_msg_->buttons[i] != 0) {
            continue;
        }
        RCLCPP_INFO (this->get_logger (), "Button %zu is pressed.", i);
        if (i >= sound_files_.size ()) {
            RCLCPP_WARN (this->get_logger (), "Sound for button %zu is not configured.", i);
            continue;
        }
        auto sound_request    = sound_play::msg::SoundRequest ();
        sound_request.sound   = sound_play::msg::SoundRequest::PLAY_FILE;
        sound_request.command = sound_play::msg::SoundRequest::PLAY_ONCE;
        sound_request.arg     = sound_directories_[i] + "/" + sound_files_[i];
        sound_request.arg2    = sound_packages_[i];
        sound_request.volume  = volumes_[i];
        publisher_->publish (sound_request);
    }
    last_joy_msg_ = msg;
}

}  // namespace button_sound

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE (button_sound::button_sound)
