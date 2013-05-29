/* Einsendearbeit Tobias Zahn*/

/*--------------------INCLUDES---------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------DEFINES----------------------------------------------------*/
#define PAUSE {printf("\nZum Fortsetzen bitte beliebige Taste druecken!\n"); fflush(stdin); getchar();}
// Umlaute und Sonderzeichen:
#define AE (unsigned char)142
#define ae (unsigned char)132
#define OE (unsigned char)153
#define oe (unsigned char)148
#define UE (unsigned char)154
#define ue (unsigned char)129
#define ss (unsigned char)225

#ifdef __unix__
   #define clrscr() system("clear")
#elif __WIN32__ || _MSC_VER
   #define clrscr() system("cls")
#else
   #define clrscr() printf("clrscr() - Fehler!!\n")
#endif

/*--------------------TYPEN-------------------------------------------------------*/
// Datenstruktur Baugruppe definieren:
typedef struct {
	int baugruppennummer;
	char bezeichnung[50];
	int vorgaengernummer;
	int nachfolger1;
	int nachfolger2;
	int anzImVorgaenger;
	int lagerbestand;
	int anzNachfolger;
	int nettobedarf;
	int definiert;
} BAUGRUPPE;

/*--------------------GLOBALE VARIABLEN--------------------------------------------*/
BAUGRUPPE stueckliste[100]; 

/*--------------------PROTOTYPEN---------------------------------------------------*/
void erfasse_baugruppe();
void zeige_baugruppe(int suche);
void aktualisiere_lagerbestand(int suche);
void stueckliste_anzeigen();
void stueckliste_laden();
void stueckliste_speichern();


/*--------------------FUNKTION main------------------------------------------------*/
void main(void)
{
	

	// Stückliste laden?
	int auswahl;
	char c;
	int suche;
	clrscr();
	// stueckliste laden
	stueckliste_laden();

   	do {
    	printf("=== Willkommen im MPS von Tobias Zahn. ===\n\n");
      	printf("=== Hauptmenue: ===\n");
      	printf("-1- Baugruppe erfassen\n");
    	printf("-2- Baugruppe anzeigen\n");
      	printf("-3- Lagerbestand aktualisieren\n");
      	printf("-4- St%cckliste anzeigen\n",ue);
      	printf("-5- Nettobedarfe ermitteln und anzeigen\n");
	  	printf("-6- Programm beenden\n");
      	printf("\nIhre Auswahl : ");
      	scanf("%d",&auswahl);
      	/* fflush(stdin); */
      	getchar();

      	switch(auswahl) {
         	case 1 : erfasse_baugruppe();
                break;
         	case 2 : printf("Bitte Baugruppennummer eingeben :");
                do {
                	scanf("%d",&suche);
                  	} while(getchar()!= '\n');
                zeige_baugruppe(suche);
                break;
        	case 3 : 
         		printf("Bitte Baugruppennummer eingeben :");
                do {
                    scanf("%d",&suche);
                } while(getchar()!= '\n');
         		aktualisiere_lagerbestand(suche);
                break;
         	case 4 : stueckliste_anzeigen();
                break;
		 	case 5 : //Sort(adressen,x);
                break;
         	case 6 : printf("Auf Wiedersehen....\n");
                break;
         	default: printf("Falsche Eingabe. Bitte eine Zahl zwischen 1 und 6 eingeben.\n");
      	}

   	}while(auswahl <6);
   	stueckliste_speichern();
   	printf("Stueckliste gespeichert!");
	return EXIT_SUCCESS;
	PAUSE;
}

/*--------------------FUNKTION stueckliste_laden-----------------------------------*/
void stueckliste_laden() {
	FILE *fp;
	int zaehler;

	fp = fopen("stckl.txt","r");
	// gesamte Stückliste laden:
	if(fp != NULL) {
		for (zaehler = 0; zaehler < 100; zaehler++) {
		fscanf(fp,"%d %s %d %d %d %d %d %d %d %d", 
			&stueckliste[zaehler].baugruppennummer,
			&stueckliste[zaehler].bezeichnung,
			&stueckliste[zaehler].vorgaengernummer,
			&stueckliste[zaehler].nachfolger1,
			&stueckliste[zaehler].nachfolger2,
			&stueckliste[zaehler].anzImVorgaenger,
			&stueckliste[zaehler].lagerbestand,
			&stueckliste[zaehler].anzNachfolger,
			&stueckliste[zaehler].nettobedarf,
			&stueckliste[zaehler].definiert);
				
	}

	//Datei Schließen
	fclose(fp);

	}
	else {
		printf("Es existiert noch keine Stückliste. Diese wird beim Verlassen des Programms neu angelegt.\n");
		PAUSE;
		clrscr();

	}
}
/*--------------------FUNKTION stueckliste_speichern-------------------------------*/
void stueckliste_speichern() {

	FILE *fp;
	int zaehler;

	fp = fopen("stckl.txt","w");
	// gesamte Stückliste speichern: sizeof(stueckliste)
	for (zaehler = 0; zaehler < 100; zaehler++) {
		fprintf(fp,"%d %s %d %d %d %d %d %d %d %d\n", 
			stueckliste[zaehler].baugruppennummer,
			stueckliste[zaehler].bezeichnung,
			stueckliste[zaehler].vorgaengernummer,
			stueckliste[zaehler].nachfolger1,
			stueckliste[zaehler].nachfolger2,
			stueckliste[zaehler].anzImVorgaenger,
			stueckliste[zaehler].lagerbestand,
			stueckliste[zaehler].anzNachfolger,
			stueckliste[zaehler].nettobedarf,
			stueckliste[zaehler].definiert);	
	}
	

	//Datei Schließen
	fclose(fp);

}
/*--------------------FUNKTION erfasse_baugruppe-----------------------------------*/
void erfasse_baugruppe() {
	clrscr();
	// Ablauf:
	// wichtig: Baugruppe erst ins Array reinschreiben wenn alle checks positiv waren, es
	// könnte ja immer noch abgebrochen werden.
	// 
	// 1. Eingabe Baugruppennummer
	//   Wenn die Baugruppe schon existiert andere Baugruppennummer (vielleicht iwann eine vorschlagen)
	//   Wenn die Baugruppe noch nicht existiert kann es weiter gehen.
	// 2. Eingabe Vorgänger:
	//   Wenn kein Vorgänger existiert muss eine andere Nummer eingegeben werden oder abgebrochen werden
	//   Wenn die Nummer existiert kann es weiter gehen. Wichtig: die Nummer muss auch als Nachfolger in das Vorgängerlement eingetragen werden!
	//   WICHTIG: der Vorgänger darf nicht schon zwei Nachfolger haben!!
	//   
	// 3. Nachfolger werden hier immer auf 0 gesetzt, da ja immer der Vorgänger vor dem Nachfolger eingegeben wird
	// 4. Eingabe Bezeichnung
	// 5. Lagerbestand eingeben
	// 6. Zunächst Mal nettobedarf auf 0 setzen und anzImVorgaenger und anzNachfolger für den Vorgänger setzen

	int vorg; // Vorgängernummer
	int bgrnr; // Baugruppennummer
	char bez[100]; // Bezeichnung
	int lagerbes; // Lagerbestand

	// Überschrift:
	printf("=== Erfassung einer neuen Baugruppe: ===\n\n");
	printf("Bitte Baugruppennummer eingeben: ");
	scanf("%d", &bgrnr);
	
	// 1. gibt es bereits eine Baugruppe gleicher Nummer?
	// Prüfung funktioniert noch nicht. Wie finde ich heraus, ob es das Element schon gibt?
	//if (strlen(stueckliste[bgrnr].baugruppennummer) != 0) {
	if (stueckliste[bgrnr].definiert != 0) {
		printf("Baugruppe unter dieser Nummer existiert bereits und kann nicht nochmals angelegt werden!\n");

	}

	// Jetzt Schleife neu starten oder Eingabe abbrechen!
	
	// Falls es sich nicht um das Root Element handelt
	
	// Mit der Eingabe kann fortgefahren werden
	// Keine Eingabe eines Vorgängers falls Baugruppennummer = 0!
	if( bgrnr != 0) {
		printf("Bitte Vorgängernummer eingeben: ");
		scanf("%d", &vorg);
	
		// 2. gibt es den Vorgänger?
		//if (strlen(stueckliste[vorg].baugruppennummer) != 0)
		if (stueckliste[vorg].definiert != 1)
			printf("Diesen Vorgänger gibt es leider nicht!\n");
			// Nochmals versuchen oder abbrechen?
		
	}
	else {
		vorg = 0;
	}
	
	
	// 3. eine Bezeichnung eingeben
	printf("Bitte Bezeichnung eingeben: ");
	scanf("%s",&bez);
	
	printf("Bitte Lagerbestand eingeben: ");
	scanf("%d", &lagerbes);

	// Speichern der Daten im Vektor:
	stueckliste[bgrnr].baugruppennummer = bgrnr;
	strcpy(stueckliste[bgrnr].bezeichnung, bez);
	stueckliste[bgrnr].vorgaengernummer = vorg;
	stueckliste[bgrnr].nachfolger1 = 0;
	stueckliste[bgrnr].nachfolger2 = 0;
	stueckliste[bgrnr].anzImVorgaenger = 0;
	stueckliste[bgrnr].lagerbestand = lagerbes;
	stueckliste[bgrnr].anzNachfolger = 0;
	stueckliste[bgrnr].nettobedarf = 0;
	stueckliste[bgrnr].definiert = 1;

	printf("Der Eintrag wurde erfolgreich erfasst.\n\n\n\n");
	getchar();
	PAUSE;
	clrscr();


}

/*--------------------FUNKTION zeige_baugruppe-------------------------------------*/
void zeige_baugruppe(int suche) {
	clrscr();
	printf("=== Einzelansicht der Baugruppe %d : ===\n\n",suche);
	if (stueckliste[suche].definiert == 1) {
		printf("Baugruppennummer:\t%d\n",stueckliste[suche].baugruppennummer);
		printf("Bezeichnung:\t\t%s\n",stueckliste[suche].bezeichnung);
		printf("Lagerbestand:\t\t%d\n",stueckliste[suche].lagerbestand);
	}
	else
		printf("Das gewaehlte Produkt existiert nicht!\n");

	PAUSE;
	clrscr();

}
/*--------------------FUNKTION aktualisiere_lagerbestand---------------------------*/
void aktualisiere_lagerbestand(int suche) {
	// Variablen
	int lager;
	// Ausgabe
	clrscr();
	printf("=== Lagerbestand der Baugruppe %d aktualiesieren ===\n\n",suche);
	printf("Alter Lagerbestand: %d\n",stueckliste[suche].lagerbestand);	
	printf("Neuer Lagerbestand: ");
	scanf("%d",&lager);

	stueckliste[suche].lagerbestand = lager;

	printf("Lagerbestand auf %d aktualisiert!",lager);
	PAUSE;
	clrscr();


}
/*--------------------FUNKTION stueckliste_anzeigen--------------------------------*/
void stueckliste_anzeigen() {
	
	int i = 0; // Zählvariable
	int j = 0; // Zählvariable 2
	int zaehler = 0; // Zählvariable für die Leerzeichen

	clrscr();
	// Ausgabe muss sortiert statt finden

	printf("=== Ausgabe der gesamten Stueckliste ===\n\n");
	// Tabellenkopf für den ersten Durchlauf
	printf("|--------|----------------------------------------------------|--------------|\n");
	printf("| Bgrnr. | Bezeichnung                                        | Lagerbestand |\n");
	printf("|--------|----------------------------------------------------|--------------|\n");

	// Schleife für die Ausgabe der Baugruppen
	for (i=0; i < 100; i++) {
		if(stueckliste[i].definiert == 1) {
			// erste Spalte:
			printf("| ");
			printf("%d", stueckliste[i].baugruppennummer);
			// Anzahl Leerzeichen ermitteln:
			if(stueckliste[i].baugruppennummer<10)
				zaehler = 6;
			else if (stueckliste[i].baugruppennummer<100)
				zaehler = 5;
			else if (stueckliste[i].baugruppennummer<1000)
				zaehler = 4;
			for(j=0;j<zaehler;j++)
				printf(" ");
			// Zweite Spalte:
			printf("| ");
			printf("%s", stueckliste[i].bezeichnung);
			zaehler = 50 - strlen(stueckliste[i].bezeichnung);
			for(j = 0; j <= zaehler; j++)
				printf(" ");
			// Dritte Spalte:
			printf("| ");
			printf("%d", stueckliste[i].lagerbestand);
			if(stueckliste[i].lagerbestand<10)
				zaehler = 12;
			else if (stueckliste[i].lagerbestand<100)
				zaehler = 11;
			else if (stueckliste[i].lagerbestand<1000)
				zaehler = 10;
			else if (stueckliste[i].lagerbestand<10000)
				zaehler = 9;
			for(j=0;j<zaehler;j++)
				printf(" ");
			printf("|\n");
		}

	}

	printf("|--------|----------------------------------------------------|--------------|\n");
	PAUSE;
	clrscr();

}
/*--------------------FUNKTION nettobedarfe_anzeigen-------------------------------*/

