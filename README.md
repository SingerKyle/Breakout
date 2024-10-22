# Breakout

W Kavanagh. June Summer 2024

## controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

## Suggested fixes

* Fix the compiler issues in the code

## Suggested tasks

* Implement mouse input for pad X 
* Improved VFX (ball trail, paddle hit response, brick particle destruction) 
* Better UI (progress bar rather than timer for countdown).
* GameLoop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball [where you shoot the ball from the paddle every time], tiny ball, big ball, brick-tracking ball)
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as LLama

# Time Details and Changelist
<Add information to this section about the time you've taken for this task along with a professional changelist.>

Fixed Compiler errors - 15 mins.

Switched pointers in game manager to unique smart pointers - 5 mins

Added random ball spawning (then limited to lower half of the screen) - 15 mins.

Added Mouse Control to Paddle - 20 mins.

added ball acceleration - 20 mins.

Started Particle Class - 20 mins.

Added Camera Shake - 25 mins.

Added Particle System to ball movement - 1 hour.

