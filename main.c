#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Player{
    char* firstName;
    char* secondName;
    int points;
};

typedef struct Team{
    char* name;
    struct Player* players;
    int numPlayers;
    float medie_puncte;
    struct Team* next;
}Team;

void print(Team *head, FILE *out) {
    while (head != NULL) {
        fprintf(out, "%s\n", head->name);
        head = head->next;
    }
}    

void addAtBeginning(Team** head, Team v) {
    Team *newNode = (Team*)malloc(sizeof(Team));
    *newNode = v;
    newNode->next = *head;
    *head = newNode;
}

int cea_mai_apropiata_put_2(int numar_echipe) {
    int putere=1;
    while(putere * 2 <= numar_echipe) {
        putere *= 2;
    }
    return putere;
}

Team* echipa_de_sters(Team *head) {
    if(head==NULL) return NULL;
    float min_punct=head->medie_puncte;
    Team *aux=head;
    Team *acm=head;
    while(acm!=NULL)
    {
        if(acm->medie_puncte < min_punct)
        {
            min_punct=acm->medie_puncte;
            aux=acm;
        }
        acm=acm->next;
    }
    return aux;
}

void deleteTeam(Team **head, Team *target) {
    if (*head == NULL || target == NULL) return;
    Team *temp = *head;
    Team *prev = NULL;
    if (temp == target) {
        *head = temp->next;
        for (int i = 0; i < temp->numPlayers; i++) {
            free(temp->players[i].firstName);
            free(temp->players[i].secondName);
        }
        free(temp->players);
        free(temp->name);
        free(temp);
        return;
    }
    while (temp != NULL && temp != target) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    for (int i = 0; i < temp->numPlayers; i++) {
        free(temp->players[i].firstName);
        free(temp->players[i].secondName);
    }
    free(temp->players);
    free(temp->name);
    free(temp);
}

typedef struct Meci {
    Team *echipa1;
    Team *echipa2;
    struct Meci *next;
}Meci;

typedef struct queue{
    Meci *front;
    Meci *rear;
}queue;

typedef struct stack{
    Team *echipa;
    struct stack *next;
}stack;

queue* createQueue() {
    queue *q = (queue*)malloc(sizeof(queue));
    if (q) {
        q->front = q->rear = NULL;
    }
    return q;
}

int isEmptyQueue(queue* q) {
    return (q->front == NULL);
}

void enQueue(queue* q, Team* t1, Team* t2) {
    Meci* newNode = (Meci*)malloc(sizeof(Meci));
    newNode->echipa1 = t1;
    newNode->echipa2 = t2;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

Meci* deQueue(queue* q) {
    if (isEmptyQueue(q)) return NULL;
    Meci* temp = q->front;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

int isEmptyStack(stack* top) {
    return (top == NULL);
}

void push(stack** top, Team* t) {
    stack* newNode = (stack*)malloc(sizeof(stack));
    newNode->echipa = t;
    newNode->next = *top;
    *top = newNode;
}

Team* pop(stack** top) {
    if (isEmptyStack(*top)) return NULL;
    stack* temp = *top;
    Team* aux = temp->echipa;
    *top = (*top)->next;
    free(temp);
    return aux;
}

void updatePoints(Team *t) {
    t->medie_puncte += 1.0f;
    for (int i = 0; i < t->numPlayers; i++) {
        t->players[i].points++;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 4) return 1;
    FILE *fc=fopen(argv[1],"r");
    int cerinte[5]={0};
    for(int i=0;i<5;i++)
        if(fscanf(fc,"%d",&cerinte[i]) != 1) break;
    fclose(fc);
    FILE *file=fopen(argv[2],"r");
    Team *head=NULL;
    Team *top8=NULL;
    int numar_echipe;
    fscanf(file,"%d",&numar_echipe);
    for(int i=0;i<numar_echipe;i++)
    {
        struct Team echipa_curenta;
        fscanf(file,"%d",&echipa_curenta.numPlayers);
        fgetc(file);
        char buffernume[256];
        fscanf(file," %[^\n]",buffernume);
        int len = strlen(buffernume);
        while(len > 0 && (buffernume[len-1] == '\r' || buffernume[len-1] == ' ' || buffernume[len-1] == '\t' || buffernume[len-1] == '\n')) {
            buffernume[len-1] = '\0';
            len--;
        }
        echipa_curenta.name=(char*)malloc((strlen(buffernume)+1)*sizeof(char));
        strcpy(echipa_curenta.name,buffernume);
        echipa_curenta.players=(struct Player*)malloc(echipa_curenta.numPlayers*sizeof(struct Player));
        float suma_puncte=0;
        for(int k=0;k<echipa_curenta.numPlayers;k++)
        {
            char bufferfirstname[256];
            char buffersecondname[256];
            int points;
            fscanf(file,"%s",bufferfirstname);
            fscanf(file,"%s",buffersecondname);
            fscanf(file,"%d",&points);
            echipa_curenta.players[k].firstName=(char*)malloc((strlen(bufferfirstname)+1)*sizeof(char));
            strcpy(echipa_curenta.players[k].firstName,bufferfirstname);
            echipa_curenta.players[k].secondName=(char*)malloc((strlen(buffersecondname)+1)*sizeof(char));
            strcpy(echipa_curenta.players[k].secondName,buffersecondname);
            echipa_curenta.players[k].points=points;
            suma_puncte+=points;
        }
        echipa_curenta.medie_puncte=suma_puncte/echipa_curenta.numPlayers;
        addAtBeginning(&head,echipa_curenta);
    }
    int putere_2=cea_mai_apropiata_put_2(numar_echipe);
    while(numar_echipe!=putere_2)
    {
        Team *aux=echipa_de_sters(head);
        if(aux!=NULL)
        {
            deleteTeam(&head, aux);
            numar_echipe--;
        }
    }
    FILE *fout = fopen(argv[3], "w");
    if(cerinte[0]==1)
        print(head,fout);
    if(cerinte[1]==1)
    {
        queue *q = createQueue();
        Team *temp=head;
        while(temp!=NULL)
        {
            Team *echipa1=temp;
            Team *echipa2=temp->next;
            enQueue(q,echipa1,echipa2);
            if(echipa2!=NULL)
                temp=echipa2->next;
            else
                temp=NULL;
        }
        int runda=1;
        while(numar_echipe>1)
        {
            fprintf(fout,"\n--- ROUND NO:%d\n",runda);
            stack *winners=NULL;
            stack *losers=NULL;
            while(!isEmptyQueue(q))
            {
                Meci *m=deQueue(q);
                fprintf(fout,"%-33s-%33s\n",m->echipa1->name,m->echipa2->name);
                if(m->echipa1->medie_puncte > m->echipa2->medie_puncte)
                {
                    updatePoints(m->echipa1);
                    push(&winners,m->echipa1);
                    push(&losers,m->echipa2);
                }
                else
                {
                    updatePoints(m->echipa2);
                    push(&winners,m->echipa2);
                    push(&losers,m->echipa1);
                }
                free(m);
            }
            fprintf(fout,"\nWINNERS OF ROUND NO:%d\n",runda);
            stack *tempS=winners;
            while (tempS!=NULL)
            {
                fprintf(fout,"%-34s-  %.2f\n",tempS->echipa->name,tempS->echipa->medie_puncte);
                tempS=tempS->next;
            }
            numar_echipe/=2;
            if (numar_echipe == 8) 
            {
                stack *tempTop = winners;
                while (tempTop != NULL) {
                    Team *copie = (Team*)malloc(sizeof(Team));
                    *copie = *(tempTop->echipa);
                    copie->next = top8;
                    top8 = copie;
                    tempTop = tempTop->next;
                }
            }
            while(!isEmptyStack(winners))
            {
                Team *echipa1=pop(&winners);
                Team *echipa2=pop(&winners);
                if(echipa1!=NULL && echipa2!=NULL)
                    enQueue(q,echipa1,echipa2);
            }
            while(!isEmptyStack(losers))
            {
                Team *loserTeam = pop(&losers);
                if(loserTeam != NULL) {
                    for(int i = 0; i < loserTeam->numPlayers; i++) {
                        free(loserTeam->players[i].firstName);
                        free(loserTeam->players[i].secondName);
                    }
                    free(loserTeam->players);
                    free(loserTeam->name);
                    free(loserTeam);
                }
            }
            runda++;
        }
        free(q);
    }
    fclose(file);
    fclose(fout);
    return 0;
}