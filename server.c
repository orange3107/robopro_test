#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define COLSROWS 6
#define DET_SIZE 5

void clear_matrix(int n, double (*matrix)[n], double (*submatrix)[n-1], int skipRow, int skipCol) {
    int r = 0, c = 0;

    for (int i = 0; i < n; i++) {
        if (i == skipRow)
            continue;

        c = 0;
        for (int j = 0; j < n; j++) {
            if (j == skipCol)
                continue;

            submatrix[r][c] = matrix[i][j];
            c++;
        }
        r++;
    }
}

double Determinant(int n, double (*matrix)[n]) {
    if (n == 1)
        return matrix[0][0];

    if (n == 2)
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    double submatrix[n][n];

    double d = 0;
    int k = 1;

    for (int i = 0; i < n; i++) {
        clear_matrix(n, matrix, submatrix, 0, i);
        d += k * matrix[0][i] * Determinant(n - 1, submatrix);
        k = -k;
    }

    return d;
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

    double summ_det = 0;

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

        last_det = Determinant(COLSROWS, matrix);
        
        //last_det = count_det + 1;

        matrix_det[count_det % DET_SIZE] = last_det;
        count_det++;
        
        summ_det += last_det;

        printf("Det.= %f\n", last_det);

        if(count_det < DET_SIZE){

            printf("Avg. det = N/A\n");
            printf("Del. det = N/A\n");

        }
        else{

            summ_det -= del_det;
            avg_det = summ_det / DET_SIZE;
            del_det = matrix_det[count_det % DET_SIZE];

            printf("Avg. det = %f\n", avg_det);
            printf("Del. det = %f\n", del_det);

        }

        printf("\n");
        
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
