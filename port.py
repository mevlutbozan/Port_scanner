import socket
from concurrent.futures import ThreadPoolExecutor

def port_tarama(ip,port):
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    s.settimeout(1)
    result = s.connect_ex((ip,port))
    if result == 0:
        print(f"PORT {port} Açık kanka")
    s.close()
ip = input("bir ip adresi girin: ")
ports = range(1,1024)

with ThreadPoolExecutor(max_workers=100) as çalıştırma:
    for port in ports:
        çalıştırma.submit(port_tarama,ip,port)