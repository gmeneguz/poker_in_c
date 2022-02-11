# Poker in C

## ![Alt Poker in C](sample.png?raw=true "Poker in C")

It was the final project for my C Language course in College (late 2010).

Many nights without sleep trying to understand how pointers and matrixes work. It was fun, I learned A LOT and it was a great experience. There are probably many bugs, but I will try to write the general idea from what I remember.

It was all writen and documented in Portuguese, since that time all my effort was about learning to program.

## Basic Gameplay

It is a Texas Holden Poker for 2 players in a SINGLE COMPUTER.

The game shows information about which player turn it currently is and then the other player should turn his face while the player is playing (Yeh, I planned to do multiplayer via sockets, but it didn't pan out at the time).

In his turn, the player normally have 3 options:

1. Check / Call (pay)
2. Raise
3. Fold

The player press the number of the action and extra info if requested (raise).

Game runs directly on Prompt / Terminal interface printing and repainting the screen.

## Limitations

- It runs only on Windows :´( but there are almost no dependencies.

- The size of the Terminal Window have to be resized after run the game. I didn't find a solution at the time.

- It probably has some (if not many) bugs, the objective was to learn and enjoy.

## Basic logic

#### Deck

- Each card range from 102 to 414.
- The first number (get value from / 100) is related to **suit**
- The last two numbers (get value from % 100) are related to **card value**

For instance, 405 means 5 of Spades. Card value of 14 means Ace, so 414 means Ace of Spades.

```c
temp_naipex = cartas_t[cont_exib] / 100;
temp_numex = cartas_t[cont_exib] % 100;
```

#### Winner Calculation

Each result possibility steps by thousand.(1xxx , 2xxx up to 10xxx as I remember)

Example:

2005 - Two of a kind (two fives in hand)

3050 - Two pairs (I dont remember the logic to sum the pairs but the higher has more weight certainly)

4007 - Three of a kind (three sevens in hand)

5010 - Sequence, as ten higher

....

So on.
