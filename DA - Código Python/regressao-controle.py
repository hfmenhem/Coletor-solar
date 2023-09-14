from scipy.special import lambertw
from scipy.optimize import curve_fit
import numpy as np
import csv
import os
import math
import matplotlib.pyplot as plt

print("insira ano do experimento")
ano = str(2023)
print("ano do experimento: " + ano)
ano = float(ano)

print("insira mês do experimento")
mes = str(9)
print("mês do experimento: " + mes)
mes = float(mes)

print("insira dia do experimento")
dia = str(8)
print("dia do experimento: " + dia)
dia = float(dia)

print("insira longitude do experimento")
lon = str(44.896)
print("longitude do experimento: " + lon)
lon = (45-float(lon))*math.pi/180


print("insira latitude do experimento")
lat = str(44.124)
print("longitude do latitude: " + lat)
lat = float(lat)*math.pi/180


print("insira nome do experimento")
nome = input()
print("nome do experimento: " + nome)

print("insira limite de R^2")
Rdef = input()
print("limite de R^2: " + Rdef)
Rdef = float(Rdef)
Rdef = str(Rdef)

print("insira limite inferior de hora do intervalo de regressão")
Lmin = input()
print("limite inferior: " + Lmin)
Lmin = float(Lmin)

print("iinsira limite superior de hora do intervalo de regressão")
Lmax = input()
print("limite superior: " + Lmax)
Lmax = float(Lmax)

os.chdir(nome)

print (os.getcwd())

#Determina número de repetições 
file = open(Rdef + "controle.csv")
csvreader = csv.reader(file)
next(csvreader) # "apaga" o header

T=[]
P=[]
for row in csvreader:
    row = next(csvreader)
    if(float(row[0])<Lmax and float(row[0])>Lmin):
        T.append(float(row[0]))
        P.append(float(row[1]))   
    
file.close()


diaTemp = dia + 30.42 * (mes - 11) + 365.25 * (ano - 2022)
tDia = round(diaTemp)
# equação do tempo e declinação
L = 3.838 + (0.0172 * tDia)
M = 5.175 + (0.0172 * tDia)
eqT = (0.04302 * math.sin(2 * L)) - (0.0334 * math.sin(M))
dec = math.asin(0.39773 * math.sin(L))
def func(hora, k):
    tRad = np.divide (hora, 3.8197)
    #ângulo horário
    AH = lon + eqT + tRad - math.pi #em radiano
    hsin = (np.cos(lat) * np.cos(dec) * np.cos(AH)) - (np.sin(lat) * np.sin(dec))

    return (hsin*k) #k = constante solar * área do painel


popt, pcov = curve_fit(func, T, P, bounds = ((0),(np.inf)))
k = popt[0]

#Calcula R^2
ss_res = np.sum((P- func(T, *popt))**2)
ss_tot = np.sum((P-np.mean(T))**2)
R2 = 1 - (ss_res / ss_tot)

#Calcula sigma_k
perr = np.sqrt(np.diag(pcov))
sigma = perr[0]

#Calcula constrantes da função resumidamente
A=math.cos(lat) * math.cos(dec)
B=-1*math.sin(lat) * math.sin(dec)
C=lon + eqT - math.pi
D = 1/3.8197
#y = k((A * np.cos(C + (x*D))) - B)

#Salva dados
with open(Rdef + "regressao-controle.csv", 'w', newline="") as fileWrite:
    csvwriter = csv.writer(fileWrite) # 2. cria o arquivo
    csvwriter.writerow(["k = (A*F)" , k])
    csvwriter.writerow(["Sigma_k" , sigma])
    csvwriter.writerow(["R^2" , R2])
    csvwriter.writerow(["y = k(B + A*cos(C + D*x))"])
    csvwriter.writerow(["A" , A])
    csvwriter.writerow(["B" , B])
    csvwriter.writerow(["C" , C])
    csvwriter.writerow(["D" , D])









