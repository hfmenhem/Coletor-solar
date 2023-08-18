from scipy.special import lambertw
from scipy.optimize import curve_fit
import numpy as np
import csv
import os
import math
import matplotlib.pyplot as plt

def func(x,Iph,Is, k):
    return Iph + (Is*(np.exp(x/k)-1))

print("insira nome do arquivo")
nome = input()
print("nome do arquivo: " + nome)

if not os.path.exists(nome):
    os.mkdir(nome)
os.chdir(nome)
if not os.path.exists("experimental"):
    os.mkdir("experimental")
if not os.path.exists("controle"):
    os.mkdir("controle")
os.chdir("..")

print (os.getcwd())


#Determina número de repetições 
file = open(nome + '.csv')
csvreader = csv.reader(file)
next(csvreader) # "apaga" o header
linha = 0
for row in csvreader:
    linha+=1
linha = math.floor(linha/514)


#Lê as linhas do grupo controle ou experimental

for j in range (0,2):
    file = open(nome + '.csv')
    csvreader = csv.reader(file)
    os.chdir(nome)
    if j == 0:
        os.chdir("controle")
    if j == 1:
        os.chdir("experimental")

    next(csvreader)
    row = []
    for n in range(0, linha):
        row = next(csvreader)
        V = []
        I = []
        T = row[6]

        I.append(float(row[0+(j*3)]))                 #desloca o valor de leitura dependnedo se for exprimental ou controle
        V.append(float(row[1+(j*3)]))                 #desloca o valor de leitura dependnedo se for exprimental ou controle

        for i in range(1, 512):
            row = next(csvreader)
            I.append(float(row[0+(j*3)]))             #desloca o valor de leitura dependnedo se for exprimental ou controle
            V.append(float(row[1+(j*3)]))             #desloca o valor de leitura dependnedo se for exprimental ou controle

        next(csvreader)
        next(csvreader)

        #calcula coeficientes
        try:
            popt, pcov = curve_fit(func, V, I, bounds = ((0,-np.inf, 0),(np.inf, 0, np.inf)))
            Iph = popt[0]
            Is = popt[1]
            k = popt[2]
            
        except RuntimeError:
            Iph = math.nan
            Is = math.nan
            k = math.nan
        
        #Calcula R^2
        ss_res = np.sum((I- func(V, *popt))**2)
        ss_tot = np.sum((I-np.mean(I))**2)
        R2 = 1 - (ss_res / ss_tot)

        #Calcula Vpmax
        Vpmax = k * (lambertw(math.e * (Is-Iph)/Is)-1)
        
        #Calcula Pmax
        Pmax = Vpmax*func(Vpmax, Iph, Is, k)

        if Vpmax.imag != 0 or Pmax.imag != 0:
            Vpmax = math.nan
            Pmax = math.nan
        else:
            Vpmax = Vpmax.real
            Pmax = Pmax.real

        #Salvar dados em arquivo
        
        dados = [] #modifica a formatação dos dados de I e V para ficar cada par ordenado em um item da lista
        for i in range(0, len(V)):
            dados.append([V[i], I[i]])
        with open(str(n) + ".csv", 'w', newline="") as fileWrite:
            csvwriter = csv.writer(fileWrite) # 2. cria o arquivo
            csvwriter.writerow(["Iph", "Is", "k", "R2", "t[ms]", "Vpmax", "Pmax"])
            csvwriter.writerow([*popt, R2, T, Vpmax, Pmax])
            csvwriter.writerow(['V[V]', 'I[ma]'])
            csvwriter.writerows(dados)
    file.close()
    os.chdir("..")
    os.chdir("..")

os.replace(nome +".csv", nome + "\\" + nome +".csv")
    

