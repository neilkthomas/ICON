import socket
import sys

HOST, PORT = "192.168.18.102", 9000  


SGN =  "{\"Index\": 11112,\"MAC Address\": \"00:CF:52:35:00:03\",\"Date & Time\": \"2022-02-06 17:16:00\",\"Firmware Version\": \"Icon.02.22\",\"Device Name\": \"Icon\"}"

LGN = "{\"Index\":11111}"

LINP = "{\"Index\":11930}"

SINP = "{\"Index\":11931,\"InputList\":[{\"ChannelNo\":\"1\",\"Name\":\"5V CHN1\",\"Type\":\"5V-Analog\",\"Min\":\"0.00\",\"Max\":\"5.00\",\"Offset\":\"0.00\",\"Unit\":\"V\"},{\"ChannelNo\":\"2\",\"Name\":\"5v CHN2\",\"Type\":\"5V-Analog\",\"Min\":\"0.00\",\"Max\":\"5.00\",\"Offset\":\"0.00\",\"Unit\":\"V\"},{\"ChannelNo\":\"3\",\"Name\":\"5V CHN3\",\"Type\":\"5V-Analog\",\"Min\":\"0.00\",\"Max\":\"5.00\",\"Offset\":\"0.00\",\"Unit\":\"V\"}]}"

"""
{"Index":11931,"InputList":[{"ChannelNo":"1","Name":"5V CHN1","Type":"5V-Analog","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"},{"ChannelNo":"2","Name":"5v CHN","Type":"5V-Analog","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"},{"ChannelNo":"3","Name":"5V CHN","Type":"5V-Analog","Min":"0.00","Max":"5.00","Offset":"0.00","Unit":"V"}]}
"""

LOUT = "{\"Index\":11940}"

SOUT = "{\"Index\":11941,\"OutputList\":[{\"ChannelNo\":\"1\",\"Name\":\"5V CHN1\",\"Type\":\"5V-Analog\",\"Logic\":\"Direct\",\"Unit\":\"V\",\"Setpoint\":\"20.00\"},{\"ChannelNo\":\"2\",\"Name\":\"5V CHN2\",\"Type\":\"5V-Analog\",\"Logic\":\"Direct\",\"Unit\":\"V\",\"Setpoint\":\"80.00\"},{\"ChannelNo\":\"3\",\"Name\":\"Green Relay\",\"Type\":\"Digital\",\"Logic\":\"Direct\",\"Unit\":\"V\",\"Setpoint\":\"1.00\"},{\"ChannelNo\":\"4\",\"Name\":\"Red Relay\",\"Type\":\"Digital\",\"Logic\":\"Direct\",\"Unit\":\"V\",\"Setpoint\":\"0.00\"}]}"

"""
{"Index":11941,"OutputList":[{"ChannelNo":"1","Name":"5V CHN1","Type":"5V-Analog","Logic":"Direct","Unit":"V","Setpoint":"40.00"},{"ChannelNo":"2","Name":"5V CHN2","Type":"5V-Analog","Logic":"Direct","Unit":"V","Setpoint":"90.00"},{"ChannelNo":"3","Name":"Green Relay","Type":"Digital","Logic":"Direct","Unit":"V","Setpoint":"0.00"},{"ChannelNo":"4","Name":"Red Relay","Type":"Digital","Logic":"Direct","Unit":"V","Setpoint":"1.00"}]}
"""

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Connect to server and send data
    sock.connect((HOST, PORT))
    #sock.sendall(data + "\n")

    print "\nICon - Command Line Configuration App "
    print "====================================="
    print "KeyCode - Option             "
    print "LGN  - Load General Settings "
    print "SGN  - Save General Settings "
    print "LINP - Load Input Settings   "
    print "SINP - Save Input Settings   "
    print "LOUT - Load Output Settings  "
    print "SOUT - Save Output Settings  "
    print "=====================================\n"    

    sel = sys.argv[1]

    print sel

    if sel=="LGN" :
	
		    print "Load:     {}".format(LGN)
		    sock.sendall(LGN)
    
    elif sel=="SGN" :
	
		    print "Save:     {}".format(SGN)
		    sock.sendall(SGN)   

    elif sel=="LINP" :
	
		    print "Load:     {}".format(LINP)
		    sock.sendall(LINP)       

    elif sel=="SINP" :
	
		    print "Save:     {}".format(SINP)
		    sock.sendall(SINP)     

    elif sel=="LOUT" :
	
		    print "Load:     {}".format(LOUT)
		    sock.sendall(LOUT)       

    elif sel=="SOUT" :
	
		    print "Save:     {}".format(SOUT)
		    sock.sendall(SOUT)             

    print sock.recv(1024)
finally:
    sock.close()


print "connection close \n"

