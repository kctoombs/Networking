'''Some of this code (specifically any of the lines that begin with 'serverSocket') are from the textbook Computer  Networking:  A Top - Down  Approach  Featuring  the  Internet,  Kurose  and  Ross,  Addison-Wesley,  7th Edition,  2016 '''

from socket import *
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
serverSocket.bind(('', serverPort))
print 'server_python_udp 12000'
while 1:
    message, clientAddress = serverSocket.recvfrom(128)
    sumToString = ''
    while len(sumToString) != 1:
        sum = 0
        for i in message:
            if i.isdigit():
                sum += int(i)
                sumToString = str(sum)
            else:
                sumToString = 'Sorry, cannot compute!'
                serverSocket.sendto(sumToString, clientAddress)
                serverSocket.close()
                exit()
                
        message = sumToString
        '''print('Message: ') , message
        print('sumToString ') , sumToString
        print('Before send')'''
        serverSocket.sendto(sumToString, clientAddress)
        '''print('After send')'''
    serverSocket.close()
    exit()
