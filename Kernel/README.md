#Introduction
The folder contains source file for kernel implementation independent from target.
RTOS is a collection of running Active objects (or processes). Active objects (AO) have no shared
memory so they do not need synchronization mechanism. AOs exchange data by passing messages or events.

## Active object (AO)
AO is process that running infinite loop and doing only one thing - reading of messages 
from incoming mail box (Ring buffer), processing its and writing outcoming messages to 
outcoming mail box (or Ring buffer)

- stack
- mailboxes
- execution - infinite loop
- processing messages (events)