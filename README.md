# Bruteforce Password Cracker
Course project repository for CP-631-OC1 - Advanced Parallel Programming at WLU (Group 3)

Design:

Output: 
The password the user typed, time it took to crack the password.

Input: 
A key of size? (Let's make this loadable via file) 
A password of the user's choice.

Process:
Take user's password
Encrypt it using an algorithm with key to Hash
Identify the number of process available -> p
identify starting point for each process upto p from 0
Distribute Hash, and key to all process
distribute starting point to the processes (including self)

Processes start with starting point with generated password
encrypt password with key -> hashTest
compare hash to hastTest
end when match found

End all processes on the process that found the match
Display message to user 'Password is [password] and it was cracked in x time'
