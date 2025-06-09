# Tugas Pemrograman UAS Komputasi Numerik - Analisis Diode Circuit dengan Taylor Series dan Metode Picard

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

## Struktur File
- `code.c` : Source code utama program
- `code.exe` : File executable hasil kompilasi
- `data.csv` : File input parameter untuk analisis

## Cara Menjalankan Program
1. **Kompilasi program:**
   ```powershell
   gcc code.c -o code -lm
   ```
2. **Jalankan program:**
   ```powershell
   ./code
   ```

## Format Data Input (data.csv)
Baris pertama adalah header. Setiap baris berikutnya berisi data dengan format:
```
problem_type,param1_name,param1_value,param2_name,param2_value,...
```

### Parameter untuk Dioda:
- `problem_type`: diode
- `Is`: Arus saturasi
- `n`: Faktor idealitas
- `Vt`: Tegangan thermal
- `I`: Arus input

### Parameter untuk Dioda dengan Resistor:
- `problem_type`: diode_resistor
- `Is`: Arus saturasi
- `n`: Faktor idealitas
- `Vt`: Tegangan thermal
- `R`: Nilai resistor
- `Vs`: Tegangan sumber
- `I`: Arus input

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

## Output Program
Program memberikan output untuk setiap kasus:
1. **Kasus Dioda:**
   - Nilai parameter (Is, n, Vt, I)
   - Hasil Taylor Series
   - Hasil 3 iterasi Metode Picard

2. **Kasus Dioda dengan Resistor:**
   - Nilai parameter (Is, n, Vt, R, Vs, I)
   - Arus rangkaian hasil perhitungan

## Kesimpulan
Implementasi menunjukkan bahwa kedua metode (Taylor Series dan Picard) dapat digunakan untuk menganalisis rangkaian dioda dengan hasil yang stabil dan masuk akal secara fisik. Program ini juga mendemonstrasikan pentingnya penanganan numerik yang tepat untuk menghindari masalah overflow dan ketidakstabilan dalam perhitungan.
