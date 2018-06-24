#include <iostream>
#include <complex>
#include <chrono>
#include <omp.h>

// #define TEST_MODE 

using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {
    typedef high_resolution_clock Clock;
    int max_row, max_column, max_n;
    cin >> max_row;
    cin >> max_column;
    cin >> max_n;

    char **mat = (char**) malloc(sizeof(char*) * max_row);

    for (int i = 0; i < max_row; i++)
        mat[i] = (char*) malloc(sizeof(char) * max_column);

    auto t_start = Clock::now();
    #pragma omp parallel for schedule(static, 1)
    for (int r = 0; r < max_row; r++) {
        for (int c = 0; c < max_column; c++) {
            complex<float> z;
            int n = 0;
            while ( (abs(z) < 2) && (++n < max_n) )
                z = pow(z, 2) + decltype(z) (
                                  (float) c * 2 / max_column - 1.5,
                                  (float) r * 2 / max_row - 1
                                );
            mat[r][c] = (n == max_n) ? '#' : '.';
        }
    }
    auto t_end = Clock::now();
    
    #ifndef TEST_MODE
        for (int r = 0; r < max_row; r++) {
            for (int c = 0; c < max_column; c++)
                cout << mat[r][c];
            cout << '\n';
        }	
        cout << duration_cast<nanoseconds>(t_end - t_start).count() << " nanoseconds" << endl;
    #else
        cout << duration_cast<nanoseconds>(t_end - t_start).count() << endl;
    #endif
    
    return 0;
}
