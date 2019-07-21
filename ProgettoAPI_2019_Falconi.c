/*  Progetto di API anno accademico 2018-2019   *
 *       studente Matteo Falconi, 866146        *
 *        matteo1.falconi@mail.polimi.it        *
 */

#include <stdio.h>
#include <string.h>
#define MAX_LEN 33

//Sostituisce le virgolette a fine parola con il carettere terminatore.
void parseWord (char *word) {
    int len, i;
    len = strlen(word);
    for (i = 0; i <= len; i++) {
        if (word[i] == '"') {
            word[i] = '\0';
        }
    }
} 

int main () {

    //Variabili
    char comand[7], rel[MAX_LEN], ent1[MAX_LEN], ent2[MAX_LEN], temp;
    int len, loop;
    int i = 0;
    int ext_loop = 1;

    //External Loop: cicla fino alla fine dell'esecuzione del programma; ovvero fino a che viene letto END.
    while (ext_loop == 1) {
        loop = 1;
        
        //Leggo il comnado.
        while (loop) {
            temp = getchar();
            if (temp != '\n' || temp != 'EOF') {
                if (temp == ' ') {
                    comand[i] = '\0';
                    break;
                }
                comand[i] = temp;
                i++;
            }
        }

        //Confronto il comando con i casi noti.
        if (strcmp(comand, "end") == 0) {
            ext_loop = 0;
            break;
        }
        //Se ADDENT o DELENT, devo leggere l'entità.
        else if (strcmp (comand, "addent") == 0 || strcmp(comand, "delent") == 0) {
            scanf("\"%s", &ent1);
            parseWord(ent1);
            if (comand[0] == 'a') {
            
                //todo -> aggiungi ent a struttura
            } else {

                //todo -> rimuovi ent da struttura e rimuovi tutti i suoi collegamenti
            }
        }
        //Se ADDREL o DELREL, devo leggere la relazione e le due entità
        else if (strcmp(comand, "addrel") == 0 || strcmp(comand, "delrel") == 0) {
            /* ent1 -> entità che genere relazione
                * ent2 -> entità che subisce relazione
                * rel -> tipo di relazione
                */
            scanf("\"%s \"%s \"%s", &ent1, &ent2, &rel);
            parseWord(ent1);
            parseWord(ent2);
            parseWord(rel);

            if (comand[0] == 'a') {

                //todo -> aggiungi relazione. (Controlla prima presenza delle due entità)
            }
            else {

                //todo -> rimuovi relazione.
            }
        }
        //Se REPORT, stampo il tutto
        else if (strcmp(comand, "report") == 0) {

            //todo
        }
    }

}
