
sum polynom by threads
Authored by sagi ben shushan
209351147

==Description for ex4a ==

The program open terminal for the user and get input of polynom and value of variable 'x'.
The program keep run till the user's input will be "done".
There is 4 global variables: totalSum, numOfX, countThreads, pthread_t threads[3].
This globlal variables are used in some functions and i wanted to save the update of the changes for the duration of the program.
First i seperate the input string to polynom and value of x, the polynom saved in string and for the value i made casting to int and keep him in numOfX.
After that, i seperate the polynom by plus and make thread to every part exept the part without the char 'x'.
If there is part that dont have char 'x' i cast this string to int and add it to the totalSum and dont make thread for him. 
In the other cases every part make thread and send the string to sumPartOfArray.
In the function i sum and return the number that keep in to the threads array.
In the main i make the pthread_join that wait for all the thread till they done their work and then sum all the result from the thread and add them to the totalSum.

==functions for ex4a==
cleanStringAndSaveVar:  After reciving the input from the user i divide the input into two parts, the first part is up to the char ',' which represents the polynom and i keep it in a new string.
The part after the char ',' is the value of 'x' which saved in global variable

splitByPlus: Make the function strtok which divided by plus the polynom that recived as input to the function splitByPlus. Each part is kept as a string and checked if there is 'x', if dont have 'x' i dont send this part to the function makeThread

makeThreads: Create thread to the parts of the polynom that sent as a input for this function.

sumPartOfAraay: This function is called from the function makeThreads and make calculation of this part of the polynom that got as input and return the value which enter into the array of threads.

powerOfx: Calculation of x in power of number.

thereIsX: Check if there is char 'x' in the string, if char 'x' is not exsist i make casting for str to int and add it to the totalSum (global variable).




==Description for ex4b ==
This program get the input and seperate the string in the same way by the same function(cleanStringAndSaveVar).
This program is also run until the input will be "done".
There is 6 global variables: totalSum, numOfX, shm_ptr, shm_id, numOfFork, shareMemIndex
This globlal variables are used in some functions and i wanted to save the update of the changes for the duration of the program.
The difference between program a to program b is that in program we create processes by fork instead of threads.
In the main function i create a share memory which will serve as an array of reasults of each of the processes which i will summarize and add to the totalSum at the end of the program after all the sons will done their work.
In program b i used the function wait in loop for all the son in diffrence to program a that i used in join

-The functions are the same exept the function makeThread that changed to makeForks and called from the same function.

==Program Files==
ex4a.c ,ex4b.c

==How to compile ex4a?==
compile: gcc ex4a.c -o ex4a -pthread
run: ./ex4a

==How to compile ex4b?==
compile: gcc ex4b.c -o ex4b
run: ./ex2b

==Input:==
a string of polynom, value of 'x'

==Output:==
the result of the polynom after placing the value.






