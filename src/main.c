#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include "semphr.h"


//Declaração das funções que serão as threads.
void create_car(void *pvParameters);
void create_train(void *pvParameters);

void remove_car(void *param);
void remove_train(void *param);

//Declaração dos semáforos
xSemaphoreHandle xSemaphore = NULL;
xSemaphoreHandle semaphore_chegando_trem = NULL;

//Declaração de variáveis usadas no programa
int delay_carro = 300;
int delay_train = 1000;

int delay_remove_car = 600;
int delay_remove_train = 200;


//Definindo as estruturas de Carro e Trem
typedef struct CarNode {
    int id;
    int entrada;
    struct CarNode* next;
} CarNode;

typedef struct TrainNode {
    int id;
    int entrada;
    struct TrainNode* next;
} TrainNode;

//Criando as listas que receberão carros e trens
CarNode* car_list = NULL;
CarNode* car_list2 = NULL;
TrainNode* train_list = NULL;
TrainNode* train_list2 = NULL;

//Criando funções para adicionar carros e trens em filas diferentes
void add_car(int id, int entrada);
void add_car2(int id, int entrada);
void add_train(int id, int entrada);
void add_train2(int id, int entrada);

//Criando as funções que printam as filas
void print_car_queue();
void print_car_queue2();
void print_train_queue();
void print_train_queue2();


int main(void)
{
    //Criando os semáforos
    xSemaphore = xSemaphoreCreateMutex();
    semaphore_chegando_trem = xSemaphoreCreateMutex();

    //Checando se o semáforo foi criado
    if (xSemaphore !=  NULL && semaphore_chegando_trem != NULL){
        printf("\n Semaforos Criados \n");
    }
    
    //Criando as tasks
    xTaskCreate(&create_car, "Carro", 1024, NULL, 1, NULL);
    xTaskCreate(&create_train, "Trem", 1024, NULL, 1, NULL);
    xTaskCreate(&remove_car, "Remove Carro", 1024, NULL, 1, NULL);
    xTaskCreate(&remove_train, "Remove Trem", 1024, NULL, 2, NULL);
    

    vTaskStartScheduler();

    return 0;
}

//Função para criar carro 
void create_car(void *pvParameters){
    int id_car = 0;
    int entrada;

    for (;;) {
        
        //A entrada pode ser estar em 0 ou 1 (já que so existem duas)
        entrada = (rand() + rand()) % 2;
        printf("\nCARRO CHEGANDO PELA ENTRADA %d\n", entrada);

        //Aloca o carro em algumas das filas baseado na entrada
        if(entrada == 0){
            add_car(id_car, entrada);
        }
        else{
            add_car2(id_car, entrada);
        }

        id_car++;

        //Tempo de espera p/ chegada de outro carro 
        vTaskDelay(delay_carro);
    }
    vTaskDelete(NULL);
}


//Função p/ criar o trem 
void create_train(void *pvParameters){   
    int id_train = 0;
    int entrada;

    for (;;) {
        // Tenta pegar o semáforo que indica a chegada de trem, posteriormente, esse semáforo
        //será usado para impedir a passagem de carros quando um trem estiver chegando.
        if(xSemaphoreTake(semaphore_chegando_trem, portMAX_DELAY) == pdTRUE){
        entrada = rand() % 2;
        printf("\nTREM CHEGANDO PELA ENTRADA %d \n", entrada);
        //Aloca um trem em alguma das duas filas baseado na entrada.
        if(entrada == 0){
            add_train(id_train, entrada);
        }
        else{
            add_train2(id_train, entrada);
        }
        id_train++;
        }
        //Tempo de espera p/ chegada do prox trem.
        vTaskDelay(delay_train);
    }
    vTaskDelete(NULL);
}

//Func p add carro na fila 0
void add_car(int id, int entrada)
{
    CarNode* new_car = malloc(sizeof(CarNode));
    if (new_car == NULL) {
        return;  
    }
    new_car->id = id;
    new_car->entrada = entrada;
    new_car->next = NULL;

    if (car_list == NULL) {
        car_list = new_car;
    } else {
        CarNode* current = car_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_car;
    }
}

//Func para adicionar carro na fila 1
void add_car2(int id, int entrada)
{
    CarNode* new_car = malloc(sizeof(CarNode));
    if (new_car == NULL) {
        return;  
    }
    new_car->id = id;
    new_car->entrada = entrada;
    new_car->next = NULL;

    if (car_list2 == NULL) {
        car_list2 = new_car;
    } else {
        CarNode* current = car_list2;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_car;
    }
}

//Func p add trem na fila 0
void add_train(int id, int entrada)
{
    TrainNode* new_train = malloc(sizeof(TrainNode));
    
    if (new_train == NULL) {
        return;  
    }
    new_train->id = id;
    new_train->entrada = entrada;
    new_train->next = NULL;

    if (train_list == NULL) {
        train_list = new_train;
    } else {
        TrainNode* current = train_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_train;
    }
}

//Func p add trem na fila 2
void add_train2(int id, int entrada)
{
    TrainNode* new_train = malloc(sizeof(TrainNode));
    if (new_train == NULL) {
        return; 
    }
    new_train->id = id;
    new_train->entrada = entrada;
    new_train->next = NULL;

    if (train_list2 == NULL) {
        train_list2 = new_train;
    } else {
        TrainNode* current = train_list2;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_train;
    }
}

//Função p printar fila 0
void print_car_queue()
{
    CarNode* current = car_list;

    printf("Fila de carros vindo da entrada 0:\n");
    while (current != NULL) {
        printf("Carro %d -  Entrada %d\n", current->id, current->entrada);
        current = current->next;
    }
    printf("\n");
}

//Função p printar fila 1
void print_car_queue2()
{
    CarNode* current = car_list2;
    printf("\nFila de carros vindo da entrada 1:\n");
    while (current != NULL) {
        printf("Carro %d - Entrada %d\n", current->id, current->entrada);
        current = current->next;
    }
    printf("\n");
}

//Função p printar fila 0 
void print_train_queue()
{
    TrainNode* current = train_list;
    printf("Fila de trens vindo da entrada 0:\n");
    while (current != NULL) {
        printf("Trem %d - Entrada %d\n", current->id, current->entrada);
        current = current->next;
    }
    printf("\n");
}

//Função p printar fila 1
void print_train_queue2()
{
    TrainNode* current = train_list2;
    printf("Fila de trens vindo da entrada 1:\n");
    while (current != NULL) {
        printf("Trem %d - Entrada %d\n", current->id, current->entrada);
        current = current->next;
    }
    printf("\n");
}


//Função p remover carro das filas
void remove_car(void *param)
{
    for(;;){

        CarNode* current = car_list;
        CarNode* current2 = car_list2;

        if(car_list == NULL && car_list2 == NULL){
            vTaskDelay(delay_remove_car);
            continue;
        }
        printf("\n ----------------------------- \n");

        print_car_queue();
        print_car_queue2();
        printf("\n ----------------------------- \n");

        //P essa função conseguir acesso para fzer com que os carros cruzem, é necessário que ela 
        //tenha acesso a dois semáforos, o "semaphore_chegando_trem" indica que um trem está chegando,
        // logo o carro não pode passar, já o "xSemaphore" indica que um trem está cruzando, então os carros
        //ainda não podem passar, somente quando ele tiver pego os dois semáforos é que os carros cruzarão.
        if(xSemaphoreTake(semaphore_chegando_trem, portMAX_DELAY) == pdTRUE){
        if(xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE){

        printf("\nCarro cruzando\n");
        Sleep(10);

        printf("\n ----------------------------- \n");
    
        if (car_list != NULL) {
            CarNode* removed_node = car_list;  

            car_list = car_list->next;  

            printf("\n Carro %d saiu da fila 0.\n", removed_node->id);
            free(removed_node);  

        } 
        else {
            printf("\n FILA 0 CARRO - VAZIA \n");
        }
        if (car_list2 != NULL) {
            CarNode* removed_node = car_list2;  

            car_list2 = car_list2->next;  

            printf("\n Carro %d saiu da fila 1.\n", removed_node->id);
            free(removed_node);  


        } 
        else {
            printf("\n FILA 1 CARRO - VAZIA.\n");
        }
        printf("\n ----------------------------- \n");

        }
        }
        xSemaphoreGive(semaphore_chegando_trem);
        xSemaphoreGive(xSemaphore);
        vTaskDelay(delay_remove_car);
    }
}
        

//Função p remover os trens das filas
void remove_train(void *param)
{
    for(;;){
        TrainNode* current = train_list;
        TrainNode* current2 = train_list2;
        if(train_list == NULL && train_list2 == NULL){
            vTaskDelay(delay_remove_train);
            continue;
        }
        printf("\n ----------------------------- \n");

        print_train_queue();
        print_train_queue2();
        printf("\n ----------------------------- \n");

        //P fazer com que o trem possa cruzar, ele precisa ter acesso ao semáforo, quando isso acontece
        //o trem passa
        if(xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE){
        
        printf("\nTrem cruzando\n");
        Sleep(10);

        
        printf("\n ----------------------------- \n");

        
        printf("\n TREM:  \n");

        if (train_list != NULL) {
            TrainNode* removed_node = train_list;  

            train_list = train_list->next;  

            printf("\n Trem %d saiu da fila 0.\n", removed_node->id);
            free(removed_node);  
        } 
        else {
            printf("\n FILA TREM 0 - VAZIA.\n");
        }

        
        if (train_list2 != NULL) {
            TrainNode* removed_node = train_list2;  

            train_list2 = train_list2->next;  

            printf("\n Trem %d saiu da fila 1.\n", removed_node->id);

            free(removed_node);  
        } 
        else {
            printf("\n FILA TREM 1 - VAZIA\n");

        }
        printf("\n ----------------------------- \n");

        }

        xSemaphoreGive(semaphore_chegando_trem);
        xSemaphoreGive(xSemaphore);
        vTaskDelay(delay_remove_train);
    }     
}

    
