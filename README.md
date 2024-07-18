main.c, database.c and database.h are a mini-library that acts as an in-memory database. Features:
-Use dynamic memory to store records.

-IndexNodes are nodes in an ordered binary tree used to find references to various records.

-Implement an insertion function that updates index data structures.

-Implement search functions (which navigate the appropriate IndexNode to reach the record).


-------------------------------------------------------------------------------------------------------------------


greenis.c is a reimagining of Redis with the sole ability to set and get keys, compatible with the Redis protocol. The mini Redis server allows for setting/getting keys with or without expiration time. Protocol informations are in PROTOCOL.md Additional resources can be found here:
https://github.com/SENSES-Lab-Sapienza/Lab_Reti/tree/aa_2023_24/Resources/greenis
