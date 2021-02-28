# Homework 4: Reasoning about Correctness

*CSci 2041: Advanced Programming Principles, Fall 2020*

**Due Wednesday, Nov 4 at 5:00pm**

Recall that late days are to be requested using 
[this form](https://forms.gle/mZSdyRTZYpSyHTBQ6).

If you need more days, just fill out the form again with the new
**total** number of late days that you want to use.

If you want a refund of unused days, just fill out the form again with
the new reduced total number of late days that you ended up using.

Please be mindful of the rules for homework - discussing problems is
OK, but not your specific solution. Software is used to look for
similarities in code between student submissions. This isn't meant as
a threat, just a reminder about what the rules are.


## Getting started

The proofs of program properties are easily written up
electronically.  You will be required to turn in an electronic version
of your proofs.  To make this simple, provide your answer to the
questions below in a MarkDown file.  These are the files with a
``.md`` extension that you find in the public course repository.  For
example the files in the ``Notes`` directory that contain sample
proofs are MarkDown files.  

Follow the examples in those files and you can easily create the
MarkDown files that you need to turn in for this assignment.

To get started, create a ``Hwk_04`` directory in your individual
repository.  In that, create a file named ``proofs.md`` and add the
following text to that file:
```
# Homework 4: Reasoning about Correctness.

by <<write your name here>>
```

Now use Git to ``add``, ``commit`` and ``push`` this file.  The view
it in your browser in the same way that you view the Markdown
``Feedback`` files that are created for some of your programming
assignments.

By pushing your file up to GitHub you can check that it is formatted
in a way that makes it easy to read. This is the view your TAs will
have when your work is graded, so please make sure it formats
correctly and looks nice. 


## Proofs of program properties

Before beginning this assignment.  Make sure you have completed all of
the in-class exercises in the "S2" slides.   These slides can be found here:

https://github.umn.edu/umn-csci-2041-F20/public-class-repo/blob/master/Course-Resources/Slides/S2_Reasoning_about_correctness-handout.pdf

These are good "warm-up" exercises that will ensure that you have a
good understanding of the material.  If you start this assignment
without doing these exercises then you are not yet prepared and thus
this assignment will take you longer to complete.

## Problem 1

Consider the following OCaml function definition over lists:
```ocaml
let rec prod = function
  | [] -> 1
  | y::ys -> y * prod ys
```
Using the definition above, show by induction that
```ocaml
prod (l1 @ l2) = prod l1 * prod l2
```
The work you turn in must explicitly and clearly indicate
1. the base case that you prove, 
2. the inductive case that your prove, 
3. and the inductive hypothesis used in your proof.
4. the principle of induction that you are using in your
   proof.  This is tied to the type of the values over which you are
   doing induction.
5. proof of the base case
6. proof of the inductive case

Each step in your proof must be accompanied by a justification
describing why that step could be taken.

## Problem 2

Consider the following OCaml function definitions over lists:
```ocaml
let rec sum = function
  | [] -> 0
  | y::ys -> y + sum ys

let rec length = function
  | [] -> 0
  | y::ys -> 1 + length ys

let rec inc_all = function
  | [] -> []
  | y::ys -> (y+1) :: inc_all ys
```
Using the definitions above, show by induction that
```ocaml
sum (inc_all l) = length l + sum l
```

The work you turn in must explicitly and clearly indicate
1. the base case that you prove, 
2. the inductive case that your prove, 
3. and the inductive hypothesis used in your proof.
4. the principle of induction that you are using in your
   proof.  This is tied to the type of the values over which you are
   doing induction.
5. proof of the base case
6. proof of the inductive case

Each step in your proof must be accompanied by a justification
describing why that step could be taken.


## Problem 3
Consider the following OCaml function definition over lists and the
definition of ``inc_all`` above:
```ocaml
let rec map f l = match l with
  | [] -> []
  | y::ys -> f y :: map f ys

let inc x = x + 1
```
Using the definitions above, show by induction that
```ocaml
map inc l = inc_all l
```

The work you turn in must explicitly and clearly indicate
1. the base case that you prove, 
2. the inductive case that your prove, 
3. and the inductive hypothesis used in your proof.
4. the principle of induction that you are using in your
   proof.  This is tied to the type of the values over which you are
   doing induction.
5. proof of the base case
6. proof of the inductive case

Each step in your proof must be accompanied by a justification
describing why that step could be taken.


## Problem 4
Consider the following OCaml definitions.  The first is the definition
of the ``tree`` type from parts 1-3 of Homework 3.  The second is a
function that converts trees of this type to lists.
```ocaml
type 'a tree = Empty
             | Node of 'a * 'a tree * 'a tree

let rec to_list (t: 'a tree) : 'a list = match t with
  | Empty -> []
  | Node (v, tl, tr) -> to_list tl @ [v] @ to_list tr
```
Add your implementation of ``product`` from part 1 of Homework 3 to
your solution Markdown file.

Using the implementation of ``prod`` in Problem 1 of this assignment,
your implementation of ``product`` and the definitions above, show by
induction that 
```ocaml
prod (to_list t) = product t
```

The work you turn in must explicitly and clearly indicate
1. the base case that you prove, 
2. the inductive case that your prove, 
3. and the inductive hypothesis used in your proof.
4. the principle of induction that you are using in your
   proof.  This is tied to the type of the values over which you are
   doing induction.
5. proof of the base case
6. proof of the inductive case

Each step in your proof must be accompanied by a justification
describing why that step could be taken.

If your implementation of ``product`` is complicated, now might be a
good time to see if it can be simplified. You do not need to use the
same implementation of ``product`` that you turned in for Homework 3.
You can use a version that you improved.


## Problem 5

Add your implementation of ``size`` from part 1 of Homework 3 to
your solution Markdown file.

Now, add your implementation of ``size`` from part 3 that uses your
``reduce`` function to Markdown file - but rename it to ``size_r`` to
distinguish it from your ``size`` function from part 1.

Next, copy your ``reduce`` function from part 3 into your Markdown
file as well.

If your implementation of these functions is complicated, now might be a
good time to see if they can be simplified. You do not need to use the
same implementations that you turned in for Homework 3.
You can use a versions that you improved.

Using your implementations of these functions, show by induction that
```ocaml
size t = size_r t
```

The work you turn in must explicitly and clearly indicate
1. the base case that you prove, 
2. the inductive case that your prove, 
3. and the inductive hypothesis used in your proof.
4. the principle of induction that you are using in your
   proof.  This is tied to the type of the values over which you are
   doing induction.
5. proof of the base case
6. proof of the inductive case

Each step in your proof must be accompanied by a justification
describing why that step could be taken.


# What to turn in.

Push your ``proofs.md`` file to GitHub just as you push all your
previous assignments.  Look at the result that is rendered in your
browser.  Fix any formatting issues so that it is easy for the TAs to
read.  A small number of point will be awarded for Markdown files that
are easy to read.

The deadline for this assignment is Wednesday, Nov 4 at 5:00pm.
