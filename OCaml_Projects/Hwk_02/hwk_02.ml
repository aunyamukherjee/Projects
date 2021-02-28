(* This file contains a few helper functions and type declarations
   that are to be used in Homework 2. *)

(* Place part 1 functions 'take', 'drop', 'length', 'rev',
   'is_elem_by', 'is_elem', 'dedup', and 'split_by' here. *)
let rec length (lst: 'a list) : int= 
  match lst with
  | [] -> 0
  | x::xs -> 1 + length xs

let rec take (n: int) (l: 'a list) : 'a list = 
  match l with
  | [] -> []
  | x::xs -> if n > 0 
             then x::take (n-1) xs 
             else []

 let rec drop (n: int) (l: 'a list) : 'a list = 
  match l with
  | [] -> []
  | x::xs -> if n > 0 
             then drop (n-1) xs 
             else l

let rev (lst: 'a list) : 'a list = 
  List.fold_left (fun a b -> b::a) [] lst

let is_elem_by (f: 'a -> 'b -> bool) (testval: 'b) (lst: 'a list) : bool = 
  List.fold_left (fun a b -> (f b testval) || a) false lst

let is_elem (testval: 'a) (lst: 'a list) : bool = 
  is_elem_by (=) testval lst

let dedup (lst: 'a list) : 'a list = 
  List.fold_right (fun b a -> if (is_elem b a) then a
                              else b::a) 
                              lst []

let split_by (eqlfun: 'a -> 'b -> bool) (lst: 'b list) (splst: 'a list) : 'b list list =
  let accum = ([],[])
  in let f elem (elems,groups) =
    if (is_elem_by eqlfun elem splst)
    then (elems@[groups],[])
    else (elems, (groups@[elem]))
  in
  let (a,b) = List.fold_right f lst accum
  in rev(List.map rev (a@[b]))

 (* Some functions for reading files. *)
 let read_file (filename:string) : char list option =
   let rec read_chars channel sofar =
     try
       let ch = input_char channel
       in read_chars channel (ch :: sofar)
     with
     | _ -> sofar
   in
   try
     let channel = open_in filename
     in
     let chars_in_reverse = read_chars channel []
     in Some (rev chars_in_reverse)
   with
     _ -> None

 (* Part 2 *)
 type word = char list
 type line = word list

 let helpbreaklines (lst: char list): line=
   let lowerlist = List.map Char.lowercase_ascii lst
   in
   let newlist =  (rev (List.fold_left (fun a b -> if b ='\n' then ' '::'\n'::' '::a else b::a) [] lowerlist))
   in
   let secondlist = split_by (=) newlist [' ';'.';',';'-';'\t';'!';'?';';';':']
   in
   List.filter (fun elem-> elem != []) secondlist

 let helpbreaktolists (linelst: line): line list=
   List.filter (fun elem-> elem != []) (split_by (=) linelst [['\n']])

 let convert_to_non_blank_lines_of_words (wrd: char list) : line list=
   let intowords = helpbreaklines wrd
   in
   helpbreaktolists intowords

 type result = OK
        | FileNotFound of string
        | IncorrectNumLines of int
        | IncorrectLines of (int * int) list
        | IncorrectLastStanza

(* helper function that returns the "nth" index of a list *)
 let getNth (n: int) (lst: 'a list) : 'a list =
  take 1 (drop n lst)

(* helper function that removes one layer of lists. Only used in cases where we have 
a layer of lists that only has one list element in it *)
let deNestList (lst: 'a list list) : 'a list = match lst with
  | [] -> []
  | x::[] -> x
  | _ -> failwith "should not denest this list!"

(* helper function that goes through 2 lists and returns true if all elements are the same *)
let listEqls (lst1: 'a list) (lst2: 'b list) : bool =
  if (length lst1 = length lst2)
  then
  let (boolval, count) =List.fold_left (fun (truthval,counter) b ->
                  (truthval && (b::[] = getNth counter lst2), counter +1)) (true, 0) lst1
      in boolval
  else false

(* helper function that returns the lines of a specified stanza *)
 let bystanza (stanza: int) (lst: 'a list) : 'a list =
  match stanza with
  | 1 -> (take 6 lst)
  | 2 -> take 6 (drop 6 lst)
  | 3 -> take 6 (drop 12 lst)
  | 4 -> (drop 18 lst)
  | _ -> failwith "Invalid stanza number"

(* helper function that performs the line checks for the first 3 stanzas through nested if statements *)
let checkingStanza (linecounter: int) (lst: line list) : (int*int) list =
  if ((listEqls (getNth 0 lst) (getNth 1 lst)) = false)   
  then if ((listEqls (getNth 2 lst)(getNth 3 lst)) = false)
       then [(linecounter +1,linecounter +2);(linecounter +3,linecounter +4)]
       else [(linecounter +1,linecounter +2)]
  else if ((listEqls (getNth 2 lst) (getNth 3 lst)) = false)
       then [(linecounter +3,linecounter +4)]
       else if (listEqls (List.sort (compare) (deNestList (getNth 0 lst) @ deNestList(getNth 2 lst))) 
                         (List.sort (compare) (deNestList (getNth 4 lst)@ deNestList(getNth 5 lst))) 
                         = false)   
          then [(linecounter +5,linecounter +6)]
          else []

(* helper function that does the sorting and equality of the last stanza *)
let lastStanzaHelper (lineList: 'a list list) : bool = 
  let sorted123 = List.sort (compare) 
                  (dedup ((deNestList(getNth 4 lineList))
                  @ (deNestList(getNth 5 lineList))
                  @ (deNestList(getNth 10 lineList))
                  @ (deNestList(getNth 11 lineList))
                  @ (deNestList (getNth 16 lineList)) 
                  @ (deNestList(getNth 17 lineList))))
  in
  let sorted4 = List.sort (compare) 
                (dedup ((deNestList(getNth 18 lineList)) 
                @ (deNestList(getNth 19 lineList))
                @ (deNestList(getNth 20 lineList))
                @ (deNestList(getNth 21 lineList))
                @ (deNestList(getNth 22 lineList))
                @ (deNestList(getNth 23 lineList))))
  in
  listEqls sorted123 sorted4

(* master paradelle function. Note, the name is paradelle to match the feedback file,
as opposed to isParadelle*)
 let paradelle (filename: string) : result =
   let optnCharList = read_file filename
   in
   match optnCharList with
   | None -> FileNotFound(filename)
   | Some lst -> let lineList = convert_to_non_blank_lines_of_words lst
   in
   if (((length lineList)= 24) = false) then IncorrectNumLines(length lineList)
   else
    let wrongLines = checkingStanza (0) (bystanza 1 lineList) @
                     checkingStanza (6) (bystanza 2 lineList) @ 
                     checkingStanza (12) (bystanza 3 lineList)
    in
    let stanza4Check =
      if (wrongLines != [])
      then IncorrectLines(wrongLines)
      else 
        if lastStanzaHelper lineList = false
        then IncorrectLastStanza
        else OK
    in 
    stanza4Check
