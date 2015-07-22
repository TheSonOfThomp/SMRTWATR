import websocket
import serial
 
if __name__ == "__main__":

	websocket.enableTrace(True)
	ws = websocket.create_connection("ws://smrtwatr.me/game/ws")
	print("connected")

	ser4 = serial.Serial('/dev/cu.usbmodem1139931', baudrate=9600, timeout=2)
	ser5 = serial.Serial('/dev/cu.usbmodem1141421', baudrate=9600, timeout=2)
	ser4.write('q80000')
	ser5.write('q80000')
	
	score = 0
	result = 1
	while result:
		print("Receiving...")
		result = ws.recv()
		print("Received {}".format(result))
		if result.startswith('pi:'):
			if result.startswith("end", 4):
				print("Sending teensy: q80000")
				ser4.write('q80000')
				ser5.write('q80000')
				score = 0
			elif result.startswith("s:", 4):
				seq = int(result.split("s:")[1][0]) + 3
				print("Sending teensy: q" + str(seq) + "0000")
				ser4.write("q" + str(seq) + "0000")
				ser5.write("q" + str(seq) + "0000")
			elif result.startswith("q:", 4):
				if result.split("c:",1)[1][0] == '1' :
					score += 10 ** (4-int(result.split("p:",1)[1][0]))
				print("Sending teensy: q" + result.split("q:",1)[1][0] + '%04d' % score )
				ser4.write("q" + result.split("q:",1)[1][0] + '%04d' % score )
				ser5.write("q" + result.split("q:",1)[1][0] + '%04d' % score )


	print("Closing")
	ws.close()