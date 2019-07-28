#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 34
#define RED 'r'
#define BLACK 'b'

typedef struct rel_list_s {
    char * rel_name;
    int number_of_entities;
    struct ent_list_s *ent_list;
    struct rel_list_s *next;
}rel_list;

typedef struct rb_node_s {
    char *key;
    char color;
    struct rb_node_s *left, *right, *parent;
    rel_list *relations;
} rb_node;

typedef  struct ent_list_s {
    rb_node *ent_node;
    struct ent_list_s *next;
}ent_list;

typedef struct rb_tree_s {
    rb_node *root, *nil;
}rb_tree;

void Left_Rotate (rb_tree * T, rb_node * x){
    rb_node * y = x -> right;
    x -> right = y -> left;
    if (y -> left != T -> nil) {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == T -> nil) {
        T->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void Right_Rotate (rb_tree * T, rb_node * y) {
    rb_node * x = y -> left;
    y->left = x->right;
    if (x->right != T->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == T->nil) {
        T->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

rb_node * Minimum(rb_tree *T, rb_node *n) {
    while (n->left != T->nil) {
        n = n->left;
    }
    return n;
}

void Insert_Fixup(rb_tree* T, rb_node *z) {
    while (z->parent->color == RED) {
        rb_node * y;
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    Left_Rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Right_Rotate(T, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    Right_Rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Left_Rotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

void Insert(rb_tree * T, rb_node * z) {
    rb_node *x, *y;
    int cmp;
    y = T->nil;
    x = T->root;
    while (x != T->nil) {
        y = x;
        cmp = strcmp(z->key, x->key);
        if (cmp < 0) {
            x = x->left;
        } else if (cmp == 0) {
            return;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == T->nil) {
        T->root = z;
    } else if (strcmp(z->key, y->key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    Insert_Fixup(T, z);
}

void Inorder (rb_node * root, rb_tree *T) {
    if (root == T->nil) {
        return;
    }
    Inorder(root->left, T);
    printf("%s\n", root->key);
    if (root ->relations == NULL) {
        printf("Nessuna Relazione\n");
    } else {
        printf("Relazioni entita':\n");
        rel_list *temp;
        for (temp = root ->relations; temp != NULL; temp = temp ->next) {
            printf("Nome Relazione: %s; # entita' presenti: %d\nNomi entita': ", temp->rel_name, temp->number_of_entities);
            ent_list *temp_ent;
            if (temp->ent_list == NULL) {
                printf("Vuoto");
            }
            for (temp_ent = temp->ent_list; temp_ent != NULL; temp_ent = temp_ent->next) {
                printf("%s, ", temp_ent->ent_node->key);
            }
        }
        printf("\n");
    }
    Inorder(root->right, T);

}

void Transplant(rb_tree * T, rb_node *u, rb_node *v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

rb_node* Find (rb_tree *T, char *name, rb_node *root) {
    if (root != T->nil) {
        if (strcmp(root->key, name) == 0) {
            return root;
        } else if (strcmp(root -> key, name) > 0) {
            Find (T, name, root -> left);
        } else {
            Find(T, name, root -> right);
        }
    }
    else {
        return NULL;
    }
}

void Delete_Fixup(rb_tree *T, rb_node *x) {
    rb_node *w;
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Left_Rotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    Right_Rotate(T, w);
                    w->color = x->parent->color;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate(T, x->parent);
                x = T->root;
            }
        } else {w = x->parent->left;
            if (x->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Right_Rotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    Left_Rotate(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                Right_Rotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

void Delete(rb_tree *T, rb_node *z) {
    rb_node *y, *x;
    y = z;
    char y_original_color = y->color;
    if (z->left == T->nil) {
        x = z->right;
        Transplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        Transplant(T, z, z->left);
    } else {
        y = Minimum(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            Transplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        Delete_Fixup(T, x);
    }
}

// Relations tree
typedef struct rb_node_rel_s {
    char *key;
    char color;
    int num_of_receivers;
    struct rb_node_rel_s *left, *right, *parent;
} rb_node_rel;

typedef struct rb_tree_rel_s {
    rb_node_rel *root, *nil;
}rb_tree_rel;

void Left_Rotate_Rel (rb_tree_rel * T, rb_node_rel * x){
    rb_node_rel * y = x -> right;
    x -> right = y -> left;
    if (y -> left != T -> nil) {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == T -> nil) {
        T->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void Right_Rotate_Rel (rb_tree_rel * T, rb_node_rel * y) {
    rb_node_rel * x = y -> left;
    y->left = x->right;
    if (x->right != T->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == T->nil) {
        T->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

rb_node_rel * Minimum_Rel (rb_tree_rel *T, rb_node_rel *n) {
    while (n->left != T->nil) {
        n = n->left;
    }
    return n;
}

void Insert_Fixup_Rel(rb_tree_rel *T, rb_node_rel *z) {
    while (z->parent->color == RED) {
        rb_node_rel * y;
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    Left_Rotate_Rel(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Right_Rotate_Rel(T, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    Right_Rotate_Rel(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Left_Rotate_Rel(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

void Insert_Rel(rb_tree_rel * T, rb_node_rel * z) {
    rb_node_rel *x, *y;
    int cmp;
    y = T->nil;
    x = T->root;
    while (x != T->nil) {
        y = x;
        cmp = strcmp(z->key, x->key);
        if (cmp < 0) {
            x = x->left;
        } else if (cmp == 0) {
            return;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == T->nil) {
        T->root = z;
    } else if (strcmp(z->key, y->key) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    Insert_Fixup_Rel(T, z);
}

void Inorder_Rel (rb_node_rel * root, rb_tree_rel *T) {
    if (root == T->nil) {
        return;
    }
    Inorder_Rel(root->left, T);
    printf("%s, entita' che la usano: %d\n", root->key, root->num_of_receivers);
    Inorder_Rel(root->right, T);

}

void Transplant_Rel(rb_tree_rel * T, rb_node_rel *u, rb_node_rel *v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

rb_node_rel * Find_Rel (rb_tree_rel *T, char *name, rb_node_rel *root) {
    if (root != T->nil) {
        if (strcmp(root->key, name) == 0) {
            return root;
        } else if (strcmp(root -> key, name) > 0) {
            Find_Rel (T, name, root -> left);
        } else {
            Find_Rel(T, name, root -> right);
        }
    }
    else {
        return NULL;
    }
}

void Delete_Fixup_Rel(rb_tree_rel *T, rb_node_rel *x) {
    rb_node_rel *w;
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Left_Rotate_Rel(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    Right_Rotate_Rel(T, w);
                    w->color = x->parent->color;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate_Rel(T, x->parent);
                x = T->root;
            }
        } else {w = x->parent->left;
            if (x->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Right_Rotate_Rel(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    Left_Rotate_Rel(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                Right_Rotate_Rel(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

void Delete_Rel(rb_tree_rel *T, rb_node_rel *z) {
    rb_node_rel *y, *x;
    y = z;
    char y_original_color = y->color;
    if (z->left == T->nil) {
        x = z->right;
        Transplant_Rel(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        Transplant_Rel(T, z, z->left);
    } else {
        y = Minimum_Rel(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            Transplant_Rel(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant_Rel(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        Delete_Fixup_Rel(T, x);
    }
}

rb_tree* Create_Tree () {
    rb_tree * T;
    T = (rb_tree*)malloc(sizeof(rb_tree));
    rb_node *nil;
    nil =(rb_node*)malloc(sizeof(rb_node));
    T->root = nil;
    T->nil = nil;
    nil -> key = NULL;
    nil ->color = BLACK;
    nil->right = nil;
    nil->left = nil;
    nil->parent = T->root;
    return T;
}

rb_node* Create_Node (char *name) {
    rb_node *temp;
    temp =(rb_node*)malloc(sizeof(rb_node));
    int i;
    int len = 0;
    for (len = 0; name[len] != '\0'; len++);
    char * temp_name;
    temp_name = (char*)malloc((len+1) *sizeof(char));
    for (i = 0; i < len; i++) {
        temp_name[i] = name[i];
    }
    temp_name[len] = '\0';
    temp->relations = NULL;
    temp->key = temp_name;
    return temp;
}

//Reletions Creation
rb_tree_rel * Create_Tree_Rel () {
    rb_tree_rel * T;
    T = (rb_tree_rel *)malloc(sizeof(rb_tree_rel));
    rb_node_rel *nil;
    nil =(rb_node_rel *)malloc(sizeof(rb_node_rel));
    T->root = nil;
    T->nil = nil;
    nil -> key = NULL;
    nil ->color = BLACK;
    nil->right = nil;
    nil->left = nil;
    nil->parent = T->root;
    return T;
}

rb_node_rel * Create_Node_Rel(char *name) {
    rb_node_rel *temp;
    temp =(rb_node_rel *)malloc(sizeof(rb_node_rel));
    int i;
    int len = 0;
    for (len = 0; name[len] != '\0'; len++);
    char * temp_name;
    temp_name = (char*)malloc((len+1)*sizeof(char));
    for (i = 0; i < len; i++) {
        temp_name[i] = name[i];
    }
    temp_name[len] = '\0';
    temp->key = temp_name;
    temp->num_of_receivers = 1;
    return temp;
}

ent_list * Create_Ent_List_Element (rb_node *node) {
    ent_list *temp;
    temp = (ent_list *)malloc(sizeof(ent_list) + 1);
    temp->ent_node = node;
    temp->next = NULL;
    return temp;
}

int Find_Ent_in_Ent_List (rel_list *rel_List, char *name_ent) {
    ent_list *temp = rel_List->ent_list;
    while (1) {
        if (temp == NULL) {
            return 0;
        } else if (strcmp(temp->ent_node->key,name_ent) == 0) {
            return 1;
        } else {
            temp = temp->next;
        }
    }
}

void Add_Ent_List_In_Rel_List(rel_list *rel_list, ent_list *ent_element) {
    ent_list *temp;
    temp = rel_list->ent_list;
    rel_list->ent_list = ent_element;
    ent_element->next = temp;
}

rel_list * Create_Rel_List_Element(char *name) {
    int len, i;
    rel_list *temp;
    for (len = 0; name[len] != '\0'; len ++);
    char *key = (char *)malloc((len+1)*sizeof(char));
    for (i = 0; i < len; i++) {
        key[i] = name [i];
    }
    key[len] = '\0';
    temp = (rel_list *)malloc(sizeof(rel_list) +1 );
    temp->rel_name = key;
    temp->next = NULL;
    temp->number_of_entities = 0;
    temp->ent_list = NULL;
    return temp;
}

void Add_Rel_List_In_Ent_Node (rb_node *node, rel_list *list) {
    rel_list *temp = node->relations;
    node->relations = list;
    list->next = temp;
}

//deprecata
/*void Remove_Ent_From_Ent_List (rb_node *root, char *key, rb_tree *T) {
    if (root == T->nil) {
        return;
    }
    Remove_Ent_From_Ent_List(root->left, key, T);
    if (root ->relations != NULL) {
        rel_list *prev_rel, *head, *temp = root ->relations;
        head = root->relations;
        prev_rel = root->relations;
        //Per ogni relazione presente nel nodo entità
        while (temp != NULL) {
            int deleted = 0;
            //Se è la testa della lista
            ent_list *temp_ent= temp->ent_list;
            if (strcmp(temp->ent_list->ent_node->key, key) == 0) {
                temp->ent_list = temp->ent_list->next;
                free(temp_ent);
                temp->number_of_entities--;
            }
            //Temp_Ent diventa il next;
            else {
                temp_ent = temp->ent_list->next;
                ent_list *prev = temp->ent_list;
                while (!deleted && temp_ent != NULL && prev != NULL) {
                    if (strcmp(temp_ent->ent_node->key, key) == 0) {
                        prev->next = temp_ent->next;
                        free(temp_ent);
                        temp->number_of_entities--;
                        deleted = 1;
                    }
                    if (!deleted) {
                        prev = temp_ent;
                        temp_ent = temp_ent->next;
                    }
                }
            }
            if (temp->number_of_entities == 0) {
                rel_list *current = temp;
                rel_list *next = temp->next;
                //Se è la testa
                if (strcmp(head->rel_name, current->rel_name) == 0) {
                    head = head->next;
                    free(current->rel_name);
                    free(current);
                } else {
                    for ( current = head; current != NULL && strcmp(current->next->rel_name,temp->rel_name) == 0; current = current->next);
                    prev_rel = current;
                    prev_rel->next = prev_rel->next->next;
                    temp = temp->next;
                    free(current->rel_name);
                    free(current);
                }
            }
        }
    }
    Remove_Ent_From_Ent_List(root->right, key, T);
}*/

void Find_Ent_In_Rel_List(rb_node *rb_root, char *ent_name, rb_tree_rel *T) {
    rel_list *head_rel = rb_root->relations;
    rel_list *temp_rel = head_rel;
    rel_list *prev_rel = NULL;
    while (temp_rel != NULL) {
        ent_list *temp_ent = temp_rel->ent_list, *prev_ent;
        //Se l'entità da eliminare è presente.
        //Se sono in testa
        if (temp_ent != NULL && strcmp(temp_ent->ent_node->key, ent_name) == 0) {
            temp_rel->ent_list = temp_ent->next;
            temp_rel->number_of_entities--;
            free(temp_ent);
        } else {
            while (temp_ent != NULL && strcmp(temp_ent->ent_node->key, ent_name) != 0) {
                prev_ent = temp_ent;
                temp_ent = temp_ent->next;
            }
            if (temp_ent != NULL) {
                prev_ent->next = temp_ent->next;
                temp_rel->number_of_entities--;
                free(temp_ent);
            }
        }

        // Fino a qua funziona

        //Se la lista della relazione risulta vuota
        if (temp_rel->number_of_entities == 0) {
            char *rel_name = temp_rel->rel_name;
            rel_list *to_delete;
            rb_node_rel *check_to_delete = NULL;
            //Se è in testa
            if (strcmp(rel_name, head_rel->rel_name) == 0 ) {
                rb_root->relations = rb_root->relations->next;
                check_to_delete = Find_Rel(T, temp_rel->rel_name, T->root);
                check_to_delete->num_of_receivers--;
                to_delete = temp_rel;
                temp_rel = temp_rel->next;
                free(to_delete->rel_name);
                free(to_delete);
            } else {
                while (temp_rel != NULL && strcmp(temp_rel->rel_name, rel_name) != 0) {
                    prev_rel = temp_rel;
                    temp_rel = temp_rel->next;
                }
                if (prev_rel != NULL && temp_rel != NULL) {
                    prev_rel->next = temp_rel->next;
                    check_to_delete = Find_Rel(T, temp_rel->rel_name, T->root);
                    check_to_delete->num_of_receivers--;
                    to_delete = temp_rel;
                    temp_rel = temp_rel->next;
                    free(to_delete->rel_name);
                    free(to_delete);
                }
            }
            if (check_to_delete!= NULL && check_to_delete->num_of_receivers == 0) {
                Delete_Rel(T, check_to_delete);
            }
        } else {
            prev_rel = temp_rel;
            temp_rel = temp_rel->next;
        }
    }
}
void Inorder_Delete_Ent(rb_tree *T, rb_node *root,char *key, rb_tree_rel *T_rel) {
    if (root == T->nil) {
        return;
    }
    Inorder_Delete_Ent(T, root->left, key, T_rel);
    //Se il nodo ha delle relazioni
    if (root->relations != NULL) {
        Find_Ent_In_Rel_List(root, key, T_rel);
    }
    Inorder_Delete_Ent(T, root->right, key, T_rel);
}

rel_list * Find_Rel_In_Ent (rb_node *ent, char *name_rel) {
    rel_list *temp = ent->relations;
    while (1) {
        if (temp == NULL) {
            return NULL;
        } else if (strcmp(temp->rel_name, name_rel) == 0) {
            return temp;
        } else {
            temp = temp->next;
        }
    }
}

void Debug_Delete_All_Relations(rb_tree_rel *T_rel, rb_node_rel *root) {
    if (root == T_rel->nil) {
        return;
    }
    Debug_Delete_All_Relations(T_rel, root->left);
    rb_node_rel *temp = root->right;
    Delete_Rel(T_rel, root);
    free(root->key);
    free(root);
    Debug_Delete_All_Relations(T_rel,temp);
}

void Debug_Delete_All_Entities (rb_tree *T_ent, rb_node *root) {
    if (root == T_ent->nil) {
        return;
    }
    Debug_Delete_All_Entities(T_ent, root->left);
    rb_node *next_right = root->right;
    if (root->relations != NULL) {
        rel_list *current = root->relations; //Head
        rel_list *next;
        while (current != NULL) {
            next = current->next;
            if (current->ent_list != NULL) {
                ent_list *current_ent = current->ent_list; //Head
                ent_list *next_ent;
                while (current_ent != NULL) {
                    next_ent = current_ent->next;
                    free(current_ent);
                    current_ent = next_ent;
                }
            }
            free(current->rel_name);
            free(current);
            current = next;
        }
    }
    free (root->key);
    free(root);
    Debug_Delete_All_Entities(T_ent, next_right);
}

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
    rb_tree * T_ent;
    T_ent = Create_Tree();
    rb_tree_rel *T_rel;
    T_rel = Create_Tree_Rel();


    //External Loop: cicla fino alla fine dell'esecuzione del programma; ovvero fino a che viene letto END.
    while (ext_loop == 1) {
        loop = 1;
        //printf("Scrivi comando");
        comand = (char *)calloc(8, sizeof(char));
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
            Debug_Delete_All_Relations(T_rel, T_rel->root);
            Debug_Delete_All_Entities(T_ent, T_ent->root);
            free(comand);
            //free(T_rel->nil->key);
            free(T_rel->nil);
            free(T_rel);
            //free(T_ent->nil->key);
            free(T_ent->nil);
            free(T_ent);
            return 0;
        }
            //Se ADDENT o DELENT, devo leggere l'entità.
        else if (strcmp (comand, "addent") == 0 || (strcmp (comand, "delent") == 0)) {
            scanf(" \"%s", ent1);
            parseWord(ent1);
            if (comand[0] == 'a') {
                if (1) {
                    Insert(T_ent, Create_Node(ent1));
                }
            } else {
                /*
                 * TODO -> Bisogna assicurarsi che, se cancello un entità, sto rimuovendo tale entià da ogni altra entità che
                 * TODO    potrebbe subire una relazione da suddetta entità.
                 * TODO    Controllare memory leaks nelle liste relazioni e entità (fatto)
                 */
                rb_node *to_be_deleted = Find(T_ent, ent1, T_ent->root);
                if (to_be_deleted != NULL) {
                    Inorder_Delete_Ent(T_ent, T_ent->root, ent1, T_rel);
                    //Remove_Ent_From_Ent_List(T_ent->root, ent1, T_ent); deprecated
                    if (to_be_deleted->relations != NULL) {
                        rel_list *check_if_to_be_deleted = to_be_deleted->relations;
                        //Per ogni relazione della ent che voglio rimuovere, se è l'unica ad usarla, rimuovo la relazione dall'albero
                        // altrimenti, diminuisco il contatore.
                        while (check_if_to_be_deleted != NULL) {
                            rel_list *temp_rel_list = check_if_to_be_deleted;
                            while (check_if_to_be_deleted->ent_list != NULL) {
                                ent_list *temp_ent_list = check_if_to_be_deleted->ent_list;
                                check_if_to_be_deleted->ent_list = check_if_to_be_deleted->ent_list->next;
                                free(temp_ent_list);
                            }
                            rb_node_rel *check_rel = Find_Rel(T_rel, check_if_to_be_deleted->rel_name, T_rel->root);
                            if (check_rel->num_of_receivers == 1) {
                                Delete_Rel(T_rel, check_rel);
                                free(check_rel->key);
                                free(check_rel);
                            } else {
                                check_rel->num_of_receivers--;
                            }
                            check_if_to_be_deleted = check_if_to_be_deleted->next;
                            free(temp_rel_list);
                        }
                    }
                    Delete(T_ent, to_be_deleted);
                    free(to_be_deleted);
                }
            }
        }else if (strcmp (comand, "addrel") == 0 || (strcmp (comand, "delrel") == 0)) {
            //SOLO PER TEST
            scanf(" \"%s \"%s \"%s", ent1, ent2, rel);
            parseWord(ent1);
            parseWord(ent2);
            parseWord(rel);
            if (comand[0] == 'a') {
                if (1) {
                    //Se esiste l'entità1
                    rb_node *ent1_node = Find(T_ent, ent1, T_ent->root);
                    if (ent1_node != NULL) {
                        rb_node *receiver = Find(T_ent, ent2, T_ent->root);
                        if (receiver != NULL) {
                            //Se non esiste la relazione, la creo
                            rb_node_rel *rel_node = Find_Rel(T_rel, rel, T_rel->root);
                            //1 se creo il nodo, 0 altrimenti
                            int created = 0;
                            if ( rel_node == NULL) {
                                created = 1;
                                Insert_Rel(T_rel, Create_Node_Rel(rel));
                            }
                            //Guardo se il ricevente subisce già relazioni di tipo rel. NULL se non ne ha
                            rel_list *temp_rel_list = Find_Rel_In_Ent(receiver, rel);
                            if (temp_rel_list == NULL) {
                                rel_list *new_rel_list_element = Create_Rel_List_Element(rel);
                                Add_Rel_List_In_Ent_Node(receiver, new_rel_list_element);
                                //Se created == 0, devo aggiungere 1 al numero di entità che usano questa relazione
                                if (!created) {
                                    rel_node->num_of_receivers++;
                                }
                                //Aggiungo ent1
                                ent_list *new_giver = Create_Ent_List_Element(ent1_node);
                                Add_Ent_List_In_Rel_List(new_rel_list_element, new_giver);
                                new_rel_list_element->number_of_entities++;
                            } else {
                                //Se è già presente la relazione, verifico se ho già l'ent1 (1 già presente, 0 altrimenti)
                                if (Find_Ent_in_Ent_List(temp_rel_list, ent1) == 0) {
                                    ent_list *new_giver = Create_Ent_List_Element(ent1_node);
                                    Add_Ent_List_In_Rel_List(temp_rel_list, new_giver);
                                    temp_rel_list->number_of_entities++;
                                }
                            }
                        }
                    }
                }
            } else {
                /*
                 * rb_node_rel * to_be_deleted = Find_Rel(T_rel, rel, T_rel->root);
                if (to_be_deleted != NULL) {
                    Delete_Rel( T_rel, to_be_deleted);
                    free(to_be_deleted);
                }*/
            }
        } else if(strcmp(comand, "debug") == 0) {
            printf("\nentities:\n");
            Inorder( T_ent -> root, T_ent);
            printf("\nrelationships\n");
            Inorder_Rel(T_rel -> root, T_rel);
            //printRelationshipTree(relationships);
            printf("\n");
        }
        free(comand);
    }

}