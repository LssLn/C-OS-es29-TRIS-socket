/*
Implementare un'applicazione che offra un servizio ECHO utilizzando i socket nel dominio di internet. 

Si devono implementare sia un server che un client

Il client richiede una stringa da tastiera e lo invia al server

Il server risponde trasmettendo la stessa stringa inviata dal client.

se il client invia "esci" il server termina.

PASSAGGI:
1. creazione SOCKET
2. bind di un indirizzo
3. Listen sul socket
4. Accept.

Riceve una stringa
Invia la stessa stringa
Chiude socket client e si ritorna alla 4.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int controlla_vittoria();

int main (){
	int serv_socket, g1_socket, g2_socket;
	struct sockaddr_in baddr;
	struct sockaddr_in cliaddr;
	socklen_t slen;
	int len, turno = 0, r, c;
	char buff[100];	
	char campo[3][3];
	int time_to_exit, tsock;

	
	
	//faccio la socket
	serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_socket == -1){
		perror("Errore socket\n");
		exit(1);
	}
	
	//riempiamo i campi della struttura baddr
	baddr.sin_family = AF_INET;			//famiglia
	baddr.sin_port = htons(7777);			//porta su cui ci vogliamo mettere in ascolto
	baddr.sin_addr.s_addr = htonl(INADDR_ANY);	//indirizzo su cui ci vogliamo mettere in ascolto
	
	//faccio la bind
	if(bind(serv_socket, (struct sockaddr *)&baddr, sizeof(baddr)) == -1){
		perror("Errore bind\n");
		close(serv_socket);
		exit(1);
	}

	//faccio la listen
	
	listen(serv_socket, 5);
	
	//faccio la accept
	while(1){

		for (r=0; r<3; r++){
			for(c=0;c<3;c++) {
				campo[r][c] = ' ';
			}
		}
		time_to_exit = 0;		
		
		slen = sizeof(cliaddr);
		g1_socket = accept(serv_socket, (struct sockaddr *)&cliaddr, &slen);
		
		sprintf(buff, "1");
		write(g1_socket, (void *)buff, 1); 
	
		g2_socket = accept(serv_socket, (struct sockaddr *)&cliaddr, &slen);
	
		sprintf(buff, "1");
		write(g1_socket, (void *)buff, 1); 

		while(!time_to_exit){
			if(turno % 2 == 0) {
				tsock = g1_socket;
			} else {
				tsock = g2_socket;
			}
			turno++;
	
			sprintf(buff, "%c | %c | %c\n---------\n%c | %c | %c\n---------\n%c | %c | %c", campo[0][0], campo[0][1], campo[0][2], campo[1][0], campo[1][1], campo[1][2], campo[2][0], campo[2][1], campo[2][2]);
	
			write(tsock, (void *)buff, strlen(buff)+1);

			len = read(tsock, ( void *)buff, 100);
			if(len <= 0){
				close(g1_socket);
				close(g2_socket);
				break;
			}
		
			r = controlla_vittoria();
			if(sscanf(buff, "%d %d%*c", &r, &c) >= 2) {
				if(tsock = g1_socket)
					campo[r][c] = 'X';
				else
					campo[r][c] = 'Y';
			}
		
			if(r == 0){
				sprintf(buff, "Parità");
				write(g1_socket, (void *)buff, strlen(buff)+1);
				write(g1_socket, (void *)buff, strlen(buff)+1);
				break;
			} else if(r > 0){
				sprintf(buff, "Ha vinto il giocatore %d!\n", r);
				write(g1_socket, (void *)buff, strlen(buff)+1);
				write(g1_socket, (void *)buff, strlen(buff)+1);
				break;
		}

	}
				close(g1_socket);
				close(g2_socket);
}
	
	close(serv_socket);
	exit(0);

}

int controlla_vittoria() {
    char campo[3][3];
    int i, j;
    
    /* Controlla righe */
    for(i=0; i<3; i++) {
        if(campo[i][0] == campo[i][1] && campo[i][0] == campo[i][2] && campo[i][0] != ' ') {
            if(campo[i][0] == 'X')
                return 1;
            else
                return 2;
        }
    }
    
    /* Controlla colonne */
    for(i=0; i<3; i++) {
        if(campo[0][i] == campo[1][i] && campo[0][i] == campo[2][i] && campo[0][i] != ' ') {
            if(campo[0][i] == 'X')
                return 1;
            else
                return 2;
        }
    }
    
    /* Controlla diagonali */
    if(campo[0][0] == campo[1][1] && campo[0][0] == campo[2][2] && campo[0][0] != ' ') {
        if(campo[0][0] == 'X')
            return 1;
        else
            return 2;
    }
    
    if(campo[0][2] == campo[1][1] && campo[0][2] == campo[2][0] && campo[0][2] != ' ') {
        if(campo[0][2] == 'X')
            return 1;
        else
            return 2;
    }
    
    /* Controlla che NON vi sia paritÃ  */
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            if(campo[i][j] == ' ')
                return -1;
        }
    }
    
    /* Se si arriva qui allora Ãš paritÃ  */
    return 0;
    
    
}



















