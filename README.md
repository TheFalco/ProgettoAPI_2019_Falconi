# Social Network simulator
*This project has been developed as part of the "Algoritmi e Principi dell'Informatica" course at [Politecnico di Milano](https://www.polimi.it/).*

A C program that monitor relations between elements.

## Input Format
The program expects its input from stdio with the following format.

_**NOTE**: Entities' and Relations' name cannot have whitespace characters in them_.
#### Addition of an Entity
To add an Entity that will be monitored, it is needed to write it between quotation marks and after ```addrel``` keyword.
#### Addition of a Relation
This simulator allows entities to have monodirectional Relations. 
To add a Relation between two Elements, it is needed to use the following format:
```
addrel "<giver-entity>" "<receiver-entity>" "<relation-ID>"
```
The Relation will be created only if both Entities are being monitored.
#### Deletion of Relation
Similarly to the addition of a Relation, to delete a Relation it is needed to use the following format:
```
delrel "<giver-entity>" "<receiver-entity>" "<relation-ID>"
```
#### Deletion of an Entity
In order to stop monitoring an Entity and to delete all of its occurrences in the Relations that are being monitored, it is needed to write the name of the Entity between quotation marks and after ```delent``` keyword.
#### Creation of an Output
The ```report``` keyword will generate on stdout the list of the Relations that are being monitored, including for each Relation the entity/entities with the largest number of received relation, followed by the number of them; alphabetically ordered.
The output follows this format:
 ```
"<relation-ID>" "<max-receiver-entity>" "<max-receiver-entity>" <max-number-of-givers>;
 ```
If no Relation is being monitored, the simulator will write in the stdout  ```none```.
#### End of the simulation
The ```end``` keyword will terminate the simulation.

***Example of the input stream:***
 ```
 addent "Luke_Skywalker"
 addent "Leia_Organa"
 report
 addrel "Leia_Organa" "Luke_Skywalker" "is_brother_of"
 addrel "Darth_Vader" "Luke_Skywalker" "is_father_of"
 report
 addrel "Luke_Skywalker" "Leila_Organa" "is_loved_by"
 report
 delent "Luke_Skywalker"
 report
 end
 ```

***Example of the output stream:***
 ```
 none
 "is_brother_of" "Luke_Skywalker" 1;
 "is_loved_by" "Leila_Organa" 1; "is_brother_of" "Luke_Skywalker" 1;
 none
 ```
 
 ## Compiling
 Run  ```make```.
