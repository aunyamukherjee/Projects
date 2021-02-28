# Final Project

*CSci 2041: Advanced Programming Principles, Fall 2020*

**Due Friday, Dec 18 at 11:59pm**

Note that there will be no extensions on the final project.  Course
staff need adequate time to grade this work and we will be starting 
right away on the morning of Saturday, Dec 19.

## Problems from which to choose

Below are a number of problems that you may choose from as the problem
you will solve for your final project.

Since your circumstances may be challenging right now, you may want to
make some calculations about how much time and energy you can afford
to commit to this effort.  Some are easier and are thus worth less
when your final grade is computed. Some are more challenging and thus
worth more.

Problems have a specified maximum letter grade that you can receive
for that problem.  Problems range from a maximum letter grade of a C
to a maximum of a grade of A.  This is the maximum grade **on this
project** only.

I encourage you to read over all of the problems and consider what
sort of commitment you can make to this project.  Note that choosing a
problem that can earn a B will not be interpreted in a negative way.
Of course, we would like to see everyone choosing the most challenging
problems to maximize what you may learn in this effort - but reality
being what it is, we are not so naive!


## Getting Started

Create a directory in your individual repository named
`Final_Project`.  All files that you create will be put into this
directory.

There are no automated feedback tests yet for the project.  These will
be set up soon.

## Option 1: Revisiting Paradelles

Successfully completing this problem will earn a maximum grade of C.

**You may only work on this problem if you had significant problems on
the Paradelle portion of Hwk 02.*** Specifically, if you missed more
than 1 of the paradelle tests that were to report an
`IncorrectLastStanza` or `IncorrectLines` error, **only then** may you
complete the `paradelle` function from Hwk 02 as your final project.

This condition has been checked and if you are allowed to complete
this problem for the final project then there will be a file named
``Final_Project_Paradelle.md`` in your individual course repository.
If you do not have this file but believe that you should, please email
the TAs to ask for clarification.

Copy `hwk_02.ml` from your `Hwk_02` directory into the `Final_Project`
directory.

**The problem:**
Please see the specification for Homework 2 to review what is expected
for the `paradelle` function.



## Option 2: Word games

Successfully completing **the first** of the word problems here will earn a
maximum grade of B.

If you **also** complete the second one, the maximum grade is an A.

It is **not** an option to only complete the second problem.  It will
not be graded if the first one is not solved.


### Option 2-A: The Q-U Quiz

This problem asks you to compute answers for the Q-U Quiz as heard on the
Sunday Puzzle on NPR.  The puzzle definition and the radio
broadcast of it can be found here:

  https://www.npr.org/2020/11/08/932548059/sunday-puzzle-the-q-u-quiz

You are to write a function `qu_quiz` with the following type:

```
string -> (string * string) list
```

This function takes in the name of a file containing a list of words
and produces a list of answers.  Each answer is a pair such that if
the letters 'q' and 'u' are added to the first word, the letters can
be rearranged to spell the second word in the pair.

This work is to be put in a file named `qu_quiz.ml` in the
`Final_Project` directory of your individual repository.

#### A new function for reading files

To read a list of words from a file, please use the updated
`read_file` function provided in the file `read_file.ml` in the
`Homework/Files/Final_Project` directory in the course assignments
directory of the public class repository.

The version you were given previously is rather inefficient and will
overflow the stack on the larger word files and **only return some of
that file contents**.


#### Word lists

Also in the `Files/Final_Project` directory are 3 word lists
- `words-small.txt`
- `words-google-10000.txt`, 
  taken from here: https://github.com/first20hours/google-10000-english
- `words-corncob.txt`, 
  taken from here: http://www.mieliestronk.com/wordlist.html

#### Sample results

With the sample solution to this problem, evaluating `qu_quiz
"words-small.txt"` will produce the following results:

```
[ ("crooner", "conqueror");
  ("italy", "quality");
  ("stir", "squirt");
  ("teens", "sequent")
]
```
The above text is reformatted to make it easier to read.

The order of word pairs in the list does not matter.  Tests of the
functionality of your function will have the following form that only
checks for list membership of answers:
```
List.mem ("italy", "quality") (qu_quiz "words-small.txt")
```
This expression should evaluate to `true`.

### Option 2-B: It Takes Two


This problem asks you to compute answers for the It Takes Two puzzle
as heard on the Sunday Puzzle on NPR.  The puzzle definition and the
radio broadcast of it can be found here:

  https://www.npr.org/2018/01/21/579110492/sunday-puzzle-it-takes-two


You are to write a function `it_takes_two` with the following type:

```
string -> (string * string) list
```

This function takes in the name of a file containing a list of words
and produces a list of answers.  

Each answer is a pair such that the first string is a 4 letter word
(no not that kind!) found in the input file and the second is a 6
letter work, also found in the input file, that is created from the 4
letter word by adding a single letter at the front of the word and a
single letter at the end of the word.

The same word files described above in the QU Quiz problem will also
be used for this problem.

For example, the file "words-small.txt" contains "planet" and "lane"
and thus ``it_takes_two words-small.txt`` will return a list
containing `("lane", "planet")`

``answers`` will return the string ``"planet"`` in its output.

This work is to be put in a file named `it_takes_two.ml` in the
`Final_Project` directory of your individual repository.

With the sample solution to this problem,
evaluating `it_takes_two "words-small.txt"` will produce the following
results:
```
[("lane", "planet"); ("moot", "smooth"); ("hang", "change");
 ("went", "twenty"); ("nigh", "knight"); ("tree", "street");
 ("refi", "prefix"); ("brad", "abrade"); ("awes", "rawest");
 ("onto", "wonton"); ("craw", "scrawl"); ("isle", "misled");
 ("rest", "presto"); ("plan", "upland"); ("afar", "safari")]
```

The order of word pairs in the list does not matter.  Tests of the
functionality of your function will have the following form that only
checks for list membership of answers:
```
List.mem ("lane", "planet") (it_takes_two "words-small.txt")
```
This expression should evaluate to `true`.



## Option 3: Extending the `expr` language in Hwk 05

If you choose this problem, the maximum grade is an A.

To begin, copy your `Hwk_05/expr.ml` file into your `Final_Project`
directory.

### Getting started
To begin, copy your `Hwk_05/expr.ml` file into your `Final_Project`
directory.

Before starting on this new work, you must first solve any parts of
Hwk 05 that you did not previously solved.  If you solved part 3, you
now need to solve part 4.  If you solve part 4, you now need to solve
part 3.

If you did not complete part 4 at the time, it would be good to go
back are review the slides on how we define closures and use them in
the evaluation of recursive functions.  These start around slide 32 in
the `S3_Programs_as_Data` slides.

### Adding pairs and lists.

Add the following constructors to your `expr` datatype:
```
  | Pair of expr * expr
  | Fst of expr
  | Snd of expr

  | Nil 
  | Cons of expr * expr
  | Head of expr
  | Tail of expr
```

These will be familiar to you:

- `Pair` constructs a pair of values.  We will only construct tuples
  of size 2 in this work, not tuples of any other size.

  `Pair (x, y)` corresponds to `(x, y)` in OCaml.

- `Fst` returns the first element of a pair

  `Fst e` corresponds to `fst e` in OCaml.

- `Snd` returns the second element of a pair

  `Snd e` corresponds to `snd e` in OCaml.

- `Nil` indicates an empty list.

  `Nil` corresponds to `[ ]` in OCaml.

- `Cons` indicates is a list constructor.

  `Cons (h, t)` corresponds to `h :: t` in OCaml.

- `Head` extracts the head of a list.

  `Head lst` corresponds to `List.hd lst` in OCaml.

- `Tail` extracts the tail of a list.

  `Tail lst` corresponds to `List.tl lst` in OCaml.


In addition to these new forms of expression, we need to represent
values of these new types.  Thus, add the following constructors to
your `value` type:
```
  | PairV of value * value
  | ListV of value list
```

Finally, we need to represent these types for these new values.  Thus
add the following constructors to your `typ` type:
```
  | Pair_type of typ * typ
  | List_type of typ
```

One important problem you will face in type checking lists, is
determining a type for the empty list `Nil`.  It may not be reasonable
to use the constructors for `typ` that we have so far for this kind of
value.  Thus, you may decide to add additional constructors of your
choosing to the `type` type.


### Extending `type_check` and `eval`

The main effort in this problem is to extend `type_check` and `eval`
to work with pairs and lists.

You will also want to extend `unparse` to handle the new kinds of
expressions, values, and types.

In addition to this, you need to extend the behavior of the `Eq`
constructor so that it can check for equality of any non-functional
types.  That is, integers, Booleans, and pairs and lists that do not
contains functional values.


Below are a number of sample results:

```
OCaml expression # 1:
   Pair (Val (Int 3), Val (Bool false))

unparse:
   "(3, false)"

type:
   (int * bool)

value:
   PairV (Int 3, Bool false)

value (using unparse):
   (3, false)


-----------------

OCaml expression # 2:
   Pair (Add (Val (Int 3), Val (Int 4)), Val (Bool false))

unparse:
   "((3 + 4), false)"

type:
   (int * bool)

value:
   PairV (Int 7, Bool false)

value (using unparse):
   (7, false)


-----------------

OCaml expression # 3:
   Let ("p", Pair_type (Int_type, Int_type), Pair (Val (Int 3), Val (Int 4)), Add (Fst (Id "p"), Snd (Id "p")))

unparse:
   "(let p : (int * int) = (3, 4) in ((fst p) + (snd p)))"

type:
   int

value:
   Int 7

value (using unparse):
   7


-----------------

OCaml expression # 4:
   Fst (Val (Int 3))

unparse:
   "(fst 3)"

type errors (1):
   (Fst (Val (Int 3)), "expected Pair type")


-----------------

OCaml expression # 5:
   Snd (Val (Int 3))

unparse:
   "(snd 3)"

type errors (1):
   (Snd (Val (Int 3)), "expected Pair type")


-----------------

OCaml expression # 6:
   Cons (Add (Val (Int 3), Val (Int 4)), Nil)

unparse:
   "((3 + 4) :: [])"

type:
   (int list)

value:
   ListV [Int 7]

value (using unparse):
   [7]


-----------------

OCaml expression # 7:
   Let ("lst", List_type (Int_type), Cons (Val (Int 3), Cons (Val (Int 4), Nil)), Id "lst")

unparse:
   "(let lst : (int list) = (3 :: (4 :: [])) in lst)"

type:
   (int list)

value:
   ListV [Int 3; Int 4]

value (using unparse):
   [3; 4]


-----------------

OCaml expression # 8:
   Eq (Nil, Nil)

unparse:
   "([] = [])"

type:
   bool

value:
   Bool true

value (using unparse):
   true


-----------------

OCaml expression # 9:
   Eq (Cons (Val (Int 3), Nil), Nil)

unparse:
   "((3 :: []) = [])"

type:
   bool

value:
   Bool false

value (using unparse):
   false


-----------------

OCaml expression # 10:
   Head (Cons (Val (Int 3), Nil))

unparse:
   "(List.hd ((3 :: [])))"

type:
   int

value:
   Int 3

value (using unparse):
   3


-----------------

OCaml expression # 11:
   Tail (Cons (Val (Int 3), Nil))

unparse:
   "(List.tl ((3 :: [])))"

type:
   (int list)

value:
   ListV []

value (using unparse):
   []


-----------------

OCaml expression # 12:
   Tail (Cons (Val (Int 3), Cons (Val (Int 4), Nil)))

unparse:
   "(List.tl ((3 :: (4 :: []))))"

type:
   (int list)

value:
   ListV [Int 4]

value (using unparse):
   [4]


-----------------

OCaml expression # 13:
   LetRec ("sum", Func_type (List_type (Int_type), Int_type),Lam ("lst", List_type (Int_type), If (Eq (Id "lst", Nil), Val (Int 0), Add (Head (Id "lst"), App (Id "sum", Tail (Id "lst"))))), App (Id "sum", Cons (Val (Int 1), Cons (Val (Int 2), Cons (Val (Int 3), Nil)))))

unparse:
   "(let rec sum : ((int list) -> int) = (fun (lst: (int list)) -> (if (lst = []) then 0 else ((List.hd (lst)) + (sum (List.tl (lst)))))) in (sum (1 :: (2 :: (3 :: [])))))"

type:
   int

value:
   Int 6

value (using unparse):
   6
```
