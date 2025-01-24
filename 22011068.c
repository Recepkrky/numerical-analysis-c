//
//  main.c
//  Sayisal Analiz projesi
//
//  Created by Recep Karakaya on 27.04.2024.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define PI 3.14159265358979323846

float max(float a, float b) {
    if (a > b) {
        return a; // a daha buyukse a'yi dondur
    } else {
        return b; // b daha buyukse b'yi dondur
    }
}
float min(float a, float b) {
    if (a < b) {
        return a; // a daha kucukse a'yi dondur
    } else {
        return b; // b daha kucukse b'yi dondur
    }
}
unsigned long long factoriyel(int n) {
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factoriyel(n - 1);
}
// Stack yapısı ve işlemleri
typedef struct {
    float* data;
    int top;
    int capacity;
} Stack;

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->data = (float*)malloc(sizeof(float) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

bool isStackFull(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

bool isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, float value) {
    if (isStackFull(stack)) {
        printf("Stack overflow\n");
        return;
    }
    stack->data[++(stack->top)] = value;
}

float pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Stack underflow\n");
        return -1;
    }
    return stack->data[(stack->top)--];
}

// Çift yönlü bağlantılı liste düğümü
typedef struct Node {
    float data;  // Düğümdeki veri (float türünde).
    struct Node* next;  // Bir sonraki düğüme işaretçi.
    struct Node* prev;  // Önceki düğüme işaretçi.
} Node;

// Çift yönlü bağlantılı liste için yeni düğüm oluşturma
Node* createNode(float value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}
//cift yonlu listede listeyi yazdirma
void printList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%.2f <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
// Çift yönlü listeye başa öğe ekleme
void addToHead(Node** head, float value) {
    Node* newNode = createNode(value);
    newNode->next = *head;
    if (*head != NULL) {
        (*head)->prev = newNode;
    }
    *head = newNode;
}

// Çift yönlü listeye sona öğe ekleme
void addToTail(Node** head, float value) {
    Node* newNode = createNode(value);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
}
// Çift yönlü listede aradaki bir düğümü silme fonksiyonu
void deleteNode(Node** head, Node* nodeToDelete) {
    if (nodeToDelete == NULL) {
        printf("Node is NULL, cannot delete.\n");
        return;
    }
    
    // Baş düğümse
    if (nodeToDelete == *head) {
        *head = nodeToDelete->next;
    }
    
    // Eğer silinen düğüm son değilse
    if (nodeToDelete->next != NULL) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }
    
    // Eğer silinen düğüm baş değilse
    if (nodeToDelete->prev != NULL) {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    
    free(nodeToDelete);  // Düğümü bellekten kaldır.
}
void deleteAndReset(Node** head_ref, Node** current) {
    if (*current != NULL && (*current)->next != NULL) {
        *current = (*current)->next; // İleri hareket
        deleteNode(head_ref, (*current)->prev); // Önceki düğümü sil
        *current = *head_ref; // Liste başına dön
    } else if (*current != NULL && (*current)->next == NULL) {
        deleteNode(head_ref, *current); // Mevcut düğümü sil
        *current = *head_ref; // Liste başına dön
    }

   
}

// Çift yönlü listeden baştan öğe çıkarma
float removeFromHead(Node** head) {
    if (*head == NULL) {
        printf("List is empty\n");
        return -1;
    }
    Node* temp = *head;
    float value = temp->data;
    *head = temp->next;
    if (*head != NULL) {
        (*head)->prev = NULL;
    }
    free(temp);
    return value;
}

// Çift yönlü listeden önceki iki düğüme erişme
Node* getPreviousTwo(Node* node) {
    if (node == NULL || node->prev == NULL || node->prev->prev == NULL) {
        printf("Cannot access previous two nodes\n");
        return NULL;
    }
    return node->prev->prev;  // Önceki iki düğüme erişim.
}
// Gauss eliminasyonu ile denklem sistemini çözen fonksiyon
void gaussElimination(int n,float matrix[n][n+1]) {
    int i, j, k;
    float scale;

    // İleriye dönük eleme
    for (i = 0; i < n- 1; i++) {
        for (j = i + 1; j < n; j++) {
            scale = matrix[j][i] / matrix[i][i];
            for (k = 0; k <= n; k++) {
                matrix[j][k] -= scale * matrix[i][k];
            }
        }
    }

    // Geriye dönük eleme
    for (i = n - 1; i >= 0; i--) {
        for (j = i - 1; j >= 0; j--) {
            scale = matrix[j][i] / matrix[i][i];
            for (k = n; k >= i; k--) {
                matrix[j][k] -= scale * matrix[i][k];
            }
        }
    }

    // Denklemleri normalize etme
    for (i = 0; i < n; i++) {
        scale = matrix[i][i];
        for (j = 0; j <= n; j++) {
            matrix[i][j] /= scale;
        }
    }
}
void gauss_seidel(float A[][10], float x[], int n, int iterasyon_sayisi , float epsilon) {
    int i, j = 0;
    float fark1 = 1.0 ,fark2 = 1.0 ,fark3 = 1.0 ,prev;
        while(iterasyon_sayisi != j+1 && (epsilon < fark1 || epsilon < fark2 || epsilon < fark3 )){
            printf("\n%d.iterasyon\n",j+1);
            prev = x[0];
            x[0] = (A[0][3] - A[0][1]*x[1] - A[0][2]*x[2])/A[0][0] ;
            fark1 = max(x[0],prev)-min(prev,x[0]);
            for(i=0 ; i < n ; i++){
                printf("x%d: %f\n",i+1,x[i]);
            }
            printf("Aradaki fark degeri: %fdir.\n",fark1);
            prev = x[1];
            x[1] = (A[1][3] - A[1][2]*x[2] - A[1][0]*x[0])/A[1][1] ;
            fark2 = max(x[1],prev)-min(prev,x[1]);
            for(i=0 ; i < n ; i++){
                printf("x%d: %f\n",i+1,x[i]);
            }
            printf("Aradaki fark degeri: %fdir.\n",fark2);
            prev = x[2];
            x[2] = (A[2][3] - A[2][0]*x[0] - A[2][1]*x[1])/A[2][2] ;
            fark3 = max(x[2],prev)-min(prev,x[2]);
            for(i=0 ; i < n ; i++){
                printf("x%d: %f\n",i+1,x[i]);
            }
            printf("Aradaki fark degeri: %fdir.\n",fark3);
            j++;
    }
    if (iterasyon_sayisi == j+1) {
        printf("\nrMax iterasyona ulasildi");
    }
    else if (epsilon > fark1 && epsilon > fark2 && epsilon > fark3){
        printf("\nAradaki fark degeri epsilondan daha kucuk");
    }
}

void fonksiyonalma (char* fonki, float* diz){
    printf("Fonksiyonu giriniz.\n");
    printf(" '^' üs operatörü, '_' taban operatörü olarak kullaniniz.\n");
    scanf("%s",fonki);
    int    i=0,j=0,k,l,sonuc;
    
    while ( fonki[i] != '\0'){
        if (fonki[i] == 's'){
            diz[j]=10007;
            i+=2;
            j++;
        }
        if (fonki[i] == 'c'){
            if(fonki[i+2] == 's'){
                diz[j]=10008;
                i+=2;
                j++;
            }
        }
        if (fonki[i] == 't'){
            diz[j]=10009;
            i+=2;
            j++;
        }
        if (fonki[i] == 'c'){
            if(fonki[i+2] == 't'){
                diz[j]=10010;
                i+=2;
                j++;
            }
        }
        //arcsin
        if (fonki[i] == 'a'){
            if(fonki[i+2] == 'c'){
                if(fonki[i+3] == 's'){
                    diz[j]=10011;
                    i+=5;
                    j++;
                }
            }
        }
        
        //arccos
        if (fonki[i] == 'a'){
            if(fonki[i+2] == 'c'){
                if(fonki[i+5] == 's'){
                    diz[j]=10012;
                    i+=5;
                    j++;
                }
            }
        }
        //arctan
        if (fonki[i] == 'a'){
            if(fonki[i+2] == 'c'){
                if(fonki[i+3] == 't'){
                    diz[j]=10013;
                    i+=5;
                    j++;
                }
            }
        }
        //arccot
        if (fonki[i] == 'a'){
            if(fonki[i+2] == 'c'){
                if(fonki[i+3] == 't'){
                    diz[j]=10014;
                    i+=5;
                    j++;
                }
            }
        }
        //toplama cikartma carpma bolme
        if(fonki[i] == '+'){
            diz[j] = 10001;
            j++;
        }
        if(fonki[i] == '-'){
            diz[j] = 10002;
            j++;
        }
        if(fonki[i] == '*'){
            diz[j] = 10003;
            j++;
        }
        if(fonki[i] == '/'){
            diz[j] = 10004;
            j++;
        }
        if(fonki[i] == '^'){
            diz[j] = 10005;
            j++;
        }
        if(fonki[i] == '_'){
            diz[j] = 10006;
            j++;
        }
        if(fonki[i] == 'x'){
            if(fonki[i-1] >= '0' && fonki [i-1] <= '9' ){
                diz[j]=10003;
                j++;
            }
            diz[j] = 9997;
            j++;
        }
        if(fonki[i] == '('){
            diz[j] = 9998;
            j++;
        }
        if(fonki[i] == ')'){
            diz[j] = 9999;
            j++;
        }
        if(fonki[i] == 'e'){
            diz[j] = 2.7182818284 ;
            j++;
        }
        if( fonki[i] >= '0' && fonki[i] <= '9' ) {
            k=0;
            while ( (fonki[i+k] >= '0' && fonki[i+k] <= '9') ){
                k++;
            }
            for (l=0;l<k;l++){
                diz[j] = diz[j]+((fonki[i+l]-48)*pow(10, k-l-1));
            }
            i = i+k-1 ;
            j++;
        }
        i++;
    }
    }
void Linkedlistcopy(Node** copyList, Node** Newlist){
    Node* current = *copyList;
    while(current != NULL){
        addToTail(Newlist, current->data);
        current = current->next;
    }
    
}
// Function to get the minor of a matrix
void get_minor(float **matrix, float **minor, int row, int col, int n) {
    int i, j, k = 0, l = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != row && j != col) {
                minor[k][l] = matrix[i][j];
                l++;
                if (l == n - 1) {
                    k++;
                    l = 0;
                }
            }
        }
    }
}

// Function to calculate the determinant of a matrix
int determinant(float **matrix, int n) {
    float det = 0;
    int i;
    float **minor;
    minor = (float **)malloc((n - 1) * sizeof(float *));
    for (i = 0; i < n - 1; i++)
        minor[i] = (float *)malloc((n - 1) * sizeof(float));
    if (n == 1)
        return matrix[0][0];
    if (n == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    for (i = 0; i < n; i++) {
        get_minor(matrix, minor, 0, i, n);
        det += ((i % 2 == 0) ? 1 : -1) * matrix[0][i] * determinant(minor, n - 1);
    }
    for (i = 0; i < n - 1; i++)
        free(minor[i]);
    free(minor);
    return det;
}
// Function to transpose a matrix
void transposeM(float **matrix, float **transpose, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            transpose[i][j] = matrix[j][i];
        }
    }
}
// Function to calculate the cofactor matrix
void cofactor_matrix(float **matrix, float **cofactor, int n) {
    int i, j;
    float **minor;
    minor = (float **)malloc((n - 1) * sizeof(float *));
    for (i = 0; i < n - 1; i++)
        minor[i] = (float *)malloc((n - 1) * sizeof(float));
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            get_minor(matrix, minor, i, j, n);
            cofactor[j][i] = ((i + j) % 2 == 0) ? determinant(minor, n - 1) : -determinant(minor, n - 1);
        }
    }
   //transposeM(cofactor, cofactor, n);
    for (i = 0; i < n - 1; i++)
        free(minor[i]);
    free(minor);
}
void Tersmatris( float **matrix, float **ters ,int n , float divisor){
    int i,j;
        for ( i = 0; i < n; i++) {
            for ( j = 0; j < n; j++) {
                ters[i][j] = ( matrix[i][j] / divisor);
            }
        }
    }

void fx_hesaplama( Node* node2){
    
    Node* current = node2;
    while (current != NULL && node2->next != NULL) {
        while (current->data < 10001 && current != NULL) {
            current = current->next;
        }
        if ( current->data == 10001){
            getPreviousTwo(current)->data = getPreviousTwo(current)->data + current->prev->data;
            deleteNode(&node2, current->prev);
            deleteAndReset(&node2, &current);
            
        }
        else if ( current->data == 10002){
            getPreviousTwo(current)->data = getPreviousTwo(current)->data - current->prev->data;
            deleteNode(&node2, current->prev);
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10003){
            getPreviousTwo(current)->data = getPreviousTwo(current)->data * current->prev->data;
            deleteNode(&node2, current->prev);
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10004){
            getPreviousTwo(current)->data = getPreviousTwo(current)->data / current->prev->data;
            deleteNode(&node2, current->prev);
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10005){
            getPreviousTwo(current)->data = pow(getPreviousTwo(current)->data , current->prev->data);
            deleteNode(&node2, current->prev);
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10006){
            getPreviousTwo(current)->data = log(current->prev->data) / log(getPreviousTwo(current)->data) ;
            deleteNode(&node2, current->prev);
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10007){
            double angle_in_degrees = current->prev->data ;
            double angle_in_radians = angle_in_degrees * (PI / 180); // Radyana dönüştürme
            double sin_value = sin(angle_in_radians);
            current->prev->data = (float) sin_value;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10008){
            double angle_in_degrees = current->prev->data ;
            double angle_in_radians = angle_in_degrees * (PI / 180); // Radyana dönüştürme
            double cos_value = cos(angle_in_radians);
            current->prev->data = (float) cos_value;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10009){
            double angle_in_degrees = current->prev->data ;
            double angle_in_radians = angle_in_degrees * (PI / 180); // Radyana dönüştürme
            double tan_value = tan(angle_in_radians);
            current->prev->data = (float) tan_value;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10010){
            double angle_in_degrees = current->prev->data ;
            double angle_in_radians = angle_in_degrees * (PI / 180); // Radyana dönüştürme
            double value = tan(angle_in_radians);// math kutuphanesinde cot ifadesi olmadigi icin 1 bolu tan yaparak cot ifadesini elde ettim
            current->prev->data = 1/(float) value;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10011){
            double angle_in_degrees = current->prev->data ;
            double value = asin(angle_in_degrees);
            double value2 = value * (180 / PI);
            current->prev->data = (float) value2;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10012){
            double angle_in_degrees = current->prev->data ;
            double value = acos(angle_in_degrees);
            double value2 = value * (180 / PI);
            current->prev->data = (float) value2;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10013){
            double angle_in_degrees = current->prev->data ;
            double value = atan(angle_in_degrees);
            double value2 = value * (180 / PI);
            current->prev->data = (float) value2;
            deleteAndReset(&node2, &current);
        }
        else if ( current->data == 10014){
            double angle_in_degrees = current->prev->data ;
            double arccot_value = (PI / 2) - atan(angle_in_degrees); // Arccot hesaplama
            double value2 = arccot_value * (180 / PI);
            current->prev->data = (float) value2;
            deleteAndReset(&node2, &current);
        }
    }
}
void shunting_yard(float* diz , Stack* stack1 , Node** node1){
    
    int  j=0;
    while(diz[j] != 0){
        if (diz[j] < 9998){
            addToTail(node1, diz[j]);
        }
        else if (diz[j] == 9998){
            push(stack1,diz[j]);
        }
        else if (diz[j] == 9999){
            while(stack1->data[stack1->top] != 9998){
                addToTail(node1, stack1->data[stack1->top]);
                pop(stack1);
            }
            pop(stack1);
        }
        else if (diz[j] >= 10001 ){
            if ( diz[j] == 10001 || diz[j] == 10002 ){
                while(stack1->data[stack1->top] >= 10001 ){
                    addToTail(node1, stack1->data[stack1->top]);
                    pop(stack1);
                }
                push(stack1, diz[j]);
            }
            else if ( diz[j] == 10003 || diz[j] == 10004 ){
                while(stack1->data[stack1->top] >= 10003 ){
                    addToTail(node1, stack1->data[stack1->top]);
                    pop(stack1);
                }
                push(stack1, diz[j]);
            }
            else if ( diz[j] == 10005){
                while(stack1->data[stack1->top] >= 10005 ){
                    addToTail(node1, stack1->data[stack1->top]);
                    pop(stack1);
                }
                push(stack1, diz[j]);
            }
            else if (diz[j] == 10006 ){
                while(stack1->data[stack1->top] >= 10006 ){
                    addToTail(node1, stack1->data[stack1->top]);
                    pop(stack1);
                }
                push(stack1, diz[j]);
            }
            else if (diz[j] >= 10007 && diz[j] <= 10010){
                while(stack1->data[stack1->top] >= 10007 && stack1->data[stack1->top] <= 10014 ){
                    addToTail(node1, stack1->data[stack1->top]);
                    pop(stack1);
                }
                    push(stack1, diz[j]);
            }
            else if (diz[j] >= 10011 && diz[j] <= 10014){
                while(stack1->data[stack1->top] >= 10011 && stack1->data[stack1->top] <= 10014 ){
                    addToTail(node1, stack1->data[stack1->top]);
                    pop(stack1);
                   
                }
                push(stack1, diz[j]);
                
            }
        }
        j++;
    }
    while(!isStackEmpty(stack1)){
        addToTail(node1, pop(stack1));
    }
}
void xdegeriniyerlestirme(Node* node3 , Node** node4 ,float xdeger){
    
    while(node3->next != NULL){
        if(node3->data == 9997){
            node3->data = xdeger;
        }
        node3 = node3->next;
    }
    
}
void bisection(){
    
    char fonk[100];
    float sayi_diz[100];
    int stack_capacity = 100 ;
    float x1 = 0, x2 = 0 ,x3 =0, epsilon, sonuc1, sonuc2,sonuc3, maxiterasyon , islem =0 ;
    // Stack oluştur
    Stack* stack = createStack(stack_capacity);
 
    Node* head1 = NULL;  // Boş liste.
    Node* head2 = NULL;
    fonksiyonalma(fonk,sayi_diz);
    shunting_yard(sayi_diz, stack, &head1);
    Linkedlistcopy(&head1, &head2);
    printf("Koku icene alan x1 ve x2 degerlerini giriniz.\n");
    scanf("%f %f",&x1,&x2);
    printf("Epsilon degeri nedir ?\n");
    scanf("%f",&epsilon);
    printf("Kac iterasyon olsun ?\n");
    scanf("%f",&maxiterasyon);
    if(epsilon < (max(x1,x2)-min(x1,x2))/2 && islem < maxiterasyon ){
        xdegeriniyerlestirme(head1, &head1, x1);
        fx_hesaplama(head1);
        sonuc1 =  head1->data;
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
        xdegeriniyerlestirme(head1, &head1, x2);
        fx_hesaplama(head1);
        sonuc2 = head1->data;
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
        while(epsilon < (max(x1,x2)-min(x1,x2))/2 && islem < maxiterasyon ){
            printf("%.0f . iterasyon\n",islem+1 );
            if(sonuc1 > 0 ){
                printf("ust = %f ==>> f(ust) = %f\n",x1,sonuc1);
                x3=(max(x1,x2)+min(x1,x2))/2 ;
                xdegeriniyerlestirme(head1, &head1, x3);
                fx_hesaplama(head1);
                sonuc3 =  head1->data;
                printf("orta = %f ==>> f(orta) = %f\nalt = %f ==>> f(alt) = %f\n",x3,sonuc3,x2,sonuc2);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
                if(sonuc3 >= 0){
                    x1=x3;
                    sonuc1=sonuc3;                 }
                else if(sonuc3 <=0 ){
                    x2=x3;
                    sonuc2 = sonuc3;
                    
                }
            }
            else if(sonuc2 > 0 ){
                printf("ust = %f ==>> f(ust) = %f\n",x2,sonuc2);
                x3=(max(x1,x2)+min(x2,x1))/2;
                xdegeriniyerlestirme(head1, &head1, x3);
                fx_hesaplama(head1);
                sonuc3 =  head1->data;
                printf("orta = %f ==>> f(orta) = %f\nalt = %f ==>> f(alt) = %f\n",x3,sonuc3,x1,sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
                if(sonuc3 >= 0){
                    x2=x3;
                    sonuc2=sonuc3;
                }
                else if(sonuc3 <=0 ){
                    x1=x3;
                    sonuc1 = sonuc3;
                }
            }
                        islem++;
            printf("Fark => %f\n\n\n", (max(x1,x2)-min(x1,x2))/2 );
        }
    }
    if (islem == maxiterasyon ){
        printf("Max iterasyona ulasildi.\nKok yaklasik olarak : %f dir.\nf(%f)nin degeri %f dir.\n",x3,x3,sonuc3);
    }
    else if( epsilon > (max(x1,x2)-min(x1,x2))/2){
        printf("Epsilon degeri farktan daha buyuk.\nKok yaklasik olarak : %f dir.\nf(%f)nin degeri %f dir.\n",x3,x3,sonuc3);

    }
}
void Regulafalsi(){
    char fonk[100];
    float sayi_diz[100];
    int stack_capacity = 100 ;
    float x1 = 0, x2 = 0 ,x3 =0, epsilon, sonuc1, sonuc2,sonuc3, maxiterasyon , islem =0 ;
    // Stack oluştur
    Stack* stack = createStack(stack_capacity);
 
    Node* head1 = NULL;  // Boş liste.
    Node* head2 = NULL;
    fonksiyonalma(fonk,sayi_diz);
    shunting_yard(sayi_diz, stack, &head1);
    Linkedlistcopy(&head1, &head2);
    printf("Koku icene alan x1 ve x2 degerlerini giriniz.\n");
    scanf("%f %f",&x1,&x2);
    printf("Epsilon degeri nedir ?\n");
    scanf("%f",&epsilon);
    printf("Kac iterasyon olsun ?\n");
    scanf("%f",&maxiterasyon);
    if(epsilon < (max(x1,x2)-min(x1,x2))/pow(2,islem+1) && islem < maxiterasyon ){
        xdegeriniyerlestirme(head1, &head1, x1);
        fx_hesaplama(head1);
        sonuc1 =  head1->data;
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
        xdegeriniyerlestirme(head1, &head1, x2);
        fx_hesaplama(head1);
        sonuc2 = head1->data;
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
        while(epsilon < (max(x1,x2)-min(x1,x2))/pow(2,islem+1) && islem < maxiterasyon ){
            printf("%.0f . iterasyon\n",islem+1 );
            if(sonuc1 > 0 ){
                printf("ust = %f ==>> f(ust) = %f\n",x1,sonuc1);
                x3=(x1*sonuc2-x2*sonuc1)/(sonuc2-sonuc1);
                xdegeriniyerlestirme(head1, &head1, x3);
                fx_hesaplama(head1);
                sonuc3 =  head1->data;
                printf("orta = %f ==>> f(orta) = %f\nalt = %f ==>> f(alt) = %f\n",x3,sonuc3,x2,sonuc2);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
                if(sonuc3 >= 0){
                    x1=x3;
                    sonuc1=sonuc3;
                }
                else if(sonuc3 <=0 ){
                    x2=x3;
                    sonuc2=sonuc3;
                }
            }
            else if(sonuc2 > 0 ){
                printf("ust = %f ==>> f(ust) = %f\n",x2,sonuc2);
                x3=(x2*sonuc1-x1*sonuc2)/(sonuc1-sonuc2);
                xdegeriniyerlestirme(head1, &head1, x3);
                fx_hesaplama(head1);
                sonuc3 =  head1->data;
                printf("orta = %f ==>> f(orta) = %f\nalt = %f ==>> f(alt) = %f\n",x3,sonuc3,x1,sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
                if(sonuc3 >= 0){
                    x2=x3;
                    sonuc2=sonuc3;
                }
                else if(sonuc3 <=0 ){
                    x1=x3;
                    sonuc1=sonuc3;
                }
            }
                        islem++;
            printf("Fark => %f\n\n\n", (max(x1,x2)-min(x1,x2))/2 );
        }
    }
    if (islem == maxiterasyon ){
        printf("Max iterasyona ulasildi.\nKok yaklasik olarak : %f dir.\nf(%f)nin degeri %f dir.\n",x3,x3,sonuc3);
    }
    else if( epsilon > (max(x1,x2)-min(x1,x2))/pow(2,islem+1)) {
        printf("Epsilon degeri farktan daha buyuk.\nKok yaklasik olarak : %f dir.\nf(%f)nin degeri %f dir.\n",x3,x3,sonuc3);
    }
}
void Newtonraphsin (){
    char fonk[100];
    float sayi_diz[100];
    int stack_capacity = 100 ;
    float x1 = 0, x2=0 ,epsilon, sonuc1, sonuc2,  maxiterasyon , islem =0 ;
    // Stack oluştur
    Stack* stack = createStack(stack_capacity);
 
    Node* head1 = NULL;  // Boş liste.
    Node* head2 = NULL;
    fonksiyonalma(fonk,sayi_diz);
    shunting_yard(sayi_diz, stack, &head1);
    Linkedlistcopy(&head1, &head2);
    printf("x1  degerini giriniz.\n");
    scanf("%f",&x1);
    printf("Epsilon degeri nedir ?\n");
    scanf("%f",&epsilon);
    printf("Kac iterasyon olsun ?\n");
    scanf("%f",&maxiterasyon);
    xdegeriniyerlestirme(head1, &head1, x1);
    fx_hesaplama(head1);
    sonuc1 =  head1->data;
    deleteNode(&head1, head1);
    Linkedlistcopy(&head2, &head1);
    printf("f(%f) in degeri %f dir.\n",x1,sonuc1);
    x2=x1+10;
    while( epsilon < max(x1,x2)-min(x1,x2) && maxiterasyon > islem ){
        printf("%.0f . iterasyon\n",islem+1 );
        if(islem != 0)x1=x2;
        xdegeriniyerlestirme(head1, &head1, x1+0.000001);
        fx_hesaplama(head1);
        sonuc2 =  head1->data;
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
        x2=x1-((sonuc1)/((sonuc2-sonuc1)/0.000001));       //xi+1 =xi - (  (f(xi) / f'(xi)  )    f'(xi) yerine (f(xi+h)-f(xi) ) / h yazdim.
        xdegeriniyerlestirme(head1, &head1, x2);
        fx_hesaplama(head1);
        sonuc2 =  head1->data;
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
        printf("f(%f) in degeri %f dir.\n ",x2,sonuc2);
        printf("Fark => %f\n",max(x1,x2)-min(x1,x2) );
        sonuc1=sonuc2;
        islem++;
    }
    if (islem == maxiterasyon ){
        printf("Max iterasyona ulasildi.\nKok yaklasik olarak : %f dir.\nf(%f)nin degeri %f dir.\n",x2,x2,sonuc2);
    }
    else if( epsilon > (max(x1,x2)-min(x1,x2))) {
        printf("Epsilon degeri farktan daha buyuk.\nKok yaklasik olarak : %f dir.\nf(%f)nin degeri %f dir.\n",x2,x2,sonuc2);
    }
    
    
    
    
    
}
void Matrisintersinibulma(){
    
    int i,j,n;
    printf("Kare matrisin N boyutunu giriniz: ");
    scanf("%d", &n);
    float **matrix, **cofactor, **transpose , **tersmatrix;
    matrix = (float **)malloc(n * sizeof(float *));
    cofactor = (float **)malloc(n * sizeof(float *));
    transpose = (float **)malloc(n * sizeof(float *));
    tersmatrix = (float **)malloc(n * sizeof(float *));
    
    for ( i = 0; i < n; i++) {
        matrix[i] = (float *)malloc(n * sizeof(float));
        cofactor[i] = (float *)malloc(n * sizeof(float));
        transpose[i] = (float *)malloc(n * sizeof(float));
        tersmatrix[i] = (float *)malloc(n * sizeof(float));

    }
    printf("Matrisin elemanlarini giriniz:\n");
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            printf("%d %d elemanı giriniz.",i+1,j+1);
            scanf("%f", &matrix[i][j]);
        }
    }
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    cofactor_matrix(matrix, cofactor, n);
    transposeM(cofactor, transpose, n);
    float det = determinant(matrix, n);
    printf("Kofaktor matrisi:\n");
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            printf("%.2f ", cofactor[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Kofaktor matrisinin transpozesi:\n");
    for ( i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%.2f ", transpose[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    Tersmatris(cofactor, tersmatrix, n, det);
    printf("Matrisin tersi:\n");
    for ( i = 0; i < n; i++) {
        for ( j = 0; j < n; j++) {
            printf("%.2f ", tersmatrix[i][j]);
        }
        printf("\n");
    }
    printf("Matrisin determinanti: %.2f\n", det);
    for ( i = 0; i < n; i++) {
        free(matrix[i]);
        free(cofactor[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(cofactor);
    free(transpose);
}

void gausseliminasyonyontemi(){
    int i, j,n;
    printf("Bilinmeyen sayisini giriniz");
    scanf("%d",&n);
    float matrix[n][n+1]; // Genişletilmiş katsayı matrisi
    // Kullanıcıdan denklem katsayılarını alma
    printf("Denklem katsayilarini sirasiyla girin:\n");
    for (i = 0; i < n; i++) {
        printf("Denklem %d katsayilari: ", i + 1);
        for (j = 0; j < n+1; j++) {
            scanf("%f", &matrix[i][j]);
        }
    }

    // Gauss eliminasyonu uygulama
    gaussElimination(n,matrix);

    // Sonuçları yazdırma
    printf("\nDenklem sisteminin cozumleri:\n");
    for (i = 0; i < n; i++) {
        printf("x%d = %.4f\n", i + 1, matrix[i][n]);
    }
}
void Gauss__seidal(){
    printf("Gauss Seidal yontemine hosgeldiniz!\n");
    int i,j,l,n=3,iterasyon_sayisi,enbuyukdeger=0,enbuyuk=0;
    float A[10][10], x[10],temp , epsilon;
    printf("Bu yontem 3 e 3 bir matris icin uygulanir.\n");
    //    Matrisi alma
    for ( i = 0; i < n; i++) {
      printf("%d. denklemi giriniz (katsayılar ve sabit terim): ", i + 1);
      for ( j = 0; j < n + 1; j++) {
        scanf("%f", &A[i][j]);
      }
    }
    //pivota gore satir islemleri yapma
    for ( j = 0; j < n ; j++){
        for ( i = 0; i < n; i++) {
            if(enbuyukdeger < A[i][j]){
                enbuyuk=i;
                enbuyukdeger=A[i][j];
            }
        }
        if(enbuyuk != j){
            for (l = 0; l < n+1 ; l++){
                temp = A[j][l];
                A[j][l] = A[enbuyuk][l];
                A[enbuyuk][l] = temp;
            }
        }
        (void)(enbuyukdeger=0),enbuyuk=0;
    }
    // Başlangıç değerlerini alma
    printf("\nBaşlangıç değerlerini giriniz:\n");
    for ( i = 0; i < n; i++) {
      printf("x%d: ", i + 1);
      scanf("%f", &x[i]);
    }

    // İterasyon sayısını alma
    printf("\nİterasyon sayısını giriniz: ");
    scanf("%d", &iterasyon_sayisi);
      
      
    // Epsilon degerini alma
      printf("\nEpsilon giriniz: ");
      scanf("%f", &epsilon);

    // Gauss-Seidel yöntemi ile çözme
    gauss_seidel(A, x, n, iterasyon_sayisi, epsilon);

    // Çözümleri yazdırma
    printf("\nÇözümler:\n");
    for ( i = 0; i < n; i++) {
        printf("x%d = %f\n", i + 1, x[i]);
    }
}
void sayisalturev(){
    char fonk[100];
    float sayi_diz[100];
    int stack_capacity = 100 , islem =0;
    float x1 = 0 ,x2=0 , epsilon, sonuc1, sonuc2,  maxiterasyon  ;
    // Stack oluştur
    Stack* stack = createStack(stack_capacity);
 
    Node* head1 = NULL;  // Boş liste.
    Node* head2 = NULL;
    fonksiyonalma(fonk,sayi_diz);
    shunting_yard(sayi_diz, stack, &head1);
    Linkedlistcopy(&head1, &head2);
    printf("Hangi nokta icin turev almak istedinizi ve h degerini giriniz\n");
    scanf("%f %f",&x1,&x2);
    printf("Asagidakilerden birini seciniz\n");
    printf("1.Ileri Farkli Sayisal Turev\n2.Geri Farkli Sayisal Turev\n3.Merkezi Farkli Sayisal Turev\n");
    scanf("%d",&islem);
    
    switch (islem) {
        case 1:
            printf("Ileri Farkli Sayisal Turev\n");
            xdegeriniyerlestirme(head1, &head1, x1+x2);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            xdegeriniyerlestirme(head1, &head1, x1);
            fx_hesaplama(head1);
            sonuc2 =  head1->data;
            printf("f'(%f) in degeri %fdir.\n",x1,((sonuc1-sonuc2)/x2));
            break;
        case 2:
            printf("Geri Farkli Sayisal Turev\n");
            xdegeriniyerlestirme(head1, &head1, x1+x2);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            xdegeriniyerlestirme(head1, &head1, x1);
            fx_hesaplama(head1);
            sonuc2 =  head1->data;
            printf("f'(%f) in degeri %fdir.\n",x1,((sonuc2-sonuc1)/x2));
            break;
        case 3:
            printf("Merkezi Farkli Sayisal Turev\n");
            xdegeriniyerlestirme(head1, &head1, x1+x2);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            xdegeriniyerlestirme(head1, &head1, x1-x2);
            fx_hesaplama(head1);
            sonuc2 =  head1->data;
            printf("f'(%f) in degeri %fdir.\n",x1,((sonuc1-sonuc2)/2*x2));
            break;
        default:
            printf("Gecersiz sayi");
            break;
    }
}
void Simpsonmetodu(){
    char fonk[100];
    float sayi_diz[100];
    int stack_capacity = 100 , islem =0,  i;
    float x1 = 0 ,x2=0 ,n=0, alan = 0.0 ,h=0 ,sonuc1 ;
    // Stack oluştur
    Stack* stack = createStack(stack_capacity);
 
    Node* head1 = NULL;  // Boş liste.
    Node* head2 = NULL;
    fonksiyonalma(fonk,sayi_diz);
    shunting_yard(sayi_diz, stack, &head1);
    Linkedlistcopy(&head1, &head2);
    printf("Asagidakilerden birini seciniz\n");
    printf("1.1/3Simpson Metodu\n2.3/8Simpson Metodu\n");
    scanf("%d",&islem);
    switch (islem) {
        case 1:
            printf("n ikinin kati olmasi lazim\n");
            printf("Hangi noktalar icin Simpson metodunu yapmak istedinizi ve n degerini giriniz\n");
            scanf("%f %f %f",&x1,&x2,&n);
            xdegeriniyerlestirme(head1, &head1, x1);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            alan += sonuc1;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            xdegeriniyerlestirme(head1, &head1, x2);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            alan += sonuc1;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            
            h=(max(x1, x2)-min(x1, x2))/n;
            for(i=1 ; i < n ; i += 2){
                xdegeriniyerlestirme(head1, &head1, x1+i*h);
                fx_hesaplama(head1);
                sonuc1 =  head1->data;
                alan += (4*sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
            }
            for(i=2 ; i < n-1 ; i += 2){
                xdegeriniyerlestirme(head1, &head1, (x1+(i*h)));
                fx_hesaplama(head1);
                sonuc1 =  head1->data;
                alan += (2*sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
            }
            alan=h*alan/3;
            printf("Verdiginiz fonksiyonun  alani Simpson 1/3 e gore %f dir.\n ",alan);
            break;
        case 2:
            printf("n üçün kati olmasi lazim\n");
            printf("Hangi noktalar icin Simpson metodunu yapmak istedinizi ve n degerini giriniz\n");
            scanf("%f %f %f",&x1,&x2,&n);
            xdegeriniyerlestirme(head1, &head1, x1);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            alan += sonuc1;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            xdegeriniyerlestirme(head1, &head1, x2);
            fx_hesaplama(head1);
            sonuc1 =  head1->data;
            alan += sonuc1;
            deleteNode(&head1, head1);
            Linkedlistcopy(&head2, &head1);
            
            h=(max(x1, x2)-min(x1, x2))/n;
            for(i=1 ; i < n ; i += 3){
                xdegeriniyerlestirme(head1, &head1, (x1+(i*h)));
                fx_hesaplama(head1);
                sonuc1 =  head1->data;
                alan += (3*sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
            }
            for(i=2 ; i < n-1 ; i += 3){
                xdegeriniyerlestirme(head1, &head1, (x1+(i*h)));
                fx_hesaplama(head1);
                sonuc1 =  head1->data;
                alan += (3*sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
            }
            for(i=3 ; i < n-2 ; i += 3){
                xdegeriniyerlestirme(head1, &head1, (x1+(i*h)));
                fx_hesaplama(head1);
                sonuc1 =  head1->data;
                alan += (2*sonuc1);
                deleteNode(&head1, head1);
                Linkedlistcopy(&head2, &head1);
            }
            alan=(3*h/8)*alan;
            printf("Verdiginiz fonksiyonun  alani Simpson 3/8 e gore %f dir.\n",alan);
            break;
        default:
            printf("Gecersiz sayi");
            break;
    }
}
void TrapezYontemi(){
    char fonk[100];
    float sayi_diz[100];
    int stack_capacity = 100 , islem =0,  i;
    float x1 = 0 ,x2=0 ,n=0, alan = 0.0 ,h=0 ,sonuc1 ;
    // Stack oluştur
    Stack* stack = createStack(stack_capacity);
    
    Node* head1 = NULL;  // Boş liste.
    Node* head2 = NULL;
    fonksiyonalma(fonk,sayi_diz);
    shunting_yard(sayi_diz, stack, &head1);
    Linkedlistcopy(&head1, &head2);
    printf("Hangi noktalar icin Trapez metodunu yapmak istedinizi ve n degerini giriniz\n");
    scanf("%f %f %f",&x1,&x2,&n);
    h=(max(x1, x2)-min(x1, x2))/n;
    xdegeriniyerlestirme(head1, &head1, x1);
    fx_hesaplama(head1);
    sonuc1 =  head1->data;
    alan += sonuc1/2;
    deleteNode(&head1, head1);
    Linkedlistcopy(&head2, &head1);
    xdegeriniyerlestirme(head1, &head1, x2);
    fx_hesaplama(head1);
    sonuc1 =  head1->data;
    alan += sonuc1/2;
    deleteNode(&head1, head1);
    Linkedlistcopy(&head2, &head1);
    for(i=1 ; i < n ; i++){
        xdegeriniyerlestirme(head1, &head1, (x1+(i*h)));
        fx_hesaplama(head1);
        sonuc1 =  head1->data;
        alan += (sonuc1);
        deleteNode(&head1, head1);
        Linkedlistcopy(&head2, &head1);
    }
    alan = h*alan;
    printf("Verdiginiz fonksiyonun  alani Trapez Yontemine gore %f dir.\n",alan);
    }

void GregoryNewtonEnterpolasyonu(){
    printf("Gregory Newton Enterpolasyonuna Hosgeldiniz!\n");
    int xsayisi=0,i,secim=1 , bayrak = 0 ,deltaysayac=0 ,j=1 ;
    float x[10] ={0}, y[10]={0} ,yfark[10]={0} , xdeger , sonuc=0 , arasonuc=1;
    float deltay1[10],deltay2[10];
    printf("Kac tane nokta girmek istiyorsunuz");
    scanf("%d",&secim);
    for(i = 0 ; i < secim ; i++){
        printf("x%d koordinatini ve y%d degerieni giriniz",xsayisi+1,xsayisi+1);
        scanf("%f %f",&x[xsayisi],&y[xsayisi]);
        xsayisi++;
    }
    for( i=0 ; i<xsayisi-1 ; i++ ){
                deltay1[i]=y[i+1]-y[i];
            }
    for( i=0 ; i<xsayisi-1 ; i++ ){
                if( deltay1[i] == deltay1[i+1] ) deltaysayac++;
            }
            if(deltaysayac == xsayisi-2) {
                bayrak =1;
            }
            else if (deltaysayac != xsayisi-1) deltaysayac =0 ;
    printf("Hesaplanmasini istediginiz x degerini giriniz.\n");
    scanf("%f",&xdeger);
      
    yfark[0]= deltay1[0];
    sonuc += y[0];
    sonuc +=( xdeger * yfark[0] ) / factoriyel(1);
    while (bayrak == 0){
        
        for( i=0 ; i<xsayisi-j-1 ; i++ ){
            deltay2[i]=deltay1[i+1]-deltay1[i];
        }
        for( i=0 ; i<xsayisi-j-1 ; i++ ){
            if( deltay2[i] == deltay2[i+1] ) deltaysayac++;
        }
        if(deltaysayac == xsayisi-j-1) {
            bayrak =1;
        }
        else if (deltaysayac != xsayisi-j-1)deltaysayac = 0;
        
        for( i=0 ; i<xsayisi-j-1 ; i++ ){
            deltay1[i] = deltay2[i];
        }
        yfark[j] = deltay1[0];
        j++;
        arasonuc =1 ;
        for( i = 0 ; i < j ; i++){
            arasonuc = arasonuc * (xdeger -i );
        }
        arasonuc = arasonuc * yfark[j-1] / factoriyel(j);
        sonuc += arasonuc;
    }
  
    
    printf("istediginiz %f degeeri icin sonuc : %.2f dir.\n",xdeger,sonuc);
}

int main() {
    int secim;
    printf("Asagidaki metodlardan birini seciniz.\n");
    printf("1. Bisection yöntemi\n2. Regula-Falsi yöntemi\n3. Newton-Rapshon yöntemi\n4. NxN’lik bir matrisin tersi\n5. Gauus Eleminasyon Yontemi\n6. Gauss Seidal yöntemi\n7. Sayısal Türev (merkezi, ileri ve geri farklar opsiyonlu)\n8. Simpson yöntemi\n9. Trapez yöntemi\n10. Değişken dönüşümsüz Gregory newton Enterpolasyonu\n");
    scanf("%d",&secim);
    switch (secim) {
        case 1:
            bisection();
            break;
        case 2:
            Regulafalsi();
            break;
        case 3:
            Newtonraphsin();
            break;
        case 4:
            Matrisintersinibulma();
            break;
        case 5:
            gausseliminasyonyontemi();
            break;
        case 6:
            Gauss__seidal();
            break;
        case 7:
           sayisalturev();
            break;
        case 8:
            Simpsonmetodu();
            break;
        case 9:
            TrapezYontemi();
            break;
        case 10:
            GregoryNewtonEnterpolasyonu();
            break;
        default:
            printf("Gecersiz sayi\n");
            break;
    }
    return 0;
}

