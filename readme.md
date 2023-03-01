Dining Philosophers
===================

> The dining philosophers problem is an example problem often used in concurrent algorithm design to illustrate synchronization issues and techniques for resolving them. It was originally formulated in 1965 by Edsger Dijkstra as a student exam exercise, presented in terms of computers competing for access to tape drive peripherals.

![Dining Philosopers](https://upload.wikimedia.org/wikipedia/commons/thumb/7/7b/An_illustration_of_the_dining_philosophers_problem.png/463px-An_illustration_of_the_dining_philosophers_problem.png)

This lab provides an opportunity to explore the dining philosophers problem. You task is to provide an implementation for the concurrent `run_philosopher` routine such that philosophers are able to concurrently eat successfully.

Problem
-------

> A number of philosophers live in a house where the table is laid for them; each philosopher has their own place at the table. Their only problem – besides those of philosophy – is that the dish served is a very difficult kind of spaghetti, that has to be eaten with two chopsticks. There are two chopsticks next to each plate, so that presents no difficulty: as a consequence, however, no two neighbours may be eating simultaneously.
>
> A philosopher can only eat spaghetti when they have both left and right chopsticks. Each chopstick can be held by only one philosopher at a time and so a philosopher can use the chopstick only if it is not being used by another philosopher. After an individual philosopher finishes eating, they need to put down both chopsticks so that the chopsticks become available to others. A philosopher can only take the chopstick on their right or the one on their left as they become available, and they cannot start eating before getting both chopsticks.

A more detailed description of the problem and possible solutions is available on [Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem). 

Running
-------

An example program, `philisophers.c` is included. It contains a stub implementation that leads to a deadlock. You can run and test the program by calling:

```
make test
```

You will likely notice the program stall due to the deadlock. A successful algorithm with allow the program to run until all philosophers are finished eating. Philosophers must eat concurrently. It is not acceptable for a few philiosphers to eat until satisfied before handing over their chopsticks.
