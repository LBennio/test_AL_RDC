Progettare ed implementare un'applicazione TCP client/server, dove il server è un generatore di password che risponde alle richieste del client nel seguente modo:

Una volta avviato, il client stabilisce automaticamente la connessione al server (sulla porta e indirizzo da voi stabiliti).

Ricevuta la connessione, il server visualizza sullo standard output l'indirizzo e il numero di porta del client, scrivendo "New connection from xxx.xxx.xxx.xxx:yyyyy".

Stabilita la connessione, il client legge il tipo di password da generare dallo standard input usando i caratteri:

n: password numerica (solo cifre)
a: password alfabetica (solo lettere minuscole)
m: password mista (lettere minuscole e numeri)
s: password sicura (lettere maiuscole, minuscole, numeri e simboli)
seguito dalla lunghezza desiderata (es. n 8 per una password numerica di 8 caratteri).

Il server legge quanto inviato dal client, genera la password secondo i criteri richiesti e la invia al client (es. 45237891).

Il client legge la password generata dal server e la visualizza sullo standard output.

Il client legge dallo standard input la successiva richiesta di generazione.

Se invece di una richiesta valida è inserito il carattere q, il client chiude la connessione con il server e termina qui il suo processo; altrimenti, torna al punto 3.

Il server non termina mai il suo processo e deve essere in grado di accettare una coda massima di 5 client (parametro qlen).

Autori codice sorgente risultante: Linda Germinario[796992], Ennio Lo Buono[801933]
