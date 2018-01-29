all: clean	 portscanner honeypot honeypotFull

portscanner:
	gcc -Wall -o portscanner portScanner.c

honeypot:
	gcc -Wall -pthread -o honeypot honeyPot.c

honeypotFull:
	gcc -Wall -pthread -DFULL -o  honeypot_full honeyPot.c

clean:
	-rm portscanner honeypot honeypotFull