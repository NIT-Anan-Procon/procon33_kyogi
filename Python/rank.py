import re

file1 = open('hehehe.wavE.txt', 'r')
count = 0
pattern = '(?<= \/ ).*'
  
array = []

# Using for loop
for line in file1:
    result = re.findall(pattern, line.strip())
    if result:
      array.append(float(result[0]))
      count += 1
      

ranks = array.copy()
ranks.sort()

count = 1
for num in array:
     rank = 44
     for score in ranks:
        if num == score:
            print("Card number {}: {}".format(count, rank))
        rank -= 1
     count += 1
  
# Closing files
file1.close()