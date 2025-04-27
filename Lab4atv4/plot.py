#!/usr/bin/env python3

import pandas as pd #importando lib pandas
import matplotlib.pyplot as plt #importando matplotlib
numThreads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10] #numero de threads
df = pd.read_csv('tempos.csv') #leitura do csv
bar = False #mudar para True caso queira plot com barras 

#separa o dataframe em dois(N = 1000 e N = 1000000)
df3 = df[df['N'] <= 1000] 
df6 = df[df['N'] > 1000]

#remove a coluna N
df3 = df3.drop(columns=['N'])
df6 = df6.drop(columns=['N'])
#media dos tesmpos com base no numero de threadss
avgTempo3 = df3.groupby('numThreads')['tempo'].mean()
avgTempo6 = df6.groupby('numThreads')['tempo'].mean()
acel3 = []
acel6 = []
ef3 = []
ef6 = []
ind = 0
#calculo da aceleracao e da eficiencia
for i in numThreads:
  acel3.append(avgTempo3[1]/avgTempo3[i])
  ef3.append(acel3[ind]/i)
  acel6.append(avgTempo6[1]/avgTempo6[i])
  ef6.append(acel6[ind]/i)
  ind += 1


fig, axes = plt.subplots(1, 2, figsize=(16, 6))
#plot tempo medio 1000
if bar:
  axes[0].bar(numThreads, avgTempo3, label='Tempo Médio (N = 10^3)', color='skyblue')
else:
  axes[0].plot(numThreads, avgTempo3, label='Tempo Médio (N = 10^3)', marker='o', color='skyblue')
axes[0].set_xlabel('Número de Threads')
axes[0].set_ylabel('Tempo Médio (ms)')
axes[0].set_title('Tempo Médio (N = 10^3)')

#plot tempo medio 1000000
if bar:
  axes[1].bar(numThreads, avgTempo6, label='Tempo Médio (N = 10^6)', color='lightcoral')
else:
  axes[1].plot(numThreads, avgTempo6, label='Tempo Médio (N = 10^6)', marker='o', color='lightcoral')
axes[1].set_xlabel('Número de Threads')
axes[1].set_ylabel('Tempo Médio (ms)')
axes[1].set_title('Tempo Médio (N = 10^6)')

plt.show()

fig, axes = plt.subplots(1, 2, figsize=(16, 6))
#plot aceleracao 1000
if bar:
  axes[0].bar(numThreads, acel3, label='Aceleração (N = 10^3)', color='skyblue')
else:
  axes[0].plot(numThreads, acel3, label='Aceleração (N = 10^3)', marker='o', color='skyblue')
axes[0].set_xlabel('Número de Threads')
axes[0].set_ylabel('Aceleração')
axes[0].set_title('Aceleração (N = 10^3)')

#plot aceleracao 1000000
if bar:
  axes[1].bar(numThreads, acel6, label='Aceleração (N = 10^6)', color='lightcoral')
else:
  axes[1].plot(numThreads, acel6, label='Aceleração (N = 10^6)', marker='o', color='lightcoral')
axes[1].set_xlabel('Número de Threads')
axes[1].set_ylabel('Aceleração')
axes[1].set_title('Aceleração (N = 10^6)')

plt.show()

fig, axes = plt.subplots(1, 2, figsize=(16, 6))
#plot eficiencia 1000
if bar:
  axes[0].bar(numThreads, ef3, label='Eficiência (N = 10^3)', color='skyblue')
else:
  axes[0].plot(numThreads, ef3, label='Eficiência (N = 10^3)', marker='o', color='skyblue')
axes[0].set_xlabel('Número de Threads')
axes[0].set_ylabel('Eficiência')
axes[0].set_title('Eficiência (N = 10^3)')
#plot eficiencia 1000000
if bar:
  axes[1].bar(numThreads, ef6, label='Eficiência (N = 10^6)', color='lightcoral')
else:
  axes[1].plot(numThreads, ef6, label='Eficiência (N = 10^6)', marker='o', color='lightcoral')
axes[1].set_xlabel('Número de Threads')
axes[1].set_ylabel('Eficiência')
axes[1].set_title('Eficiência (N = 10^6)')

plt.show()


