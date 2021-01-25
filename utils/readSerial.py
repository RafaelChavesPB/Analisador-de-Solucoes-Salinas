import serial 
import json
import os
ser = serial.Serial('/dev/ttyUSB0')
cont = 0
durat = 3
freq = 440 
while True:
    try:
        fileName = input('Digite o nome do arquivo que receberá os dados: ')
        data = open(fileName+'.csv','x')
        data.write('Capacitor_sensor(V),Capacitor(V),Controle(V),Temp(C°)\n')
        break
    except:
        print('Nome indisponivel!')

while True:
    try:
        d = json.loads(ser.readline().decode('utf-8'))
        if(cont>=20):
            print(f"Capacitor:{d['tensao']:.5f},\tCapacitorStd: {d['tensaoStd']:.5f},\tControle:{d['controle']:.5},\t Temp:{d['temperatura']}")
            data.write(f"{d['tensao']:.5f},{d['tensaoStd']:.5f},{d['controle']:.5f},{d['temperatura']},\n")
            if(cont>220):
                break
        else:
            print('ignorando...')
        cont+=1
    except KeyboardInterrupt:
        break
    except:        
        continue
os.system(f'play -nq -t alsa synth {durat} sine {freq}')

        