import socket
import threading

def send_request(i):
    s = socket.socket()
    s.connect(("127.0.0.1", 9090))
    s.sendall(b"GET / HTTP/1.1\r\nHost: localhost\r\n\r\n")
    s.close()

threads = []

for i in range(100):
    t = threading.Thread(target=send_request, args=(i,))
    threads.append(t)
    t.start()

for t in threads:
    t.join()

print("100 clients sent requests")
