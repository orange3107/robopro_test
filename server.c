#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define COLSROWS 6

double Determinant(double** a, int n) {
    if (n == 1)
        return a[0][0];

    if (n == 2)
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];

    // Выделяем память под временную матрицу
    double** b = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        b[i] = (double*)malloc((n - 1) * sizeof(double));
    }

    double d = 0;
    int k = 1;  // Знак чередования начинается с +1

    for (int i = 0; i < n; i++) {
        clear(a, b, n, 0, i);  // Формирование подматрицы
        d += k * a[0][i] * Determinant(b, n - 1);  // Рекурсивный вызов
        k = -k;  // Чередование знаков
    }

    // Освобождаем память
    for (int i = 0; i < n; i++) {
        free(b[i]);
    }
    free(b);

    return d;
}

// Функция для формирования подматрицы
void clear(double** a, double** b, int n, int skipRow, int skipCol) {
    int r = 0, c = 0;

    for (int i = 0; i < n; i++) {
        if (i == skipRow)
            continue;  // Пропускаем строку

        c = 0;
        for (int j = 0; j < n; j++) {
            if (j == skipCol)
                continue;  // Пропускаем столбец

            b[r][c] = a[i][j];  // Копируем элемент
            c++;
        }
        r++;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    double matrix[COLSROWS][COLSROWS] = {0};

    double matrix_det[5];
    
    double avg_det = 0;
    double del_det = 0;
    double last_det = 0;
    int count_det = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Чтение данных из клиента
        ssize_t bytes_read = read(new_socket, matrix, sizeof(matrix));

        if (bytes_read < 0) {
            perror("read failed");
            break;
        } 
        
        else if (bytes_read == 0) {
            printf("Client disconnected\n");
            break;
        }

        double** matrix_ptr = (double**)malloc(COLSROWS * sizeof(double*));
        for (int i = 0; i < COLSROWS; i++) {
            matrix_ptr[i] = (double*)malloc(COLSROWS * sizeof(double));
            
            for (int j = 0; j < COLSROWS; j++) {
                matrix_ptr[i][j] = matrix[i][j]; // КОСТЫЛЬ)
            }
        }

        last_det = Determinant(matrix_ptr, COLSROWS);
        
        printf("Det. = %f\n", last_det);
        

        if(count_det < 4){
            matrix_det[count_det] = last_det;

            printf("Avg. det = N/A\n");
            printf("Del. det = = N/A\n");

        }
        else{
            double summ_det = 0;
            matrix_det[4] = last_det;

            for(int i = 0; i < 4; i++){
                summ_det += matrix_det[i];
                matrix_det[i] = matrix_det[i+1];
            }

            summ_det += last_det;

            avg_det = summ_det/5;
            del_det = matrix_det[0];

            printf("Avg. det = %f\n", avg_det);
            printf("Del. det = %f\n", del_det);

        }

        printf("\n");
        count_det++;
        
        

    }

    close(new_socket);
    close(server_fd);

    return 0;
}
