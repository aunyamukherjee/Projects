(*Sample Tree start ----------------------------------------------------------------------*)

(* A tree type declaration. *)
type 'a tree = Empty
             | Node of 'a * 'a tree * 'a tree

(* A sample tree containing ints *)
let int_tree : int tree =
  Node (3, 
        Node (1,
              Node (4, Empty, Empty), Empty), 
        Node (2, Empty, Empty) 
       )

let int_tree2 : int tree =
  Node (4, 
        Node (9,
              Node (5, 
                Node(3, Empty, Node(2,Empty,Empty)), Empty), Empty), 
        Node (1, Empty, Empty) 
        )

(* A sample tree containing strings *)
let str_tree : string tree = 
  Node ("love ", 
        Node ("really ", 
              Node ("I ", Empty, Empty), Empty), 
        Node ("OCaml!", Empty, Empty) 
       )

(* A test tree containing strings *)
let str_tree2 : string tree = 
  Node ("think ", 
        Node ("really ", 
              Node ("couldn't ", 
                Node ("I ", Empty, Empty), Empty), Empty), 
        Node ("of a better example", Empty, Empty) 
        )

let ints_tree: int list tree =
  Node ([1;3],
        Node ([4;5;6], 
              Empty,
              Node ([], Empty, Empty)
             ),
        Node ([],
              Node ([1;6], Empty, Empty),
              Node ([9;2;8],Empty,Empty)
             )
       )

let ints_tree2: int list tree =
  Node ([2;4],
        Node ([6;8], 
              Empty,
              Node ([13;12], Empty, Empty)
              ),
        Node ([],
              Node ([1;9], 
                Node ([11], Empty, Empty), Empty),
              Node ([7;5;23],Empty,Empty)
              )
        )

let strs_tree: string list tree = 
  Node (["Ocaml!  "; "It "; "must "; "be "],
        Node (["do "; "love "], 
              Node (["I "; "really "], Empty, Empty), Empty), 
        Node (["your "; "favorite "; "too!"], Empty, Empty) 
       )

let strs_tree2: string list tree = 
  Node (["of ";"more "; "exciting "; "examples "],
        Node (["could "; "think "], 
              Node (["I "; "wish "; "I "], Empty, Empty), Empty), 
        Node (["for "; "my "], Empty, Node(["test ";"cases ";"but ";"alas!"], Empty, Empty)) 
        )
(*Sample Tree end ----------------------------------------------------------------------*)

(*Part 1 start ----------------------------------------------------------------------*)

let rec size (tree: 'a tree) : int = match tree with
  | Empty -> 0
  | Node(_, Empty,Empty) -> 1
  | Node(_, nodel, noder) -> 1 + size nodel + size noder

let rec sum (tree: int tree) : int = match tree with
  | Empty -> 0
  | Node(a, Empty,Empty) -> a
  | Node(b, nodel, noder) -> b + sum nodel + sum noder

let rec product (tree: int tree) : int = match tree with
  | Empty -> 1
  | Node(a, Empty,Empty) -> a
  | Node(b, nodel, noder) -> b * product nodel * product noder

let rec charcount (tree: string tree) : int = match tree with
  | Empty -> 0
  | Node(a, Empty,Empty) -> String.length a
  | Node(b, nodel, noder) -> String.length b + charcount nodel + charcount noder

(*Helper function for concat to make the list of strings*)
let rec concatHelper (tree: string tree) : string list = match tree with
  | Empty -> [""]
  | Node(a, Empty,Empty) -> [a]
  | Node(b, nodel, noder) -> ((concatHelper nodel)@[b]) @ (concatHelper noder)
  
let concat (tree: string tree) : string =
  String.concat "" (concatHelper tree)
(*Part 1 end ----------------------------------------------------------------------*)

(*Part 2 start ----------------------------------------------------------------------*)

(*Could use this helper function instead of List.length
let rec length (lst: 'a list) : int= 
  match lst with
  | [] -> 0
  | x::xs -> 1 + length xs
  *)

let rec list_tree_size (tree: 'a list tree) : int = match tree with
  | Empty -> 0
  | Node(a, Empty, Empty) -> List.length a
  | Node(b, nodel, noder) -> List.length b + (list_tree_size nodel) + (list_tree_size noder)

(*Helper function summing the ints in an int list*)
let sumlist (lst: int list) : int = 
  List.fold_left (+) 0 lst

let rec list_tree_sum (tree: int list tree) : int = match tree with
  | Empty -> 0
  | Node(a, Empty, Empty) -> sumlist a
  | Node(b, nodel, noder) ->  sumlist b + (list_tree_sum nodel) + (list_tree_sum noder)

(*Helper function summing the ints in an int list*)
let prodlist (lst: int list) : int = 
  List.fold_left (fun a b -> a*b) 1 lst

let rec list_tree_product (tree: int list tree) : int = match tree with
  | Empty -> 1
  | Node(a, Empty, Empty) -> prodlist a
  | Node(b, nodel, noder) -> (prodlist b) 
                             * (list_tree_product nodel) 
                             * (list_tree_product noder)

(*Helper function for charcount*)
let charcountList (lst: string list) : int =
  List.fold_left (fun a b -> a + String.length b) 0 lst

let rec list_tree_charcount (tree: string list tree) : int = match tree with
  | Empty -> 0
  | Node(a, Empty, Empty) -> charcountList a
  | Node(b, nodel, noder) -> (charcountList b)  
                             + (list_tree_charcount nodel) 
                             + (list_tree_charcount noder)

let rec concatHelper2 (tree: string list tree) : string list = match tree with
  | Empty -> []
  | Node(a, Empty, Empty) -> a
  | Node(b, nodel, noder) -> ((concatHelper2 nodel)@b) 
                             @ (concatHelper2 noder)

let list_tree_concat (tree: string list tree) : string = 
  String.concat "" (concatHelper2 tree)

(*Part 2 end ----------------------------------------------------------------------*)

let () = 
  print_string "Testing part 1 ... " ;
  try
    assert (size Empty = 0);
    assert (size str_tree = 4);
    assert (size str_tree2 = 5);
    assert (sum Empty = 0);
    assert (sum int_tree = 10);
    assert (sum int_tree2 = 24);
    assert (product Empty = 1);
    assert (product int_tree = 24);
    assert (product int_tree2 = 1080);
    assert (charcount Empty = 0);
    assert (charcount str_tree = 20);
    assert (charcount str_tree2 = 43);
    assert (concat Empty = "");
    assert (concat str_tree = "I really love OCaml!");
    assert (concat str_tree2 = "I couldn't really think of a better example");
    print_string "tests passed.\n"
  with
    Assert_failure (file, line, column) -> 
    let msg = "\n\n\nAssert failed on line " ^ string_of_int line ^ 
                ", column " ^ string_of_int column ^ "\n\n\n\n"
    in print_string msg


let () = 
  print_string "Testing part 2 ... " ;
  try
    assert (list_tree_size Empty = 0);
    assert (list_tree_size strs_tree = 11);
    assert (list_tree_size strs_tree2 = 15);
    assert (list_tree_sum Empty = 0);
    assert (list_tree_sum ints_tree = 45);
    assert (list_tree_sum ints_tree2 = 101);
    assert (list_tree_product Empty = 1);
    assert (list_tree_product ints_tree = 311040);
    assert (list_tree_product ints_tree2 = 4774049280);
    assert (list_tree_charcount Empty = 0);
    assert (list_tree_charcount strs_tree = 54);
    assert (list_tree_charcount strs_tree2 = 74);
    assert (list_tree_concat Empty = "");
    assert (list_tree_concat strs_tree = 
              "I really do love Ocaml!  It must be your favorite too!");
    assert (list_tree_concat strs_tree2 = 
              "I wish I could think of more exciting examples for my test cases but alas!");
    print_string "tests passed.\n"
  with
    Assert_failure (file, line, column) -> 
    let msg = "\n\n\nAssert failed on line " ^ string_of_int line ^ 
                ", column " ^ string_of_int column ^ "\n\n\n\n"
    in print_string msg

    
