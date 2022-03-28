James Ensminger

# Lab4: Reverse Polish Notation

## Summary <br />
In this lab, I created an interactive Reverse Polish Notation calculator along with a string-parsing and stack libraries <br />
to implement such a program. The string and backspace handling as well as user input error detections were the most important <br />
aspects of the lab since they ensure a smooth UI while the program runs in a loop.

## Approach <br />
Before even working on any code, I first read the manual thoroughly and then layed out a plan for what I would work on <br />
each day. I started with creating the stack library and then tested it until all corner cases were ruled out. I then did <br />
the same thing for the string-parsing library. This step took exceptionally longer than the first since it was the core <br />
of the program and evidently had many bugs that I had to weed out during my tests. At first, I ran into issues on how to <br />
properly work with pointers and tokens from tokenizing the Reverse Polish Notation strings and then determining the conditions <br />
for each error that the program may have encountered. After I figured out how they work, I began to parse through each token <br />
successfully after numerous tests. If I were to do this lab again, I would try to make more print statements during each <br />
test on a function from either library since it would have pointed out the issue in the terminal rather than having to guess <br />
the exact location of where the program falls short. Since I worked alone, I mainly looked up definitions of functions provided <br />
by libraries in C that I used in my string-parsing and stack libraries for further clarification on what they could do.

## Result <br />
The calculator I programmed ended up working better than I thought. The program passed all tests that I gave it and even looks <br />
good when running on Linux. I spent probably at least 15 hours or more trying to understand pointers and string manipulation in <br />
C. I really enjoyed how simple it was to construct the final main for the project since it looks clean and easy to follow as all <br />
main files should be. Additionally, the functionality of the program was fun to play with in the end since it has so many. My least <br />
favorite part of the whole lab was struggling with string manipulation since we barely covered it when we started this lab. The <br />
hardest parts were working on the functions in rpn.c since they involved extensive string handling. I feel like the lectures and <br />
manual could have covered the necessary topics needed to complete this lab before we started it. Overall, this was a worthwhile <br />
lab that extensively taught me how to construct/implement a stack and manipulate string with tokenization. <br />