A game of Boogle, where a board of n x n size is filled with characters, and the player must try to guess words by connecting adjacent characters in the board
An adjacent character is considered to be any character on top, on bottom, to the side, or directly diagonal to another character

<link rel="stylesheet" href="https://badge.coolcodersj.repl.co/style.css">
<a class="badge" href="https://www.replit.com/cobyj33/Boogle" target="_blank">
<img src="https://upload.wikimedia.org/wikipedia/commons/b/b2/Repl.it_logo.svg" alt="replit" />Play cobyj33/Boogle now on Replit</a>

For Example, in this generated 5 x 5 size board by Boogle:
```
E X K U J 
A W R L B 
T D S P I 
N O F Y H 
G Q C V Z
```
Some possible words are SONG, SWEAT, DOG, SWAT, COD, and EAT

|                   |                   |               |                   |               |               |     
|       SONG        |        SWEAT      |      DOG      |        SWAT       |      COD      |      EAT      |
|   -----------     |     -----------   |  -----------  |     -----------   |  -----------  |  -----------  |
| E X K U J         | **E** X K U J     | E X K U J     | E X K U J         | E X K U J     | **E** X K U J | 
| A W R L B         | **A** **W** R L B | A W R L B     | **A** **W** R L B | A W R L B     | **A** W R L B |
| T D **S** P I     | **T** D **S** P I | T **D** S P I | **T** D **S** P I | T **D** S P I | **T** D S P I |
| **N** **O** F Y H | N O F Y H         | N **O** F Y H | N O F Y H         | N **O** F Y H | N O F Y H     |
| **G** Q C V Z     | G Q C V Z         | **G** Q C V Z | G Q C V Z         | G Q **C** V Z | G Q C V Z     |

This project includes a solver for a board, a full dictionary of words, and is fully playable in the terminal

To compile the project, simply use the terminal command ```g++ main.cpp -I ./ -o main``` from the root directory of the downloaded project with g++ installed

This was the first real C++ project I ever made so I let it live here to look back upon :)
