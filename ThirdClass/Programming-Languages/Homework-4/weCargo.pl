time(5). % day, hour, minute
% defination of delivery personnels
delivery_personnel(1, 10, [4, 8, 12, 16, 20], none, adminOffice ). % id, capacity, working hours % personnel take it only on these hours, currentDeliveryJob, location
delivery_personnel(2, 10, [5, 9, 13, 17, 21], none, cafeteria ).
delivery_personnel(3, 10, [4, 8, 12, 16, 20], none, instituteY ). 

% places to pick up from and deliver objects to
place(adminOffice). % only unique place names which is id
place(cafeteria).
place(socialSciencesBuilding).
place(library).
place(engineeringBuilding).
place(lectureHallA).
place(instituteY).
place(instituteX).

% objects to be delivered
object(obj1, 8, adminOffice, instituteX, low, 1). % id, weight, source, destination, priority, delivery_personnel
object(obj2, 5, cafeteria, instituteX, medium, 1).
object(obj3, 5, socialSciencesBuilding, instituteY, low, in_transit(2)).
object(obj4, 5, library, instituteX, high, 1).
object(obj5, 5, engineeringBuilding, instituteY, high, 1).

% define routes between places and tim_e to travel between them
route(adminOffice, cafeteria, 4). % source, destination, timeToConsume
route(adminOffice, engineeringBuilding, 3).
route(adminOffice, library, 1).

route(cafeteria, adminOffice, 4).
route(cafeteria, socialSciencesBuilding, 2).
route(cafeteria, library, 5).

route(socialSciencesBuilding, cafeteria, 2).
route(socialSciencesBuilding, instituteX, 8).
route(socialSciencesBuilding, library, 2).

route(instituteX, socialSciencesBuilding, 8).

route(library, adminOffice, 1).
route(library, cafeteria, 5).
route(library, socialSciencesBuilding, 2).
route(library, instituteY, 3).
route(library, engineeringBuilding, 5).

route(engineeringBuilding, adminOffice, 3).
route(engineeringBuilding, library, 5).
route(engineeringBuilding, lectureHallA, 2).

route(lectureHallA, engineeringBuilding, 2).
route(lectureHallA, instituteY, 3).

route(instituteY, library, 3).
route(instituteY, lectureHallA, 3).

% rules to check if a delivery personnel is available

available_person_for_object(ObjectId, PersonId, TotalTime) :-
    % for objects that are in transit
    object(ObjectId, Weight, Source, Destination, _, in_transit(PersonId)),
    find_distance(Source, Destination, TotalTime).

available_person_for_object(ObjectId, PersonId, TotalTimeF) :-
    % set time
    time(Time),

    object(ObjectId, Weight, Source, Destination, _, PersonId),
    % find route from source to destination and update time
    find_distance(Source, Destination, TimeToDestination),

    delivery_personnel(1, Weight_Capacity_First, CONTROL_TIMES_FIRST, CONTROL_FIRST, Location),
    
    find_distance(Location, Source, TimeToSource),

    delivery_personnel(2, Weight_Capacity_Second, CONTROL_TIMES_SECOND, CONTROL_SECOND, LocationS),
    find_distance(LocationF, Source, TimeToSourceS),

    delivery_personnel(3, Weight_Capacity_Third, CONTROL_TIMES_THIRD, CONTROL_THIRD, LocationT),
    find_distance(LocationT, Source, TimeToSourceT),

    TotalTimeF is TimeToSource + TimeToDestination,
    TotalTimeS is TimeToSourceS + TimeToDestination,
    TotalTimeT is TimeToSourceT + TimeToDestination,
    
    (member(Time, CONTROL_TIMES_FIRST)  -> 
         (CONTROL_FIRST = none -> 
            (Weight =< Weight_Capacity_First -> 
               writeln("Person1 : "+ TotalTimeF)
               ; 
               writeln("Person1 Cannot Handle It. It Is Too Heavy" )
            )
            ; 
            writeln("Person1 is not available")
         )
         ; 
         writeln("Person1 is not available at this time.")
    ),


    (member(Time, CONTROL_TIMES_SECOND)  -> 
         (CONTROL_SECOND = none -> 
               (Weight =< Weight_Capacity_Second -> 
                  writeln("Person2 : "+ TotalTimeS)
                  ; 
                  writeln("Person2 Cannot Handle It. It Is Too Heavy" )
               )
            ; 
            writeln("Person2 is not available")
         )
         ; 
         writeln("Person2 is not available at this time.")
    ),
    

    (member(Time, CONTROL_TIMES_THIRD)  ->  
            (CONTROL_THIRD = none -> 
                  (Weight =< Weight_Capacity_Third -> 
                           writeln("Person3 : "+ TotalTimeT)
                           ; 
                           writeln("Person3 Cannot Handle It. It Is Too Heavy" )
                  )
                  ;
                  writeln("Person3 is not available")
            )
            ; 
         writeln("Person3 is not available at this time.")
    ),

    writeln("OBJECT TRANSFER by : ")
    .


% rules to assign a delivery personnel to an object% Define the split_location_distance predicate (assuming it's previously defined)
split_location_distance([], _, _).
split_location_distance([Location-Distance|_], Location, Distance).

% this function helps to find the distance between two places it uses dijkstra algorithm
find_distance(Source, Destination, TimeToExec) :-
    min_dist(Graph, Source, MinDist), 
    member(Location-Distance, MinDist), 
    Location = Destination, 
    TimeToExec = Distance. % Assign the Distance to MinDist

% dijkstra algo  Computes the shortest path from the Start node to the End node.
neighbourhood(Graph, Node, Neighbors) :-
    findall(Adjacent-Distance, route(Node, Adjacent, Distance), Neighbors).

% finds the minimum distance between two nodes
min_dist(Graph, Start, MinDist):-
    dijkstra(Graph,[],[Start-0],MinDist).

 % dijkstra algorithm
 dijkstra(_, MinDist, [], MinDist).

 dijkstra(Graph, Closed, Open, MinDist):-

    choose_v(Open, V-D, ROpen),
    neighbourhood(Graph, V, NB),  % NB is a list of adjacent vertices+distance to V

    diff(NB, Closed, NNB),
    merge(NNB, ROpen, D, NOpen),

    dijkstra(Graph, [V-D|Closed], NOpen, MinDist).
    
 % choice of next vertex to expand
 choose_v([Head|Tail], MinVertex, Rest):-
    choose_minv(Tail, Head, MinVertex, Rest).

 choose_minv([], MinV, MinV, []).

 choose_minv([Head|Tail], Mid, MinV, [Head2|Rest]):-

   Head = V1 - D1, 
   Mid = V - D,

   (D1 < D -> 
      NextM = Head,
      Head2 = Mid
      ; 
      NextM = Mid,
      Head2 = Head
   ),

   choose_minv(Tail, NextM, MinV, Rest).
    
 % diff function (removes vertices already in Closed from NB)
 diff([], _, []).

 diff([Head|Tail], Closed, Leng):-

   Head = V - D,

   (
      member(V-_, Closed) -> 
                           Leng = NewT 
                           ; 
                           Leng = [Head|NewT]
   ),
   diff(Tail, Closed, NewT).
    
 % merge function
 merge([], L, _, L).

 merge([V1 - D1 | Tail] , Open, Dest, NewOpen):-

      (
      remove(Open, V1 - D2, RestOpen)
            -> 
               VD is min(D2, Dest + D1)
               ; 
               RestOpen = Open, VD is Dest + D1
      ),

    NewOpen = [V1 - VD | SubOpen],

    merge(Tail, RestOpen, Dest, SubOpen).
    
 remove([H | T], H, T). % base case for member.d

 remove([H | T], X, [H | NT]):-
    H \= X,
    remove(T, X, NT).