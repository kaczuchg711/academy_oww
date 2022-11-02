#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void create_matrix_from_csr(double **pDouble, double *pDouble1, int *pInt, int *pInt1, int n, int m, int k);

void check_saved_matrix_with_crc_matrix(double **pDouble, double **pDouble1, int n, int m);

int rand_integer(int lower, int upper) {
    int num;
    int i;
    num = rand() % (upper - lower) + lower;

    return num;
}

double rand_double(double lower, double upper);

void print_matrix(double **matrix, int n, int m);

void save_matrix_to_file(double **matrix, int n, int m, string file_name);

void load_matrix_from_file(double **matrix, int n, int m, string file_name);

int rand_integer(int lower, int upper);

void print_matrix(double **matrix, int n, int m) {
    fstream myfile;


    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << setw(8) << matrix[i][j] << ", ";
        }
        cout << endl;
    }
    myfile.close();
}

double rand_double(double lower, double upper) {
    double div;
    double num;

    div = RAND_MAX / (upper - lower);
    num = (rand() / div) + lower;

    return num;
}

void create_sparse_matrix(double **matrix, int n, int m, int k);

void create_crs_format(double *values, int *row_index, int *col_index, double **matrix, int n, int m);

void csr_matrix_multiplication_by_vector(double *values, int *row_index, int *col_index, double *vector, int n, int m, int k);

int main() {
    bool check_result = true;

    int n = 4;
    int m = 5;
    int k = 2;
    if (k > m) {
        printf("Error: k should be less that n");
        exit(2);
    }

    srand(time(NULL));
    double **saved_matrix_before_crs;
    double **matrix;


    matrix = (double **) calloc(n, sizeof(double *));

    if (check_result) saved_matrix_before_crs = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double *) calloc(m, sizeof(double));
        if (check_result) saved_matrix_before_crs[i] = (double *) calloc(m, sizeof(double));
    }
//  k - number of non 0 elements in row
    load_matrix_from_file(matrix, n, m, "matrix1.txt");
//    save_matrix_to_file(matrix,n,m,"matrix0.txt");
//    create_sparse_matrix(matrix, n, m, k);
//    save_matrix_to_file(matrix,n,m,"matrix1.txt");
    if (check_result) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                saved_matrix_before_crs[i][j] = matrix[i][j];
            }
        }
    }

    double *values = (double *) malloc(k * n * sizeof(double));
    int *col_index = (int *) malloc(k * n * sizeof(int));
    int *row_index = (int *) malloc(k * n * sizeof(int));


    create_crs_format(values, row_index, col_index, matrix, n, m);


    cout << endl;

    create_matrix_from_csr(matrix, values, row_index, col_index, n, m, k);
    save_matrix_to_file(matrix, n, m, "matrix2.txt");

    double vector[] = {1, 2, 3, 4, 5};

    if ((sizeof vector )/8 != m) {
        cout <<"wrong size of vector";
        exit(3);
    }
    csr_matrix_multiplication_by_vector(values, row_index, col_index, vector, n, m, k);


    if (check_result) check_saved_matrix_with_crc_matrix(matrix, saved_matrix_before_crs, n, m);
    if (check_result) free(saved_matrix_before_crs);
    free(row_index);
    free(col_index);
    free(values);
    free(matrix);

    return 0;
}

void csr_matrix_multiplication_by_vector(double *values, int *row_index, int *col_index, double *vector, int n, int m, int k) {
    k = 2;
    for (int i = 0; i < k*n; ++i) {
        cout << values[i] << ", ";
    }
    cout << endl;
    for (int i = 0; i < k * n; ++i) {
        cout << col_index[i] << ", ";
    }
    cout << endl;

    for (int i = 0; i < k * n; ++i) {
        cout << row_index[i] << ", ";
    }
    cout << endl;
    for (int i = 0; i < m; ++i) {
        cout << vector[i] << ", ";
    }
    cout<<endl;

//    mnożymy każdy element wiersza macierzy z odpowiadającym mu elmentem w wektorze potem sumujemy
//    4.87503 * 2 + 7.96319 * 4 = 41.60282;
//       0            1
//              1             3
//              0             1
    double sum;
    double res[10];
    int column_index = col_index[0];
    for (int j = 0; j < n; ++j) {
        sum = 0;
        for (int i = 0; i < k; ++i) {
            sum += values[i + j*k] * vector[col_index[i + j*k]];
        }

        res[j] = sum;
        printf("%lf", sum);
    }
}

void load_matrix_from_file(double **matrix, int n, int m, string file_name) {
    fstream myfile;
    string number;
    myfile.open(file_name, fstream::in);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            myfile >> number;
            matrix[i][j] = stod(number);
        }
    }
}

bool _check_if_rand_number_is_already_in_vector(int *vector, int checking_number, int last_checking_position) {
    for (int i = 0; i <= last_checking_position; ++i) {
        if (checking_number == vector[i])
            return true;
    }
    return false;
}

void create_sparse_matrix(double **matrix, int n, int m, int k) {

    int *non_zero_coordinates_in_row = (int *) malloc(k * sizeof(int));


    int rand_number;
    for (int j = 0; j < n; ++j) {

        for (int i = 0; i < k; ++i) {
            do {
                rand_number = rand_integer(0, m);
            } while (_check_if_rand_number_is_already_in_vector(non_zero_coordinates_in_row, rand_number, i));
            non_zero_coordinates_in_row[i] = rand_number;
        }
        for (int i = 0; i < k; ++i) {

            matrix[j][non_zero_coordinates_in_row[i]] = rand_double(0, 10);
        }
    }
    free(non_zero_coordinates_in_row);
}

void save_matrix_to_file(double **matrix, int n, int m, string file_name) {
    fstream myfile;

    myfile.open(file_name, fstream::out);


    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            myfile << setw(8) << matrix[i][j] << ", ";
        }
        myfile << "\n";
    }
    myfile.close();
}

void create_crs_format(double *values, int *row_index, int *col_index, double **matrix, int n, int m) {
    int non_zero_iterator = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] != 0) {
                double a = matrix[i][j];
                values[non_zero_iterator] = matrix[i][j];
                row_index[non_zero_iterator] = i;
                col_index[non_zero_iterator] = j;
                non_zero_iterator++;
            }
        }
    }
}

void check_saved_matrix_with_crc_matrix(double **matrix1, double **matrix2, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix1[i][j] != matrix2[i][j]) {
                printf("POLICJAAAAAAAAAAA");
                exit(997);
            }
        }
    }
    printf("Test PASS");
}

void create_matrix_from_csr(double **matrix, double *value, int *row_index, int *col_index, int n, int m, int k) {
    for (int i = 0; i < n * k; ++i) {
        int a = row_index[i];
        int b = col_index[i];
        matrix[row_index[i]][col_index[i]] = value[i];
    }
}