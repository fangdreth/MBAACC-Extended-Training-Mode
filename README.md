![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/Images/Extended%20Training%20Mode%20v2.1%20Corner.png?raw=true)

![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/NewReadMe/Images/Features.png?raw=true)

+ New "EXTENDED SETTINGS" menu
	+ Reversals (Page 1)
		+ Four reversal slots
		+ Five modes
			+ OFF: no reversals
			+ ON: perform one of the selected reversals always
			+ ON GUARD: only reversal after blocking
			+ ON HIT: only reversal after being hit
			+ ON WAKEUP: only reversal on wakeup
			+ ON SHIELD: only reversal after shielding
		+ Pressing Shield (D) on a reversal switches it to a shield reversal
		+ Probability weights for each reversal plus chance for no reversal
		+ Support for *nearly* every action in the game as a reversal
		+ Fixed 1f delay glitch from vanilla
		+ Reversal delay to simulate fuzzy defense
	+ Training (Page 2)
		+ Penalty Reset
			+ Put Reverse Beat back to zero faster
		+ Guard Bar QoL
			+ Instantly regenerate instead of slowly refilling
			+ Exact guard health and quality written to the screen
		+ Precise Meter and Health Controls
			+ Exact health and red health written to the screen
		+ Hits until burst
			+ Practice burst safe combos and punishes
		+ Hits until bunker
			+ Learn to play around EX Bunker
		+ Hits until force guard stance
			+ Use this to practice f shiki/fuzzy overhead setups
	+ Highlights (Page 3)
		+ Toggle on and off highlights for various game states
			+ Guarding
			+ Hitstun
			+ Armor
			+ Throw Protection
			+ Idle
	+ Position Controls (Page 4)
		+ Control both players and assist positions
		+ LOCK positions for custom reset positions
	+ Character Specific Resources (Page 5)
		+ Roa visible and hidden charges
		+ Sion bullets
		+ F-Maids heart meter
		+ Ryougi knife refill
		+ Normal, auto-refill, and infinite options
	+ Hitboxes (Page 6)
		+ See all the hitboxes in-game
		+ Lots of control over how it is displayed
	+ Save States (Page 7)
		+ Three slots available for save state slots
		+ Supports saving and importing save states as files
	+ Frame Data (Page 8)
		+ Shows a frame bar very similar to Street Fighter 6 on screen
		+ Frame advantage
		+ Move startups
		+ Simply an insane amount of info
		+ Plus an option to look at the launcher window to get *even more* stats that don't fit on screen
	+ RNG (Page 9)
		+ Pick a custom seed or custom RNG value
		+ Can refresh the custom RNG every frame or every training mode reset depending on need
	+ UI (Page 10)
		+ Display a lot of information about health, meter, and other resources on screen
		+ Custom player input display with more accuracy than vanilla
		+ Input display can be clicked and dragged around the screen
	+ System (Page 11)
		+ Customize the game speed
		+ Turn various HUD and background elements on and off


+ New “HOTKEY SETTINGS” menu
	+ Hotkey Settings Menu
		+ Allows you to bind many of the training settings to keyboard keys to make life easier


+ TAS Tools
	+ The game can be advanced forward and *backwards* using hotkeys to examine interactions frame-by-frame


+ Debug Menu
	+ Accessed with Shift + Left Mouse
	+ Click and drag the yellow circle to move
	+ Much of the UI settings can be to toggled from here
	+ You want to dig into the super technical stuff?  This is where you go
	+ Plus some easter eggs


+ Etcetera
	+ Helper text has been added to the new settings to explain their uses
	+ Replaced the stock FPS counter with one that is more accurate
	+ A custom mouse cursor has been added
	+ The mod will auto update

![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/NewReadMe/Images/Discord.png?raw=true)

We have a [Discord](https://discord.gg/6QUpZcThg7) where you can report bugs, get help, request features, or just discuss the mod.  If you do not want to join a discord, you can ping or message Fang___ instead.

![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/NewReadMe/Images/Get.png?raw=true)

- [Download the launcher from this link or go to the Releases page.](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases)  

- The first time you run the .EXE, your anti-virus might warn you it is dangerous.  You can safely dismiss this message.  If you are curious why, it says this because the training mod has code which modifies memory directly, which sets off the anti-virus.

- Run the .EXE while you have Melty Blood open.

- It is not necessary to put the executable in the same directory as MBAA.exe.

![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/NewReadMe/Images/build.png?raw=true)

Right now there isn't a CMAKE file, so if you want to build the mod yourself, get the [latest version of Visual Studio](https://visualstudio.microsoft.com/vs/), [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812), and be sure to target x86.

![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/NewReadMe/Images/faq.png?raw=true)

### Is it compatible with the Steam release?
At the moment, no.  You must use the latest version of the [community edition](https://play.meltyblood.club/).

### Is it compatible with Concerto?
Yes.  Concerto is just an external program for CCCaster, so it will work just fine.

### I found a bug, how do I report it?
You can join our Discord linked above, or send a message to Fang

### "Please launch Melty Blood with CCCaster..." when MBAA.exe is already open
Check Task Manager too see if you have a zombie MBAA.exe process running.  You may have to restart your computer to kill this program.

### "VCRUNTIME140.dll" was not found
This means your VC++ redistributable is missing or needs repaired.  There are step-by-step instructions on Google, so you're best off searching this error to find the best way to fix it.

### MBAA crashes as soon as I open the mod
There is a good chance this is being caused by your DirectX9 installation.  Try to reinstall DirectX9 if you can, and if you still have this problem, please reach out and we can try to help you.

### Can I make my own version?
Yes.

### Is there Linux support?

The official version of the mod does not support Linux at this point in time, but several people have made forks in the past which reportedly worked.  You may be able to fin them by using the discord search feature.

![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/NewReadMe/Images/changelog.png?raw=true)



# License

tl;dr: You can copy and modify this project as much as you want.

Copyright (c) 2024 Fang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.