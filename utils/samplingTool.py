import serial
import json
import statistics

# Variables declaration #

sampleCounter = 0
samplesToIgnore = 0
samplesRequired = 20
labels = ["Tempo","Tensão", "Temperatura", "Concentração",
          "Densidade", "Vazão volumétrica", "Vazão mássica"]
keys = ["capacitorVoltage", "temperature", "concentration",
            "density", "volumetricFlowRate", "massFlowRate"]
serialInput = serial.Serial('/dev/ttyUSB0')
inputDataList = {}
for key in keys:
    inputDataList[key]=[]
    
# Functions #

def unpackJSON(data):
    line = ","
    for key in keys:
        line += str(data[key]) + ","
        inputDataList[key].append(float(data[key]))
    return line

# Receiving file name #

while True:
    try:
        fileName = input('Digite o nome do arquivo que receberá os dados: ')
        csvFile = open('./output_files/'+fileName+'.csv', 'x')
        header = ""
        for key in labels:
            header += key + ","
        csvFile.write(header+"\n")
        break
    except:
        print('Nome indisponivel!')

# Receiving data from serial and printing it in csv file #

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

# Calculing final statistics #

finalStatistics = '\nEstatísticas Finais\nMédia,'
for key in keys:
    finalStatistics+=str(statistics.mean(inputDataList[key]))+','
finalStatistics +='\nModa,'
for key in keys:
    finalStatistics+=str(statistics.mode(inputDataList[key]))+','
finalStatistics +='\nMediana,'
for key in keys:
    finalStatistics+=str(statistics.mean(inputDataList[key]))+','
csvFile.write(finalStatistics)


