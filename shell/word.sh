awk '{for (i=1; i <= NF; i++) sum[]+=1} END { for (i in sum) print i :  sum[i];}'
