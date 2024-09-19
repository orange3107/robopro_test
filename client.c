#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define COLSROWS 6

double fill_rand_matrix(double (*matrix)[COLSROWS]){
    double min = -10.0;
    double max = 10.0;

    for(int i = 0; i < COLSROWS; i++){
        for(int j = 0; j < COLSROWS; j++){

            double random_range_double = min + (double)rand() / RAND_MAX * (max - min);
            matrix[i][j] = random_range_double;
        }
    } 

}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *message = "Hello from client";
    double timer_per = 5;

    // Создаем сокет
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Задаем адрес сервера
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразуем IP-адрес из текста в двоичную форму
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Подключаемся к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Отправляем сообщение серверу
    while(1){

        double matrix[COLSROWS][COLSROWS];
        fill_rand_matrix(matrix); // заполняем матрицу
        send(sock, matrix, sizeof(matrix), 0); // отправляем матрицу

        printf("Отправленная матрица:\n");
        for(int i = 0; i < COLSROWS; i++){
            for(int j = 0; j < COLSROWS; j++){
                printf("%f ", matrix[i][j]);
            }
            printf("\n");
        } 

        sleep(1);
    }

    close(sock);

    return 0;
}
