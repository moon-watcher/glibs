

                                 Mod2PSG2


Mod2PSG2 is a music tracker for the SN76489 sound chip running on MS Windows.
The SN76489 is used in various video game consoles, for example in the SEGA
Master System and Game Gear.

Mod2PSG2 uses its own PSGMOD file format for loading and saving music modules.
Various other file formats are supported for importing and exporting.

A programming library for playing back music and sound effects you create in
Mod2PSG2 is included. It's written in Z80 assembler for SEGA Master System and
Game Gear.

Mod2PSG2 has been developed by Kontechs Ltd. (http://www.kontechs.com).

Feature list:
  * Easy-to-use user interface
  * Full use of the SN76489 features
  * 16 instruments with volume envelopes and vibrato effects
  * Undo, redo, cut, copy, paste and other editing functions
  * Import .MID, .MOD, .VGM/Z and .BAS files
  * Export .MID, .VGM and .EPSGMOD files
  * MIDI keyboard support
  * A programming library for playing back music and sound effects
    on SEGA Master System and Game Gear

-----------
 0 Content
-----------
 The contents of this readme file are:
 
 1   System requirements
 2   File formats
 3   Modules
     3.1 Instruments
     3.2 Effects
     3.3 Commands
     3.4 Settings
 4   User interface
     4.1 The control panel
     4.2 Adding new notes, effects and commands
     4.3 Changing existing notes, effects and commands
     4.4 Keyboard control
     4.5 MIDI keyboard input
     4.6 Undo/Redo
 5   Programming library
 6   Questions and answers
 7   Tips and tricks
 8   Known bugs/issues
 9   Example songs
10   Website and contact
11   Release log

-----------------------
 1 System requirements
-----------------------
* MS Windows XP/2000 or newer
* Intel x86 compatible processor
  Recommended: At least 500 MHz with MMX
* The latest DirectX9 installed
* The latest graphics drivers installed
* Fast Direct3D9-compatible graphics hardware
  Recommended: At least Radeon or GeForce class with hardware T&L

----------------
 2 File formats
----------------
Mod2PSG2 uses PSGMOD as its native file format. Importing and exporting of
other file formats has various restrictions.

For importing, binary file formats are detected by reading the file's header
and text file formats are detected by their file extension.

Import file formats are:
  * PSGMOD (old)
      There is one restriction when importing old PSGMOD files from early
      versions of Mod2PSG2:
      Notes on the noise channel with non-standard psg frequencies (others
      than 0, 32, 64 or 1024) are changed into standard low/mid/high noise
      channel notes. For example, a note with a psg frequency of 555 is
      converted into a low noise channel note. But the module will still sound
      the same, because the player of the old Mod2PSG2 versions did the same
      conversion internally when it played such notes.
  * MID
  * MOD
      These two will sound differently than the original. The SN76489 simply
      can't reproduce the instruments exactly. And for MID files, it doesn't
      have enough channels. If more than one MIDI channel/voice is on one
      SN76489 channel, it is multiplexed onto it. Not all MIDI/MOD effects
      are supported yet.
      The info texts can't be completly imported if they are too long.
  * VGM/VGZ
      This should sound exactly like the original for standard frame-accurate
      SEGA Master System and Game Gear music. The info texts and names from the
      GD3-tag can't be completly imported if they are too long.
  * BAS
      You can import music and sound from MS QBasic programs.
      But it's very limited because the importer doesn't use a full QBasic
      interpreter. It can only interpret commands with constants (no variables).
      
      The following commands can be imported:
      SOUND const, const
      PLAY  const-string
      BEEP
      Everything else is ignored. The command names are case-sensitive.

  For all import formats except PSGMOD, the imported song can exceed the size limit
  of modules in Mod2PSG2.
    
Export file formats are:
  * MID
      The exported file is going to sound different because of the
      different instruments. You can use this to rearrange your songs
      on more powerful sound hardware. Detuned notes and vibrato effects
      are not yet supported.
  * VGM
      The exported file should sound exactly like the original.
  * EPSGMOD
      Modules in this format can be played back on systems with the Z80 and
      the SN76489 using the Mod2PSG2 Music and Sound Effects Library, which
      you can find in the player_z80/ subdirectory of the Mod2PSG2 archive.
      
      Notes or effects that can't be played (displayed red in the pattern
      display), are automatically removed from the module.
  
===========

 3 Modules

===========
The song you make in Mod2PSG2 is called a module. A module consists of one or more
patterns. A pattern is like a small piece of a song. In the pattern order list,
you can specify which patterns are played when in the song.

One module can't have more than 256 patterns. The pattern order list can't have
more than 256 entries.

A pattern is divided into 4 channels and two command columns. For each channel,
there's a list of notes and effects. A note can be detuned, and also has an
instrument number and a volume. An effect consists of an effect number and an
effect parameter.

This is how a pattern looks like:
                                     ______ Solo-button
                                     |
-------------------------------------\/-----------------------------------------------------
  #| Channel 0    [s]| Channel 1    [s]| Channel 2    [s]| Channel 3    [s]| Cmd.0 | Cmd.1 |
--------------------------------------------------------------------------------------------
  1|           :     |           :     |           :     |           :     |       |       |
  2|           :     |           :     |           :     |           :     |       |       |
  3|           :     |           :     |           :     |           :     |       |       |
  4|           :     |           :     |           :     |           :     |       |       |
 /\     /\        /\                                                          /\      /\
 |      |         |                                                            |       |
 |      |         |___ Effects                                    Commands ____|_______|
 |      |
 |      |___ Notes
 |
 |___ Line number

One channel in detail looks like:
                      _______ Solo-button
                      |
----------------------\/--
| Channel x           [s]| <- All this is the channel header
==========================
| C-2      0 15 : PV   0 | <- Play the note C-2 with instrument 0 and
| ...     .. .. : .. ... |    volume 15, and set pulse vibrato to 0
| ...     .. .. : .. ... |
| ...     .. .. : PV   4 | <- Set pulse vibrato to 4
| ...     .. .. : .. ... |
| ...     .. .. : .. ... |
| C-4 -1   1 10 : .. ... | <- Play the note C-4 (detuned by -1) with
| ...     .. .. : .. ... |    instrument 1 and volume 10
| ...     .. .. : .. ... |
   /\      /\  /\   /\  /\
   |       |   |    |   |
   |       |   |    |   |___ Effect parameter
   |       |   |    |
   |       |   |    |___ Effect number
   |       |   |
   |       |   |___ Volume
   |       |
   |       |___ Instrument number
   |
   |___ Note

Click the channel-header to enable or disable a channel. When you press the
Solo-button of a channel, then this channel is enabled and all others are
disabled. If the channel was already the only one enabled, then all channels
will be enabled.

When a pattern is played, its lines are played successively, starting from line 0.
How long a pattern line takes can be changed using the SetSpeed-command. The
initial speed can be set in the module settings. The default is 6.00.

Each pattern has a name with a maximum length of 32 characters.

-----------------
 3.1 Instruments
-----------------
A module has 16 instruments, numbered 0 to 15. Each instrument has a volume
curve with 5 segments. For each segment, you can set:
1. The delay. This is the length of this segment.
2. The volume-add. This is the value that is added to the volume every video frame.
3. The vibrato speed and strength.
The last segment has no delay, it lasts forever.
Each instrument has name with a maximum length of 32 characters.

-------------
 3.2 Effects
-------------
The following effects are available:

Name                Description                      Key
---------------------------------------------------------
PulseVibrato (PV)   Set the pulse vibrato strength   P
SetVolume    (SV)   Set the volume                   V

PulseVibrato (PV):
  With this you can change the current pulse vibrato strength of
  a channel. The pulse vibrato adds the pulse vibrato strength
  to the psg frequency whenever the lowest bit of the pulse vibrato
  mask is 1. You can change the pulse vibrato mask using the
  PulseVibratoMask-command. The pulse vibrato mask is circularly
  rotated one bit to the right after every pattern line.
  
  For example, if you have a note with a psg frequency of 500, the
  pulse vibrato mask is set to AA (10101010) and the pulse vibrato
  strength to 3, then it will be played line by line with the
  psg frequencies 500, 503, 500, 503 and so on.

SetVolume (SV):
  This sets the volume of a channel.
  
  When it's in the same line of a new note, then it will overwrite
  the note's volume value. But in this case it's useless, because
  you could just change the note's volume value instead.

--------------
 3.3 Commands
--------------
The following commands are available:

Name                    Description                   Key
----------------------------------------------------------
SetSpeed         (SP)   Set the playback speed        S
SetStereo        (ST)   Set the GG stereo mask        T
Callback         (CB)   Calls the callback function   C
PulseVibratoMask (PV)   Sets the pulse vibrato mask   P

SetSpeed (SP):
  With this you can set the playback speed in number of frames per line.
  The player can only play for whole frames, even if you set the speed
  to a non-integer number.

SetStereo (ST):
  This command sets the Game Gear stereo register. It can be used
  to enable or disable channels on the left or right sound output.
  The parameter is a hexadecimal number. The left nibble is for the
  left output, the right nibble is for the right output.
  You can calculate the number by adding some of the following values:
  $01: Channel 0, left
  $02: Channel 1, left
  $04: Channel 2, left
  $08: Channel 3, left
  $10: Channel 0, right
  $20: Channel 1, right
  $40: Channel 2, right
  $80: Channel 3, right
  For example, if you want the channels 0 and 1 played on the left side,
  and the channel 2 and 3 on the right side, you use the number $3C.
  The default number is $FF, which means that all channels are played on
  both left and right.

Callback (CB):
  This can be used to give back info to your program. When a callback
  command is reached, the callback function is called.
  
  For example, special events could be triggered.

PulseVibratoMask (PV)
  This command sets the pulse vibrato mask for the PulseVibrato-effects.
  The default pulse vibrato mask is $AA (%10101010).
  
  Standard values are:
  $AA (%10101010) (fast pulse vibrato)
  $CC (%11001100)
  $F0 (%11110000) (slow pulse vibrato)
  
  See the PulseVibrato-effect for more information.

--------------
 3.4 Settings
--------------
In the module settings window you can:
- Set a title and author, each with upto 256 characters
- Write a description/notes with upto 2048 characters
- Set the system type to NTSC (60 Hz) or PAL (50 Hz)
  (Game Gear is always 60 Hz, Master System can be both.)
- Set the frequency divider to 1x or 2x. The SN76489's clock frequency is divided
  by this number.
  Warning: For real Master System or Game Gear systems this must be set to 1x.
- Set the repeat position. After the module has finished playing, it will go
  to this position in the pattern
  order list and continue there.
- Set the initial playing speed. The default is 6.00.
- Set the initial GG stereo value. The default is $FF.

==================

 4 User interface

==================
The user interface consists of the menu at the top, the control panel and the
pattern display.

-----------------------
 4.1 The control panel
-----------------------
Here's picture of the control panel:

---------------------------------------------------------------------------------
| [  Insert   ] | Pattern      |  |          | [Edit]  [Play] [C.Play]   [Test] |
| [  Remove   ] | order list   |  |__________|         [Stop] [P.Play] Step:[ ] |
| [+][-] [U][D] |              |  -------------------                           |
| [Make new...] |              |  | Instrument list |  Speed : [  ]  [Pat.set.] |
| [Delete old.] |              |  |                 |  Stereo: [  ]  [Pat.res.] |
|               |              |  |                 |  Octave: [  ]  [Mod.set.] |
| [ Overview  ] |              |  |                 |  Volume: [  ]  [Mod.info] |
--------------------------------------------------------------------------------
  \____________________________/  \_________________/  \_____________________/
        Pattern order list            Instruments        Playing and editing

The control panel is divided into 3 parts:
1. Pattern order list
  This part includes the pattern order list itself and various buttons to control it.
  
2. Instruments
  This part has a list of the instruments, a display of the currently selected
  instrument and an edit button for calling the instrument editor.

3. Playing and editing
  This parts has buttons and number fields for playing and editing the module.

--------------------------------------------
 4.2 Adding new notes, effects and commands
--------------------------------------------
To add new notes, effects and commands, both width and height of the
cursor-rect must be 1.

For new notes, the cursor-rect has to be in the first column of a channel.
Use the following keys to add a new notes on the square wave channels:
z../ and s..':  Add new notes from C to E (from the current octave)
q..] and 1..=: Add new notes from C to G (from the next octave)
a            : Add null note
The instrument number and volume for this new note are taken from the
currently selected instrument in the instruments list and the volume field.

On channel 3 (the noise channel), there are only 8 different notes possible.
Use the folowing keys to add a new note on the noise channel:
z..v: Add notes from WLo to WHi and then WC2
b..,: Add notes from PLo to PHi and then PC2

For new effects, the cursor-rect has to be in an effect-column. Use the
effect's key to add it.

For new commands, the cursor-rect has to be in the command-column. Use
the command's key to add it.

---------------------------------------------------
 4.3 Changing existing notes, effects and commands
---------------------------------------------------
Notes can be transposed and detuned using the following keys:
F9      /F10     : Note   up/down
F11     /F12     : Octave up/down
Numpad +/Numpad -: Detune +1/-1
Numpad *         : Invert detune sign
Numpad /         : Set detune to 0

To change the instrument numbers and volumes of a note, use the following keys:
v/V: Change volume            +1/-1
i/I: Change instrument number +1/-1
The cursor-rect has to cover instrument number/volume columns for this.

To change the parameters of effects and commands, use the following keys:
q/Q: Change the high   part of the parameter +/-
w/W: Change the middle part of the parameter +/-
e/E: Change the low    part of the parameter +/-
The cursor-rect has to cover effect and/or command columns for this.

----------------------
 4.4 Keyboard control
----------------------
To navigate through the module, use the cursor keys and the Home/End/PageUp/PageDown-keys.
If you press the Shift key at the same time, you can resize the selection frame.

F1..F8: Select the current octave

New notes (the cursor-rect has to be in the first column of a channel with width and height 1)
z../ and s..': Add new notes from C to E (from the current octave)
q..] and 1..=: Add new notes from C to G (from the next octave)
a            : Add null note
On the noise channel (channel 3):
z..v: Add notes from WLo to WHi and then WC2
b..,: Add notes from PLo to PHi and then PC2

Transposing and detuning:
F9      /F10     : Note   up/down
F11     /F12     : Octave up/down
Numpad +/Numpad -: Detune +1/-1
Numpad *         : Invert detune sign
Numpad /         : Set detune to 0

CTRL + n: New  module
CTRL + o: Load module
CTRL + s: Save module

Delete:   Delete the selected notes, effects and commands
CTRL + x: Cut to clipboard (cut = first copy and then delete)
CTRL + c: Copy to clipboard
CTRL + v: Paste from clipboard

CTRL + y: Undo
CTRL + z: Redo

CTRL + g: Gradient
          For the gradient you have to select more than 2 lines inside a single column.
          Depending on this column, the notes, volumes or effect/command parameters are
          interpolated from the first to the last selected line. New notes, effects
          or commands are not added inbetween.

CTRL + p: Play/Stop

Return:          Move down the columns under the cursor
                 (of each selected channel)
Shift+Return:    Move down the columns under the cursor
                 (note and effect columns separately)
Backspace:       Move up all the columns under the cursor
                 (of each selected channel)
Shift+Backspace: Move up all the columns under the cursor
                 (note and effect columns separately)
Space:           Move the cursor 'line-step' lines down
Shift + Space:   Select all lines of the current columns

Alt + Return: Toggle between fullscreen and windowed video mode

The keys can be different on keyboards of different countries. For example,
on German keyboards, y and z are reversed.

-------------------------
 4.5 MIDI keyboard input
-------------------------
If you have a MIDI keyboard, you can use it in Mod2PSG2.
First you have to enable MIDI-In and set the correct DeviceID in the preferences.

You can add new notes by pressing MIDI keyboard keys. This is especially useful
while the module is currently played back. Select one or more channels and you
can play in a song using your MIDI keyboard.

A new note is put on one of the selected channels in the following way:
If a MIDI keyboard key is pressed, then a note will be added on the first free
channel and this channel is allocated. As soon as the same keyboard key is
released, the channel is deallocated.

---------------
 4.6 Undo/Redo
---------------
With the undo and redo functions, you can undo and redo up to 1023 changes to
the module. But this number can decrease if you do bigger/more complex changes.

=======================

 5 Programming library

=======================
The programming library is located in the player_z80/ subdirectory in the
Mod2PSG2-archive. It has been made for the WLA-Z80 assembler, which is part
of WLA-DX by Ville Helin. The code is written in Z80 assembler for the SEGA
Master System and Game Gear.

In the player_z80/ subdirectory, there's another readme.txt that has descriptions
of all the functions in the library and how to use them.

=========================

 6 Questions and answers

=========================
Q1. Can I make money with the music I make with Mod2PSG2?
A1. Yes, you can. The license is not supposed to restrict your rights on
    the files you create. You can do with them whatever you want.

===================

 7 Tips and tricks

===================
* On the plus, minus, up and down buttons for changing the pattern order list,
  you can hold down the mouse button instead of repeatedly pressing it.
* You can doubleclick on the instrument list to edit the selected instrument.
* You can click on the pattern order list while the module is playing to
  continue playing at the selected position.
* You can load a module at startup by using the module's filename as
  command line parameter.
* You can configure MS Windows to use Mod2PSG2 as the standard program
  for PSGMOD files. Mod2PSG2 will then start and load a PSGMOD file when you open it.
* When the player has finished playing a module, it starts again from the
  beginning, but without a reset.
  For example speed, stereo settings and effects are not reset to their defaults
  and notes that have started playing will continue to play, even though they
  don't play if you start the module from the beginning.

=====================

 8 Known bugs/issues

=====================
If you find a bugs or issues in Mod2PSG2, then please don't hesitate to tell us about them.

Here's a list of currenty known bugs/issues:
* Sound effects are not supported in the programming library

Troubleshooting:
----------------
If Mod2PSG2 doesn't run, you can try to edit the preferences file (mod2psg2.prefs).
You have to start Mod2PSG2 one time before the preferences file is created.
- Enable fullscreen mode     (set appeareance->fullscreen to true)
- Use 16 bit video modes     (set appeareance->fullscreen_color_depth to 16)
- Use lower quality textures (set appearance->texture_quality to low)

=================

 9 Example songs

=================
The example songs that are in the examples/ subdirectory of Mod2PSG2 were
made by the following people:

Alex Mauer
Chris Kelly
Shiru

Let us know if you want songs you made available there, too.

========================

 10 Website and contact

========================
The Mod2PSG2 website is at: http://mod2psg2.kontechs.com
The eMail address for this program is: _m_o_d_2_p_s_g_2_@kontechs.com with all
the underscores removed.

Suggestions, questions and bug reports about Mod2PSG2 are welcome.

================

 11 Release log

================
[2.04] 23.11.2007 - Bugfix release
 - Some bug fixes and tweaks
 - All the gradient functions can now be used with only one key combination (CTRL + g)
 - Added the Multi-Pattern-Settings dialog to set the pattern settings of multiple patterns easily
 - Added testing of noise channel tones in the instrument editor
 - Changed the keyboard keys for the noise channel (high and low reversed)
[2.03] 31.10.2007 - EPSGMOD release
 - Added EPSGMOD export
 - Added a programming library for playing back EPSGMOD files
 - A few bug fixes
 - Added the PulseVibratoMask command
 - Added the parameter gradient function (for effect and command parameters)
 - The note gradient now also uses detuning
 - An initial GG stereo mask can now be set in the module settings
 - Changed the way channels are enabled and disabled (also with Solo-buttons)
[2.02] 23.10.2007 - Tweak release
 - Tweaked the GUI
 - Tweaked VGM importing
 - Tweaked MOD importing
 - A few bug fixes
 - Added support for importing MOD files with 2 channels ("2CHN")
 - Added support for the 640x480 resolution and 16 bit video modes
 - Removed the option to force texture to be 16 bit and added a texture quality option instead
   (These video options can only be configured in the preferences file at the moment)
 - Added a second command column
 - Added the Callback command (CB)
 - Added the edit functions Half/Double with pattern resize
 - Press * on keypad to invert detune sign and / on keypad to set detune to 0
 - Press Alt + Return to toggle between fullscreen and windowed video mode
 - Doubleclick on the instrument list to edit an instrument
 - Click on the pattern order list while playing to change the playing position
 - New key for null notes is A
[2.01] 10.10.2007 - Bugfix release
 - Added lots of example songs in the examples/ subdirectory
 - Fixed out-of-range repeat positions of PSGMOD files made with the old version
   (It's reset to 0, which is what the old player did internally.)
 - Fixed writing into the number-fields on the main window
 - Fixed a few operations not scrolling to the cursor position
 - Fixed form of directory names in the file requester
 - Fixed GG stereo for the noise channel
 - Fixed noise channel taking frequency from channel 2 (WC2/PC2 notes)
 - Some little graphics optimizations
 - Added an option to force the textures to be 16 bit (A4R4G4B4 format)
 - Improved the error messages when initializing
 - Fixed MIDI-In device not being used after pressing Okay in the preferences window
 - Changed the way MIDI-In devices are chosen in the preferences window
   (The old way is not supported anymore - you have to choose again.)
[2.00] 7.10.2007 - First public release of one of the new versions
