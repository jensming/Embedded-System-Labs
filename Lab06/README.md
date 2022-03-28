James Ensminger

# Lab5: Basic IO on a Microcontroller

## Summary <br />
In this lab, I implemented programs along with timers as well as ISRs that interact with hardware on the I/O shield. Such programs <br />
include utilizing the OLED, potentiometer, switches, buttons, and LEDs. I thought that learning how to use interrupts to drive events <br />
that would control how hardware components such as the buttons and LEDs behaved against the system clock were most important since <br />
they enabled the bouncing state for each program. <br />

## Approach <br />
My general approach to the lab was to move from step 1 all the way to step 5. For each step, I would break down what I needed to get done <br />
by hand writing pseudocode and then translating it into code. If I ran into any issues, I would debug the program and review both my <br />
pseudocode and the lab manual for any errors or key info that I had missed. Other than that, following through with this approach worked <br />
well and I wouldn't change it if I were to do this lab again.

## Results <br />
In the end, parts 1 through 5 and even the extra credit portion went really well after I figured out how the ISR worked along with the <br />
program in each of the main(). Overall, this lab took me about 15 hours in total to complete as the learning curve concerning reading <br />
sensors and manipulating the outputs obtained by them was a bit heavy for me. Aside from that, I really enjoyed how satisfying it was <br />
to get the LEDs and OLED working along with the switches and buttons for each part. The hardest parts were trying to interpret the lab <br />
as many portions were unclear and lacking discriptions when compared to what was shown in the example videos. To reduce any unnecessary <br />
confusion, I would add additional info about how to implement each part of the lab. Additionally, I would clarify on the extra credit <br />
portion for any specific features that are required for it to count or not. As for the grading rubric, it seems reasonable for the most part.