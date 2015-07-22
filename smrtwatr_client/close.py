import websocket
import serial
 
if __name__ == "__main__":

	
	ser4 = serial.Serial('/dev/cu.usbmodem1139931', baudrate=9600, timeout=2)
	ser5 = serial.Serial('/dev/cu.usbmodem1141421', baudrate=9600, timeout=2)
	ser4.write('q80000')
	ser5.write('q80000')
	ser4.close()
	ser5.close()
	
