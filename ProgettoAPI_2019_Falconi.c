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
                    w->color = x->parent->color;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate_Ent(T, x->parent);
                x = T->root;
            }
        } else {w = x->parent->left;
            if (x->color == RED) {
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
    RB_Tree_Ent_Rel * y = x -> right;
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
    while (z->parent->color == RED) {
        RB_Tree_Ent_Rel * y;
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
                    w->color = x->parent->color;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                Left_Rotate_Ent_Rel(T, x->parent);
                x = T->root;
            }
        } else {w = x->parent->left;
            if (x->color == RED) {
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
        HashTable[key]->ent_name = name;
        HashTable[key]->next = NULL;
        return;
    }
    hash_element *new = (hash_element *)malloc(sizeof(hash_element));
    new->ent_name = name;
    new->next = HashTable[key]->next;
    HashTable[key]->next = new;
}

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
/*
void Clear_Max_Entities(RB_Tree_Ent_Head *rel_node, RB_Tree_Ent *root) {
    if (root == rel_node->nil) {
        return;
    }
    Clear_Max_Entities(rel_node, root->left);
    RB_Tree_Ent *right = root->right;
    Delete_Ent(rel_node, root);
    free(root);
    Clear_Max_Entities(rel_node, right);
}*/

void Clear_Max (RB_Tree_Ent_Head *rel_node) {
    RB_Tree_Ent *temp;
    while (rel_node->root != rel_node->nil) {
        temp = rel_node->root;
        Delete_Ent(rel_node, rel_node->root);
        free(temp);
    }
}

void Check_Max(RB_Node *rel_node, RB_Tree_Ent_Rel *ent_node, hash_element *ent2_element) {
    //se max_size è il massimo, non faccio niente
    if (rel_node->max_size > ent_node->num_of_givers) {
        return;
    }
    if (rel_node->max_size < ent_node->num_of_givers) {
        Clear_Max(rel_node->T_max_ent);/*
        free(rel_node->T_max_ent->nil);
        free(rel_node->T_max_ent);
        rel_node->T_max_ent = Create_Ent_Head();*/
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




    /*if (rel_node->max_size > ent_node->num_of_givers) {
        return;
    }
    if (rel_node->max_size < ent_node->num_of_givers) {
        rel_node->max_size = ent_node->num_of_givers;
        Clear_Max_Entities(rel_node, rel_node->T_max_ent->root);
        //Aggiungi lo fa la riga sotto
        Insert_Ent(rel_node->T_max_ent, Create_Ent_Node(ent2_element->ent_name));
        return;
    }
    //hash_element *temp = Find_In_Hash((long) ent_node->hash_key, hash, ent_node->ent_name);
    RB_Tree_Ent *temp = Find_Ent(rel_node->T_max_ent, ent2_element->ent_name, rel_node->T_max_ent->root);
    if (temp != NULL) {
        Insert_Ent(rel_node->T_max_ent, Create_Ent_Node(ent2_element->ent_name));
    }*/
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
    fputs(root->rel_name, stdout);
    fputc(' ', stdout);
    Inorder_Max_Tree(root->T_max_ent, root->T_max_ent->root);
    printf("%d", root->max_size);
    fputs("; ", stdout);
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


void Delete_One_Ent_From_Max (RB_Tree *T, RB_Node *root, char *name) {
    //Se ho più di un entita, elimino semplicemnte la mia entita dai massimi
    if (root->num_of_max_entities > 1) {
        RB_Tree_Ent *temp = Find_Ent(root->T_max_ent, name, root->T_max_ent->root);
        Delete_Ent(root->T_max_ent, temp);
        free(temp);
        root->num_of_max_entities--;
        return;
    }
    //Se ho più di un entita, elimino l'entità e...
    RB_Tree_Ent *temp = Find_Ent(root->T_max_ent, name, root->T_max_ent->root);
    Delete_Ent(root->T_max_ent, temp);
    free(temp);
    root->num_of_max_entities--;
    root->max_size = 0;
    //E... ricerco il/i nuovo/i max
    Search_For_Max_Entities(root, root->T_ent, root->T_ent->root);
}

void Delete_One_Ent_From_EntRel (RB_Tree_Ent *ent1_node, RB_Tree_Ent_Rel *ent2_node, RB_Node *rel_node, RB_Tree *T) {
    Delete_Ent(ent2_node->T_givers, ent1_node);
    free(ent1_node);
    ent2_node->num_of_givers--;
    //Controllo se era tra i max
    if (rel_node->max_size == (ent2_node->num_of_givers +1)) {
        //TODO -> Elimino ent2_node dai max. Se i max ha 1 sola entità (ent2_node) devo ricercare il massimo
        Delete_One_Ent_From_Max(T, rel_node, ent2_node->ent_name);
    }
    if (ent2_node->num_of_givers == 0) {
        Delete_Ent_Rel(rel_node->T_ent, ent2_node);
        free(ent2_node->T_givers->nil);
        free(ent2_node->T_givers);
        free(ent2_node);
        rel_node->num_of_entities--;
        if (rel_node->num_of_entities == 0) {
            Clear_Max(rel_node->T_max_ent);
            Delete_Rel(T, rel_node);
            free(rel_node->T_max_ent->nil);
            free(rel_node->T_max_ent);
            free(rel_node->T_ent->nil);
            free(rel_node->T_ent);
            free(rel_node);
        }
    }
}


//Debug Functions
void debug_print_all_3 (RB_Tree_Ent_Head *T, RB_Tree_Ent *root) {
    if (root == T->nil) {
        return;
    }
    debug_print_all_3(T, root->left);
    //
    printf("%s", root->ent_name);
    debug_print_all_3(T, root->right);
}

void debug_print_all_2 (RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *root) {
    if (root == T->nil) {
        return;
    }
    debug_print_all_2(T, root->left);
    //
    printf("Entita %s, #givers %d\n", root->ent_name, root->num_of_givers);
    debug_print_all_3(root->T_givers, root->T_givers->root);
    printf("\n");
    debug_print_all_2(T, root->right);
}

void debug_print_all_max(RB_Tree_Ent_Head *T, RB_Tree_Ent *root) {
    if (root == T->nil) {
        return;
    }
    debug_print_all_max(T, root->left);
    //
    printf("%s, ", root->ent_name);
    debug_print_all_max(T, root->right);
}

void debug_print_all(RB_Tree *T, RB_Node *root){
    if (root == T->nil) {
        return;
    }
    debug_print_all(T, root->left);
    //
    printf("Relazione: %s MAX: %d, num_of_max_ent %d\nMax_Ents: ", root->rel_name, root->max_size, root->num_of_max_entities);
    debug_print_all_max(root->T_max_ent, root->T_max_ent->root);
    printf("\n");
    debug_print_all_2(root->T_ent, root->T_ent->root);
    debug_print_all(T, root->right);
}

void debug_delete_max_entities(RB_Tree_Ent_Head *rel_node, RB_Tree_Ent *root) {
    RB_Tree_Ent *temp;
    while (rel_node->root != rel_node->nil) {
        temp = rel_node->root;
        Delete_Ent(rel_node, rel_node->root);
        free(temp);
    }
}

void debug_inorder_ent (RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *root) {
    if (root == T->nil) {
        return;
    }
    debug_inorder_ent(T, root->left);
    debug_delete_max_entities(root->T_givers, root->T_givers->root);
    debug_inorder_ent(T, root->right);
}

void debug_delete_t_divers_check(RB_Tree_Ent_Rel_Head *T, RB_Tree_Ent_Rel *root) {
    if (root == T->nil) {
        return;
    }
    debug_delete_t_divers_check(T, root->left);
    //
    debug_delete_max_entities(root->T_givers, root->T_givers->root);
    free(root->T_givers->nil);
    free(root->T_givers);
    //
    debug_delete_t_divers_check(T, root->right);
}

void debug_delete_T_Ent (RB_Tree_Ent_Rel_Head *T) {
    RB_Tree_Ent_Rel *temp;
    while (T->root != T->nil) {
        temp = T->root;
        Delete_Ent_Rel(T, T->root);
        free(temp);
    }
}

void debug_delete_max_check (RB_Tree *T, RB_Node *root) {
    if (root == T->nil) {
        return;
    }
    debug_delete_max_check(T, root->left);
    //
    //Cancello le max ent e la struttura che ne fa riferimento
    debug_delete_max_entities(root->T_max_ent, root->T_max_ent->root);
    free(root->T_max_ent->nil);
    free(root->T_max_ent);
    //Chiamo la funz che cancellerà i T_Givers di ogni nodo delle entita
    debug_delete_t_divers_check(root->T_ent, root->T_ent->root);
    //Cancello le entità e ogni riferimento alla struttura
    debug_delete_T_Ent(root->T_ent);
    free(root->T_ent->nil);
    free(root->T_ent);
    //
    debug_delete_max_check(T, root->right);
}

void debug_delete_T_Rel (RB_Tree *T) {
    RB_Node *temp;
    while (T->root != T->nil) {
        temp = T->root;
        Delete_Rel(T, T->root);
        free(temp->rel_name);
        free(temp);
    }
}

void debug_free_hashmap (hash_element *ht[]) {
    int i;
    for ( i = 0; i < Hash_Size; i++) {
        if (ht[i] != NULL) {
            hash_element *temp = ht[i];
            while (temp != NULL) {
                ht[i] = ht[i]->next;
                free(temp->ent_name);
                free(temp);
                temp = ht[i];
            }
        }
    }
}

int main() {
    hash_element *hashTable[Hash_Size];
    int i;
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
    size_t bufsize = 100;
    size_t  chars;
    line = (char *)malloc(sizeof(char) *bufsize);
    //
    int ent1_key;
    int ent2_key;
    char *ent1;
    char *ent2;
    char *rel;
    //DA TOGLIEREEEE
    freopen("input.txt", "r", stdin);
    //
    while (1) {
        //chars = getline(&line, &bufsize, stdin);
        fgets(line, 100, stdin);
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

            ent1_key = hash((unsigned char*) ent1)%Hash_Size;
            hash_element *ent1_elem = Find_In_Hash((long) ent1_key, hashTable, ent1);
            if (ent1_elem != NULL) {
                hash_element *ent2_elem;
                if (strcmp(ent1, ent2) == 0) {
                    ent2_elem = ent1_elem;
                } else {
                    ent2_key = hash((unsigned char *) ent2)%Hash_Size;
                    ent2_elem = Find_In_Hash((long) ent2_key, hashTable, ent2);
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
                        //Verifico se ho già ent2 nella rel
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

                    /*
                    if (rel_node == NULL) {
                        //Se rel_node NON esiste, creo il nodo
                        rel_node = Insert_Rel(T_rel, Create_Node_Rel(rel));
                        //Creo il rel_list ent2, creo il nodo ent_list ent1, associo ent_1 a ent2 e ent_2 a rel_node
                        rel_list_element = Create_Rel_List_Element(ent2_elem->ent_name);
                        Add_Ent_List_In_Rel_List(rel_list_element, Create_Ent_List_Element(ent1_elem->ent_name));
                        Add_Rel_List_In_Ent_Node(rel_node, rel_list_element);
                        Update_Max(rel_node, rel_list_element);
                    } else {
                        //Verifico se rel_node ha già la mia ent2
                        rel_list *relList = Find_Rel_In_Ent(rel_node, ent2_elem->ent_name);
                        if (relList == NULL) {
                            //Se non ho l'elemento Rel_List
                            relList = Create_Rel_List_Element(ent2_elem->ent_name);
                            Add_Ent_List_In_Rel_List(relList, Create_Ent_List_Element(ent1_elem->ent_name));
                            Add_Rel_List_In_Ent_Node(rel_node, relList);
                            Update_Max(rel_node, relList);
                        } else {
                            //Verifico se ent1_node è già presente nella rel_list
                            if (Find_Ent_in_Ent_List(relList, ent1_elem->ent_name) == 0) {
                                Add_Ent_List_In_Rel_List(relList, Create_Ent_List_Element(ent1_elem->ent_name));
                                Update_Max(rel_node, relList);
                                //Update_Order_Add(relList, rel_node);
                            }
                        }
                    }*/
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

            hash_element *ent1_element = Find_In_Hash(((long)hash((unsigned char*) ent1)%Hash_Size), hashTable, ent1);
            if (ent1_element != NULL) {
                hash_element *ent2_element;
                if (ent2 == ent1) {
                    ent2_element = ent1_element;
                } else {
                    ent2_element = Find_In_Hash(((long)hash((unsigned char*) ent2)%Hash_Size), hashTable, ent2);
                }
                if (ent2_element != NULL) {
                    RB_Node *rel_node = Find_Rel(T_rel, rel, T_rel->root);
                    if (rel_node != NULL) {
                        //controllo se esiste entità 2 in relazione con entità 1
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
            //
        } else if (strcmp(tok, "delent") == 0) {
            tok = strtok(NULL, " \n");
            hash_element *to_delete = Find_In_Hash(((long) hash((unsigned char*)tok)), hashTable, tok);
            if (to_delete != NULL) {
                //pulisci tutto
            }
            /*
            tok = strtok(NULL, " \"\n");
            rb_node *to_be_deleted = Find(T_ent, tok, T_ent->root);
            //If ent1 is being monitored
            if (to_be_deleted != NULL) {
                Inorder_Delete_Ent_Elements (T_ent, T_ent->root, to_be_deleted->key, T_rel);
                //Delete_Rel_Node(T_rel, T_rel->root);   Faccio dopo
                if (to_be_deleted->relations != NULL) {
                    Delete_All_Of_My_Entity(to_be_deleted, T_rel);
                }
                //Nuova Aggiunta
                Clear_No_Relations_Node(T_rel, T_rel->root);
                //
                Delete(T_ent, to_be_deleted);
                free (to_be_deleted->key);
                free(to_be_deleted);
            }*/
        }
        else if (strcmp(tok, "end") == 0) {
            //pulisco in ordine, Tmax, Tgivers e Tent
            debug_delete_max_check(T_rel, T_rel->root);
            //Pulisco Trel
            debug_delete_T_Rel(T_rel);
            free(T_rel->nil);
            free(T_rel);
            free(line);
            debug_free_hashmap(hashTable);
            return 0;
        } else if(strcmp(tok, "debug") == 0) {/*
            printf("\nentities:\n");
            Inorder( T_ent -> root, T_ent);
            printf("\nrelationships\n");
            Inorder_Rel(T_rel -> root, T_rel);
            printf("\n");*/
            debug_print_all(T_rel, T_rel->root);
            printf("\n");
            printf("\n");

        }
        tok = strtok(NULL, " \n");
        free(tok);
    }


}
