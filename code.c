/*
 * Program Analisis Rangkaian Dioda
 * --------------------------------
 * Nama: Adhi Rajasa Rafif
 * NPM: 2306266943
 * 
 * Program ini mengimplementasikan analisis rangkaian dioda menggunakan:
 * 1. Metode Taylor Series (Order 2)
 * 2. Metode Picard (3 iterasi)
 * 
 * Input: File data.csv berisi parameter rangkaian
 * Output: Hasil perhitungan untuk setiap kasus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Konstanta program */
#define MAX_LINE 1024   // Panjang maksimum baris CSV
#define H 0.1          // Step size default
#define T0 0.0         // Waktu awal
#define Y0 0.0         // Nilai awal

/* 
 * Fungsi persamaan dioda
 * ----------------------
 * Is: Arus saturasi
 * n: Faktor idealitas
 * Vt: Tegangan thermal
 * I: Arus input
 * 
 * Returns: Nilai fungsi dioda f(I) = (n*Vt/Is)*ln(I/Is + 1)
 */
double f(double Is, double n, double Vt, double I) {
    return (n * Vt / Is) * log(I / Is + 1.0);
}

/* 
 * Turunan parsial terhadap t
 * -------------------------
 * Dalam kasus ini bernilai 0 karena f tidak bergantung pada t
 */
double df_dt(double Is, double n, double Vt, double I) {
    return 0.0;
}

/* 
 * Turunan parsial terhadap y
 * -------------------------
 * Menghitung df/dy untuk digunakan dalam Taylor Series
 */
double df_dy(double Is, double n, double Vt, double I) {
    return (n * Vt) / (I + Is);
}

/* 
 * Implementasi Taylor Series Order 2
 * --------------------------------
 * Menggunakan formula: y(t+h) = y(t) + h*f + (h²/2)*f*df/dy
 * 
 * Returns: Nilai pendekatan pada t+h
 */
double taylor_method(double Is, double n, double Vt, double I) {
    double y = Y0;
    double f0 = f(Is, n, Vt, I);
    double df_dy0 = df_dy(Is, n, Vt, I);
    
    // Gunakan step size kecil untuk stabilitas numerik
    double h = H / 1000.0;
    
    // Hitung suku-suku deret Taylor
    double first_order = h * f0;
    double second_order = (h * h / 2.0) * f0 * df_dy0;
    
    // Scaling untuk menghindari overflow
    double scale = 1e-9;
    first_order *= scale;
    second_order *= scale;
    
    // Hitung hasil dan terapkan batas fisik
    double result = y + first_order + second_order;
    if (result < Is) result = Is;      // Tidak boleh kurang dari Is
    if (result > 1.0) result = 1.0;    // Batas arus maksimum 1A
    
    return result;
}

/* 
 * Implementasi Metode Picard
 * -------------------------
 * Melakukan 3 iterasi dengan formula:
 * y_(n+1)(t) = y₀ + ∫[t₀ to t] f(s,y_n(s))ds
 */
void picard_method(double Is, double n, double Vt, double I) {
    // Mulai dengan nilai awal = I
    double yk = I;
    printf("  Picard Iterations:\n");
    
    // Gunakan step size kecil untuk stabilitas
    double h = H / 1000.0;
    
    // Lakukan 3 iterasi
    for (int i = 1; i <= 3; ++i) {
        double fk = f(Is, n, Vt, yk);
        double next_yk = Y0 + h * fk;
        
        // Scaling dan pembatasan nilai
        double scale = 1e-9;
        next_yk *= scale;
        if (next_yk < Is) next_yk = Is;
        if (next_yk > 1.0) next_yk = 1.0;
        
        printf("    y_%d(t) = %.10f\n", i, next_yk);
        yk = next_yk;
    }
}

/* 
 * Fungsi perhitungan arus untuk rangkaian dioda-resistor
 * ---------------------------------------------------
 * Menghitung arus rangkaian menggunakan:
 * 1. Persamaan dioda untuk Vd
 * 2. Hukum Ohm untuk arus
 * 
 * Returns: Nilai arus rangkaian
 */
double f_resistor(double Is, double n, double Vt, double R, double Vs, double I) {
    double Vd = (n * Vt) * log(I / Is + 1.0);    // Tegangan dioda
    double current = (Vs - Vd) / R;               // Arus = (Vs-Vd)/R
    return current;
}

/* 
 * Program Utama 
 * ------------
 * 1. Baca data dari file CSV
 * 2. Proses setiap kasus (dioda/dioda-resistor)
 * 3. Tampilkan hasil perhitungan
 */
int main() {
    // Buka dan periksa file data.csv
    FILE *file = fopen("data.csv", "r");
    if (!file) {
        printf("Gagal membuka data.csv\n");
        return 1;
    }

    // Siapkan buffer dan skip header
    char line[MAX_LINE];    
    fgets(line, MAX_LINE, file);

    printf("=== Hasil Perhitungan dari data.csv ===\n");    // Proses setiap baris data
    while (fgets(line, MAX_LINE, file)) {
        char *token;
        char *rest = line;
        
        // Baca tipe masalah (diode/diode_resistor)
        token = strtok_r(rest, ",", &rest);
        char problem_type[20];
        strncpy(problem_type, token, sizeof(problem_type) - 1);
        problem_type[sizeof(problem_type) - 1] = '\0';

        token = strtok_r(rest, ",", &rest);
        token = strtok_r(rest, ",", &rest);
        double Is = atof(token);        token = strtok_r(rest, ",", &rest);
        token = strtok_r(rest, ",", &rest);
        double n = atof(token);

        token = strtok_r(rest, ",", &rest);
        token = strtok_r(rest, ",", &rest);
        double Vt = atof(token);        // Kasus rangkaian dioda tunggal
        if (strcmp(problem_type, "diode") == 0) {
            // Baca parameter arus input (I)
            token = strtok_r(rest, ",", &rest);
            token = strtok_r(rest, ",", &rest);
            double I = atof(token);

            // Tampilkan parameter dan hasil perhitungan
            printf("\nDiode Case: Is=%.2e, n=%.2f, Vt=%.5f, I=%.4f\n", Is, n, Vt, I);
            double yt = taylor_method(Is, n, Vt, I);
            printf("  Taylor Approx y(t+H) = %.10f\n", yt);
            picard_method(Is, n, Vt, I);
        }        // Kasus rangkaian dioda dengan resistor
        else if (strcmp(problem_type, "diode_resistor") == 0) {
            // Baca parameter resistor (R)
            token = strtok_r(rest, ",", &rest);
            token = strtok_r(rest, ",", &rest);
            double R = atof(token);

            // Baca parameter tegangan sumber (Vs)
            token = strtok_r(rest, ",", &rest);
            token = strtok_r(rest, ",", &rest);
            double Vs = atof(token);

            // Baca parameter arus input (I)
            token = strtok_r(rest, ",", &rest);
            double I = atof(token);

            printf("\nDiode with Resistor Case: Is=%.2e, n=%.2f, Vt=%.5f, R=%.1f, Vs=%.1f, I=%.4f\n", 
                   Is, n, Vt, R, Vs, I);
              double current = f_resistor(Is, n, Vt, R, Vs, I);
            printf("  Circuit Current = %.10f A\n", current);
        }
    }

    fclose(file);
    return 0;
}