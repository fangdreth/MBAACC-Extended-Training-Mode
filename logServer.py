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
						d = data.decode("ascii")#.strip()
					except:
						try:
							d = data.decode("utf-8")#.strip()
						except:
							packetQueue.put(f"{data}")
							continue
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
		
		if data == "CLEAR":
			print(chr(27) + "[2J")
			print(chr(27) + "[H")
			continue
			
		print(f'{data}')

		pass
	
def main():
	packetQueue = queue.Queue() # queues are thread safe by default in python

	writerThread = threading.Thread(target=writerFunc, args=(packetQueue,))
	writerThread.start()

	server(packetQueue)

	print("putting")
	packetQueue.put(None)
	print("waiting on writer to join")
	writerThread.join()

if __name__ == '__main__':
	main()