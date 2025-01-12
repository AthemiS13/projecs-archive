# TimeGrapher

## Description
The TimeGrapher project is a custom-built device designed to analyze the accuracy of automatic watches. As a watch enthusiast building and selling custom timepieces with Seiko movements, I needed a reliable tool to measure deviations in timekeeping. Instead of purchasing a costly commercial TimeGrapher, I opted to create my own, combining 3D printing, custom electronics, and off-the-shelf components to deliver precise results.

## Design
The TimeGrapher consists of two primary 3D-printed bodies. The upper body acts as a holder to secure the watch and strap in place. Watches are positioned on a three-centimeter piezoelectric disc, commonly used in electric acoustic guitars, to detect mechanical vibrations. The lower body houses the electronics, features a nine-volt battery connector or power input, and includes a sensitivity adjustment knob for the piezoelectric preamplifier.

## Principle
At the heart of the TimeGrapher is the TL071 operational amplifier, which boosts the faint vibrations captured by the piezoelectric disc. These amplified signals are then routed to a microphone input or directly to a computer via a compatible sound card. The amplification ensures that the subtle ticks of the watch movement are detectable, even when standard microphones fail to pick them up.

## Processing
Using TG software for analysis, the amplified signals are processed into detailed graphs and metrics, displaying the watch’s accuracy and any deviations in timing. This device allows me to fine-tune automatic movements, ensuring the watches I sell meet high reliability standards. The TimeGrapher project demonstrates the effectiveness of combining custom hardware with software for professional-grade results at a fraction of the cost.
