#  Midterm-Tests

Before running the tests, do the following:

 - Add any necessary includes to run your pointers.
 - Since I don't know
   the names of your classes, I created the aliases at the beginning of
   the test.cpp file. Currently, the aliases use the std implementation
   of the smart pointers (which will pass all tests), you will need to
   rename it to use the name of your implementation.

To run this, you can use 

    g++ -std=c++20 ./test.cpp -g -Wall -o test
and then run 

    ./test
If this works without segfaulting and passes all the tests. And if Valgrind reports no memory leaks. You can be fairly confident that your solution works.