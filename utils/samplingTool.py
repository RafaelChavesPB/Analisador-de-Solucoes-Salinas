import serial
import json
import os

def unpackJSON(data):
    keys = ["capacitorVoltage", "temperature", "concentration",
            "density", "volumetricFlowRate", "massFlowRate"]
    line = "rafael"
    for key in keys:
        line += str(data[key]) + ","
    return line

ser = serial.Serial('/dev/ttyACM0')
labels = ["Tensão", "Temperatura", "Concentração",
          "Densidade", "Vazão volumétrica", "Vazão mássica"]

while True:
    try:
        fileName = input('Digite o nome do arquivo que receberá os dados: ')
        csvFile = open('./samplingData/'+fileName+'.csv', 'x')
        header = ""
        for key in labels:
            header += key + ","
        csvFile.write(header+"\n")
        break
    except:
        print('Nome indisponivel!')


sampleSize = 0
while True:
    try:
        data = json.loads(ser.readline().decode('utf-8'))
        if sampleSize < 0:
            print('ignorando...')
        elif sampleSize <= 5:
            print(sampleSize)
            print(data)
            line = unpackJSON(data)
            # print(line)
            # csvFile.write(line+"\n")
        else:
            print('Encerrando')
            break
        sampleSize = sampleSize + 1
    except KeyboardInterrupt:
        print('Encerrando')
        break
    except:
        continue

os.system(f'play -nq -t alsa synth {3} sine {440}')
