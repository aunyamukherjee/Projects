# Class Projects
Projects with C++, C, Java, Python, Assembly, LaTex

## C++
- Currently working on a Delivery System Simulation Project, inside docs/html Directory, index.html will eventually have more in depth explanation of code

## C
- Projects Relating to Operating Systems (Basic Map Reduce, IPC-based Map Reduce, Multi-Threaded Web Server and Web Socket Programming) each have README files as well as Project Write Ups included in their respective directories.

- Project 1
	- Problem1
		- Read ints from command line, write them into a file and then read them back.
	- Problem 2
		- Create a graphing model that will read data from a file and print it out to scale
	- Problem 3
		- Through a variety of commands, creates and modifies a BST of key value pairs that can be searched for
- Project 2
	- Clock
		- LCD Clock simulator. Read a number of seconds from the beginning of the day from the command line and show the results of running functions from clock_update.c on the screen.
	- PuzzleBox
		- puzzlebox.c: Inputs are read from a file on the command line. The inputs are modified in various ways to checked to match specific patterns associated with "phases". Each set of correct inputs "passes" a phase and scores points. Correct imputs are found in input.txt
- Project 4
	- Made several data structures to find which were most efficient to search through in time and memory (tested using a benchmark I wrote)
	- Tested most efficient ways to sum the diagonal of a matrix through data placement and tested against a benchmark

## Java
- Anagram Tree
	- A Java program that finds all sets of anagrams in a text file of English words, using an algorithm based on binary search trees.
	- We start with an empty anagram tree. Then we read words as strings from a text file. Every time we read a word, we construct its summary. Then we search for a node in the tree that has the summary as its key, using the K-and-R algorithm to control the search. If we find the node, then we add the word to the node’s list (if it’s not already there). If we don’t find the node, then we add a new node to the list. The new node’s key is the summary, and its value is a list that contains the word.  
After we’ve read all the words, there will be many nodes in the anagram tree, each with a list of one or more words. The words in each list are anagrams of each other, because they all have the same summary. We then traverse the tree, visiting each node. If we find a node whose list has only one word, then we ignore it, because the word is an anagram only of itself. However, if we find a node whose list has two or more words, then we print those words on the same line, because they are anagrams of each other.  
Suppose the program reads _N_ words. If we assume the words are read in random order, then it takes _O_(log _N_) time to add a word to the anagram tree. Since we add _N_ words, it takes _O_(_N_ log _N_) time to build the entire tree. Since the tree has _N_ nodes, it takes _O_(_N_) time to traverse it. As a result, the program runs in _O_((_N_ log _N_) + _N_) = _O_(_N_ log _N_) time.
- Int Sort
	- Implements an efficient algorithm to sort linear singly-linked lists of integers.
	- I started with a notation for lists of integers. Something like [5, 8, 4, 9, 1, 2, 3, 7, 6] to mean such a list. This list has nine nodes: its first node contains a 5, its second node contains an 8, its third node contains a 4, etc., all the way to its ninth node, which contains a 6. I wrote an empty list of integers as []. I wanted to sort lists like these into _nondecreasing order._ That means that the integers in the list doesn’t decrease from left to right. For example, if I sort [2, 3, 1, 1, 2] into nondecreasing order, then I get [1, 1, 2, 2, 3]. Similarly, if I sort [5, 8, 4, 9, 1, 2, 3, 7, 6] into nondecreasing order, then I get [1, 2, 3, 4, 5, 6, 7, 8, 9]. I also wanted my sorting algorithm to be as efficient as possible. As an example, an inefficient sorting algorithm might sort a list by traversing it, looking for pairs of adjacent integers that are in the wrong order, like 8 and 4 in the list above. Whenever it finds such a pair of integers, it exchanges their positions, so that 4 now comes before 8. The algorithm might repeatedly traverse the list in this way, exchanging integers until all are in the right order. Although this algorithm is simple, it needs too many comparisons to sort all the integers. The sorting algorithm I implemented here is more complex, but also more efficient. It works without exchanging adjacent integers, and without traversing lists repeatedly, both of which can make a sorting algorithm slow. This algorithm needs fewer comparisons to sort a list of _n_ integers. The algorithm works in four phases: testing, halving, sorting, and combining.

## Python
- Game of Othello
	- Using python turtle graphics to build the board and game logic to implement the I/O.
- Turtle Video Game
	- Built upon existing code to create a playable lazer shooting game.
- Wheel of Fortune Simulator
	- Using a bank of phrases, one is randomly picked and through I/O the user can make guesses that will either increase or decrease their overall winnings.

## Assembly
- Project 3
	- See Project 2: Clock of C Projects. Reimplements LCD Clock simulator using only assembly. Read a number of seconds from the beginning of the day from the command line and show the results of running functions on the screen.

## LaTex
- Includes LaTex code and corresponding pdf files from past writeups
