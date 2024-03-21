#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PORT 2024


extern int errno;

 void intermediarptclient(int client)
       {
    char response[147];
    bzero(response,100);
    const char *prompt = "Va rog trimiteti un mesaj de forma: <Nume>, <parola>,<numar de telefon>, <email> ,<tip_client>";
	    if (write(client, prompt, strlen(prompt)) <= 0) 
		    {
			perror("[server]Eroare la trimiterea prompt-ului");
			return;
		    }

    char InformatiiClient[300]; 
    bzero(InformatiiClient,300);   
    if (read(client, InformatiiClient, sizeof(InformatiiClient)) <= 0) 
		    {
			perror("[server]Eroare la citirea informațiilor de înregistrare");
			return;
		    }

     char nume[100], mail[100], numartelefon[100], categorie[100], parola[100];

    bzero(nume, 100);
    bzero(mail, 100);
    bzero(numartelefon, 100);
    bzero(categorie, 100);
    bzero(parola, 100);

    sscanf(InformatiiClient, "%[^,], %[^,],%[^,], %[^,], %[^\n]", nume, parola, mail, numartelefon, categorie);

    FILE *fisier = fopen("utilizatori.txt", "a");
    FILE *storesFile = fopen("magazine.txt", "a");  // File to store store and restaurant names

    if (fisier == NULL || storesFile == NULL) {
        perror("Eroare la deschiderea fisierului");
        exit(EXIT_FAILURE);
    }

    fprintf(fisier, "Nume: %s,Parola:%s,NumarTelefon:%s,Mail:%s,Tip_Client:%s\n", nume, parola, numartelefon, mail, categorie);

    // Check if the category is "magazin" (store) or "restaurant"
    if (strcmp(categorie, "magazin_alimentar") == 0 || strcmp(categorie, "restaurant") == 0) {
        fprintf(storesFile, "%s\n", nume);  // Store the name in the stores file
    }

    fclose(fisier);
    fclose(storesFile);
    FILE* inventarFile;
	if(strcmp(categorie,"restaurant")==0 || strcmp(categorie,"magazin_alimentar")==0)
	{
	inventarFile=fopen(nume,"w");
if (inventarFile == NULL) {
        perror("Eroare la deschiderea fisierului de inventar");
        snprintf(response, sizeof(response), "Eroare la deschiderea fisierului de inventar %s!\n", nume);
        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului de inventar");
        }
        return;
    }
 
    fclose(inventarFile); 
	}
    snprintf(response, 128, "Inregistrare realizata cu succes pentru %.80s!\n", nume);

    if (write(client, response, strlen(response)) <= 0) {
        perror("[server]Eroare la trimiterea raspunsului de inregistrare");
    }
}

void login(int client,int *conexiune,char *numedeut)

{

char rasp[100];
bzero(rasp,100);
const char* mesajdeid="Va rog introduceti numele de utilizator";
const char* mesajdeps="Va rog introduceti parola";
char tipclient[100];
bzero(tipclient,100);
if(write(client,mesajdeid,strlen(mesajdeid))<=0)
	{
	perror("Eroare la trimiterea mesajului de cere usernameul");
	return;
	}
char nume[100],parola[100];
if(read(client,rasp,sizeof(rasp))<=0)
	{
	perror("Eroare la citirea usernameulul");
	return;
	}
	printf("Abia am citit %s\n",rasp);
strcpy(nume,rasp);
bzero(rasp,strlen(rasp));
if(write(client,mesajdeps,strlen(mesajdeid))<=0)
	{
	perror("Eroare la trimiterea mesajului de cere parola");
	return;
	}
	if(read(client,rasp,sizeof(rasp))<=0)
	{
	perror("Eroare la citirea parolei");
	return;
	}

	printf("Acum am citit %s",rasp);
strcpy(parola,rasp);
bzero(rasp,strlen(rasp));
FILE* fisier=fopen("utilizatori.txt","r");
                if(fisier==NULL)
                    {
                        perror("Eroare la deschiderea fisierului");
                        exit(EXIT_FAILURE);
                    }   
int ok=0;                    
while(fgets(rasp,sizeof(rasp),fisier)!=NULL)
{

    char numedeutilizator[100], mail[100], numartelefon[100], categorie[100],paroladinfisier[100];


if(sscanf(rasp,"Nume: %[^,],Parola:%[^,],NumarTelefon:%[^,],Mail:%[^,],Tip_Client:%[^\n]",numedeutilizator,paroladinfisier,numartelefon,mail,categorie)==5)
	{
	printf("%s\n",numedeutilizator);
	printf("%s\n",paroladinfisier);
	printf("%s\n",categorie);
	strcpy(tipclient,categorie);
	if(strcmp(nume,numedeutilizator)==0 && strcmp(parola,paroladinfisier)==0)
	{
	strcpy(numedeut,nume);
	ok=1;
	break;
}
}
}
fclose(fisier);
printf("%s\n",nume);
printf("%s\n",parola);
const char*raspafirmativ="Conectare realizata cu succes!";
const char* raspnegativ="Username/parola gresite!";


if(ok==1)
{	
	if(strcmp(tipclient,"client")==0 || strcmp(tipclient,"Companie_caritabila")==0)
	*conexiune=1;
	else
	*conexiune=2;
	if(write(client,raspafirmativ,strlen(raspafirmativ))<=0)
	{
	
	perror("Eroare la trimiterea mesajului de succes");
	return;
	}
}
else
	{*conexiune=0;
	if(write(client,raspnegativ,strlen(raspnegativ))<=0)
	{
	perror("Eroare la trimiterea mesajului de eroare");
	return;
	}
	}
}

// ... (existing code)

// ... (existing code)

void InventarNou(int client, const char *name) {
    char response[150];  // Increased buffer size
    FILE *inventarFile;
    FILE *allProduceFile;
    /*
  */
    // Open the file with the given name in write-only mode
    inventarFile = fopen(name, "r");
    if (inventarFile == NULL) {
        perror("Eroare la deschiderea fisierului de inventar");
        snprintf(response, sizeof(response), "Eroare la deschiderea fisierului de inventar %s!\n", name);
        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului de inventar");
        }
        return;
    }

    // Open the larger file to store all the produce information
    allProduceFile = fopen("all_produce.txt", "a");
    if (allProduceFile == NULL) {
        perror("Eroare la deschiderea fisierului pentru toate produsele");
        snprintf(response, sizeof(response), "Eroare la deschiderea fisierului pentru toate produsele!\n");
        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului pentru toate produsele");
        }
        fclose(inventarFile);
        return;
    }

    fseek(allProduceFile, 0, SEEK_END);  // Move to the end of the file

 //fseek(inventarFile, 0, SEEK_SET);
    char buffer[100];
// Copy existing inventory from inventarFile to allProduceFile
while (fgets(buffer, sizeof(buffer), inventarFile) != NULL) {
    fprintf(allProduceFile, "%s %s", name,buffer);
}

fclose(inventarFile);
inventarFile=fopen(name,"w");
if (inventarFile == NULL) {
        perror("Eroare la deschiderea fisierului de inventar");
        snprintf(response, sizeof(response), "Eroare la deschiderea fisierului de inventar %s!\n", name);
        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului de inventar");
        }
        return;
    }
    strcpy(response, "Va rog sa trimiteti mesaje de forma <produs>,<cantitate> sau gata pentru a termina");
    if (write(client, response, strlen(response)) <= 0) {
        perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului pentru toate produsele");
    }

    // Request pairs from the client until "gata" is received
    while (1) {
        char inventarPair[100];
        bzero(inventarPair, sizeof(inventarPair));

        // Assuming the client sends pairs in the format "<produce>,<quantity>"
        if (read(client, inventarPair, sizeof(inventarPair)) <= 0) {
            perror("[server]Eroare la citirea perechii de inventar");
            break;
        }

        // Check for the termination condition
        if (strcmp(inventarPair, "gata") == 0) {
            break;
        }

        // Write the pair to the individual inventory file
        fprintf(inventarFile, "%s\n", inventarPair);
        // Append the pair to the all_produce.txt file
        // Send a confirmation message to the client
        snprintf(response, sizeof(response), "Pereche de inventar %s adaugata cu succes!\n", inventarPair);
        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de adaugare a perechii de inventar");
            break;
        }
    }

    // Close the files
    fclose(inventarFile);
    fclose(allProduceFile);

    snprintf(response, sizeof(response), "Fisierul de inventar %s a fost actualizat cu succes!\n", name);
    if (write(client, response, strlen(response)) <= 0) {
        perror("[server]Eroare la trimiterea raspunsului de actualizare a fisierului de inventar");
    }
}




// ... (existing code)
void ModificaPromotii(int client, const char *storeName) {
    char response[150];  // Increased buffer size

    // Open the file for the specific store's promotions
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "%s_promotions.txt", storeName);

    FILE *promotionsFile = fopen(fileName, "w");  // Open in write mode

    if (promotionsFile == NULL) {
        perror("Eroare la deschiderea fisierului de promotii");
        snprintf(response, sizeof(response), "Eroare la deschiderea fisierului de promotii pentru magazinul %s!\n", storeName);

        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului de promotii");
        }

        return;
    }

    // Request new promotions from the client until "gata" is received
    snprintf(response, sizeof(response), "Va rog sa trimiteti promotiile noi sau gata pentru a termina\n");
    if (write(client, response, strlen(response)) <= 0) {
        perror("[server]Eroare la trimiterea raspunsului de solicitare a promotiilor noi");
        fclose(promotionsFile);
        return;
    }

    while (1) {
        char promotion[100];
        bzero(promotion, sizeof(promotion));

        // Assuming the client sends promotions in the format "<product>: <discount>"
        if (read(client, promotion, sizeof(promotion)) <= 0) {
            perror("[server]Eroare la citirea promotiei");
            break;
        }

        // Check for the termination condition
        if (strcmp(promotion, "gata") == 0) {
            break;
        }

        // Write the promotion to the store's promotions file
        fprintf(promotionsFile, "%s\n", promotion);

        // Send a confirmation message to the client
        snprintf(response, sizeof(response), "Promotie %s adaugata cu succes!\n", promotion);
        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de adaugare a promotiei");
            break;
        }
    }

    // Close the file
    fclose(promotionsFile);

    snprintf(response, sizeof(response), "Promotiile pentru magazinul %s au fost actualizate cu succes!\n", storeName);
    if (write(client, response, strlen(response)) <= 0) {
        perror("[server]Eroare la trimiterea raspunsului de actualizare a promotiilor");
    }
}

void displayPromotions(int client) {
    FILE *storesFile = fopen("magazine.txt", "r");

    if (storesFile == NULL) {
        perror("Eroare la deschiderea fisierului magazine.txt");
        return;
    }

    char storeName[100];

    // Read store names from magazine.txt
    while (fgets(storeName, sizeof(storeName), storesFile) != NULL) {
        // Remove newline character from the store name
        storeName[strcspn(storeName, "\n")] = 0;

        // Construct the promotion file name
        char promotionFileName[120];
        snprintf(promotionFileName, sizeof(promotionFileName), "%s_promotions.txt", storeName);

        FILE *promotionFile = fopen(promotionFileName, "r");

        if (promotionFile == NULL) {
            perror("Eroare la deschiderea fisierului de promotii");
            continue;  // Skip to the next store if the promotion file cannot be opened
        }

        // Display promotions for the current store
       char mesajIntermediar[2000];
        bzero(mesajIntermediar,2000);
        snprintf(mesajIntermediar,sizeof(mesajIntermediar),"Promotii pentru magazinul/restaurantul %s:\n", storeName);

        char promotion[1000];
        int count=0;
        while (fgets(promotion, sizeof(promotion), promotionFile) != NULL) {
        	
        	if(write(client,mesajIntermediar,strlen(mesajIntermediar))<=0){
	perror("Eroare la trimiterea mesajului intermediar");
	break;
	}
	
            // Remove newline character from the promotion entry
		printf("Acum trimitem: %s", promotion);
             if (write(client, promotion, strlen(promotion)) <= 0) {
                perror("Eroare la trimiterea promotiei catre client");
                break;
            }
        }

        fclose(promotionFile);
       
    }

    fclose(storesFile);
}
void displayUnsoldProduce(int client) {
    char response[150];
    FILE *allProduceFile = fopen("all_produce.txt", "r");

    if (allProduceFile == NULL) {
        perror("Eroare la deschiderea fisierului all_produce.txt");
        snprintf(response, sizeof(response), "Eroare la deschiderea fisierului all_produce.txt!\n");

        if (write(client, response, strlen(response)) <= 0) {
            perror("[server]Eroare la trimiterea raspunsului de deschidere a fisierului all_produce.txt");
        }

        return;
    }

    // Display and send produce information to the client
    snprintf(response, sizeof(response), "Produse nevandute:\n");
    if (write(client, response, strlen(response)) <= 0) {
        perror("[server]Eroare la trimiterea raspunsului de afisare a produselor nevandute");
        fclose(allProduceFile);
        return;
    }

    char produce[100];
    while (fgets(produce, sizeof(produce), allProduceFile) != NULL) {
        // Remove newline character from the produce entry


        // Send the produce information to the client
        if (write(client, produce, strlen(produce)) <= 0) {
            perror("[server]Eroare la trimiterea informatiilor despre produsele nevandute");
            break;
        }
    }

    fclose(allProduceFile);
}

 char utilizator[100]; 
int main()
{
    int statusconexiune=0;
    struct sockaddr_in server;	
    struct sockaddr_in from;
    char msg[100];		
    char msgrasp[100]=" ";
          
    int sd;			

    
		    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
			    {
			    	perror ("[server]Eroare la socket().\n");
			    	return errno;
			    }

    
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));
    
    server.sin_family = AF_INET;    
    server.sin_addr.s_addr = htonl (INADDR_ANY);    
    server.sin_port = htons (PORT);

    
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
	    {
	    	perror ("[server]Eroare la bind().\n");
	    	return errno;
	    }
  
    if (listen (sd, 3) == -1)
	    {
	    	perror ("[server]Eroare la listen().\n");
	    	return errno;
	    }

    
    while (1)
    {
    	int client;
    	int length = sizeof (from);

    	printf ("[server]Asteptam la portul %d...\n",PORT);
    	fflush (stdout);   	  
		client = accept (sd, (struct sockaddr *) &from, &length);    	
    	if (client < 0)
    	{
    		perror ("[server]Eroare la accept().\n");
    		continue;
    	}

    	int pid;
    	if ((pid = fork()) == -1) {
    		close(client);
    		continue;
    	} else if (pid > 0) {    		
    		close(client);
    		while(waitpid(-1,NULL,WNOHANG));
    		continue;
    	} else if (pid == 0) {    		
    		close(sd);
		while(1)
		{	    		
	    		bzero (msg, 100);
	    		printf ("[server]Asteptam mesajul...\n");
	    		fflush (stdout);		    	    		    		
	    		if (read (client, msg, 100) <= 0)
	    		{
	    			perror ("[server]Eroare la read() de la client.\n");
	    			close (client);	
	    			continue;		
	    		}

	    		printf ("[server]Mesajul a fost receptionat...%s\n", msg);
		    if(strncmp(msg,"login",5)==0 && statusconexiune==0)
		    {
		       char user[100];
		       login(client,&statusconexiune, utilizator);
		        printf("Utilizatorul conectat este:%s\n",utilizator);
		        printf("%d",statusconexiune);
		    }
		    else
		    if(strcmp(msg,"register")==0)
		        {
		       
		            intermediarptclient(client);
		        
		        }
		    else
		    if(statusconexiune==2 && strcmp(msg,"Inventar_nou")==0)//daca tipul de client este un magazin
		        {
		    InventarNou(client,utilizator);
		            //adauga la baza de date ce produse are disponibile, sau care au fost vandute
		            //atat si termenul limita de expirare                
		        }
		    else
		    if(statusconexiune==1&&strcmp(msg,"Produse_ce_ar_fi_risipite")==0)
		        {
		    displayUnsoldProduce(client);
		            
		        }            
		     else
		        if(statusconexiune==1 && strcmp(msg,"Promotii")==0)
		            {
		            displayPromotions(client);

		            }
		        else
		            if(statusconexiune==2&&strcmp(msg,"Info_magazin")==0)
		                {
		       
		                    ModificaPromotii(client,utilizator);
		                    //reduceri la produse cu pretul nou si cat la suta este oferta
		                }
		        else
		            if(strcmp(msg,"exit")==0)
		                {
		                   printf("Am primit exit\n");
		                        	if (write (client, "Am primit exit", 14) <= 0)
	    		{
	    			
	    			perror ("[server]Eroare la write() catre client.\n");
	    			continue;		
	    		}
		                    close(client);
		                    exit(0);
		                }
		                else
		                if(strncmp(msg,"login",5)==0 && statusconexiune!=0)
		    {
		      char raspuns[166];
		      snprintf(raspuns,sizeof(raspuns),"Sunteti deja conectat pe profilul %s\n",utilizator);
		    if(write(client,raspuns,strlen(raspuns))<=0)
		    {perror("[server] Eroare la write()\n");
		    continue;
		    
		    }
		    }
		    	else
		    	if(strncmp(msg,"logout",6)==0 && statusconexiune!=0)
		    	{
		    	char raspuns[166];
		      snprintf(raspuns,sizeof(raspuns),"Vati deconectat de pe profilul %s\n",utilizator);
		      if(write(client,raspuns,strlen(raspuns))<=0)
		    {perror("[server] Eroare la write()\n");
		    continue;
		    
		    }
		    	statusconexiune=0;
		    	bzero(utilizator,100);
		    	}
		                else
		                {
		                if (write (client, "Nu exista comanda", 17) <= 0)
	    		{
	    			
	    			perror ("[server]Eroare la write() catre client.\n");
	    			continue;		
	    		}
	    		
		                }
	    	}	    		
       
    	}

    }				
}				
