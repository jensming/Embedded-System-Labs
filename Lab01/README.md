James Ensminger

# Lab1: Compiling, Running, Debugging

## Part 1 Results: <br />
[25, 193, 60, 104, 70] <br />
[25, 193, 60, 104, 70] <br />
[25, 193, 60, 104, 70] <br />
[25, 60, 193, 104, 70] <br />
[25, 60, 104, 193, 70] <br />
. <br />
. <br />
. <br />
[25, 60, 70, 104, 193]

## Part 1 Lines of Code Change: <br />
12-15 <br />
17 <br />
22-23 <br />
30 <br />
35 <br />

## Description <br />
In this lab, I ran basic C programs in the simulator and UNO32 hardware component <br />
through the XC32 compiler and PIC32 processor in MPLAB X IDE. For part0, I performed <br />
the simple "Hello World" program with UNO32 and on my local OS using GCC. In part1, <br />
I ran a program in MPLAB X IDE that sorts temperature values and learned how to debug <br />
as well as alter the format of the code. Similarly in part2, I implemented code and <br />
ran a program in the same IDE that outputs temperature conversion tables, one from <br />
Fahrenheit to Celsius and the other from Kelvin to Fahrenheit. Finally for part3, I <br />
implemented code to program the OLED on the physical UNO32 board to display <br />
"Hello World!" Additionally, I learned to pick up Git when adding, committing, and <br />
pushing project files to GitLab.

## Testing & Simulating <br />
When formatting the temperature conversion tables in part 2, I made syntax mistakes <br />
by including braces around the format specifiers within the printf() statements. <br />
I identified this problem after examining that my program wasn't outputting the expected <br />
numerical values found in the lab manual's examples. Getting rid of the braces quickly <br />
resolved the issue and my program evidently outputs the correct tables as a result.

## Feedback <br />
The instructions were straight forward after I had gotten oriented around Git and <br />
MPLAB X IDE's interface. It might be helpful to uniformly specify what files to commit to <br />
GitLab across all manuals found on Canvas.