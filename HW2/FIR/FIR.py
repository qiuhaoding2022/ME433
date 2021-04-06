import matplotlib.pyplot as plt
import numpy as np
import csv

t = [] # column 0
data = [] # column 1
with open('sigC.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data.append(float(row[1])) # second column
h = [
    0.040387893253289342,
    0.919224213493421316,
    0.040387893253289342,
] #Gaussian weight with samples # = X

X=len(h) # number of data points sampled
newdata=[0]*(X-1)
i=0
while i+X<=len(t):
    avglist=data[i:i+X]
    products=[cofh*data for cofh,data in zip(h,avglist)]
    new=sum(products)
    newdata.append(new)
    i=i+1


Fs = len(t)/(t[-1]-t[0]) #sampling rate
print(Fs)
Ts = 1.0/Fs # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = data # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

plt.loglog(frq,abs(Y),'k') # plotting the fft

Fs = len(t)/(t[-1]-t[0]) #sampling rate
Ts = 1.0/Fs # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = newdata # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

plt.loglog(frq,abs(Y),'r') # plotting the fft


plt.xlabel('Freq (Hz)')
plt.ylabel('|Y(freq)|')
title="FIR sample number = %d , Gaussian weighting " % X
plt.title(title)

plt.show()




