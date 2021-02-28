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
(*Part 3 start ----------------------------------------------------------------------*)

let rec reduce (tree: 'a tree) (b: 'b) (f: 'a -> 'b -> 'b -> 'b)
           : 'b =
    match tree with
    | Empty -> b
    | Node (v, t1, t2) -> f v (reduce t1 b f) (reduce t2 b f)

let size (tree: 'a tree) : int =
  let sizeof x y z = 1 + y + z
  in 
  reduce tree 0 sizeof 

let sum (tree: int tree) : int = 
    let add3 x y z = x+y+z
    in
    reduce tree 0 add3

let product (tree: int tree) : int = 
    let prodOf x y z = x*y*z
    in
    reduce tree 1 prodOf

let charcount (tree: string tree) : int =
    let charCounter x y z = (String.length x) + y + z
    in
    reduce tree 0 charCounter

let concat (tree: string tree) : string =
    let concatOf x y z = (y@[x]) @ z
    in
    String.concat "" (reduce tree [""] concatOf)

let list_tree_size (tree: 'a list tree) : int =
    let treeSize x y z = (List.length x) + y + z
    in
    reduce tree 0 treeSize
 
let list_tree_sum (tree: int list tree) : int =
    let sumlist (lst: int list) : int = 
      List.fold_left (+) 0 lst
    in
    let treeSum x y z = (sumlist x) + y + z
    in
    reduce tree 0 treeSum

let list_tree_product (tree: int list tree) : int =
      let prodlist (lst: int list) : int = 
            List.fold_left (fun a b -> a*b) 1 lst
      in
      let treeProd x y z = (prodlist x) * y * z
      in
      reduce tree 1 treeProd

let list_tree_charcount (tree: string list tree) : int =
      let charcountList (lst: string list) : int =
            List.fold_left (fun a b -> a + String.length b) 0 lst
      in
      let treeCharcount x y z = (charcountList x) + y + z
      in
      reduce tree 0 treeCharcount

let list_tree_concat (tree: string list tree) : string =
      let treeConcat x y z = (y @ x) @ z
      in
      String.concat "" (reduce tree [] treeConcat)

(*Part 3 end ----------------------------------------------------------------------*)

let () = 
  print_string "Testing part 3 ... " ;
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

