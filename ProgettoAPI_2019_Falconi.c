/*  Progetto di API anno accademico 2018-2019   *
 *       studente Matteo Falconi, 866146        *
 *        matteo1.falconi@mail.polimi.it        *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 33
#define RED 'R'
#define BLACK 'B'

//Strutture

//Lista bidirezionale per salvare
typedef struct list_s {
    char *ent;
    char *rel;
    struct list_s * next;
    struct list_s * prev;
} list_t;

//Nodo per le entità
typedef struct rb_node_ent_s {
    char *key;
    char color;
    struct rb_node_ent_s *left;
    struct rb_node_ent_s *right;
    struct rb_node_ent_s *parent;
    list_t *sub_rel;
} rb_node_ent;

//Nodo per le relazioni
typedef struct rb_node_rel_s {
    char *key;
    char color;
    struct rb_node_rel_s *left;
    struct rb_node_rel_s *right;
    struct rb_node_rel_s *parent;
} rb_node_rel;

//Struttura dell'albero entità
typedef struct rb_tree_ent_s {
    rb_node_ent *root;
    rb_node_ent *nil;
} rb_tree_ent;

//Struttura dell'albero relazioni
typedef struct rb_tree_rel_s {
    rb_node_rel *root;
    rb_node_rel *nil;
} rb_tree_rel;

/* Inizializzazione Alberi e Nodi */

//Inizializazzione albero entità
static inline rb_tree_ent * Create_RBTree_ent() {
    rb_tree_ent * ent_T;
    rb_node_ent *nil;
    nil = (rb_node_ent *)malloc(sizeof(rb_node_ent));
    ent_T = (rb_tree_ent *)malloc(sizeof(rb_tree_ent));
    if (nil != NULL && ent_T != NULL) {
        ent_T -> root = nil;
        ent_T -> nil = nil;
        nil -> key = NULL;
        nil -> color = BLACK;
        nil -> left = NULL;
        nil -> right = NULL;
        nil -> parent = ent_T -> root;
        return ent_T;
    }
    else {
        return NULL;
    }
}

//Inizializazzione albero relazioni
static inline rb_tree_rel * Create_RBTree_rel() {
    rb_tree_rel * rel_T;
    rb_node_rel * nil;
    nil = (rb_node_rel *)malloc(sizeof(rb_node_rel));
    rel_T = (rb_tree_rel *)malloc(sizeof(rb_tree_rel));
    if (nil != NULL && rel_T != NULL) {
        rel_T -> root = nil;
        rel_T -> nil = nil;
        nil -> key = NULL;
        nil -> color = BLACK;
        nil -> left = NULL;
        nil -> right = NULL;
        nil -> parent = rel_T -> root;
        return rel_T;
    }
    else {
        return NULL;
    }
}

//Inizializzazione Nodo entità
static inline rb_node_ent * Create_Node_ent(char *ent_name) {
    char *key;
    rb_node_ent *new_node;
    int len, j;
    len = strlen(ent_name);
    key = (char *)malloc(sizeof(sizeof(len) +1));
    for (j = 0; j < len; j++) {
    	key[j] = ent_name[j];
    }
    key[len] = '\0';
    new_node = (rb_node_ent *)malloc(sizeof(rb_node_ent));
    new_node -> key = key;
    new_node -> sub_rel = NULL;
    return new_node;
}

//Inizializzazione Nodo relazione
static inline rb_node_rel * Create_Node_rel(char *rel_name) {
    char *key;
    rb_node_rel *new_node;
    int len, j;
    len = strlen(rel_name);
    key = (char *)malloc(sizeof(sizeof(len) +1));
    for (j = 0; j < len; j++) {
    	key[j] = rel_name[j];
    }
    key[len] = '\0';
    new_node = (rb_node_rel *)malloc(sizeof(rb_node_rel));
    new_node -> key = key;
    return new_node;
}

/* Utilities degli alberi Rosso Neri */

//Minimo entità
static inline rb_node_ent * rb_ent_minimum(rb_tree_ent * T, rb_node_ent * node) {
    while (node -> left != T -> nil) {
    	node = node -> left;
    }
    return node;
}

//Minimo relazione
static inline rb_node_rel * rb_rel_minimum(rb_tree_rel * T, rb_node_rel * node) {
    while (node -> left != T -> nil) {
    	node = node -> left;
    }
    return node;
}

//Left Rotate albero entità
static inline void rb_ent_left_rotate(rb_tree_ent * T, rb_node_ent *node) {
    rb_node_ent *new;
    new = node -> right;
    node -> right = new -> left;
    if (node -> right != T -> nil) {
    	T -> root = new;
    } else if ( node == node -> parent ->left) {
        node ->parent ->left = new;
    } else {
        node -> parent -> right = new;
    }
    new -> left = node;
    node -> parent = new;
}

//Left Rotate albero relazioni
static inline void rb_rel_left_rotate(rb_tree_rel * T, rb_node_rel *node) {
    rb_node_rel *new;
    new = node -> right;
    node -> right = new -> left;
    if (node -> right != T -> nil) {
    	T -> root = new;
    } else if ( node == node -> parent ->left) {
        node ->parent ->left = new;
    } else {
        node -> parent -> right = new;
    }
    new -> left = node;
    node -> parent = new;
}

//Right Rotate albero entità
static inline void rb_ent_right_rotate(rb_tree_ent * T, rb_node_ent *node) {
    rb_node_ent * new;
    new = node -> left;
    node -> left = new -> right;
    if (node -> left != T -> nil) {
    	new -> right -> parent = node;
    }
    new -> parent = node -> parent;
    if (new -> parent == T -> nil) {
    	T -> root = new;
    } else if (node == node -> parent -> right) {
    	node ->parent ->right = new;
    } else {
    	node -> parent -> left = new;
    }
    new -> right = node;
    node -> parent = new;
}

//Right Rotate albero relazioni
static inline void rb_rel_right_rotate(rb_tree_rel * T, rb_node_rel *node) {
    rb_node_rel * new;
    new = node -> left;
    node -> left = new -> right;
    if (node -> left != T -> nil) {
    	new -> right -> parent = node;
    }
    new -> parent = node -> parent;
    if (new -> parent == T -> nil) {
    	T -> root = new;
    } else if (node == node -> parent -> right) {
    	node ->parent ->right = new;
    } else {
    	node -> parent -> left = new;
    }
    new -> right = node;
    node -> parent = new;
}

//Insert FixUp albero entità
static inline void rb_ent_insert_fixup(rb_tree_ent * T, rb_node_ent *node) {
    rb_node_ent *new;
    while (node -> parent -> color == RED) {
    	if (node -> parent == node -> parent -> parent -> left) {
    	    new = node -> parent -> parent -> right;
    	    if (new -> color == RED) {
    	    	node -> parent -> color = BLACK;
    	    	new -> color = BLACK;
    	    	node -> parent -> parent ->color = RED;
    	    	node = node -> parent ->parent;
    	    } else {
    	    	if ( node == node -> parent -> right) {
    	    	    node = node -> parent;
    	    	    rb_ent_right_rotate (T, node);
    	    	}
    	    	node -> parent -> color = BLACK;
    	    	node -> parent -> parent -> color = RED;
    	    	rb_ent_right_rotate(T, node -> parent -> parent);
    	    }
    	} else {
    	    new = node -> parent -> parent -> left;
    	    if (new -> color == RED) {
    	    	node -> parent -> color = BLACK;
    	    	new -> color = BLACK;
    	    	node -> parent -> parent -> color = RED;
    	    	node = node -> parent -> parent;
    	    } else {
    	    	if ( node == node ->parent ->left) {
    	    	    node = node -> parent;
    	    	    rb_ent_right_rotate(T, node);
    	    	}
    	    	node -> parent -> color = BLACK;
    	    	node -> parent -> parent ->color = RED;
    	    	rb_ent_left_rotate(T, node -> parent -> parent);
    	    }
    	}
    }
    if (T -> root -> color != BLACK) {
    	T -> root -> color = BLACK;
    }
}

//Insert FixUp albero relazioni
static inline void rb_rel_insert_fixup(rb_tree_rel * T, rb_node_rel *node) {
    rb_node_rel *new;
    while (node -> parent -> color == RED) {
    	if (node -> parent == node -> parent -> parent -> left) {
    	    new = node -> parent -> parent -> right;
    	    if (new -> color == RED) {
    	    	node -> parent -> color = BLACK;
    	    	new -> color = BLACK;
    	    	node -> parent -> parent ->color = RED;
    	    	node = node -> parent ->parent;
    	    } else {
    	    	if ( node == node -> parent -> right) {
    	    	    node = node -> parent;
    	    	    rb_rel_right_rotate (T, node);
    	    	}
    	    	node -> parent -> color = BLACK;
    	    	node -> parent -> parent -> color = RED;
    	    	rb_rel_right_rotate(T, node -> parent -> parent);
    	    }
    	} else {
    	    new = node -> parent -> parent -> left;
    	    if (new -> color == RED) {
    	    	node -> parent -> color = BLACK;
    	    	new -> color = BLACK;
    	    	node -> parent -> parent -> color = RED;
    	    	node = node -> parent -> parent;
    	    } else {
    	    	if ( node == node ->parent ->left) {
    	    	    node = node -> parent;
    	    	    rb_rel_right_rotate(T, node);
    	    	}
    	    	node -> parent -> color = BLACK;
    	    	node -> parent -> parent ->color = RED;
    	    	rb_rel_left_rotate(T, node -> parent -> parent);
    	    }
    	}
    }
    if (T -> root -> color != BLACK) {
    	T -> root -> color = BLACK;
    }
}

//Transplant alberi entità
static inline void rb_ent_transplant( rb_tree_ent *T, rb_node_ent * first, rb_node_ent * second) {
    if (first -> parent == T -> nil) {
    	T -> root = second;
    } else if ( first == first -> parent -> left) {
    	first -> parent -> left = second;
    } else {
    	first -> parent -> right = second;
    }
    second -> parent = first -> parent;
}

//Transplant alberi relazioni
static inline void rb_rel_transplant( rb_tree_rel *T, rb_node_rel * first, rb_node_rel * second) {
    if (first -> parent == T -> nil) {
    	T -> root = second;
    } else if ( first == first -> parent -> left) {
    	first -> parent -> left = second;
    } else {
    	first -> parent -> right = second;
    }
    second -> parent = first -> parent;
}

//Delete FixUp per alberi entità
static inline void rb_ent_delete_fixup(rb_tree_ent * T, rb_node_ent * node) {
    rb_node_ent * new;
    while (node != T -> root && node -> color == BLACK) {
    	if (node == node -> parent -> left) {
    	    new = node -> parent -> right;
    	    if (node -> color == RED) { //node o new? todo to check
    	    	new -> color = BLACK;
    	    	node -> parent -> color = RED;
    	    	rb_ent_left_rotate(T, node -> parent);
    	    	new = node -> parent -> right;
    	    }
    	    if ( new -> left -> color == BLACK && new -> right -> color == BLACK) {
    	   	new -> color = RED;
    	   	node = node -> parent;
    	    } else {
    	    	if (new -> right -> color == BLACK) {
    	    	    new -> left -> color = BLACK;
    	    	    new ->color = RED;
    	    	    rb_ent_right_rotate( T, new);
    	    	    new = node -> parent -> right;
    	    	}
    	    	new -> color = node -> parent -> color;
    	    	node -> parent -> color = BLACK;
    	    	new -> right -> color = BLACK;
    	    	rb_ent_left_rotate( T, node -> parent);
    	    	node = T -> root;
    	    }
    	} else {
	    new = node -> parent -> left;
	    if (node -> color == RED) { //node o new? todo to check
	    	new -> color = BLACK;
	    	node -> parent -> color = RED;
	    	rb_ent_right_rotate(T, node -> parent);
	    	new = node -> parent -> left;
	    }
	    if ( new -> right -> color == BLACK && new -> left -> color == BLACK) {
	   	new -> color = RED;
	   	node = node -> parent;
	    } else {
	    	if (new -> left -> color == BLACK) {
	    	    new -> right -> color = BLACK;
	    	    new ->color = RED;
	    	    rb_ent_left_rotate( T, new);
	    	    new = node -> parent -> left;
	    	}
	    	new -> color = node -> parent -> color;
	    	node -> parent -> color = BLACK;
	    	new -> left -> color = BLACK;
	    	rb_ent_right_rotate( T, node -> parent);
	    	node = T -> root;
	    }
    	}
    }
}

//Delete FixUp per alberi relazioni
static inline void rb_rel_delete_fixup(rb_tree_rel * T, rb_node_rel * node) {
    rb_node_rel * new;
    while (node != T -> root && node -> color == BLACK) {
    	if (node == node -> parent -> left) {
    	    new = node -> parent -> right;
    	    if (node -> color == RED) { //node o new? todo to check
    	    	new -> color = BLACK;
    	    	node -> parent -> color = RED;
    	    	rb_rel_left_rotate(T, node -> parent);
    	    	new = node -> parent -> right;
    	    }
    	    if ( new -> left -> color == BLACK && new -> right -> color == BLACK) {
    	   	new -> color = RED;
    	   	node = node -> parent;
    	    } else {
    	    	if (new -> right -> color == BLACK) {
    	    	    new -> left -> color = BLACK;
    	    	    new ->color = RED;
    	    	    rb_rel_right_rotate( T, new);
    	    	    new = node -> parent -> right;
    	    	}
    	    	new -> color = node -> parent -> color;
    	    	node -> parent -> color = BLACK;
    	    	new -> right -> color = BLACK;
    	    	rb_rel_left_rotate( T, node -> parent);
    	    	node = T -> root;
    	    }
    	} else {
	    new = node -> parent -> left;
	    if (node -> color == RED) { //node o new? todo to check
	    	new -> color = BLACK;
	    	node -> parent -> color = RED;
	    	rb_rel_right_rotate(T, node -> parent);
	    	new = node -> parent -> left;
	    }
	    if ( new -> right -> color == BLACK && new -> left -> color == BLACK) {
	   	new -> color = RED;
	   	node = node -> parent;
	    } else {
	    	if (new -> left -> color == BLACK) {
	    	    new -> right -> color = BLACK;
	    	    new ->color = RED;
	    	    rb_rel_left_rotate( T, new);
	    	    new = node -> parent -> left;
	    	}
	    	new -> color = node -> parent -> color;
	    	node -> parent -> color = BLACK;
	    	new -> left -> color = BLACK;
	    	rb_rel_right_rotate( T, node -> parent);
	    	node = T -> root;
	    }
    	}
    }
}

//Inserisci albero entità
void rb_ent_insert(rb_tree_ent * T, rb_node_ent * new) {
    int res;
    rb_node_ent *one, *two;
    if (!new) {
    	return;
    }
    two = T -> nil;
    one = T -> root;
    while (one != T -> nil ) {
    	two = one;
    	res = strcmp (new -> key , one -> key);
    	if (res == 0) {
    	    return;
    	}
    	if (res < 0) {
    	    one = one -> left;
    	}
    	if (res > 0) {
    	    one = one -> right;
    	}
    }
    new -> parent = two;
    if (two == T -> nil) {
    	T -> root = new;
    } else if (strcmp(new -> key, two -> key) < 0 ) {
    	two -> left = new;
    } else {
    	two -> right = new;
    }
    new -> left = T -> nil;
    new -> right = T -> nil;
    new -> color = RED;
    rb_ent_insert_fixup(T, new);
    return;
}

//Inserisci albero relazioni
void rb_rel_insert(rb_tree_rel * T, rb_node_rel * new) {
    int res;
    rb_node_rel *one, *two;
    if (!new) {
    	return;
    }
    two = T -> nil;
    one = T -> root;
    while (one != T -> nil ) {
    	two = one;
    	res = strcmp (new -> key , one -> key);
    	if (res == 0) {
    	    return;
    	}
    	if (res < 0) {
    	    one = one -> left;
    	}
    	if (res > 0) {
    	    one = one -> right;
    	}
    }
    new -> parent = two;
    if (two == T -> nil) {
    	T -> root = new;
    } else if (strcmp(new -> key, two -> key) < 0 ) {
    	two -> left = new;
    } else {
    	two -> right = new;
    }
    new -> left = T -> nil;
    new -> right = T -> nil;
    new -> color = RED;
    rb_rel_insert_fixup(T, new);
    return;
}

//Cancella nodo albero entità
void rb_ent_delete (rb_tree_ent * T, rb_node_ent * node) {
    if (!node) {
    	return;
    }
    rb_node_ent * two, * one;
    char two_original_color;
    two = node;
    two_original_color = two -> color;
    if (node -> left == T-> nil) {
    	one = node -> right;
    	rb_ent_transplant(T, node, node -> right);
    } else if (node -> right == T -> nil) {
    	one = node -> left;
    	rb_ent_transplant ( T, node, node -> left);
    } else {
        two = rb_ent_minimum (T, node -> right);
        two_original_color = two -> color;
        one = two -> right;
        if (two -> parent == node) {
            one -> parent = two;
        } else {
            rb_ent_transplant(T, two, two -> right);
            two -> right = node -> right;
            two -> right -> parent = two;
        }
        rb_ent_transplant(T, node, two);
        two -> left = node -> left;
        two -> left -> parent = two;
        two -> color = node -> color;
    }
    if (two_original_color == BLACK) {
    	rb_ent_delete_fixup(T, one);
    }
    //todo free per tutti gli elementi della lista
    free(node -> key);
    free(node);
    return;
}

//Cancella nodo albero relazione
void rb_rel_delete (rb_tree_rel * T, rb_node_rel * node) {
    if (!node) {
    	return;
    }
    rb_node_rel * two, * one;
    char two_original_color;
    two = node;
    two_original_color = two -> color;
    if (node -> left == T-> nil) {
    	one = node -> right;
    	rb_rel_transplant(T, node, node -> right);
    } else if (node -> right == T -> nil) {
    	one = node -> left;
    	rb_rel_transplant ( T, node, node -> left);
    } else {
        two = rb_rel_minimum (T, node -> right);
        two_original_color = two -> color;
        one = two -> right;
        if (two -> parent == node) {
            one -> parent = two;
        } else {
            rb_rel_transplant(T, two, two -> right);
            two -> right = node -> right;
            two -> right -> parent = two;
        }
        rb_rel_transplant(T, node, two);
        two -> left = node -> left;
        two -> left -> parent = two;
        two -> color = node -> color;
    }
    if (two_original_color == BLACK) {
    	rb_rel_delete_fixup(T, one);
    }
    free(node -> key);
    free(node);
    return;
}

//Trova nodo albero entità
static inline rb_node_ent * rb_ent_find (rb_tree_ent * T, char * key) {
    int len;
    rb_node_ent * temp;
    temp = T -> root;
    while (temp != T-> nil && temp -> key) {
    	len = strcmp(key, temp -> key);
    	if (len == 0) {
    	    return temp;
    	} else if (temp < 0) {
    	    temp = temp -> left;
    	} else {
    	    temp = temp -> right;
    	}
    }
    return NULL;
}

//Trova nodo albero relazione
static inline rb_node_rel * rb_rel_find (rb_tree_rel * T, char * key) {
    int len;
    rb_node_rel * temp;
    temp = T -> root;
    while (temp != T-> nil && temp -> key) {
    	len = strcmp(key, temp -> key);
    	if (len == 0) {
    	    return temp;
    	} else if (temp < 0) {
    	    temp = temp -> left;
    	} else {
    	    temp = temp -> right;
    	}
    }
    return NULL;
}

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
    char *comand, rel[MAX_LEN], ent1[MAX_LEN], ent2[MAX_LEN], temp;
    int len, loop, isRead;
    int i = 0;
    int ext_loop = 1;
    rb_tree_ent * T_ent;
    rb_tree_rel * T_rel;
    T_ent = Create_RBTree_ent();
    T_rel = Create_RBTree_rel();

    //External Loop: cicla fino alla fine dell'esecuzione del programma; ovvero fino a che viene letto END.
    while (ext_loop == 1) {
        loop = 1;
        
        comand = (char *)calloc(7, sizeof(char));
        //DEPRECATED
        //Leggo il comnado.
        /*while (loop) {
            temp = getchar();
            if (temp == EOF) {
            	return 0;
            	}
            if (temp != '\n') {
                if (temp == ' ') {
                    comand[i] = '\0';
                    loop = 0;
                    break;
                }
                else {
                    comand[i] = temp;
                }
            }
            i++;
        }*/
        scanf("%s", comand);

        //printf("%s\n", comand);


        //Confronto il comando con i casi noti.
        if (strcmp(comand, "end") == 0) {
            ext_loop = 0;
            return 0;
        }
        //Se ADDENT o DELENT, devo leggere l'entità.
        else if (strcmp (comand, "addent") == 0 || strcmp(comand, "delent") == 0) {
            scanf(" \"%s", ent1);
                parseWord(ent1);
                if (comand[0] == 'a') {
                if (rb_ent_find( T_ent, ent1) == NULL) {
                    rb_ent_insert(T_ent, Create_Node_ent(ent1));
                }
                    //printf("Aggiunto %s\n", ent1);
                    //todo -> aggiungi ent a struttura
                } else {
                	//printf("Rimosso %s \n", ent1);
                    //todo -> rimuovi ent da struttura e rimuovi tutti i suoi collegamenti
                }
        }
        //Se ADDREL o DELREL, devo leggere la relazione e le due entità
        else if (strcmp(comand, "addrel") == 0 || strcmp(comand, "delrel") == 0) {
            /* ent1 -> entità che genere relazione
                * ent2 -> entità che subisce relazione
                * rel -> tipo di relazione
                */
            scanf(" \"%s \"%s \"%s", ent1, ent2, rel);
                parseWord(ent1);
                parseWord(ent2);
                parseWord(rel);

                if (comand[0] == 'a') {
                	//printf("Aggiunto %s %s con rel: %s\n", ent1, ent2, rel);
                    //todo -> aggiungi relazione. (Controlla prima presenza delle due entità)
                }
                else {
                	//printf("Rimosso %s %s con rel: %s\n", ent1, ent2, rel);
                    //todo -> rimuovi relazione.
                }
        }
        //Se REPORT, stampo il tutto
        else if (strcmp(comand, "report") == 0) {
		    //printf("Ho letto Report \n");
            //todo
        }
        free(comand);
    }

}
