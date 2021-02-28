type 'a btree = Nil
              | Leaf of 'a
              | Fork of 'a btree * 'a * 'a btree

let rec insert_by (comparison: 'a -> 'a -> int) (insrtVal: 'a) (tree: 'a btree) : 'a btree = 
  match tree with
  | Nil -> Leaf(insrtVal)
  | Leaf(a) -> if ((comparison a insrtVal) < 0) then Fork(Nil, a, Leaf insrtVal)
               else if ((comparison a insrtVal) = 0) then Leaf(a)
               else Fork(Leaf insrtVal, a, Nil)
  | Fork(l,a,r) -> if ((comparison a insrtVal) < 0) then Fork(l, a, insert_by comparison insrtVal r)
                   else if ((comparison a insrtVal) = 0) then Fork(l,a,r)
                   else Fork(insert_by comparison insrtVal l, a,r)
  
let from_list (comparison: 'a -> 'a -> int) (lst: 'a list) : 'a btree =
  List.fold_left 
  (fun a b -> insert_by comparison b a) Nil lst

let rec reduce (tree: 'a btree) (b: 'b) (f1: 'a -> 'b) (f2: 'a -> 'b -> 'b -> 'b): 'b =
    match tree with
    | Nil -> b
    | Leaf(a) -> f1 a
    | Fork (t1, v, t2) -> f2 v (reduce t1 b f1 f2) (reduce t2 b f1 f2)

let to_list (tree: 'a btree) : 'a list =
  let toHelper1 a = [a]
  in
  let toHelper2 a l r = l @ [a] @ r
  in
  reduce tree [] toHelper1 toHelper2

let () = 
  print_string "Testing part 4 ... " ;
  try
    assert (insert_by compare 4 Nil = Leaf 4);
    assert (insert_by compare 2 (insert_by compare 4 Nil) =
              Fork (Leaf 2, 4, Nil));
    assert (insert_by compare 4 (insert_by compare 2 Nil) =
              Fork (Nil, 2, Leaf 4));
    assert (insert_by compare 4 (insert_by compare 4 Nil) = 
              insert_by compare 4 Nil);
    assert (from_list compare [4;2;5;3;6;7;8] =
      Fork (Fork (Nil, 2, Leaf 3), 4,
        Fork (Nil, 5, Fork (Nil, 6, Fork (Nil, 7, Leaf 8)))
        ) 
    );
    assert (from_list compare [8;3;10;1;14;6;4;7;13] =
      Fork (Fork (Leaf 1, 3, Fork (Leaf 4, 6, Leaf 7)), 8,
        Fork (Nil, 10, Fork (Leaf 13, 14, Nil)))
    );
    assert (List.sort compare [4;2;5;3;6;7;8] =
        to_list (from_list compare [4;2;5;3;6;7;8]));
    assert (List.sort compare [8;3;10;1;14;6;4;7;13] =
        to_list (from_list compare [8;3;10;1;14;6;4;7;13]));
    (* Add more asserts here as you need them *)
    print_string "tests passed.\n"
  with
    Assert_failure (file, line, column) -> 
    let msg = "\n\n\nAssert failed on line " ^ string_of_int line ^ 
                ", column " ^ string_of_int column ^ "\n\n\n\n"
    in print_string msg

  