# BloatPad
The slowest, most bloated and feature rich text editor - written in JavaScript!

This is pretty much just a prototype for me to figure out what I wanted.
The "real" thing will be written in assembly because apparently I'm a masochist.
I dunno when I'll get round to that but at least from a user standpoint everything is finished!
My suffering is just begining!

1: The two requirements are - simplicity/ease of use
2: Small file size, I even chose to not even include icons! (Totally not because I have all the artistic vision of a shrew)

### Usage

Use of the text editor consists of three hotkeys 
- [CTRL] + l to show editor when minimised to taskbar
- [CTRL] + o to open a file
- [CTRL] + s to save a file
 
![how delightful!](https://cdn.discordapp.com/attachments/1100857180527263844/1109714645612253194/wDtZQuW.png)
### To build 
Ensure you have a c compiler and cmake installed and on path
```
mkdir build && cd build
cmake ..
cmake --build .
```
