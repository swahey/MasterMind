# MasterMind
Play MasterMind on pc

compile: gcc -Wall -o MasterMind MasterMind.c $(pkg-config gtk+-3.0 --cflags --libs)

Het programma plaatst (onzichtbaar) vier gekleurde pionnen,
de speler tracht deze code te achterhalen door plaatsing van pionnen
die hijzelf kiest uit zes verschillende kleuren.
Programma toond telkens hoeveel juiste pionnen er gekozen zijn,
en ook hoeveel er op de juiste plaats zitten.


# MasterMindV2
toegevoegd op 15-12-2021 , updated op 17-12-2021

compile: gcc -Wall -o MasterMindV2 MasterMindV2.c $(pkg-config gtk+-3.0 --cflags --libs)

Een beetje mooier en gebruiksvriendelijker dan vorige versie. 
Enkel door gebruik van 'cairo' draw functies.
