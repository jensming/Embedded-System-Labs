James Ensminger

# Lab2: Calculator

## Description <br />
For this lab, I programmed a simple calculator that reads user inputs to perform mathematical calculations <br />
and then prints the results out. This was done on Linux for faster debugging purposes as well as on a serial <br />
port by using the serial terminal program called CoolTerm. The program utilizes and explores concepts of printf() <br />
and scanf(), functions and prototypes, the C standard library, and iterative code design.

## Results <br />
When "scanf(" "); executes in MPLAB X after a specified operator has been selected, the program will skip past <br />
any user input for an operand and then loop back to the beginning of the infinite loop within CalculatorRun(). <br />
This is happening due to the possibility of the whitespace specifier instructing the scanf() to discard any <br />
proceeding scanf() statements until the end of the loop instead of zeros, newlines, or whitespace characters. <br />

The lab manual states that printf() and scanf() are forbidden inside of any function other than CalculatorRun() <br />
and for good reason. A rule like this helps organize code in areas of a program where errors and bugs are likely <br />
encountered. It's also good coding practice for creating uniformity such as in longer programs where debugging may <br />
be more difficult if problems appear in multiple locations rather than one.

## Conclusion <br />
This lab took me about 6-8 hours to fully complete when considering all the git adds, commits, and pushes on each <br />
step of the manual. Debugging took longer than expected when I wasn't using the correct settings in project properties <br />
in both MPLAB X and the serial terminal program. Overall, this lab was a bit trickier than I expected, but was pretty <br />
easy once I got comfortable with the concepts covered.