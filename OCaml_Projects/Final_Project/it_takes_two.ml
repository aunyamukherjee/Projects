let read_file (filename:string) : string list =
  let rec read_lines channel sofar =
    try 
      let ln = input_line channel
      in read_lines channel (ln :: sofar)
    with
    | End_of_file -> sofar
    | e -> raise e
  in
  try 
    let channel = open_in filename
    in 
    let lines_in_reverse = read_lines channel []
    in List.rev lines_in_reverse
  with
  | e -> raise e

let is_elem_by (f: 'a -> 'b -> bool) (testval: 'b) (lst: 'a list) : bool = 
  List.fold_left (fun a b -> (f b testval) || a) false lst

let dedup (lst: 'a list) : 'a list = 
  List.fold_right (fun b a -> if (is_elem_by (=) b a) then a
                              else b::a) 
                              lst []

let rec explode : string -> char list = function
  | "" -> []
  | s  -> String.get s 0 :: explode (String.sub s 1 ((String.length s) - 1))

let rec implode : char list -> string = function
  | []    -> ""
  | c::cs -> String.make 1 c ^ implode cs

let d1 = "words-small.txt"
let d2 = "words-google-10000.txt"
let d3 = "words-corncob.txt"

let lower_sort_explode (s:string) : char list =
  let newList = explode s
  in 
  let lowerlist = List.map Char.lowercase_ascii newList
  in
  List.sort (compare) lowerlist

let rec check_qu (word: string) (lst: string list) (accum: (string * string) list)= 
  match lst with
  | [] -> []
  | s :: xs -> if (lower_sort_explode (word ^ "qu")) = (lower_sort_explode s)
                        then accum @ [(word,s)]
                        else check_qu (word) (xs) (accum)
                  
let rec check_takes_2 (word: string) (lst: string list) (accum: (string * string) list)= 
  match lst with
  | [] -> []
  | s :: xs -> 
        if (String.length s = 6) then
          match String.sub s 1 4 with 
          | word -> 
                accum @ [(word,s)]
          | _ -> check_qu (word) (xs) (accum) 
        else check_qu (word) (xs) (accum)

let rec feedWords (f: string -> string list -> (string * string) list -> (string * string) list) lst=
  match lst with 
  | [] -> []
  | x :: xs -> (f x (List.filter (fun a -> String.length a = (String.length x)+2) xs) [])@ (feedWords f xs)

let qu_quiz (fname:string) : (string * string) list =
  let wordLists = read_file fname
  in 
  feedWords check_qu wordLists

let it_takes_two (fname: string) : (string * string) list =
  let wordLists = read_file fname
  in 
  dedup (feedWords check_takes_2 wordLists)

