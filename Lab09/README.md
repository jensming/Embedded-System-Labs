James Ensminger

# Lab9: BattleBoats

I wrote: <br />
- Agent.c <br />
- AgentTest.c <br />
- Negotiation.c <br />
- NegotiationTest.c <br />
- Message.c <br />
- MessageTest.c <br />
- Field.c <br />
- FieldTest.c

NOTE: I worked alone on this lab! When running the game, I would recommend using Field_correct.o in place of my Field.c since I believe <br />
there's still issues present that I have yet to figure out or resolve. I tried programming a working version, so if you want to test it <br />
out, please feel free but I'll go with the safe route of choosing to code Message.c over Field.c when considering it in the grading report.

## Introduction: <br />
In this lab, I implemented the underlying mechanics of the BattleBoats by defining libraries which rely on two state machines to run this <br />
game. The state machine in Message.c handles varrying types of messages generated within the system and encodes as well as decodes them for <br />
the state machine in Agent.c to interpret. Depending on the message, Agent.c will then determine whether the agent or opponent is attacking <br />
or defending on a given turn until one person's boats have all sunk on thier field. I would imagine that if I had a partner with another <br />
UNO32, we could connect both microcontrollers to play a game. When starting, both the challenger and acceptor will have a randomly generated <br />
number that gets hashed and sent to one another's UNO32. These hashed secret numbers are then XORed together in Negotiation.c to determine <br />
which player gets to attack or defend first and the acceptor is able to verify that the challenger isn't cheating on their end with a <br />
tweaked secret commitment.

## Feedback: <br />
This lab was so much more complex and difficult than I had imagined. I'm really glad to have gotten a head start with it after finishing the <br />
previous lab early. As far as the lab manual goes, I thought there could've been more clarification on Field.c for solo workers since it was <br />
lacking in Field.h descriptions already (I was stuck on getting the opponent's field generated with boats without them showing). Other than <br />
that, I thought this lab was very painful yet rewarding in some areas when tests went well for various functions (especially with Message.c).