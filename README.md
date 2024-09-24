![alt text](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/Images/Extended%20Training%20Mode%20v2.0%20Corner.png?raw=true)

### Features

+ New "EXTENDED SETTINGS" menu
+ Reversals
	+ Four slots with five modes
		+ OFF: no reversals
		+ NORMAL: perform one of the selected reversals on block or hit
		+ RANDOM: 50% chance to perform one of the selected reversals on block or hit
		+ REPEAT: perform the selected reversals repeatedly
		+ SHIELD: perform one of the selected reversals after shielding
	+ Has support for nearly every action in the game
	+ Fixed 1f delay glitch
	+ Configurable delay can be added to the reversal to simulate fuzzy defense
+ EX Guard
+ Precise Meter and Health Controls
	+ Exact health and red health written to the screen
+ Guard Bar QoL
	+ Instantly regenerate instead of slowly refilling
	+ New INFINITE option
	+ Exact guard health and quality written to the screen
+ Position Controls
	+ Control both players and assist positions
	+ LOCK positions for custom reset positions
+ Burst After X Hits
+ Character Specific Resources
	+ Roa visible and hidden charges
	+ Sion bullets
	+ F-Maids heart meter
	+ Ryougi knife refill
	+ Normal, auto-refill, and infinite options
	+ Take Control of Assist Characters
+ Frame Data Display
	+ Displayed in the mod window or on the screen with the Frame Data hotkey
	+ Startup, Total, and Advantage clearly displayed
	+ The display in the mod window can be configured to display significantly more detail
+ Save States
	+ Three separate save slots.
	+ Import and Export of save states supported
+ RNG
	+ Pick a custom seed or custom RNG value
	+ Can refresh the custom RNG every frame or every training mode reset depending on need
+ Character Highlights
	+ Highlight character Idle, Block, Hit, Armor, and Throw Protection
+ Character Hitboxes
	+ Different rendering styles for preference
	+ Color blind-friendly colors or the classic colors both supported
	+ Options to extend the origin for each character
	+ Option to draw the ground level
+ Hotkey Controls
+ Etcetera
	+ Helper text has been added to the new settings to explain their uses
	+ Replaced the stock FPS counter with one that is more accurate
	+ The background can be replaced with solid colors
	+ Shadows can be disabled
	+ The HUD can be disabled

# Discord
We have a Discord [https://discord.gg/6QUpZcThg7] where you can report bugs, get help, request features, or just discuss the mod.  If you do not want to join a discord, you can ping or message Fang___ instead.

# How do I get the mod?

- [Download the latest .EXE and .DLL from this link.](https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases)  

- The first time you run the .EXE, your anti-virus might warn you it is dangerous.  You can safely dismiss this message.  If you are curious, you see this because the training mod has code which modifies memory directly, which sets off the anti-virus.

- Run the .EXE while you have Melty Blood open.

- It is not necessary to put the executable in the same directory as MBAA.exe.

# Building the mod yourself

Right now there isn't a CMAKE file, so if you want to build the mod yourself, get the [latest version of Visual Studio](https://visualstudio.microsoft.com/vs/), [DirectX SDK](https://www.microsoft.com/en-us/download/details.aspx?id=6812), and be sure to target x86.

# v2.0 Changelog
+ Uses DLL injection now in addition to the old external EXE

+ Frame data improved
  + Exact health, red health, guard bar, and guard quality values on-screen
  + Color Guide has been created to reference the meaning of the labels
  + Frame data is now written to the screen a la SF6

+ Reworked Save State controls
  + Press A on a save slot to delete it
  + Import and Export Save States
  + Save States support RNG
  + Hotkey controls
    + Save by pressing the hotkey, and delete by holding it
    + Change active Save Slot

+ Frame Step
  + Freeze the game and advance frame-by-frame

+ Hitboxes
  + Different styles of drawing and a color blind mode
  + Option to extend the characters' origin marker fullscreen
  + Option to highlight the ground

+ Color Overlay
  + Idle, Blocking, Hit, Armor, and Throw Protection
  + Red, Yellow, Green, Blue, Purple, and Black
  + Toggle on and off with a hotkey

+ More features to Enemy Reversals
  + Can be turned off without setting everything to NONE
  + Reversal out of shield
  + Reversal with a hotkey

+ Custom RNG Controls
  + Pick a custom Seed or a specific RNG value
  + Can reset every frame or just set it once when resetting training mode

+ Customization
  + Configurable hotkeys for many of the training mode functions
  + Significantly more accurate FPS
  + Disable the HUD
  + Disable the background and replace it with a solid color.  Useful for chroma keying
  + Disable shadows

+ More character specific controls
  + FMaids Hearts
  + Ryougi Knife

+ Etcetera
  + Helper text has been added to everything in the Extended Settings menu
  + Fixed a few incorrect moves in Reversals
  + Improved inputs being dropped in the menu performance
  + Moved several menus around
  + Frame advantage is not shown with the FPS counter anymore
  + Removed random tripping 

# FAQ

### Is it compatible with the Steam release?
At the moment, no.  You must use the latest version of the [https://play.meltyblood.club/](community edition).

### Is it compatible with Concerto?
Yes.  Concerto is just an external program for CCCaster, so it will work just fine.

### "Looking for MBAA.exe..." when MBAA.exe is already open
Check Task Manager too see if you have a zombie MBAA.exe process running.  You may have to restart your computer to kill this program.

### "VCRUNTIME140.dll" was not found
This means your VC++ redistributable is missing or needs repaired.  There are step-by-step instructions on Google, so you're best off searching this error to find the best way to fix it.

### Some of my settings change on their own
This is most likely because you are using the HITS UNTIL BURST option.  This setting requires MANUAL for the dummy and INFINITE for the meter, and turning HITS UNTIL BURST on automatically changes these.

### The hitboxes are being glitchy
More than likely this is because you opened the F4 menu at some point.  This is a known issue and requires you to re-open Melty to fix.

### Can I make my own version?
Yes.

# License

tl;dr: You can copy and modify this project as much as you want.

Copyright (c) 2024 Fang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.