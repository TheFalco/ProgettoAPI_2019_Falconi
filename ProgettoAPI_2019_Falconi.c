#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define Hash_Size 5471
#define RED 'r'
#define BLACK 'b'

typedef struct RB_Tree_Ent_s {
    char *ent_name;
    struct RB_Tree_Ent_s *parent, *left, *right;
    char color;
}RB_Tree_Ent;

typedef struct RB_Tree_Ent_Head_s {
    RB_Tree_Ent *root, *nil;
}RB_Tree_Ent_Head;

typedef struct RB_Tree_Ent_Rel_s {
    char *ent_name;
    struct RB_Tree_Ent_Rel_s *parent, *left, *right;
    char color;
    int num_of_givers;
    RB_Tree_Ent_Head *T_givers;
}RB_Tree_Ent_Rel;

typedef struct RB_Tree_Ent_Rel_Head_s {
    RB_Tree_Ent_Rel *root, *nil;
}RB_Tree_Ent_Rel_Head;

typedef struct RB_Node_s {
    char *rel_name;
    struct RB_Node_s *parent, *left, *right;
    char color;
    int num_of_entities, max_size, num_of_max_entities;
    RB_Tree_Ent_Head *T_max_ent;
    RB_Tree_Ent_Rel_Head *T_ent;
}RB_Node;

typedef struct RB_Tree_s {
    RB_Node *root, *nil;
}RB_Tree;

typedef struct hash_element_s {
    char *ent_name;
    struct hash_element_s *next;
}hash_element;

int modify = 0;
RB_Tree_Ent_Head *to_delete_ent_rel = NULL;
RB_Tree_Ent_Head *to_delete_rel = NULL;

//Ent Tree Functions
void Left_Rotate_Ent (RB_Tree_Ent_Head * T, RB_Tree_Ent * x){
    RB_Tree_Ent * y = x -> right;
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

void Right_Rotate_Ent (RB_Tree_Ent_Head * T, RB_Tree_Ent * y) {
    RB_Tree_Ent * x = y -> left;
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

RB_Tree_Ent * Minimum_Ent (RB_Tree_Ent_Head *T, RB_Tree_Ent *n) {
    while (n->left != T->nil) {
        n = n->left;
    }
    return n;
}

void Insert_Fixup_Ent(RB_Tree_Ent_Head *T, RB_Tree_Ent *z) {
    while (z->parent->color == RED) {
        RB_Tree_Ent * y;
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
                    Left_Rotate_Ent(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Right_Rotate_Ent(T, z->parent->parent);
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
                    Right_Rotate_Ent(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Left_Rotate_Ent(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

void Insert_Ent(RB_Tree_Ent_Head * T, RB_Tree_Ent * z) {
    RB_Tree_Ent *x, *y;
    int cmp;
    y = T->nil;
    x = T->root;
    while (x != T->nil) {
        y = x;
        cmp = strcmp(z->ent_name, x->ent_name);
        if (cmp < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == T->nil) {
        T->root = z;
    } else if (strcmp(z->ent_name, y->ent_name) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    Insert_Fixup_Ent(T, z);
}

void Transplant_Ent(RB_Tree_Ent_Head * T, RB_Tree_Ent *u, RB_Tree_Ent *v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

RB_Tree_Ent * Find_Ent (RB_Tree_Ent_Head *T, char *name, RB_Tree_Ent *root) {
    if (root != T->nil) {
        int cmp = strcmp(root->ent_name, name);
        if (cmp == 0) {
            return root;
        } else if (cmp > 0) {
            Find_Ent (T, name, root -> left);
        } else {
            Find_Ent(T, name, root -> right);
        }
    }
    else {
        return NULL;
    }
}

void Delete_Fixup_Ent(RB_Tree_Ent_Head *T, RB_Tree_Ent *x) {
    RB_Tree_Ent *w;
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Left_Rotate_Ent(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    Right_Rotate_Ent(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate_Ent(T, x->parent);
                x = T->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Right_Rotate_Ent(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    Left_Rotate_Ent(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                Right_Rotate_Ent(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

void Delete_Ent(RB_Tree_Ent_Head *T, RB_Tree_Ent *z) {
    RB_Tree_Ent *y, *x;
    y = z;
    char y_original_color = y->color;
    if (z->left == T->nil) {
        x = z->right;
        Transplant_Ent(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        Transplant_Ent(T, z, z->left);
    } else {
        y = Minimum_Ent(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            Transplant_Ent(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant_Ent(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        Delete_Fixup_Ent(T, x);
    }
}

//Ent Rel Tree Functions
void Left_Rotate_Ent_Rel (RB_Tree_Ent_Rel_Head * T, RB_Tree_Ent_Rel * x){
    RB_Tree_Ent_Rel * y;
    y = x -> right;
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

void Right_Rotate_Ent_Rel (RB_Tree_Ent_Rel_Head * T, RB_Tree_Ent_Rel * y) {
    RB_Tree_Ent_Rel * x = y -> left;
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

RB_Tree_Ent_Rel * Minimum_Ent_Rel (RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *n) {
    while (n->left != T->nil) {
        n = n->left;
    }
    return n;
}

void Insert_Fixup_Ent_Rel(RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *z) {
    RB_Tree_Ent_Rel * y;
    while (z->parent->color == RED) {
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
                    Left_Rotate_Ent_Rel(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Right_Rotate_Ent_Rel(T, z->parent->parent);
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
                    Right_Rotate_Ent_Rel(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Left_Rotate_Ent_Rel(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

RB_Tree_Ent_Rel *Insert_Ent_Rel(RB_Tree_Ent_Rel_Head * T, RB_Tree_Ent_Rel * z) {
    RB_Tree_Ent_Rel *x, *y;
    int cmp;
    y = T->nil;
    x = T->root;
    while (x != T->nil) {
        y = x;
        cmp = strcmp(z->ent_name, x->ent_name);
        if (cmp < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == T->nil) {
        T->root = z;
    } else if (strcmp(z->ent_name, y->ent_name) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    Insert_Fixup_Ent_Rel(T, z);
    return z;
}

void Transplant_Ent_Rel(RB_Tree_Ent_Rel_Head * T, RB_Tree_Ent_Rel *u, RB_Tree_Ent_Rel *v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

RB_Tree_Ent_Rel * Find_Ent_Rel (RB_Tree_Ent_Rel_Head *T, char *name, RB_Tree_Ent_Rel *root) {
    if (root != T->nil) {
        int cmp = strcmp(root->ent_name, name);
        if (cmp == 0) {
            return root;
        } else if (cmp > 0) {
            Find_Ent_Rel (T, name, root -> left);
        } else {
            Find_Ent_Rel(T, name, root -> right);
        }
    }
    else {
        return NULL;
    }
}

void Delete_Fixup_Ent_Rel(RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *x) {
    RB_Tree_Ent_Rel *w;
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Left_Rotate_Ent_Rel(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    Right_Rotate_Ent_Rel(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate_Ent_Rel(T, x->parent);
                x = T->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                Right_Rotate_Ent_Rel(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    Left_Rotate_Ent_Rel(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                Right_Rotate_Ent_Rel(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

void Delete_Ent_Rel(RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *z) {
    RB_Tree_Ent_Rel *y, *x;
    y = z;
    char y_original_color = y->color;
    if (z->left == T->nil) {
        x = z->right;
        Transplant_Ent_Rel(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        Transplant_Ent_Rel(T, z, z->left);
    } else {
        y = Minimum_Ent_Rel(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            Transplant_Ent_Rel(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant_Ent_Rel(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK) {
        Delete_Fixup_Ent_Rel(T, x);
    }
}

//Relation Tree Functions
void Left_Rotate_Rel (RB_Tree * T, RB_Node * x){
    RB_Node * y = x -> right;
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

void Right_Rotate_Rel (RB_Tree * T, RB_Node * y) {
    RB_Node * x = y -> left;
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

RB_Node * Minimum_Rel (RB_Tree *T, RB_Node *n) {
    while (n->left != T->nil) {
        n = n->left;
    }
    return n;
}

void Insert_Fixup_Rel(RB_Tree *T, RB_Node *z) {
    while (z->parent->color == RED) {
        RB_Node * y;
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

RB_Node *Insert_Rel(RB_Tree * T, RB_Node * z) {
    RB_Node *x, *y;
    int cmp;
    y = T->nil;
    x = T->root;
    while (x != T->nil) {
        y = x;
        cmp = strcmp(z->rel_name, x->rel_name);
        if (cmp < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == T->nil) {
        T->root = z;
    } else if (strcmp(z->rel_name, y->rel_name) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = T->nil;
    z->right = T->nil;
    z->color = RED;
    Insert_Fixup_Rel(T, z);
    return z;
}

void Transplant_Rel(RB_Tree * T, RB_Node *u, RB_Node *v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

RB_Node * Find_Rel (RB_Tree *T, char *name, RB_Node *root) {
    if (root != T->nil) {
        int cmp = strcmp(root->rel_name, name);
        if (cmp == 0) {
            return root;
        } else if (cmp > 0) {
            Find_Rel (T, name, root -> left);
        } else {
            Find_Rel(T, name, root -> right);
        }
    }
    else {
        return NULL;
    }
}

void Delete_Fixup_Rel(RB_Tree *T, RB_Node *x) {
    RB_Node *w;
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
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate_Rel(T, x->parent);
                x = T->root;
            }
        } else {
            w = x->parent->left;
            if (w->color == RED) {
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

void Delete_Rel(RB_Tree *T, RB_Node *z) {
    RB_Node *y, *x;
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

//Entities Creation
RB_Tree_Ent_Head * Create_Ent_Head () {
    RB_Tree_Ent_Head * T = malloc(sizeof(RB_Tree_Ent_Head));
    RB_Tree_Ent *nil = malloc(sizeof(RB_Tree_Ent));
    T->root = nil;
    T->nil = nil;
    nil -> ent_name = NULL;
    nil ->color = BLACK;
    nil->right = nil;
    nil->left = nil;
    nil->parent = T->root;
    return T;
}

RB_Tree_Ent * Create_Ent_Node(char *name) {
    RB_Tree_Ent *temp = malloc(sizeof(RB_Tree_Ent));
    temp->ent_name = name;
    temp->color = BLACK;
    return temp;
}

//Ent in Rel Creation
RB_Tree_Ent_Rel_Head * Create_Ent_Rel_Head () {
    RB_Tree_Ent_Rel_Head * T = malloc(sizeof(RB_Tree_Ent_Rel_Head));
    RB_Tree_Ent_Rel *nil = malloc(sizeof(RB_Tree_Ent_Rel));
    T->root = nil;
    T->nil = nil;
    nil -> ent_name = NULL;
    nil ->color = BLACK;
    nil->right = nil;
    nil->left = nil;
    nil->parent = T->root;
    nil->T_givers = NULL;
    nil->num_of_givers = 0;
    return T;
}

RB_Tree_Ent_Rel * Create_Ent_Rel_Node(char *name) {
    RB_Tree_Ent_Rel *temp = malloc(sizeof(RB_Tree_Ent_Rel));
    temp->ent_name = name;
    temp->num_of_givers = 0;
    temp->T_givers = Create_Ent_Head();
    temp->color= BLACK;
    return temp;
}

//Relations Creation
RB_Tree * Create_Tree_Rel () {
    RB_Tree * T;
    T = (RB_Tree *)malloc(sizeof(RB_Tree));
    RB_Node *nil;
    nil =(RB_Node *)malloc(sizeof(RB_Node));
    T->root = nil;
    T->nil = nil;
    nil -> rel_name = NULL;
    nil ->color = BLACK;
    nil->right = nil;
    nil->left = nil;
    nil->parent = T->root;
    nil->T_ent = NULL;
    nil->T_max_ent = NULL;
    return T;
}

RB_Node * Create_Node_Rel(char *name) {
    RB_Node *temp;
    temp =(RB_Node *)malloc(sizeof(RB_Node));
    int i;
    int len = 0;
    for (len = 0; name[len] != '\0'; len++);
    char * temp_name;
    temp_name = (char*)malloc((len+1)*sizeof(char));
    for (i = 0; i < len; i++) {
        temp_name[i] = name[i];
    }
    temp_name[len] = '\0';
    temp->rel_name = temp_name;
    temp->color = BLACK;
    temp->T_max_ent = Create_Ent_Head();
    temp->max_size = 0;
    temp->T_ent = Create_Ent_Rel_Head();
    temp->num_of_entities = 0;
    temp->num_of_max_entities = 0;
    return temp;
}

//Hash Functions
hash_element *Find_In_Hash (long key, hash_element *HashTable[], char *ent_name) {
    if (HashTable[key] == NULL || HashTable[key]->ent_name == NULL) {
        return NULL;
    }
    if (strcmp(HashTable[key]->ent_name, ent_name) == 0) {
        return HashTable[key];
    } else {
        hash_element *temp = HashTable[key]->next;
        while(temp != NULL) {
            if (strcmp(temp->ent_name, ent_name) == 0) {
                return temp;
            }
            temp = temp->next;
        }
    }
    return NULL;
}

void Add_In_Hash (long key, hash_element *HashTable[], char *ent_name) {
    int i, len = strlen(ent_name);
    char *name = (char*)malloc(sizeof(char)* (len+1));
    for (i = 0; i < len; i++) {
        name[i] = ent_name [i];
    }
    name [len] = '\0';
    if (HashTable[key] == NULL || HashTable[key]->ent_name == NULL) {
        /*if (HashTable[key] == NULL) {
            HashTable[key] = malloc(sizeof(hash_element));
        }*/
        HashTable[key] = malloc(sizeof(hash_element));
        HashTable[key]->ent_name = name;
        HashTable[key]->next = NULL;
        return;
    }
    hash_element *new = (hash_element *)malloc(sizeof(hash_element));
    new->ent_name = name;
    new->next = HashTable[key]->next;
    HashTable[key]->next = new;
}

void Delete_From_Hash(hash_element *to_delete, long key, hash_element *HashTable[]) {
    hash_element *temp = HashTable[key];
    if (strcmp(temp->ent_name, to_delete->ent_name) == 0) {
        HashTable[key] = temp->next;
        free(temp->ent_name);
        free(temp);
        return;
    }
    hash_element *prev;
    while (temp != NULL && strcmp(temp->ent_name, to_delete->ent_name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        return;
    }
    prev->next = temp->next;
    free(temp->ent_name);
    free(temp);
}

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void Clear_Max (RB_Tree_Ent_Head *rel_node) {
    RB_Tree_Ent *temp;
    while (rel_node->root != rel_node->nil) {
        temp = rel_node->root;
        Delete_Ent(rel_node, rel_node->root);
        free(temp);
    }
}

void Clear_Rel (RB_Tree *T, RB_Tree_Ent *root) {
    if (root == to_delete_rel->nil) {
        return;
    }
    Clear_Rel(T, root->left);
    //
    RB_Node *to_delete = Find_Rel(T, root->ent_name, T->root);
    if (to_delete != NULL) {
        Delete_Rel(T, to_delete);
        free(to_delete->rel_name);
        free(to_delete);
    }
    //
    Clear_Rel(T, root->right);
}

void Clear_Ent_Rel (RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent *root) {
    if (root == to_delete_ent_rel->nil) {
        return;
    }
    Clear_Ent_Rel(T, root->left);
    //
    RB_Tree_Ent_Rel *to_delete = Find_Ent_Rel(T, root->ent_name, T->root);
    if (to_delete != NULL) {
        Delete_Ent_Rel(T, to_delete);
        free(to_delete);
    }
    //
    Clear_Ent_Rel(T, root->right);
}

void Check_Max(RB_Node *rel_node, RB_Tree_Ent_Rel *ent_node, hash_element *ent2_element) {
    //se max_size a'¨ il massimo, non faccio niente
    if (rel_node->max_size > ent_node->num_of_givers) {
        return;
    }
    if (rel_node->max_size < ent_node->num_of_givers) {
        Clear_Max(rel_node->T_max_ent);
        rel_node->num_of_max_entities = 0;
        Insert_Ent(rel_node->T_max_ent, Create_Ent_Node(ent2_element->ent_name));
        rel_node->max_size = ent_node->num_of_givers;
        rel_node->num_of_max_entities++;
        return;
    }
    //Se uguale
    if (rel_node->max_size == ent_node->num_of_givers) {
        Insert_Ent(rel_node->T_max_ent, Create_Ent_Node(ent2_element->ent_name));
        rel_node->num_of_max_entities++;
    }
}

void Inorder_Max_Tree (RB_Tree_Ent_Head *T, RB_Tree_Ent *root) {
    if (root == T->nil) {
        return;
    }
    Inorder_Max_Tree(T, root->left);
    fputs(root->ent_name, stdout);
    fputc(' ', stdout);
    Inorder_Max_Tree(T, root->right);
}

void Inorder_Report_Tree(RB_Tree *T, RB_Node *root) {
    if (root == T->nil) {
        return;
    }
    Inorder_Report_Tree(T, root->left);
    if (root->max_size > 0 && root->num_of_max_entities > 0) {
        fputs(root->rel_name, stdout);
        fputc(' ', stdout);
        Inorder_Max_Tree(root->T_max_ent, root->T_max_ent->root);
        printf("%d", root->max_size);
        fputs("; ", stdout);
    }
    Inorder_Report_Tree(T, root->right);
}

void Search_For_Max_Entities (RB_Node *rel_node, RB_Tree_Ent_Rel_Head *T_ent, RB_Tree_Ent_Rel *root) {
    if (root == T_ent->nil) {
        return;
    }
    //Scorro tutto l'albero
    Search_For_Max_Entities(rel_node, T_ent, root->left);
    //
    if (rel_node->max_size == root->num_of_givers) {
        Insert_Ent(rel_node->T_max_ent, Create_Ent_Node(root->ent_name));
        rel_node->num_of_max_entities++;
        //aggiungo
    } else if (rel_node->max_size < root->num_of_givers) {
        //Cancello e sostituisco
        Clear_Max(rel_node->T_max_ent);
        rel_node->num_of_max_entities = 0;
        Insert_Ent(rel_node->T_max_ent, Create_Ent_Node(root->ent_name));
        rel_node->max_size = root->num_of_givers;
        rel_node->num_of_max_entities++;
    }

    Search_For_Max_Entities(rel_node, T_ent, root->right);
}

void Delete_One_Ent_From_EntRel (RB_Tree_Ent *ent1_node, RB_Tree_Ent_Rel *ent2_node, RB_Node *rel_node, RB_Tree *T) {
    int original_max_size = ent2_node->num_of_givers;
    char *original_name_ent2 = ent2_node->ent_name;
    //Cancello entità 1 dall'albero
    Delete_Ent(ent2_node->T_givers, ent1_node);
    free(ent1_node);
    ent2_node->num_of_givers--;
    //Controllo se ent2 non ha più entità
    if (ent2_node->num_of_givers == 0) {
        //Devo eliminare ent2_node
        Delete_Ent_Rel(rel_node->T_ent, ent2_node);
        free(ent2_node->T_givers->nil);
        free(ent2_node->T_givers);
        free(ent2_node);
        rel_node->num_of_entities--;
        //Controllo se rel_node ha ancora entità
        if (rel_node->num_of_entities == 0) {
            //Elimino rel_node, pulisco memoria e ritorno
            Clear_Max(rel_node->T_max_ent);
            Delete_Rel(T, rel_node);
            free(rel_node->T_max_ent->nil);
            free(rel_node->T_max_ent);
            free(rel_node->T_ent->nil);
            free(rel_node->T_ent);
            free(rel_node);
            return;
        } else {
            //Controllo se ent2 era tra i massimi
            if (rel_node->max_size == original_max_size) {
                //Se era tra i massimi, controllo se era unico, o con altre entità
                if (rel_node->num_of_max_entities > 1) {
                    //Se non era da sola
                    RB_Tree_Ent *to_delete = Find_Ent(rel_node->T_max_ent, original_name_ent2, rel_node->T_max_ent->root);
                    Delete_Ent(rel_node->T_max_ent, to_delete);
                    rel_node->num_of_max_entities--;
                    free(to_delete);
                    return;
                } else {
                    RB_Tree_Ent *to_delete = rel_node->T_max_ent->root;
                    Delete_Ent(rel_node->T_max_ent, to_delete);
                    free(to_delete);
                    rel_node->num_of_max_entities = 0;
                    rel_node->max_size = 0;
                    Search_For_Max_Entities(rel_node, rel_node->T_ent, rel_node->T_ent->root);
                    return;
                }
            } else {
                //Se non era tra i massimi, apposto
                return;
            }
        }
    } else {
        //Ent2 non si svuota, controllo se era tra i max
        if (rel_node->max_size == original_max_size) {
            //Controllo se era da sola
            if (rel_node->num_of_max_entities > 1) {
                //Se non era da sola
                RB_Tree_Ent *to_delete = Find_Ent(rel_node->T_max_ent, original_name_ent2, rel_node->T_max_ent->root);
                Delete_Ent(rel_node->T_max_ent, to_delete);
                rel_node->num_of_max_entities--;
                free(to_delete);
                return;
            } else {
                RB_Tree_Ent *to_delete = rel_node->T_max_ent->root;
                Delete_Ent(rel_node->T_max_ent, to_delete);
                free(to_delete);
                rel_node->num_of_max_entities = 0;
                rel_node->max_size = 0;
                Search_For_Max_Entities(rel_node, rel_node->T_ent, rel_node->T_ent->root);
                return;
            }
        } else {
            return;
        }
    }
}

void Search_For_To_Be_Delete_In_Ent (RB_Tree_Ent_Head *T, RB_Tree_Ent *root, char *to_delete, RB_Tree_Ent_Rel *ent2) {
    RB_Tree_Ent *temp = root;
    int cmp;
    while (temp != T->nil) {
        cmp = strcmp(temp->ent_name, to_delete);
        if (cmp < 0) {
            temp = temp->right;
        }else if (cmp > 0) {
            temp = temp->left;
        } else {
            //Ho trovato l'ent da togliere
            Delete_Ent(T, temp);
            ent2->num_of_givers--;
            free(temp);
            return;
        }
    }
}

void  Search_For_To_Be_Delete_In_Ent_Rel(RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *root, char *to_delete, RB_Node *rel_node) {
    if (root == T->nil) {
        return;
    }
    Search_For_To_Be_Delete_In_Ent_Rel(T, root->left, to_delete, rel_node);
    //
    //Se root è l'entità che vogio eliminare
    if (strcmp(root->ent_name, to_delete) == 0) {
        Clear_Max(root->T_givers);      //Clear T_givers
        free(root->T_givers->nil);
        free(root->T_givers);
        //Aggiungo entità tra quelle da cancellare
        Insert_Ent(to_delete_ent_rel, Create_Ent_Node(to_delete));
        rel_node->num_of_entities--;
        if (root->num_of_givers == rel_node->max_size) {
            modify = 1;
        }
    } else {
        //Se non è l'entità da cancellare, cerco tra i T_givers se la trovo.
        int original_num_of_givers = root->num_of_givers;
        Search_For_To_Be_Delete_In_Ent(root->T_givers, root->T_givers->root, to_delete, root);
        //Se era massimo
        if (original_num_of_givers != root->num_of_givers) {
            if (rel_node->max_size == original_num_of_givers) {
                modify = 1;
            }
        }
        //Se ent2 si è svuotata
        if (root->num_of_givers == 0) {
            free(root->T_givers->nil);
            free(root->T_givers);
            //Aggiungo entità tra quelle da cancellare
            Insert_Ent(to_delete_ent_rel, Create_Ent_Node(root->ent_name));
            rel_node->num_of_entities--;
        }
    }
    //
    Search_For_To_Be_Delete_In_Ent_Rel(T, root->right, to_delete, rel_node);
}

void Search_For_To_Be_Delete_In_Rel (RB_Tree *T, RB_Node *root, char *to_delete) {
    if (root == T->nil) {
        return;
    }
    Search_For_To_Be_Delete_In_Rel(T, root->left, to_delete);
    //
    modify = 0;
    //Cerco tra le ent_rel
    Search_For_To_Be_Delete_In_Ent_Rel(root->T_ent, root->T_ent->root, to_delete, root);
    //Elimino definitivamente le ent2 da eliminare
    Clear_Ent_Rel(root->T_ent, to_delete_ent_rel->root);
    Clear_Max(to_delete_ent_rel);
    //Controllo se rel_node si è svuotata
    if (root->num_of_entities == 0) {
        //Pulisco
        Clear_Max(root->T_max_ent);
        free(root->T_max_ent->nil);
        free(root->T_max_ent);
        free(root->T_ent->nil);
        //Aggiungo nodo tar quelli da cancellare
        Insert_Ent(to_delete_rel, Create_Ent_Node(root->rel_name));
    } else {
        //Se devo ricalcolare il max
        if (modify) {
            Clear_Max(root->T_max_ent);
            root->num_of_max_entities = 0;
            root->max_size = 0;
            Search_For_Max_Entities(root, root->T_ent, root->T_ent->root);
        }
    }
    //
    Search_For_To_Be_Delete_In_Rel(T, root->right, to_delete);
}

int main() {
    to_delete_ent_rel = Create_Ent_Head();
    to_delete_rel = Create_Ent_Head();
    hash_element *hashTable[Hash_Size];
    int i;
    // Non Inizializzo HashMap
    for (i = 0; i < Hash_Size; i++) {
        hashTable[i] = malloc(sizeof(hash_element));
        if (hashTable[i] != NULL) {
            hashTable[i]->ent_name = NULL;
            hashTable[i]->next = NULL;
        }
    }
    unsigned long num;
    RB_Tree *T_rel;
    T_rel = Create_Tree_Rel();
    //char line[100];
    char *line;
    //
    size_t bufsize = 200;
    size_t  chars;
    line = (char *)malloc(sizeof(char) *bufsize);
    //
    char *ent1;
    char *ent2;
    char *rel;

    while (1) {
        chars = getline(&line, &bufsize, stdin);
        char *tok = strtok(line, " \n");
        if (strcmp(tok, "report") == 0) {
            if (T_rel->root == T_rel->nil) {
                fputs("none\n", stdout);
            } else {
                Inorder_Report_Tree(T_rel, T_rel->root);
                fputs("\n", stdout);

            }
        } else if (strcmp(tok, "addent") == 0) {
            tok = strtok(NULL, " \n");
            num = hash((unsigned char*) tok);
            if (Find_In_Hash(num%Hash_Size, hashTable, tok) == NULL) {
                Add_In_Hash(num%Hash_Size,hashTable, tok);
            }
            //
        } else if (strcmp(tok, "addrel") == 0) {
            //1st Word read
            tok = strtok(NULL, " \n");
            int len = strlen(tok);
            ent1 = (char*)malloc(sizeof(char)*(len+1));
            strcpy(ent1, tok);

            //2nd Word read
            tok = strtok(NULL, " \n");
            len = strlen(tok);
            ent2 = (char*)malloc(sizeof(char)*(len+1));
            strcpy(ent2, tok);

            //3rd Word read
            tok = strtok(NULL, "\n");
            len = strlen(tok);
            rel = (char*)malloc(sizeof(char)*(len+1));
            strcpy(rel, tok);

            num = hash((unsigned char*) ent1)%Hash_Size;
            hash_element *ent1_elem = Find_In_Hash((long) num, hashTable, ent1);
            if (ent1_elem != NULL) {
                hash_element *ent2_elem;
                if (strcmp(ent1, ent2) == 0) {
                    ent2_elem = ent1_elem;
                } else {
                    num = hash((unsigned char *) ent2)%Hash_Size;
                    ent2_elem = Find_In_Hash((long) num, hashTable, ent2);
                }
                if (ent2_elem != NULL) {
                    //Verifico presenza o meno del nodo della relazione
                    RB_Node *rel_node = Find_Rel(T_rel, rel, T_rel->root);
                    if (rel_node == NULL) {
                        //Se rel_node NON esiste, creo il node
                        rel_node = Insert_Rel(T_rel, Create_Node_Rel(rel));
                        RB_Tree_Ent_Rel *ent2_node = Insert_Ent_Rel(rel_node->T_ent, Create_Ent_Rel_Node(ent2_elem->ent_name));
                        rel_node->num_of_entities++;
                        Insert_Ent(ent2_node->T_givers, Create_Ent_Node(ent1_elem->ent_name));
                        ent2_node->num_of_givers++;
                        Check_Max(rel_node, ent2_node, ent2_elem);
                    } else {
                        //Verifico se ho gia'  ent2 nella rel
                        RB_Tree_Ent_Rel *ent2_node = Find_Ent_Rel(rel_node->T_ent, ent2, rel_node->T_ent->root);
                        if (ent2_node == NULL) {
                            //Se non ho ent2 nella relazione
                            ent2_node = Insert_Ent_Rel(rel_node->T_ent, Create_Ent_Rel_Node(ent2_elem->ent_name));
                            rel_node->num_of_entities++;
                            Insert_Ent(ent2_node->T_givers, Create_Ent_Node(ent1_elem->ent_name));
                            ent2_node->num_of_givers++;
                            Check_Max(rel_node, ent2_node, ent2_elem);
                        } else {
                            //verifico se ho gia ent1 tra i givers di ent2
                            if (Find_Ent(ent2_node->T_givers, ent1, ent2_node->T_givers->root) == NULL) {
                                Insert_Ent(ent2_node->T_givers, Create_Ent_Node(ent1_elem->ent_name));
                                ent2_node->num_of_givers++;
                                Check_Max(rel_node, ent2_node, ent2_elem);
                            }
                        }
                    }
                }
            }
            free(ent1);
            free(ent2);
            free(rel);
        } else if (strcmp(tok, "delrel") == 0) {
            //1st Word read
            tok = strtok(NULL, " \n");
            int len = strlen(tok);
            ent1 = (char*)malloc(sizeof(char)*(len+1));
            strcpy(ent1, tok);

            //2nd Word read
            tok = strtok(NULL, " \n");
            len = strlen(tok);
            ent2 = (char*)malloc(sizeof(char)*(len+1));
            strcpy(ent2, tok);

            //3rd Word read
            tok = strtok(NULL, " \n");
            len = strlen(tok);
            rel = (char*)malloc(sizeof(char)*(len+1));
            strcpy(rel, tok);

            num = hash((unsigned char* )ent1)%Hash_Size;
            hash_element *ent1_element = Find_In_Hash(num, hashTable, ent1);
            if (ent1_element != NULL) {
                hash_element *ent2_element;
                if (ent2 == ent1) {
                    ent2_element = ent1_element;
                } else {
                    num = hash ((unsigned char *)ent2)%Hash_Size;
                    ent2_element = Find_In_Hash(num, hashTable, ent2);
                }
                if (ent2_element != NULL) {
                    RB_Node *rel_node = Find_Rel(T_rel, rel, T_rel->root);
                    if (rel_node != NULL) {
                        //controllo se esiste entita'  2 in relazione con entita'  1
                        RB_Tree_Ent_Rel *ent2_in_rel = Find_Ent_Rel(rel_node->T_ent, ent2, rel_node->T_ent->root);
                        if (ent2_in_rel != NULL) {
                            //Qui controllo se ent2 ha tra i givers ent1
                            RB_Tree_Ent *ent1_in_givers = Find_Ent(ent2_in_rel->T_givers, ent1, ent2_in_rel->T_givers->root);
                            if (ent1_in_givers != NULL) {
                                //Posso procedere ad eliminare!
                                Delete_One_Ent_From_EntRel(ent1_in_givers, ent2_in_rel, rel_node, T_rel);
                            }
                        }
                    }
                }
            }
            free(ent1);
            free(ent2);
            free(rel);
        } else if (strcmp(tok, "delent") == 0) {
            tok = strtok(NULL, " \n");
            num = hash((unsigned char *)tok)%Hash_Size;
            hash_element *to_delete = Find_In_Hash(num, hashTable, tok);
            if (to_delete != NULL) {
                //pulisci tutto
                Search_For_To_Be_Delete_In_Rel(T_rel, T_rel->root, to_delete->ent_name);
                Clear_Rel(T_rel, to_delete_rel->root);
                Clear_Max(to_delete_rel);
                //Cancella entita' 
                Delete_From_Hash(to_delete, num, hashTable);
            }
        } else if (strcmp(tok, "end") == 0) {
            return 0;
        }
        tok = strtok(NULL, " \n");
        free(tok);
    }
}
