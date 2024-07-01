![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/Images/Github%20Image.png?raw=true)

### Features

+ New "EXTENDED SETTINGS" menu
+ All features are available in-game instead of needing you to look at a separate program
+ Navigate through the menu with FN1 and FN2
+ Will re-attach to MBAACC without requiring you to close the mod
+ ENEMY REVERSAL
	+ Replaces ENEMY ACTION
	+ Four slots
	+ Can set to RANDOM or REPEAT
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
+ Character Specific Resources
	+ Roa visible and hidden charges
	+ Sion bullets
	+ Normal, auto-refill, and infinite options
+ Frame Data Display
	+ Very in-depth information about the game state
	+ Gameplay timeline
	+ Frame advantage
	+ And a lot of other nerdy bits
+ Save States
	+ When enabled, F1 and F2 are changed to save and load save states

# How do I get the mod?

- [Download the latest .EXE from this link.](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases)  

- The first time you run the .EXE, your anti-virus might warn you it is dangerous.  You can safely dismiss this message.  If you are curious, you see this because the training mod has code which modifies memory directly, which sets off the anti-virus.

- Run the .EXE while you have Melty Blood open.

- It is not necessary to put the executable in the same directory as MBAA.exe.

# Building the mod yourself

Right now there isn't a CMAKE file, so if you want to build the mod yourself, get the [latest version of Visual Studio](https://visualstudio.microsoft.com/vs/) and be sure to target x86.

# Save State Controls

First, save states must be enabled from the EXTENDED SETTINGS menu.  After that, a save state can be created anywhere (even while paused) by pressing F1 on your controller.  From that point, pressing F2 will restore that save state.  You can clear your save states by either holding up and pressing F1, or going into the EXTENDED SETTINGS menu and selecting CLEAR SAVE STATES.

Three save slots available.

# FAQ

### "Looking for MBAA.exe..." when MBAA.exe is already open
Check Task Manager too see if you have a zombie MBAA.exe process running.  You may have to restart your computer to kill this program.

### "VCRUNTIME140.dll" was not found
This means your VC++ redistributable is missing or needs repaired.  There are step-by-step instructions on Google, so you're best off searching this error to find the best way to fix it.

### Some of my settings change on their own
This is most likely because you are using the HITS UNTIL BURST option.  This setting requires MANUAL for the dummy and INFINITE for the meter, and turning HITS UNTIL BURST on automatically changes these.

### How do I get in touch?
My handle is Fang\_\_\_ (that's three underscores).  Message me on Discord.  I'm in most major Discords so you'll be able to find me pretty easily.

### Can I suggest features?
By all means.  Reach out to me on discord.  Don't be afraid your idea is stupid--I'd love to discuss it, and who knows maybe it's already possible with clever settings.

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