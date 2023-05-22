# BloatPad
The slowest, most bloated and feature rich text editor - written in JavaScript!

This is pretty much just a prototype for me to figure out what I wanted.
The "real" thing will be written in assembly because apparently I'm a masochist.
I dunno when I'll get round to that but at least from a user standpoint everything is finished!
My suffering is just begining!

1: The two requirements are - simplicity/ease of use.

2: Small file size. ~~I even chose to not even include icons! (Totally not because I have all the artistic vision of a shrew)~~ I have since added icons :D Thanks, AI!

##### Constraints

`I have decided on the following constraints to work within. I'm totally sure I won't regret this later!`

- Must be written in assembly (Note: This is not the case for this prototype which is written in C and is just a proof of concept and a way for me to test out ideas without going insane)
- Must be entirely self contained.
- Must be no more than 16kb when compiled in release form without icons and 32kb with icons included. (It is currently 15kb without icons and 20kb with icons included when compiled in release form)
- Must be able to open and save files and behave like a normal text editor would be expected to.


### Usage

Use of the text editor consists of three hotkeys 
- [CTRL] + L to show editor when minimised to taskbar
- [CTRL] + O to open a file
- [CTRL] + S to save a file
 
![how delightful!](https://cdn.discordapp.com/attachments/1100857180527263844/1109714645612253194/wDtZQuW.png)
### To build 
Ensure you have a c compiler and cmake installed and on path
```
mkdir build && cd build
cmake ..
cmake --build .
```
Optionally you can then use cpack to create an installer.
```
# From the build directory
cpack .
```