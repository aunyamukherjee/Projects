# Homework 4: Reasoning about Correctness.

by <Aunya Mukherjee>

## Problem 1
```
let rec prod = function
  | [] -> 1
  | y::ys -> y * prod ys
```
### Prove prod (l1 @ l2) = prod l1 * prod l2

### Part 1: the base case that you prove.
```
prod ([] @ l2) = prod [] * prod l1
```
### Part 2: the inductive case that you prove.
```
prod ((x::xs) @ l2) = prod (x::xs) * prod l2 
```
### Part 3: and the inductive hypothesis used in your proof.
```
prod (xs @ l2) = prod xs * prod l2
```
### Part 4: the principle of induction that you are using in your proof. This is tied to the type of the values over which you are doing induction.
```
P(lst) holds if P([]) holds and
                if P(lst') => P(v :: lst') for any value v
```

### Part 5: proof of the base case.
```
  prod ([] @ l2) 
= prod l2
  by properties of lists and @
= 1 * prod l2
  by arithmetic
= prod [] * prod l2
  by def of prod
```
### Part 6: proof of the inductive case.
```
  prod ((x::xs) @ l2) 
= prod ( x :: (xs @ l2) )
  by properties of lists and @
= x * prod (xs @ l2)
  by def of prod
= x * (prod xs * prod l2)
  by inductive hypothesis
= (x * prod xs) * prod l2
  by arithmetic
= prod (x::xs) * prod l2
  by def of prod
```

## Problem 2
```
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

### Prove sum (inc_all l) = length l + sum l

### Part 1: the base case that you prove.
```
sum (inc_all []) = length [] + sum []
```
### Part 2: the inductive case that your prove.
```
sum (inc_all (y::ys)) = length (y::ys) + sum (y::ys)
```
### Part 3: and the inductive hypothesis used in your proof.
```
sum (inc_all (ys)) = length (ys) + sum (ys)
```
### Part 4: the principle of induction that you are using in your proof. This is tied to the type of the values over which you are doing induction.
```
P(lst) holds if P([]) holds and
                if P(lst') => P(v :: lst') for any value v
```
### Part 5: proof of the base case.
```
  sum (inc_all [])
= sum ([])
  by def inc_all
= 0 + sum ([])
  by arithmetic
= length [] + sum []
  by def length
```
### Part 6: proof of the inductive case.
```
  sum (inc_all (y::ys))
= sum( (y+1) :: inc_all ys )
  by def of inc_all
= (y+1) + sum (inc_all ys)
  by def of sum
= (y+1) + length (ys) + sum (ys)
  by inductive hypothesis
= (1 + length (ys)) + (y + sum (ys))
= length (y::ys) + (y + sum (ys))
  by def of length
= length (y::ys) + sum (y::ys)
  by def of sum
```


## Problem 3
```
let rec map f l = match l with
  | [] -> []
  | y::ys -> f y :: map f ys

let inc x = x + 1
  ```

### Prove map inc l = inc_all l

### Part 1: the base case that you prove.
```
map inc [] = inc_all []
```
### Part 2: the inductive case that your prove.
```
map inc (y::ys) = inc_all (y::ys)
```
### Part 3: and the inductive hypothesis used in your proof.
```
map inc (ys) = inc_all (ys)
```
### Part 4: the principle of induction that you are using in your proof. This is tied to the type of the values over which you are doing induction.
```
P(lst) holds if P([]) holds and
                if P(lst') => P(v :: lst') for any value v
```
### Part 5: proof of the base case.
```
  map inc []
= [] 
  by def of map
= inc_all []
  by def of inc_all
```
### Part 6: proof of the inductive case.
```
  map inc (y::ys)
= inc y :: map inc ys
  by def of map
= (y+1) :: map inc ys
  by def of inc
= (y+1) :: inc_all (ys)
  by inductive hypothesis
= inc_all (y::ys)
  by def of inc_all
```

## Problem 4
```
type 'a tree = Empty
             | Node of 'a * 'a tree * 'a tree

let rec prod = function
  | [] -> 1
  | y::ys -> y * prod ys

let rec to_list (t: 'a tree) : 'a list = match t with
  | Empty -> []
  | Node (v, tl, tr) -> to_list tl @ [v] @ to_list tr

let rec product (tree: int tree) : int = match tree with
  | Empty -> 1
  | Node(a, Empty,Empty) -> a
  | Node(b, nodel, noder) -> b * product nodel * product noder
  ```

### Prove prod (to_list t) = product t

### Part 1: the base case that you prove.
```
prod (to_list Empty) = product Empty
```
### Part 2: the inductive case that your prove.
```
prod (to_list Node(a,treel,treer)) = product Node(a,treel,treer)
```
### Part 3: and the inductive hypothesis used in your proof.
```
prod (to_list treel) = product treel
prod (to_list treer) = product treer
```
### Part 4: the principle of induction that you are using in your proof. This is tied to the type of the values over which you are doing induction.
```
P(tree) holds if P(Empty) holds and
                if P(treel) and P(treer) => P(Node(v,treel,treer)) for any value v
```
### Part 5: proof of the base case.
```
  prod (to_list Empty)
= prod ([])
  by def of to_list
= 1
  by def of prod
= product Empty
  by def of product
```
### Part 6: proof of the inductive case.
```
  prod (to_list Node(a,treel,treer))

CASE 1 => treel = treer = Empty
= prod (to_list Empty @ [a] @ to_list Empty)
  by def of to_list
= prod ( [] @ [a] @ [])
  by def of to_list
= prod ([a])
  by properties of lists and @
= prod a::[]
  by properties of cons, "::"
= a * prod []
  by def of prod
= a * 1
  by def of prod
= a
  by arithmetic
= product (Node(a, Empty,Empty))
  by def of product

CASE 2 => treel = Empty
= prod (to_list Empty @ [a] @ to_list treer)
  by def of to_list
= prod ( [] @ [a] @ to_list treer)
  by def of to_list
= prod ([a] @ to_list treer)
  by properties of lists and @
= prod [a] * prod (to_list treer)
  by our proof in problem 1: prod (l1 @ l2) = prod l1 * prod l2
= prod [a] * product treer
  by inductive hypothesis
= prod a::[] * product treer
  by properties of cons, "::"
= (a * prod []) * product treer
  by def of prod
= (a * 1) * product treer
  by def of prod
= a * 1 * product treer
  by associativity of integer multiplication
= a * product Empty * product treer
  by def of product
= product (Node(a, Empty,treer))
  by def of product

CASE 3 => treer = Empty
= prod (to_list treel @ [a] @ to_list Empty)
  by def of to_list
= prod ( to_list treel @ [a] @ [])
  by def of to_list
= prod (to_list treel @ [a])
  by properties of lists and @
= prod (to_list treel) * prod [a]
  by our proof in problem 1: prod (l1 @ l2) = prod l1 * prod l2
= prod [a] * prod (to_list treel)
  by commutativity of integer multiplication
= prod [a] * product treel
  by inductive hypothesis
= prod a::[] * product treel
  by properties of cons, "::"
= (a * prod []) * product treel
  by def of prod
= (a * 1) * product treel
  by def of prod
= a * product treel * 1
  by commutativity and associativity of integer multiplication
= a * product treel * product Empty
  by def of product
= product (Node(a, treel,Empty))
  by def of product

CASE 4 => All sub trees are non-empty
= prod (to_list treel @ [a] @ to_list treer)
  by def of to_list
= prod (to_list treel) * prod ([a]) * prod (to_list treer)
  by our proof in problem 1: prod (l1 @ l2) = prod l1 * prod l2
= product (treel) * prod ([a]) * product (treer)
  by inductive hypothesis
= product (treel) * prod a::[] * product (treer)
  by properties of cons, "::"
= product (treel) * (a * prod []) * product (treer)
  by def of prod
= product (treel) * (a * 1) * product (treer)
  by def of prod
= a * product (treel) * product (treer)
  by arithmetic and the associativity and commutativity laws of integer multiplication
= product (Node(a, treel, treer))
  by def of product

```

## Problem 5
```
let rec size (tree: 'a tree) : int = match tree with
  | Empty -> 0
  | Node(_, Empty,Empty) -> 1
  | Node(_, nodel, noder) -> 1 + size nodel + size noder

let rec reduce (tree: 'a tree) (b: 'b) (f: 'a -> 'b -> 'b -> 'b): 'b =
    match tree with
    | Empty -> b
    | Node (v, t1, t2) -> f v (reduce t1 b f) (reduce t2 b f)

let size_r (tree: 'a tree) : int =
  let sizeof x y z = 1 + y + z
  in 
  reduce tree 0 sizeof 
  ```

### Prove size t = size_r t

### Part 1: the base case that you prove.
```
size Empty = size_r Empty
```
### Part 2: the inductive case that your prove.
```
size Node(a,treel,treer) = size_r Node(a,treel,treer)
```
### Part 3: and the inductive hypothesis used in your proof.
```
size tree = size_r tree
```
### Part 4: the principle of induction that you are using in your proof. This is tied to the type of the values over which you are doing induction.
```
P(tree) holds if P(Empty) holds and
                if P(treel) and P(treer) => P(Node(v,treel,treer)) for any value v
```
### Part 5: proof of the base case.
```
  size Empty
= 0
  by def of size
= size_r Empty
  by def of size_r and reduce
```
### Part 6: proof of the inductive case.
```
  size Node(a,treel,treer)

CASE 1 => treel = treer = Empty
= 1
  by def of size
= 1 + 0 + 0
  by arithmetic
= sizeof a (reduce Empty 0 sizeof) (reduce Empty 0 sizeof)
  by def of reduce, and sizeof
= size_r Node(a,treel,treer)

CASE 2 => treel = Empty
= 1 + size Empty + size treer
  by def of size
= 1 + size_r Empty + size_r treer
  by inductive hypothesis and base case
= sizeof a (reduce Empty 0 sizeof) (reduce treer 0 sizeof)
  by def of sizeof and reduce
= size_r Node(a,Empty,treer)
  by def of size_r

CASE 3 => treer = Empty
= 1 + size treel + size Empty
  by def of size
= 1 + size_r treel + size_r Empty
  by inductive hypothesis and base case
= sizeof a (reduce treel 0 sizeof) (reduce Empty 0 sizeof)
  by def of sizeof and reduce
= size_r Node(a,treel,Empty)
  by def of size_r

CASE 4 => All sub trees are non-empty
= 1 + size treel + size Empty
  by def of size
= 1 + size_r treel + size_r Empty
  by inductive hypothesis
= sizeof a (reduce treel 0 sizeof) (reduce Empty 0 sizeof)
  by def of sizeof and reduce
= size_r Node(a,treel,Empty)
  by def of size_r
```