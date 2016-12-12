'''Some of this code (specifically any of the lines that begin with 'clientSocket') are from the textbook Computer  Networking:  A Top - Down  Approach  Featuring  the  Internet,  Kurose  and  Ross,  Addison-Wesley,  7th Edition,  2016 '''

from socket import *
serverName = 'csil-01.cs.ucsb.edu'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
print('client_python_tcp 128.11.43.21 12000')
sentence = raw_input('Enter string:')
clientSocket.send(str(sentence))
isLarge = True
while isLarge:
    sumToString = clientSocket.recv(128)
    print 'From Server:' , sumToString
    if len(sumToString) == 1 or not(sumToString).isdigit():
        isLarge = False
clientSocket.close()
