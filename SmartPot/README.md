# Smart Pot

## Description
The Smart Pot began as a simple trash bin purchased from Lidl, chosen for its sleek design and durability. By repurposing this container, I avoided using additional filament for the 3D printer while creating a unique and functional outer shell. Inside the purchased pot, I designed a custom 3D-printed assembly that fits securely and houses all the necessary components, including the electronics, batteries, soil container, pump, and an integrated watering system.

## Principle
The design utilizes about one-third of the container as a water reservoir. The 3D-printed assembly is held in place by its edges, with a pipe extending into the reservoir to allow the pump to draw water. Water is distributed through silicone tubes, which are routed to the edges of the assembly. These tubes have small holes to ensure an even distribution of water across the soil, keeping the plants well-hydrated.

## Power and Control
The Smart Pot’s power system is built around two LiPo batteries providing seven volts. A buck converter steps this down for the ESP32 microcontroller, which handles Wi-Fi communication and integrates seamlessly with Home Assistant. The same batteries directly power the 12-volt pump, which moves water through the tubing system. This efficient setup ensures smooth operation and reliable performance.

## Sensors and Smarts
The Smart Pot includes several monitoring and control features. A homemade capacitive water level sensor, made from laminated foil and an aluminum can, tracks the reservoir’s water levels. A separate capacitive soil moisture sensor monitors soil hydration. For added convenience, the system includes a battery management IC, a USB-C charging module, and a power switch. With Wi-Fi connectivity and Home Assistant integration, I can easily monitor and control every aspect of the Smart Pot remotely.
