Script em Pytohn e C que gera tráfego em todas as portas UDP com o auxílio de múltiplas threads, até ser finalizado.
Para compiltar o fonte em C:
gcc dos.c -l pthread

Modo de uso de ambos é igual:

python3 dos.py <ip_alvo> <numepero_thread>

ou 

./a.out <ip_alvo> <numepero_thread>