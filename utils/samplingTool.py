import serial
import json
import os


# Functions #

def unpackJSON(data):
    keys = ["capacitorVoltage", "temperature", "concentration",
            "density", "volumetricFlowRate", "massFlowRate"]
    line = ","
    for key in keys:
        line += str(data[key]) + ","
    return line

# Variables declaration #

sampleCounter = 0
samplesToIgnore = 5
samplesRequired = 20
serialInput = serial.Serial('/dev/ttyACM0')
labels = ["Tempo","Tensão", "Temperatura", "Concentração",
          "Densidade", "Vazão volumétrica", "Vazão mássica"]

# Getting the file name #

while True:
    try:
        fileName = input('Digite o nome do arquivo que receberá os dados: ')
        csvFile = open('./samplingData/'+fileName+'.csv', 'x')
        header = str()
        for key in labels:
            header += key + ","
        csvFile.write(header+"\n")
        break
    except:
        print('Nome indisponivel!')

# Getting serial input and printing it in a csv file #

while True:
    try:
        data = json.loads(serialInput.readline().decode('utf-8'))
        if sampleCounter < samplesToIgnore:
            print('ignorando...')
        elif sampleCounter < samplesToIgnore + samplesRequired:
            line = unpackJSON(data)
            csvFile.write(line+"\n")
            print(line)
        else:
            print('Encerrando')
            break
        sampleCounter = sampleCounter + 1
    except KeyboardInterrupt:
        print('Encerrando')
        break
    except:
        continue

os.system(f'play -nq -t alsa synth {3} sine {440}')
