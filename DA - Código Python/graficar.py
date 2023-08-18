import csv
import os
import math

print("insira nome do arquivo")
nome = input()
print("nome do arquivo: " + nome)

print("insira limite inferior de R^2")
Rlim = input()
print("R^2 >=" + Rlim)
Rlim = float(Rlim)

print("insira o horário de início do experimento:")
print("Hora")
H = input()
print("Hora = " + str(H))
print("Minuto")
min = input()
print("Minuto = " + str(min))

DeltaT = float(H) + (float(min)/60)

#Determina número de repetições 
os.chdir(nome)
file = open(nome + '.csv')
csvreader = csv.reader(file)
next(csvreader) # "apaga" o header
linha = 0
for row in csvreader:
    linha+=1
linha = math.floor(linha/514)

#inicia a busca entre os arquivos de controle e experimental
for j in range (0,2):
    Pmax = []
    T = []
    R = []

    if j == 0:
        os.chdir("controle")
    if j == 1:
        os.chdir("experimental")

    for i in range(0, linha):
        file = open(str(i) + '.csv')
        csvreader = csv.reader(file)
        next(csvreader) #pula o primeiro header
        valores = next(csvreader)
        
        if (float(valores[3]) >= Rlim) and (valores[6] != "nan"):
            T.append(float(valores[4]))
            Pmax.append(float(valores[6]))
            R.append(float(valores[3]))
        file.close()
    
    #converte de ms para hora do relógio :
    k = 0
    while k < (len(T)):
        T[k] = T[k]/3600000
        T[k] += DeltaT
        k +=1

    #integração para determinar energia produzida
    k = 0
    E = 0
    while k < (len(Pmax)-1):
        dt = T[k+ 1] - T[k]
        dE = dt*(Pmax[k+ 1] + Pmax[k])/2
        E += dE
        k +=1
    
    print(E)

    os.chdir("..")

    dados = [] #modifica a formatação dos dados de Pmax, T e R^2 para ficar cada par ordenado em um item da lista
    for i in range(0, len(T)):
        dados.append([T[i], Pmax[i], R[i]])

    if j == 0:
        nomeWrite = str(Rlim) + "controle"
    if j == 1:
        nomeWrite = str(Rlim) +"experimental"

    with open(nomeWrite + ".csv", 'w', newline="") as fileWrite:
        csvwriter = csv.writer(fileWrite) # 2. cria o arquivo
        csvwriter.writerow(["T[h]", "Pmax[mWh]", "R^2", str(E)])
        csvwriter.writerows(dados)

    datwriter = csv.writer(open(nomeWrite + ".dat", 'w', newline=""), delimiter=' ')
    datwriter.writerow(["T[h]", "Pmax[mWh]", "R^2"])
    datwriter.writerows(dados)
    
    

    