# Tugas Proyek UAS Komputasi Numerik - Analisis Diode Circuit dengan Taylor Series dan Metode Picard

## Identitas
- Nama: Adhi Rajasa Rafif
- NPM: 2306266943

## Deskripsi
Proyek ini merupakan implementasi analisis rangkaian dioda menggunakan dua metode numerik:
1. Taylor Series (Order 2)
2. Metode Picard (3 iterasi)

Program membaca data dari file CSV, melakukan analisis untuk dua jenis rangkaian:
- Rangkaian dioda tunggal
- Rangkaian dioda dengan resistor

## Flowchart Program
![Flowchart Program Analisis Diode Circuit](https://imgur.com/3yWuh2u.png)

Flowchart di atas menunjukkan alur kerja program dari awal hingga akhir:
1. Program dimulai dengan membaca data dari file CSV
2. Untuk setiap baris data:
   - Identifikasi tipe rangkaian (dioda/dioda-resistor)
   - Baca parameter-parameter yang diperlukan
   - Lakukan perhitungan dengan metode Taylor Series
   - Lakukan perhitungan dengan metode Picard (3 iterasi)
   - Hitung arus rangkaian final
3. Hasil perhitungan ditulis ke file output.csv
4. Program selesai

## Struktur File
- `code.c` : Source code utama program
- `code.exe` : File executable hasil kompilasi
- `data.csv` : File input parameter untuk analisis
- `output.csv` : File output hasil analisis

## Cara Menjalankan Program
1. **Kompilasi program:**
   ```powershell
   gcc code.c -o code.exe -lm
   ```
2. **Jalankan program:**
   ```powershell
   ./code.exe
   ```

## Format Data Input (data.csv)
Baris pertama adalah header. Setiap baris berikutnya berisi data dengan format:

### Parameter untuk Dioda:
```
problem,I_s,value,n,value,V_t,value,I,value,NA,0,x0,x1,tolerance,max_iterations
```
Contoh:
```
diode,I_s,1e-12,n,1.5,V_t,0.02585,I,0.001,NA,0,0.5,0.8,1e-6,100
```

### Parameter untuk Dioda dengan Resistor:
Format 1 (dengan parameter iterasi):
```
diode_resistor,I_s,value,n,value,V_t,value,R,value,Vs,value,x0,x1,tolerance,max_iterations
```
Format 2 (tanpa parameter iterasi):
```
diode-resistor,I_s,value,n,value,V_t,value,I,value,R,value,Vs,value
```

## Format Output (output.csv)
Program menghasilkan file output.csv dengan format:
```
Problem Type,Is,n,Vt,R,Vs,Taylor Result,Picard_1,Picard_2,Picard_3,Circuit Current
```

Setiap baris berisi:
- Problem Type: Jenis rangkaian (diode/diode-resistor)
- Is: Arus saturasi
- n: Faktor idealitas
- Vt: Tegangan thermal
- R: Nilai resistor (0 untuk rangkaian dioda tunggal)
- Vs: Tegangan sumber (0 untuk rangkaian dioda tunggal)
- Taylor Result: Hasil perhitungan dengan metode Taylor
- Picard_1,2,3: Hasil 3 iterasi metode Picard
- Circuit Current: Arus rangkaian final

## Metodologi
Program mengimplementasikan dua metode numerik untuk analisis rangkaian dioda:

1. **Taylor Series Order 2**
   ```
   y(t+h) = y(t) + h*f + (h²/2)*f*df/dy
   ```
   dimana:
   - f adalah fungsi dioda
   - df/dy adalah turunan pertama terhadap y
   - h adalah step size

2. **Metode Picard (3 iterasi)**
   Menggunakan iterasi:
   ```
   y_(n+1)(t) = y₀ + ∫[t₀ to t] f(s,y_n(s))ds
   ```

## Implementasi
Program menggunakan beberapa teknik untuk menjaga stabilitas numerik:
- Penggunaan step size yang kecil (h/1000)
- Scaling factor untuk menghindari overflow
- Pembatasan nilai output dalam range fisik yang masuk akal
- Error handling untuk kasus-kasus ekstrim

## Hasil dan Analisis
Program berhasil menganalisis dua jenis rangkaian:

1. **Rangkaian Dioda Tunggal:**
   - Konvergensi stabil untuk berbagai nilai Is dan n
   - Hasil Taylor series konsisten dengan ekspektasi fisik
   - Metode Picard menunjukkan konvergensi yang baik

2. **Rangkaian Dioda-Resistor:**
   - Berhasil menghitung arus rangkaian dengan benar
   - Menunjukkan pengaruh resistor pada karakteristik rangkaian
   - Hasil perhitungan memenuhi hukum Kirchhoff

## Kesimpulan
Implementasi menunjukkan bahwa kedua metode (Taylor Series dan Picard) dapat digunakan untuk menganalisis rangkaian dioda dengan hasil yang stabil. Program ini juga mendemonstrasikan pentingnya penanganan numerik yang tepat untuk menghindari masalah overflow dan ketidakstabilan dalam perhitungan.

Data menunjukkan bahwa:
1. Metode Picard konvergen dalam 3 iterasi untuk sebagian besar kasus
2. Hasil Taylor series memberikan aproksimasi yang baik untuk kedua jenis rangkaian
3. Program dapat menangani berbagai nilai parameter dengan baik (Is: 1e-13 hingga 1e-11, n: 1.0 hingga 2.1)
