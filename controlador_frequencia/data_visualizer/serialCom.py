import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

serial_port = serial.Serial('/dev/pts/2', 9600, timeout=1)

fig = plt.figure()

ax1 = fig.add_subplot(111)

#ax2 = fig.add_subplot(222)
#ax3 = fig.add_subplot(223)
#ax4 = fig.add_subplot(224)

xs = []
ys = []

def animate(i):
    value = serial_port.read()
    if(value == b''):
        print('There is no data available')
    else:
        xs.append(i)
        ys.append(int(value[0]))
    
    if(len(xs) >= 100):
        xs.pop(0)
        ys.pop(0)


    ax1.clear()
    ax1.set_ylim([0,32])
    ax1.plot(xs, ys, 'tab:blue')
    ax1.set_title('Freq. Respiração x Tempo', fontsize=12)
    ax1.set_xlabel('Tempo (ms)')
    ax1.set_ylabel('Frequencia (resp/min)')
    
    #ax2.clear()
    #ax2.plot(xs, ys, 'tab:red')
    #ax2.set_title('Freq. Cardíaca x Tempo')

    #ax3.clear()
    #ax3.plot(xs, ys, 'tab:orange')
    #ax3.set_title('Temperatura x Tempo')

    #ax4.clear()
    #ax4.plot(xs, ys, 'tab:green')
    #ax4.set_title('Saturação de O2 x Tempo')


ani = animation.FuncAnimation(fig, animate, interval=200)
plt.show()
serial_port.close()

