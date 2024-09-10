import socket
import time
import threading
import queue

startTime = time.time()
def getTime():
	return f"{time.time() - startTime:.2f}"

pointerSet1 = set()
pointerSet2 = set()
	
def server(packetQueue):
	host='localhost'
	port=17474

	# tcp preferable, couldnt get gml to work with it tho
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.settimeout(1.0)
	# Bind the socket to the host and port
	sock.bind((host, port))

	print(f'UDP server started on {host}:{port}')

	try:
		while True:
			# Receive data from the client

			try:
				data, addr = sock.recvfrom(1024)
				if data:
					# what are these 12 junk bytes?
					try:
						d = data.decode("ascii").strip()
					except:
						packetQueue.put(f"{data}")
						continue
					#if d != "0":
					packetQueue.put(f"{d}")
			except socket.timeout: # only here so ctrl c works
				time.sleep(1)
				continue
	except KeyboardInterrupt as e:
		#print(e)
		pass

def writerFunc(packetQueue):
	# seperate thread bc although im pretty confident that udp wont mess up, i should be safe
	while True:
		try:
			data = packetQueue.get(True, timeout=0.01)
		except queue.Empty:
			continue

		if data is None:
			break
		print(f'got data: {data}')

		#if len(data) == 10:
		#	char, d = data.split(" ")
		#	if char == "0":
		#		pointerSet1.add(int(d, 16))
		#	else:
		#		pointerSet2.add(int(d, 16))
		# not ideal, but works
		#with open("MBAACC.log", "a") as f:
		#	f.write(f"{data}\n")
		pass
	
	#with open("MBAACC1.log", "a") as f:
	#	for ptr in pointerSet1:
	#		f.write(f"{ptr:08X}\n")
	#		
	#with open("MBAACC2.log", "a") as f:
	#	for ptr in pointerSet2:
	#		f.write(f"{ptr:08X}\n")
		
def main():
	packetQueue = queue.Queue() # queues are thread safe by default in python

	writerThread = threading.Thread(target=writerFunc, args=(packetQueue,))
	writerThread.start()

	server(packetQueue)

	print("putting")
	packetQueue.put(None)
	print("waiting on writer to join")
	writerThread.join()

def graph():

	vals = []

	for filename in ["MBAACC1.log", "MBAACC2.log"]:
		with open(filename, "r") as f:
			for l in [ l.strip() for l in f.readlines() if len(l.strip()) != 0]:
				vals.append(int(l, 16))

		vals.sort()

		prev = vals[0]
		
		for v in vals[1:]:
			print(f"{v - prev:08X}")
			prev = v

		print(f"start addr was {vals[0]:08X}")
		print(f"end   addr was {vals[-1]:08X}")

		# typical offset bounds of 0x54
		# is base(starting) val gotten on css?
		
		# 04D0BF68
		# 0047961b
		
		# 04B628F0
		# 04D35B38
		
		# start addr was 04B7E9B0
		# end   addr was 04D9832C
		# start addr was 04B7E9B0
		# end   addr was 19871E84
		
	pass
	
if __name__ == '__main__':
	main()
	#graph()