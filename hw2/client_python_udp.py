'''Some of this code (specifically any of the lines that begin with 'clientSocket') are from the textbook Computer  Networking:  A Top - Down  Approach  Featuring  the  Internet,  Kurose  and  Ross,  Addison-Wesley,  7th Edition,  2016 '''

import socket
serverName = 'csil-01.cs.ucsb.edu'
serverPort = 12000
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print('client_python_udp 128.11.43.21 12000')
message = raw_input('Enter string:')

clientSocket.sendto(message, (serverName, serverPort))
isLarge = True
while isLarge:
    sumToString, serverAddress = clientSocket.recvfrom(128)
    print 'From Server:' , sumToString
    if len(sumToString) == 1 or not(sumToString.isdigit()):
        isLarge = False
clientSocket.close()
exit()
