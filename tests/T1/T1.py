import os

ipmt = "~/Documentos/projeto-pcc/ipmt/ipmt-IF767/bin/ipmt"

def average_time():
    temp_folder = "./averages/"
    text_files = os.listdir(temp_folder)
    total_time = 0
    for text_file in text_files:
        f = open(temp_folder + text_file, 'r')
        for line in f:
            s = line.split()
            if(len(s) > 0 and s[0] == 'real'):
                minuto = ''
                pont = 0
                while s[1][pont] != 'm':
                    minuto += s[1][pont]
                    pont += 1
                segundo = ''
                pont = pont + 1
                while s[1][pont] != 's':
                    segundo += s[1][pont]
                    if segundo[-1] == ',':
                        segundo = segundo[:-1]
                        segundo += '.'
                    pont += 1
                total_time += float(minuto) * 60 + float(segundo)
                break
    os.system("rm {}*".format(temp_folder))
    average_time = total_time / len(text_files)
    return average_time
 
def get_average(texto, folder, tipo, opcoes,last):
    for i in range(10 if last else 3):
        arquivo = folder + texto
        comando = "{} {} {} {}".format(ipmt,tipo,opcoes,arquivo)
        time_comando = "{ time " + comando + " ;}" + " 2>./averages/{}T1{}".format(i,texto)
        os.system("/bin/bash -c '{}'".format(time_comando))
    t = average_time()
    file = open("./results/{}/T1{}{}".format("dna" if folder == dna_folder else "english","_save_" if opcoes == '-s' else "_nosave_",texto), 'w')
    file.write(str(t))
    file.close()
        

dna_folder = "~/Documentos/projeto-pcc/ipmt/ipmt-IF767/tests/files/dna/"
eng_folder = "~/Documentos/projeto-pcc/ipmt/ipmt-IF767/tests/files/english/"
tipo = "index"
textos_dna = ["dna4KB.txt","dna45KB.txt","dna450KB.txt","dna4500KB.txt","dna45000KB.txt"]
textos_eng = ["english4KB.txt","english45KB.txt","english450KB.txt","english4500KB.txt","english45000KB.txt"]

for texto in textos_dna:
    get_average(texto, dna_folder, tipo, "",False if texto == textos_dna[-1] else True)
for texto in textos_dna:
    get_average(texto, dna_folder, tipo, "-s",False if texto == textos_dna[-1] else True)
for texto in textos_dna:
    get_average(texto, dna_folder, tipo, "-s -a sort",False if texto == textos_dna[-1] else True)
for texto in textos_eng:
    get_average(texto, eng_folder, tipo, "",False if texto == textos_eng[-1] else True)
for texto in textos_eng:
    get_average(texto, eng_folder, tipo, "-s",False if texto == textos_eng[-1] else True)


    


