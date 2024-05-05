![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/Screenshots/Github%20Image.png?raw=true)

### Features

+ New "EXTENDED SETTINGS" menu
+ All features are available in-game instead of needing you to look at a separate program
+ Will re-attach to MBAACC without requiring you to close the mod
+ ENEMY REVERSAL
	+ Replaces ENEMY ACTION
	+ Can set to RANDOM
	+ Adjustable delay
	+ Fixed 1f delay glitch
	+ Compatible with DUMMY status
+ Ex Guard
	+ Can set to RANDOM
+ Meter and Health Controls
	+ Manage meter all the way down to the individual 0.1%
	+ Manage precice health values and percentages
	+ Old LIFE control changed to only manage whether health recovery is on
+ Guard Bar QoL
	+ Instantly regenerate instead of slowly refilling
	+ New INFINITE option
+ Position Controls
	+ Control both players and assist positions
	+ INVERT positions on screen i.e. switch side of screen
	+ LOCK positions for custom reset positions
+ Burst After X Hits
	+ Experimental
	+ Requires MANUAL status and UNLIMITED meter, but it sets them for you at least
	+ Does not work against Half Moons
+ Character Specific Resources
	+ Roa visible and hidden charges
	+ Sion bullets
	+ Normal, auto-refill, and infinite options

### Where is the fuzzy overhead from the trailer?
The current implementation does not produce behavior 1-to-1 to the vanilla game.  It's very important for Training Mode to behave as similar as possible to Versus mode, so I have chosen to withold this feature until I can make something appropriate for the MBAACC community to practice with.

# How do I get the mod?

- [Download the latest .EXE from this link.](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases)  

- The first time you run the .EXE, your anti-virus might warn you it is dangerous.  You can safely dismiss this message.  If you are curious, you see this because the training mod has code which modifies memory directly, which sets off the anti-virus.

- Run the .EXE while you have Melty Blood open.

And that's it.

# Building the mod yourself

Right now there isn't a CMAKE file, so if you want to build the mod yourself, get the [latest version of Visual Studio](https://visualstudio.microsoft.com/vs/) and be sure to target x86.

# This is a beta

Please understand that I am the only person working on this mod and it was created from scratch.  Bugs are expected, and some are already known about.  Let me know if you have any problems!

# Planned Features
Roughly in order of priority, but not in order they will be implemented.
+ Fuzzy Overhead Practice
+ Fuzzy Overhead Practice
+ Fuzzy Overhead Practice
+ Logging
+ Dummy action out of shield
+ Multiple reversal patterns
+ Option for dummy to repeat a move over and over
+ Air Reversals
+ Configurable Aoko Repeat Magic
+ P1 follows training mode configuration for P2 when F1 is held
+ Character stats on CSS, such as backdash data, wakeup timings, health, and more
+ Control seed for RNG elements
+ Fix training-mode glitch that allows blocking 1f early out of ground tech
+ Permanent HEAT/BLOOD HEAT mode
+ Re-add VIEW SCREEN somewhere else
+ Dummy 1AD, 1A~D

# Known Issues
+ Left and right in EXTENDED SETTINGS feels sticky sometimes
+ Random crashes
	+ Most commonly seen when changing characters' X locations
+ Specific situations can cause the configured reversal to repeat
+ Some of the enemy reversals are the wrong moves.  PLEASE LET ME KNOW IF YOU FIND ONE.
+ ALL GUARD COMBO setting does not behave correctly with Ex Guard
+ Heat reversal on hisui and kohaku
+ Randomlf 1f slow reversals
+ Some situations like wallbounce prevent bursting

# FAQ

### "Looking for MBAA.exe..." when MBAA.exe is already open
Check Task Manager too see if you have a zombie MBAA.exe process running.  You may have to restart your computer to kill this program.

### Some of my settings change on their own
This is most likely because you are using the HITS UNTIL BURST option.  This setting requires MANUAL for the dummy and INFINITE for the meter, and turning HITS UNTIL BURST on automatically changes these.

### How do I get in touch?
My handle is Fang\_\_\_ (that's three underscores).  Message me on Discord.  I'm in most major Discords so you'll be able to find me pretty easily.

### Can I suggest features?
By all means.  Reach out to me on discord, but also make sure to read the Planned Features section first.  Don't think your idea is stupid--I'd love to discuss it, and who knows maybe it's already possible with clever settings.

### Can I contribute?
Sure.  Reach out and we can talk about it.

### Can I make my own version?
Yes you can, but I would prefer not having too many similar training mode mods floating around.  I have tried to make the code as readable and flexible as possible if you do choose this path, however.

# License

tl;dr: You can copy and modify this project as much as you want.

Copyright (c) 2024 Fang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.