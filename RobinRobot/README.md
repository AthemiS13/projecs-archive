# Robin

## Description
Robin is a custom-built, AI-driven robot designed to operate autonomously in dynamic environments. It features a robust four-wheel-drive system powered by four independent motors and is equipped with two ultrasonic sensors (one at the front and one at the rear) for obstacle detection and navigation. A handcrafted 12-volt rechargeable LiPo battery powers the entire system, ensuring reliable and long-lasting performance.

## Intelligent Control System
At the core of Robin’s intelligence is an ESP32 microcontroller that connects the robot to OpenAI's GPT model. Upon startup, the ESP32 sends a set of predefined instructions to the GPT model, defining how sensor data is processed and how responses should be formatted. This communication enables Robin to receive commands tailored to its sensor readings and operational needs.

## Sensors & Processing
Robin incorporates an MPU6050 sensor for orientation, allowing it to detect and respond to climbing, descending, or flipping conditions. Combined with the ultrasonic sensors and motor state data, the robot periodically sends detailed sensor logs to OpenAI’s API. The GPT model analyzes this information and provides actionable commands, which Robin processes to perform intelligent movements and actions.

## AI in Robotics
This project highlights the potential of AI in robotics, demonstrating how OpenAI's GPT models can drive decision-making in real time. Robin represents a seamless integration of advanced AI with hardware systems, showcasing an innovative approach to autonomous operation and the possibilities of combining machine learning with embedded technologies.
