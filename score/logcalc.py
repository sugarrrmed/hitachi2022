
import math
sum = 0
cnt = 0
with open('error.txt') as f:
    for line in f:
        if(line[:9] == "ret_score"):
            #print(line)
            sum += math.log10(int(line[11:]))
            cnt += 1

mean = sum/cnt
print("mean {},\nsum {} cnt {}".format(mean,sum,cnt))
