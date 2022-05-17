import os
import random
import re

amino = ['A','C','T','G']
wordsE = {}
pat_fileD = open("patternsDNA.txt", "w")
pat_fileE = open("patternsENG.txt", "w")
txtE = open("english.txt", "r")

# pat_size =  2 4 8 16 32 64 128 256 512 1024
# num_per_size = 10

for i in range(100):
    sz = random.randint(1,50)
    for j in range(0,10):
        patD = ""
        for k in range(0,sz):
            patD += amino[random.randint(0,4)%4]
        pat_fileD.write(patD+"\n")
for line in txtE:
    for word in line.split():
        word = re.findall('[A-Za-z]+', word)
        if word:
            word = word[0]
            if word in wordsE:
                wordsE[word] += 1
            else:
                wordsE[word] = 1
freq_list = sorted(wordsE.items(), key=lambda x: x[1], reverse=True)
word_list = sorted([x[0] for x in freq_list[:1000]])
for word in word_list:
    pat_fileE.write(word + "\n")

pat_fileE.close()
pat_fileD.close()
txtE.close()
