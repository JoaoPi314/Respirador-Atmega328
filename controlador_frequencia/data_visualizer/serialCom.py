import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from PIL import Image



serial_port = serial.Serial('/dev/pts/2', 9600, timeout=1)

fig = plt.figure()

fig.suptitle('Dados do paciente', fontsize=16)
ax1 = fig.add_subplot(231)
ax2 = fig.add_subplot(232)
ax3 = fig.add_subplot(233)
ax4 = fig.add_subplot(234)
ax5 = fig.add_subplot(235)
ax6 = fig.add_subplot(236)

xs = []
resp = []
card = []
temp = []
sat = []
maxPlist = []
minPlist = []

img = Image.open('../data_visualizer/ufcg.jpg')
ax6.imshow(img)
ax6.spines['top'].set_visible(False)
ax6.spines['left'].set_visible(False)
ax6.spines['bottom'].set_visible(False)
ax6.spines['right'].set_visible(False)
ax6.set_xticks([])
ax6.set_yticks([])

def animate(i):

    maxP = 0
    minP = 0
    valueM = []
    valuem = []
    x = False

    value = serial_port.read()
    if(value == b''):
        print('There is no data available')
    else:
        xs.append(i)
        resp.append(value[0])
        temp.append(serial_port.read()[0])
        sat.append(serial_port.read()[0])
        card.append(serial_port.read()[0])

        value = serial_port.read()
        while(value[0] != 100):
            if(value[0] != 120) and not x:
                valueM.append(value[0] - 48)
            elif value[0] == 120:
                x = True
            elif (value[0] != 32):
                valuem.append(value[0] - 48)
            value = serial_port.read()

        for i in range(0, len(valueM)):
            maxP += valueM[len(valueM) - 1 - i]*(10**i)
        
        for i in range(0, len(valuem) - 1):
            minP += valuem[len(valuem) - 2 - i]*(10**i)
        
        maxPlist.append(maxP)
        minPlist.append(minP)
    
    if(len(xs) >= 100):
        xs.pop(0)
        resp.pop(0)
        card.pop(0)
        temp.pop(0)
        sat.pop(0)
        maxPlist.pop(0)
        minPlist.pop(0)


    ax1.clear()
    ax1.set_ylim([0,32])
    ax1.plot(xs, resp, 'tab:blue')
    ax1.set_title('Freq. Respiração x Tempo', fontsize=12)
    ax1.set_xlabel('Tempo (s)')
    ax1.set_ylabel('Frequencia (resp/min)')
    
    ax2.clear()
    ax2.set_ylim([0,220])
    ax2.plot(xs, card, 'tab:red')
    ax2.set_title('Freq. Cardíaca x Tempo', fontsize=12)
    ax2.set_xlabel('Tempo (s)')
    ax2.set_ylabel('Frequencia (bpm)')

    ax3.clear()
    ax3.set_ylim([0,50])
    ax3.plot(xs, temp, 'tab:orange')
    ax3.set_title('Temperatura x Tempo', fontsize=12)
    ax3.set_xlabel('Tempo (s)')
    ax3.set_ylabel('Temperatura (°C)')

    ax4.clear()
    ax4.set_ylim([0,101])
    ax4.plot(xs, sat, 'tab:green')
    ax4.set_title('Saturação de O2 x Tempo', fontsize=12)
    ax4.set_xlabel('Tempo (s)')
    ax4.set_ylabel('Saturação de O2 (\%SpO2)')

    ax5.clear()
    ax5.set_ylim([0,250])
    ax5.plot(xs, maxPlist, 'tab:red')
    ax5.plot(xs, minPlist, 'tab:blue')
    ax5.set_title('Pressão arterial x Tempo', fontsize=12)
    ax5.set_xlabel('Tempo (s)')
    ax5.set_ylabel('Pressão arterial (mmHg)')
    ax5.legend(['sistólica', 'diastólica'])
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()
serial_port.close()

