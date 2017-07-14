*Description of command-line parameters
Default value for number of words is 25143 and number of threads is 4. Dictionary file name is words and the result of found palindrome words are saved in “result” file, which will be created in the same path as code file exists. The number of threads can be changed from command line as described below.

*Instructions to build and to run the application:
$ g++ palindromic.c –o palindromic.exe -std=c++11 -fopenmp
$ ./ palindromic.exe <number of threads>

*Program description and performance evaluation.
The program intended to find all palindrome words in the given dictionary file ‘words’ and print out the result in ‘result’ file. The program is implemented to perform the task in parallel using openmp API. Also before starting, the task of finding palindromes, all words converted to lowercase to implement case insensitive program.
Speedup evaluation:

				
 
nomber of words	10000		17000		25143	
   
Sequential 	0.006s	0.0069s	0.007s

speedup:	   
2 threads		1.538		1.725		1.4	   
3 threads		2		2.3		1.795	   
4 threads		2		2.38		2.33	   
6 threads		1.538		2.3		2.33	   
8 threads		1.538		1.769		1.795	   
10 threads		1.5		1.769		1.43	   
14 threads		1.538		1.725		1.43	 

Considering Amdahl’s law:
As can be seen from the table, for a fix problem size and so fix serial part, as the number of threads increases the speed up increases as well. However, this is not true when the number of threads exceeds the actual number of threads provided by CPU, which are four. This is due to overhead of creating threads. However, the speed up is not satisfactory except in case of two threads. In addition, Amdahl does not consider varying problem size so for this problem Gustafson’s law must be considered as the problem size is varying.

Considering Gustafson’s law:
For a varying problem size hence the varying serial fraction, it can be seen that the speedup is increased very slightly as the number of processor increases. In other words, the possible speed up that could be achieved reduced by the serial fraction of the program (speedup= n-(n-1).s(n)).
10000 words and moving from 3 to 4 threads shows no improvement in speedup
17000 words and moving from 3 to 4 threads only 3% improvement in speedup
25143 words and moving from 3 to 4 threads only 30% improvement in speedup
Therefore, adding more processors it is not that much efficient as it was expected.
Conclusion:
Considering that efficiency=speedup/number of processors, it is obvious that adding to the number of processors did not bring noticeable speedup. In other word, good use of multiple cores could not be achieved due to ineffective parallelizing code. 
Even though adding processors reduces computation time but increases overheads such as communication time. Sometime, this overhead is larger than decrease in computation time by making code parallel.
On the other hand, to improve speedup, it is tried to achieve better load balance among the processors. To achieve this schedule ( static) used to break the for loop in chunks of iterations. If we have N iteration, it assigns a chunk of N/ (number of threads) iterations to each thread. This preferred over dynamic since iterations of a loop takes unifying computation load.
However, the main problem that prevents achieving better speed up is the critical section of the program that needs to be executed sequentially. This greatly affects the efficiency of the parallelism and prevents to use the multicore system effectively. So there is a need to change the algorithm.

