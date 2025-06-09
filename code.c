/*
 * Program Analisis Rangkaian Dioda
 * --------------------------------
 * Nama: Adhi Rajasa Rafif
 * NPM: 2306266943
 * 
 * Program ini mengimplementasikan analisis rangkaian dioda menggunakan:
 * 1. Metode Taylor Series (Order 2)
 * 2. Metode Picard (3 iterasi)
 * untuk menghitung karakteristik rangkaian dioda
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Konstanta untuk program
#define MAX_LINE 1024    // Panjang maksimum baris CSV
#define H 0.1           // Step size dasar untuk metode Taylor
#define T0 0.0          // Waktu awal
#define Y0 0.0          // Nilai awal

/* 
 * Function declarations
 */
double f(double Is, double n, double Vt, double I);
double calculate_diode_voltage(double Is, double n, double Vt, double I);
double calculate_circuit_current(double Is, double n, double Vt, double I, double R, double Vs);
double f_resistor(double Is, double n, double Vt, double R, double Vs, double I);
double df_dy(double Is, double n, double Vt, double I);

/*
 * Function to calculate resistor current
 * Parameters:
 * - Is: Arus saturasi dioda
 * - n: Faktor idealitas dioda
 * - Vt: Tegangan thermal
 * - R: Nilai resistor
 * - Vs: Tegangan sumber
 * - I: Arus input
 * Returns: Arus yang mengalir melalui resistor
 */
double f_resistor(double Is, double n, double Vt, double R, double Vs, double I) {
    // Menghitung tegangan dioda
    double Vd = calculate_diode_voltage(Is, n, Vt, I);
    // Menghitung arus melalui resistor
    return (Vs - Vd) / R;
}

/* 
 * Fungsi persamaan dioda
 * Implementasi persamaan Shockley untuk dioda:
 * I = Is * (e^(V/nVt) - 1)
 * dimana V adalah tegangan dioda
 */
double f(double Is, double n, double Vt, double I) {
    return (n * Vt / Is) * log(I / Is + 1.0);
}

/* 
 * Fungsi untuk menghitung tegangan dioda
 * Menggunakan persamaan: V = nVt * ln(I/Is + 1)
 */
double calculate_diode_voltage(double Is, double n, double Vt, double I) {
    return n * Vt * log(I / Is + 1.0);
}

/* 
 * Fungsi untuk menghitung arus rangkaian
 * Mengimplementasikan hukum Kirchhoff untuk:
 * 1. Rangkaian dioda tunggal
 * 2. Rangkaian dioda-resistor
 */
double calculate_circuit_current(double Is, double n, double Vt, double I, double R, double Vs) {
    if (R == 0.0) return I;  // Kasus dioda tunggal
    
    // Menghitung tegangan dioda
    double Vd = calculate_diode_voltage(Is, n, Vt, I);
    // Menghitung arus rangkaian untuk kasus dioda-resistor
    return (Vs - Vd) / R;
}

/* 
 * Turunan parsial terhadap y untuk metode Taylor
 * df/dy = (nVt)/(I + Is)
 */
double df_dy(double Is, double n, double Vt, double I) {
    return (n * Vt) / (I + Is);
}

/* 
 * Taylor method order 2
 * Implementasi: y(t+h) = y(t) + h*f + (h²/2)*f*df/dy
 * Menggunakan scaling factor untuk menghindari overflow
 * dan pembatasan nilai untuk stabilitas numerik
 */
double taylor_method(double Is, double n, double Vt, double I, double R, double Vs) {
    double y = Y0;
    double f0 = f(Is, n, Vt, I);
    double df_dy0 = df_dy(Is, n, Vt, I);
    
    // Menggunakan step size yang lebih kecil untuk akurasi
    double h = H / 1000.0;
    
    // Perhitungan komponen Taylor series
    double first_order = h * f0;
    double second_order = (h * h / 2.0) * f0 * df_dy0;
    
    // Scaling untuk menghindari overflow
    double scale = 1e-9;
    first_order *= scale;
    second_order *= scale;
    
    // Kalkulasi hasil dan pembatasan nilai
    double result = y + first_order + second_order;
    if (result < Is) result = Is;      // Batas bawah: arus saturasi
    if (result > 1.0) result = 1.0;    // Batas atas: 1A
    
    return result;
}

/* 
 * Picard method hingga iterasi ke-3
 * Mengimplementasikan metode iterasi Picard:
 * y_(n+1)(t) = y₀ + ∫[t₀ to t] f(s,y_n(s))ds
 */
void picard_method(double Is, double n, double Vt, double I, double R, double Vs, double results[3]) {
    double v_thermal = n * Vt;
    
    // Iterasi pertama
    results[0] = I;
    
    // Iterasi kedua
    double V1 = calculate_diode_voltage(Is, n, Vt, results[0]);
    results[1] = calculate_circuit_current(Is, n, Vt, results[0], R, Vs);
    
    // Iterasi ketiga
    double V2 = calculate_diode_voltage(Is, n, Vt, results[1]);
    results[2] = calculate_circuit_current(Is, n, Vt, results[1], R, Vs);
    
    printf("\nHasil Metode Picard:\n");
    printf("Iterasi 1: %.10e\n", results[0]);
    printf("Iterasi 2: %.10e\n", results[1]);
    printf("Iterasi 3: %.10e\n", results[2]);
}

/* 
 * Program Utama 
 * ------------
 * Alur program:
 * 1. Baca data dari file CSV
 * 2. Untuk setiap baris data:
 *    - Identifikasi tipe rangkaian
 *    - Baca parameter rangkaian
 *    - Hitung dengan metode Taylor
 *    - Hitung dengan metode Picard
 *    - Hitung arus rangkaian final
 * 3. Simpan hasil ke output.csv
 */
int main() {
    // Buka data.csv untuk input
    FILE *file = fopen("data.csv", "r");
    if (!file) {
        printf("Error: Tidak dapat membuka data.csv\n");
        return 1;
    }    // Buka output file
    FILE *output = fopen("output.csv", "w");
    if (!output) {
        printf("Error: Tidak dapat membuat output file\n");
        fclose(file);
        return 1;
    }

    // Tulis header ke file CSV
    if (fprintf(output, "Problem Type,Is,n,Vt,R,Vs,Taylor Result,Picard_1,Picard_2,Picard_3,Circuit Current\n") < 0) {
        printf("Error: Gagal menulis header ke results.csv\n");
        fclose(file);
        fclose(output);
        return 1;
    }

    char line[MAX_LINE];
    // Skip header line dari data.csv
    if (!fgets(line, MAX_LINE, file)) {
        printf("Error: File data.csv kosong atau tidak valid\n");
        fclose(file);
        fclose(output);
        return 1;
    }

    printf("=== Hasil Perhitungan dari data.csv ===\n");

    while (fgets(line, MAX_LINE, file)) {
        char problem[20];
        char param_names[5][20];
        double param_values[5];
        double x0, x1, tolerance;
        int max_iterations;

        // Parse CSV line with the new format
        if (sscanf(line, "%[^,],I_s,%lf,n,%lf,V_t,%lf,I,%lf",
                   problem, &param_values[0], &param_values[1], 
                   &param_values[2], &param_values[3]) >= 5) {
            
            double Is = param_values[0];
            double n = param_values[1];
            double Vt = param_values[2];
            double I = param_values[3];

            printf("\nTipe Circuit: %s\n", problem);
            printf("Is = %.10e\n", Is);
            printf("n = %.10f\n", n);
            printf("Vt = %.10f\n", Vt);
            printf("I = %.10e\n", I);

            if (strcmp(problem, "diode") == 0) {
                // Perhitungan untuk diode
                double taylor_result = taylor_method(Is, n, Vt, I, 0.0, 0.0);
                printf("\nHasil Deret Taylor: %.10e\n", taylor_result);
                
                double picard_results[3];
                picard_method(Is, n, Vt, I, 0.0, 0.0, picard_results);
                
                // For simple diode, circuit current is the final Picard iteration
                double circuit_current = picard_results[2];
                
                // Write diode results to CSV
                fprintf(output, "%s,%.10e,%.10f,%.10f,%.10f,%.10f,%.10e,%.10e,%.10e,%.10e,%.10e\n",
                        problem, Is, n, Vt, 0.0, 0.0, taylor_result, 
                        picard_results[0], picard_results[1], picard_results[2], 
                        circuit_current);
            }
            else if (strcmp(problem, "diode-resistor") == 0) {
                // Get additional parameters R and Vs if available
                double R = 0.0, Vs = 0.0;
                if (sscanf(strstr(line, "R,"), "R,%lf,Vs,%lf", &R, &Vs) == 2) {
                    printf("R = %.10f\n", R);
                    printf("Vs = %.10f\n", Vs);
                    
                    // Calculate Taylor series result
                    double taylor_result = taylor_method(Is, n, Vt, I, R, Vs);
                    printf("\nHasil Deret Taylor: %.10e\n", taylor_result);
                    
                    // Calculate Picard iterations
                    double picard_results[3];
                    picard_method(Is, n, Vt, I, R, Vs, picard_results);
                    
                    // Calculate final circuit current using the last Picard iteration
                    double circuit_current = calculate_circuit_current(Is, n, Vt, picard_results[2], R, Vs);
                    printf("\nHasil Arus Circuit: %.10e\n", circuit_current);
                    
                    // Write diode-resistor results to CSV
                    fprintf(output, "%s,%.10e,%.10f,%.10f,%.10f,%.10f,%.10e,%.10e,%.10e,%.10e,%.10e\n",
                            problem, Is, n, Vt, R, Vs, taylor_result, 
                            picard_results[0], picard_results[1], picard_results[2], 
                            circuit_current);
                }
            }
            
            // Flush the output buffer to ensure writing to file
            fflush(output);
        }
    }

    fclose(file);
    fclose(output);
    printf("\nHasil telah ditulis ke output.csv\n");
    return 0;
}