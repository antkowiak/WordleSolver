WordleSolver - Helps solve the daily Wordle puzzle from https://www.powerlanguage.co.uk/wordle/
<pre>
Usage: ws <options>

    -h or -?      - Display help
    -n <number>   - Number of results to display (default: 20)
    -f <filename> - File containing list of words (default: wordlist_all.txt)
    -z <number>   - Word Size, Number of letters in the words (default: 5)
    -a <letters>  - Specify letters that should be absent from any solutions
    -c <guesses>  - Specify an index and letter pair for correctly positioned letters
    -w <guesses>  - Specify an index and letter pair for wrongly positioned letters
</pre>
---

Written by Ryan Antkowiak 
2022-01-30

---

Note that the <letters> should be one string, not separated by any whitespace. See Example 1.
Note that the <guesses> should be one string, not separated by any whitespace.  Each guess consists of two characters. The first is a zero-based index of the position of the letter.  The second character is the letter.  See Examples 2, 3, and 4.

---

Examples:

---

1) If you know the solution does not contain any of the letters 'C' 'A' 'E' 'S' then you could run:

$ ws -a caes

---

2) If you know the solution contains the letter 'G' which is correctly placed at the fifth position (index = 4)

$ ws -c 4g

---

3) If you know the solution contains the letter 'D' which is correctly placed at the first position (index = 0),
and you know the solution contains the letter 'N' which is correctly placed at the fourth position (index = 3):

$ ws -c 0d3n

---

4) If you know the solution contains the letter 'E', and it is incorrectly placed, but you know it is not at the second position (index = 1)

$ ws -w 1e

---

