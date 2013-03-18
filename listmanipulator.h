#ifndef LISTMANIPULATOR_H
#define LISTMANIPULATOR_H

typedef struct {
        int x;
        int y;
}Point2D;

typedef struct{
        Point2D source;
        Point2D dest;
}Movement;

// Note: head nodes point the next to the 1st element and children to the end of the list
typedef struct Node_t {
        Movement movement;
        int eaten;      // for keeping track of eaten pieces when the move was done
        int alpha;
        int beta;
        int ev_sign;
        struct Node_t* children;
        struct Node_t* next;
}Node;

Node* createNode(Movement movement, int alpha, int beta, int ev_sign, Node* children, Node* next) {
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->movement = movement;
        temp->alpha    = alpha;
        temp->beta     = beta;
        temp->ev_sign  = ev_sign;
        temp->children = NULL;
        temp->next     = NULL;
        return temp;
}

Node* createHeadNode() {
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->next = NULL;
        temp->children = temp;
        return temp;
}

Node* createNodeWithHead(Movement movement, int alpha, int beta, int ev_sign, Node* children, Node* next) {
        Node* temp     = createHeadNode();
        temp->next     = createNode(movement, alpha, beta, ev_sign, children, next);
        temp->children = temp->next;
        return temp;
}

Node* push(Node* container, Node* pushee) {     // lol. pushee. hahaha
        // NOTE: container is freed
        if (container->next == NULL) {
                free(container);
                return pushee;
        }
        
        if (pushee->next == NULL) {
                free(pushee);
                return container;
        }
        Node* temp   = pushee;
        pushee       = pushee->children;        // go to the last element
        pushee->next = container->next;
        temp->children  = container->children;
        
        free(container);
        return temp;
}

#endif
