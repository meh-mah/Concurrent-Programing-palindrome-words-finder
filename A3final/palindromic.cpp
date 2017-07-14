#include <omp.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <strings.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;
#define DICTIONARY "words"
#define WORDSCOUNT 25143
#define THREADS 4

/*
 * Method used to reverse the given word.
 * the return is the reversed word
*/
string reverse(string word)
{
    reverse(word.begin(), word.end());
    return word;
}
/*
 * turn each word to lower case so it is possible to perform case insensitive comparison
 */
string make_lowercase( const string& in )
{
  string out;

  transform( in.begin(), in.end(), back_inserter( out ), ::tolower );
  return out;
}

int main(int argc, char ** argv)
{
    /*
     * Get the max number of threads from the command line or default value
     */
    int no_threads;
    no_threads = (argc > 1)? atoi(argv[1]) : THREADS;
        
    
    /* 
     * Opens the result file
     */
    ofstream result_file;
    result_file.open("result");
    /* 
     * Opens the the words dictionary file
     */
    ifstream words_file(DICTIONARY);

    /* 
     * store each word in vector and unordered_set. 
     */
    vector<string> rev_words;
    vector<string> original_words;
    unordered_set<string> lowercase_words;
    
    /*
     * keeps the palindromic words
     */
    unordered_set<string> palindrom_words;
    string line;
    
    /*
     * read each line of the words dictionary and storing words in given array
     */
    if(words_file.is_open())
    {
        while(words_file.good())
        {
            /*
             * read each line from the file
             */
            getline(words_file,line);
            /* 
             * store the line.
             */
            rev_words.push_back(make_lowercase(reverse(line)));
            original_words.push_back(line);
            lowercase_words.insert(make_lowercase(line));
        }
        words_file.close();
    }
    
    int i;
    int palCount=0;
    double sTime, eTime;
    // defines number of threads
    omp_set_num_threads(no_threads);
    /*
     * to get time to compute the performance
     */
    sTime = omp_get_wtime();
    /*
     * creates pool of threads
     */
#pragma omp parallel
    {
        /*
         * schedule( static) used to break the for loop in chunks of iterations.
         * If we have N iteration, it assigns a chunk of N/(number of threads) iterations to each thread.
         * this preferred over dynamic since iterations of a loop takes unifying computation load. 
         */
        #pragma omp for reduction(+:palCount) schedule(static)
        for(i = 0; i < rev_words.size(); ++i)
        { 
            /* check to see if the reversed word exist in the lowercase_words. */
            if(lowercase_words.find(rev_words[i]) != lowercase_words.end())
            {
                /*
                 * since insert is not thread safe it must be in critical section to prevent race condition
                 */
#pragma omp critical(insertion_palindromic)
                {
                    palindrom_words.insert(original_words[i]);
                }
                palCount++;
            }
        }
    }
    
    eTime = omp_get_wtime(); /* get end time */
    
    /* Writes to found palindromic words to the output file. */
    for(auto iterate = palindrom_words.begin(); iterate != palindrom_words.end(); ++iterate)
    {
        result_file << *iterate << endl;
    }
    /* Close file. */
    result_file.close();
    /* Prints information: parallel execution time, and number of palindromic. */
    cout << "number of palindromic words: " <<palCount << endl<< "Execution time: " << eTime - sTime << "." << endl;
    return EXIT_SUCCESS;
}