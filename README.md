# Falling-Sand
A falling sand animation created in C

<h3>Inspiration</h3>
Inspired by The Coding Train's video on the falling sand animation.
<a href= "https://www.youtube.com/watch?v=L4u7Zy_b868&t=1s">Video Here</a>

Some help being taken from ChatGPT (mainly to implement the color functionality)
<h5>The file is written in C with implementation using the SDL library.</h5>

<h2>Working</h2>
the <b>CreateGrain</b> function is used to create a grain of sand on x,y co-ordinates on the screen.<br>
the <b>HandleMouseClick</b> function to generate new grains at that registered x,y co-ordinates

<h2>Running the program</h2>
You can directly run the <b>sand.exe</b> file to start the simulation.<br><br>Incase of any error encountered you can run the <b>Makefile</b> 
<br><b>On windows:</b>
<b>Make sure mingw32 and gcc is installed</b><br>

```
mingw32-make
```

In case of this not working you can compile your code manually:

```
gcc -Isrc/Include -Lsrc/lib -o sand sand.c -lmingw32 -lSDL2main -lSDL2
```
<h3>This program is not of the most efficient simulations.</h3>
