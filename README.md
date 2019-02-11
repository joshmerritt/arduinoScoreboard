## Arduino Uno Powered Scoreboard

## Overview

A basic two player scoreboard with a 20 minute per game timer. Users can add or subtract from their score, reset the game, toggle sound, and toggle between 15 and 21 point games. 


## Motivation

I wanted a scoreboard that was easy to read, had a timer, and was nice to look at. Deciding to solder together 50 through-hole addressable RBG LEDs as my first arduino project must have been a moment of insanity and naivety, but it was a lot of fun and it's worked reliably for over 3 years, to boot! If you're looking to build a scoreboard, I encourage you to take a look at mine, but let your imagination/needs run wild and make it your own. Overall, this was a great first electronics project for me that truly enhanced the playing experience for our shuffleboard league


## Components

- 1  - Arduino Uno
- 50 - Addressable LEDs
- 50 - Capacitors
- 7  - Buttons/Switches
- 2  - Buzzers (different)
- 1  - Power Supply
- 1  - Housing (I built mine with wood)

## Tools

- Solder Iron
- Solder
- Wire (3+ different colors)
- Multimeter


## My Journey (aka brief instructions)

*disclaimer* As my first into the arduino world, I am sure there are many things that could be improved upon and countless other ways to do things. This is merely a recount of my approach to completing this project.

I initially did a lot of research into scoreboards for sale ($200+) and anyone DIYer's like me had built. Given the idea I had in my head to use color changing LEDs, I didn't find much inspiration, so resorted to ordering these through-hole LEDs, so I could create exactly the pattern I wanted. This was a BIG design decision and impacted the hours required and potential failure points dramatically, but I didn't fully grasp that at the time. 

Then I started by building an initial prototype housing, which I later replaced with a nicer walnut housing once I'd gotten the scoreboard fully functioning. I also wanted to have my buttons separate from my housing, so I built a 'box' for them and ran a length of wire to the scoreboard housing which contained the arduino, LEDs, and buzzers. I researched how to use the LEDs I'd bought (WS2811) and chose to use the FastLED library. Otherwise I wrote the functions to read the buttons, update the score, and celebrate the winner using the basic tutorials covered in the Arduino IDE. I later added some 'music' and a separate reset button.

Soldering the LEDs together was the most painstaking part, especially because it turned out mine needed to have a capacitor on each one to prevent the display from flickering or being wonky. But even this was simple enough, with 4 pins on each LEDs, 2 for signal (in/out) and 2 for power (+/-). I carefully bent the pins such that each out went to the next in and soldered together, providing additional lengths of wire for when the layout required. I then adjusted the + and - pins to be easily chained to a single + wire and a single - wire. Using my multimeter I checked for any shorts before powering it up using the power suppy. Upon successful completion of this step I hooked up the first in pin to a PWM pin on the arduino and tried out my code. 

There was a lot of trial and error in getting the code to handle all the cases gracefully as this was my first coding projects since I'd graduated college almost a decade prior. 

## Requirements

This is an after the fact accounting of my program using my improved knowledge of engineering best practices and principles. 

- It should have two scores
- It should start a 20 minute timer at the beginning of each game
- The winning score should be able to be set to 15 or 21 before a game begins
- It should reset the scores and timer when reset is pressed
- It should be able to toggle sound on/off at any point
- It should buzz for each point added
- It should beep for each point subtracted
- It should display how many minutes have elapsed
- It should buzz when 1 minute remains
- It should do an extended series of buzzes when the timelimit is reached
- It should flash the winning color across the scoreboard when the winning score is reached
- It should play the mario theme when the winning score is reached
- It should display the final score periodically during the 'celebration mode'

