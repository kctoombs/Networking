'''Some of this code (specifically any of the lines that begin with 'serverSocket') are from the textbook Computer  Networking:  A Top - Down  Approach  Featuring  the  Internet,  Kurose  and  Ross,  Addison-Wesley,  7th Edition,  2016 '''

from socket import *
import time
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print 'server_python_tcp 12000'
while 1:
    connectionSocket, addr = serverSocket.accept()
    sentence = connectionSocket.recv(128)
    sumToString = ''
    while len(sumToString) != 1:
        sum = 0
        for i in sentence:
            if i.isdigit():
                sum += int(i)
                sumToString = str(sum)
                
            else:
                sumToString = 'Sorry, cannot compute!'
                connectionSocket.send(sumToString)
                connectionSocket.close()
                exit()
                           
        sentence = sumToString
        '''print ('sumToString: ') , sumToString'''
        connectionSocket.send(sumToString)
	'''Give the client some time to process the response'''
	time.sleep(0.25)
        '''print ('after send')'''
    connectionSocket.close()
    exit()
