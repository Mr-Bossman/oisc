15 0 21 21 # get neg pc
15 13 21 # -pc - ptr to function
18 0 16 21 # sp, zero, jump to, ptr to zero
15 0 21 21 # get neg pc
15 13 21 # -pc - ptr to function
18 0 16 21 # sp, zero, jump to, ptr to zero
0
0
0
0
0
0
-24# ptr to function: (size of jump code)- abs func
0 # not used
16 # tmp
0 #tmp
4 # sub by 4 # size of return code
19 #ptr to next
0 # sp
21 # ptr to next
0 # const 0
0 # not used
0 # not used
45 # ptr to console
-66 # letter
0 # not used
24 25 21 # print
15 0 21 21 #r16 = 0-pc
15 17 21# r16 -=4
15 19 21# r16 -=19
18 19 21 # clear sp
20 21 16# jump back
0 # EOF