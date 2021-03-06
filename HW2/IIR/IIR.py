import matplotlib.pyplot as plt
import numpy as np
import csv

t = [] # column 0
data = [] # column 1
with open('sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data.append(float(row[1])) # second column

i=1
A=0.8
newdata=[]
newdata.append(data[0])
while i<len(t):
    newdata.append(A*newdata[i-1]+(1-A)*data[i])
    i=i+1



fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,data,'k')
ax1.plot(t,newdata,'r')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')


Fs = len(t)/(t[-1]-t[0]) #sampling rate
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

ax2.loglog(frq,abs(Y),'k') # plotting the fft

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

ax2.loglog(frq,abs(Y),'r') # plotting the fft

ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
title="Weight A = %.2f  B = %.2f" % (A, (1-A))
ax1.set_title(title)

plt.show()

