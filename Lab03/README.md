James Ensminger

# Lab3: Matrix Math

## Description <br />
In this lab, I was asked to write a library of functions that would perform mathematical matrix operations. <br />
I also coded a test file that would validate the accuracy of such functions as demonstrated on Linux. I've <br />
learned how to develop libraries, pass by reference for functions, work with singular as well as multidimensional <br />
arrays, and unit test with a test harness as a result.

## Result <br />
I approached the lab by initially setting up the format layout for my MatrixMath library and then attempted to <br />
setup the MatrixPrint() and MatrixEquals() functions as a first step. After successfully testing both functions in <br />
the main test file mml_test.c, I ran into some problems with finding an efficient way to tally the number of <br />
function test passes. I resolved this issue by using an int variable that would increment if matrices compared <br />
in the MatrixEquals() function came back true for each unit test. Afterwards, the variable would reset to zero <br />
so that it can be resused throughout the test harness. This method of tracking the success rate of each function <br />
worked really well since it uses less lines of code and is uniform throughout the program. With regards to limitations <br />
of matrix sizes and other notables present in the program, the MatrixPrint() only accounts for 3x3 matrices <br />
and to check whether non-array return values from functions were correct, I had to compare those values with the <br />
expected literal values. If I were to do this lab again, I would draft a better format for the test harness more <br />
carefully before unit testing functions from the MatrixMath library. This would save a lot of time in the formatting <br />
hassle that came about when dealing with several hundreds of lines of code.

## Feedback <br />
Overall, I spent about 10 hours understanding the manual and working on this lab. I enjoyed how I got into a rythmn <br />
of testing and implementing each function after getting comfortable with setting up the initial functions at the top <br />
of the list in the MatrixMath library. I disliked how tedious it became when working with functions such as the <br />
MatrixInverse() and MatrixDeterminant() ones since they outputted more complex results that made it difficult at times <br />
to determine if they would compare accurately in my tests. Other than that, this lab had been worthwhile as it has <br />
taught me how to write libraries in C and how to verify the correctness of functions within them with unit testing. <br />
Next time, I would suggest organizing the lab manual so that the concepts are all at the top rather than in the middle <br />
and to move the assignment details and grading rubric to the bottom. I'm not particularly opposed to the grading rubric <br />
structure, but I would like to see that the autograded portion is double checked by the grader so that mishaps like typos <br />
are accounted for. When working through this lab, I felt prepared and knowledgable of the material covered to start and <br />
finish it. The lectures have helped me understand the syntax needed to code such programs which I found most confusing at first.