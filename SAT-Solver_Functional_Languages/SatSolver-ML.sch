
# Sat Solver Implementation - http://en.wikipedia.org/wiki/Boolean_satisfiability_problem

fun getUnitLit [] = 0
  | getUnitLit (h::t) = 
    if (length h = 1) then (hd h) 
    else (getUnitLit t);


fun unitProp literalToDelete [] = []
  | unitProp literalToDelete (h::t) = let 
    val hexist : bool = (mem literalToDelete h)
    val neg : int = ~literalToDelete
    val hcomplementexist : bool = (mem neg h)
    val withDeleted : int list = (del neg h)
    in
      if (hexist) then (unitProp literalToDelete t)
      else if (hcomplementexist) then (withDeleted::(unitProp literalToDelete t))
      else (h::(unitProp literalToDelete t))
    end;




fun checkifNeg n = 
    if (n < 0) then true 
    else false;


fun minus l [] = l
  | minus l (x::xs) = del x (minus l xs);





fun negateList [] = []
| negateList (h::t) = 
let val t' = negateList t
in
  (~h::t')
end;



fun getPosNeg [] = [[],[]]
 | getPosNeg (h::t) =
 let
 val negatives : int list = (filter checkifNeg h)
 val positives : int list = (minus h negatives)
 val getBothLists : int list list = (getPosNeg t)
 val positivesList : int list = (hd getBothLists)
 val negativesList : int list = (hd (tl getBothLists))
 val negateNegativesList : int list = negateList negatives
 in   
   [(union positives positivesList),(union negateNegativesList negativesList)]
 end;




fun getUniques [] [] = [[],[]]
| getUniques list1 list2 = 
let
  val common : int list = intersect list1 list2
  val UniqueList1 : int list = minus list1 common
  val UniqueList2 : int list = minus list2 common
  val UniqueList3 : int list = (negateList UniqueList2);
in
 [UniqueList1,UniqueList3]
end;


fun removeEmptyClauses [] = []
| removeEmptyClauses (h::t) = let
  val remaining : int list list = (removeEmptyClauses t)
  in
  if(null h) then remaining else (h::remaining)
  end;





fun getPureLit [] = 0
| getPureLit mylist =
let 
  val PosNegatives :int list list = (getPosNeg mylist)
  val Positives : int list = (hd PosNegatives)
  val Negatives : int list = (hd (tl PosNegatives))
  val UniqueLists : int list list = (getUniques Positives Negatives)
  val removedEmpty : int list list = (removeEmptyClauses UniqueLists)
in
  if (null removedEmpty) then 0    
  else if (null (hd removedEmpty)) then (hd (hd (tl removedEmpty)))
  else (hd (hd removedEmpty))    
end;





fun pureLitRule pureLit [] = []
| pureLitRule pureLit (h::t) =
let
  val getLists = (pureLitRule pureLit t)
in
  if (mem pureLit h) then getLists else (h::getLists)
end;


fun checkIfPositiveNegativeInSameClause [] = false
| checkIfPositiveNegativeInSameClause (h::t) =
let
   val neg : int = ~h;
   val remaining : bool = (checkIfPositiveNegativeInSameClause t);
in
   if (mem neg t) then true else remaining 
end;


fun getAListofClausesForPropositionalVariable literal [] = []
| getAListofClausesForPropositionalVariable literal (h::t) =
let
  val neg : int = ~literal
  val remaining : int list list = (getAListofClausesForPropositionalVariable literal t)
in
  if (mem neg h) then (h::remaining) else remaining
end;


fun removeNegatives literal [] = []
| removeNegatives literal (h::t) = 
let 
  val neg : int = ~literal
  val remaining : int list list = (removeNegatives literal t)
  val removed: int list = (del neg h)
in
	  (removed::remaining)
end;
  

fun formPairs source [] = []
| formPairs source (h::t) =
let 
  val combination : int list = source @ h
  val remaining : int list list = (formPairs source t)
in
  (combination::remaining)
end;


fun processPairs [] = []
| processPairs (h::t) =
let
  val newpair : bool = (checkIfPositiveNegativeInSameClause h)
  val newh : int list = (setify h)
  val remaining : int list list = (processPairs t)
in
  if (newpair) then
     remaining
  else
     (newh::remaining)
end;



fun mapper destination []= []
| mapper destination (h::t) =
let
  val pair : int list list = (formPairs h destination)
  val remaining : int list list list = (mapper destination t)
  val entireList : int list list list = (pair::remaining)
  val flattenremaining : int list list = (flatten entireList)
in
  entireList
end;







fun resRule [] = []
| resRule (h::t) =
let
  val propositionalVariable : int = (hd h);
  val listOfClausesNegatives : int list list = (getAListofClausesForPropositionalVariable   propositionalVariable t)
  
  val neg : int = ~propositionalVariable;
  val listOfClausesPositives : int list list = (getAListofClausesForPropositionalVariable neg t)

  val removedNegatives : int list list = (removeNegatives propositionalVariable listOfClausesNegatives)
  val removedPositives : int list list = (removeNegatives neg listOfClausesPositives)

  val propositionalVariablePartner : int list = (tl h)
  val allPositives : int list list = (propositionalVariablePartner::removedPositives)

  val pairs : int list list list = (mapper removedNegatives allPositives)
  val flattenedPairs : int list list = (flatten pairs)

  val newpairs : int list list = (processPairs flattenedPairs)
  val replace : int list list = (minus t listOfClausesNegatives)  
  val replace1 : int list list = (minus replace listOfClausesPositives)

  val newList : int list list = (union replace1 newpairs)
  val removedEmpty : int list list = (removeEmptyClauses newList)
in
  removedEmpty
end;


fun getNextClause [] = ([],[])
  | getNextClause (0::lits) = ([], lits)
  | getNextClause (lit::lits) =
    let val (currentClause, rest) = getNextClause lits
    in
        (lit::currentClause, rest)
    end;


fun getClauses [] = []
  | getClauses lits =
    let val (currentClause, rest) = getNextClause lits
    in
        currentClause::(getClauses rest)
    end;



fun checkifContainsaNullClause [] = false
 | checkifContainsaNullClause (h::t) =
let
  val remaining = (checkifContainsaNullClause t)
in
  if (null h) then (true orelse remaining) else (false orelse remaining)
end;



fun MainFun [] = true
| MainFun list = let
val ifNullClause : bool = (checkifContainsaNullClause list)
val unitLiteralExist : int = (getUnitLit list)
in
if (ifNullClause = true ) then false
else if (not (unitLiteralExist = 0)) then (MainFun (unitProp unitLiteralExist list))
else if (not ((getPureLit list) = 0)) then (MainFun (pureLitRule (getPureLit list) list))
else (MainFun (resRule list))
end;

fun dp list = (MainFun (getClauses list))
 
 

