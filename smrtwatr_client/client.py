import websocket
#import httplib
 
if __name__ == "__main__":

	#conn  = httplib.HTTPConnection("localhost:9999")
	#conn.request("GET", "/")
	#resp = conn.getresponse()
	#cookie = resp.getheader('Set-Cookie').split(';')[0]

	#print cookie[6:]

	websocket.enableTrace(True)
	ws = websocket.create_connection("ws://localhost:9999/game/ws")
	print("connected")
	
	score = 0
	result = 1
	while result:
		print("Receiving...")
		result = ws.recv()
		print("Received {}".format(result))
		if result.startswith('pi:'):
			if result.startswith("end", 4):
				print("Sending teensy: q80000")
			elif result.startswith("s:", 4):
				seq = int(result.split("s:")[1][0]) + 4
				print("Sending teensy: q" + str(seq) + "0000")
			elif result.startswith("c:", 4):
				if result.split("c:",1)[1][0] == '1' :
					score += 10 ** (4-int(result.split("p:",1)[1][0]))
				print("Sending teensy: q" + result.split("q:",1)[1][0] + '%04d' % score )


	print("Closing")
	ws.close()