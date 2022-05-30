# COL331 - HOHLAB 2
### Lab 1
- Our long-task is ```numlt``` with high keyboard latency.
- ```numlt``` takes a number $$n \in [0, 2 \times 10^8]$$ and returns the count of numbers that are less than n, i.e, ```numlt x``` gives $$x-1$$ Please respect the function domain for correct results.

### UI Description
- For 2.1 to 2.4, whenever a long task is scheduled, we print an empty line where the output will be printed once the computation is complete.
- For 2.1 and 2.2, we provide the functionality of using short task while the long task is running, similarly, for 2.3 and 2.4, long tasks can be simultaneously scheduled.
- Use ```clear``` command when screen is full, however, avoid using ```clear``` when a task is scheduled and the output is yet to be printed.

### Lab 2.1 - 2.2
- We have provided the system-call ```numlt``` in lab 1 and ```numltc``` is the same system-call implemented using co-routines.
- ```numltc``` takes a number $$n \in [0, 10^6]$$ and returns the count of numbers that are less than n, i.e, ```numltc x``` gives $$x-1$$ Please respect the function domain for correct results.
- We can run short tasks like typing and ```fact 5``` while ```numltc``` is running and the changes are reflected on the shell.
- For lab 2, we have the system call ```numltf```, which has the exact same semantics as ```numltc```, but implemented using fibers.

### Lab 2.3 - 2.4
- We have provided system-calls ```numgt``` and ```fibb``` for additional long-tasks to be scheduled in this section.
- ```numgt``` takes a number $$n \in [0, 2 \times 10^8]$$ and returns the count of numbers in the range $$[0, 2 \times 10^8]$$ that are greater than or equal to n, i.e, ```numgt x``` gives $$(2 \times 10^8 - x)$$ Please respect the function domain for correct results.
- ```fibb n``` recursively calculates the n-th fibonacci number, please use values such as ```fibb 30``` or ```fibb 31``` for significantly long computation periods.
- The shell gives an error message if we try to schedule the same task more than 3 times or more than 5 tasks in total.

