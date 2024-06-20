% rules from decision.txt which is a text file of decision.py results
classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength =< 2.45,
    writeln('Iris-setosa'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth =< 1.75,
    PetalLength =< 4.95,
    PetalWidth =< 1.65,
    writeln('Iris-versicolor'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth =< 1.75,
    PetalLength =< 4.95,
    PetalWidth > 1.65,
    writeln('Iris-virginica'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth =< 1.75,
    PetalLength > 4.95,
    PetalWidth =< 1.55,
    writeln('Iris-virginica'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth =< 1.75,
    PetalLength > 4.95,
    PetalWidth > 1.55,
    SepalLength =< 6.95,
    writeln('Iris-versicolor'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth =< 1.75,
    PetalLength > 4.95,
    PetalWidth > 1.55,
    SepalLength > 6.95,
    writeln('Iris-virginica'),
    !.
classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth > 1.75,
    PetalLength =< 4.85,
    SepalWidth =< 3.10,
    writeln('Iris-virginica'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth > 1.75,
    PetalLength =< 4.85,
    SepalWidth > 3.10,
    writeln('Iris-versicolor'),
    !.

classify(SepalLength, SepalWidth, PetalLength, PetalWidth) :-
    PetalLength > 2.45,
    PetalWidth > 1.75,
    PetalLength > 4.85,
    writeln('Iris-virginica'),
    !.