James Ensminger

# Lab8: Role-Playing Game

## Summary <br />
For this lab, I programmed a dungeon crawler RPG in Linux with file I/O and implemented two libraries to support the gameplay features. <br />
While working on this game, I thought it was important to understand how to open, read, and close files by using functions such as fopen(), <br />
fread(), fseek(), fgetc(), and fclose(). Additionally, it was also important to know how to access such files.

## Approach <br />
I started by reviewing the lab manual and trying to understand how each component to the lab makes up the finished program in the end. <br />
In the beginning, I defined the Player functions and then proceeded to construct the Game functions after since they relied on them. Within <br />
Game.c, I wrote the room struct and GameGetCurrentRoom* functions at first since they were pretty straight forward. Next, I proceeded to <br />
write the openFile helper function to take care of file openings, reads, and closes which the GameGo* and GameInit functions utilize. <br />
After finishing completing both libraries, I then wrote a basic main which worked really well since it was easy to read when testing it in <br />
Linux. Nothing went wrong after finishing up the main since it was just simply running the main and checking for appropriate user inputs <br />
and prompts during the gameplay. If I were to do this lab again, I would approach it the same way.

## Results <br />
The final version of the game turned out well since I was able to navigate to any room within the RPG world without error. It took me about <br />
10 hours to plan and code for this lab. I really enjoyed seeing the file I/O aspect of this program working when testing out the main. On <br />
the downside, I didn't like how the rooms were predesigned and often lacking flavor since there weren't many traps or special items to be <br />
found. I would suggest adding the option to create our own storyline and or rooms to spice up the RPG. Overall, this was quite a worthwhile <br />
lab since it was a good way to learn about file I/O and how to utilize it in a game. The grading rubric looks appropriate and I thought the <br />
lectures covered the material used in this lab well.