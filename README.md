Berikut isi lengkap dari file `README_IoT_Monitoring.md` yang telah dirapikan dan mencakup semua bagian:

---

# 🌐 IoT Smart Room Monitoring System

Sistem ini adalah proyek Internet of Things (IoT) yang bertujuan untuk memantau kondisi ruangan secara real-time dan memberikan respons otomatis ketika kondisi tertentu terdeteksi. Proyek ini mengintegrasikan **ESP8266** sebagai node sensor, **ESP32** sebagai node aktuator, dan **Firebase** sebagai penghubung cloud, serta dashboard **website** untuk monitoring data secara visual.

## 📌 Fitur Utama

* 📡 **Monitoring Real-time**:

  * Deteksi gerakan dengan **PIR sensor**
  * Monitoring suhu dan kelembapan dengan **DHT11**
  * Deteksi pencahayaan dengan **LDR**

* 💡 **Respon Otomatis**:

  * **ESP32** membaca data dari Firebase
  * Menyalakan **LED** sebagai indikator
  * Mengaktifkan **buzzer** sebagai alarm saat kondisi berbahaya terdeteksi

* 🌍 **Integrasi Cloud dengan Firebase**:

  * Pengiriman data sensor dari ESP8266 ke Firebase Realtime Database
  * Pengambilan data oleh ESP32 dari Firebase untuk respons otomatis

* 🖥️ **Web Dashboard**:

  * Visualisasi data suhu, kelembapan, intensitas cahaya, dan status gerakan
  * Dibangun menggunakan **Next.js** dan terhubung langsung ke Firebase

## ⚙️ Arsitektur Sistem

```
[PIR, LDR, DHT11]
       │
   [ESP8266] ──────► [Firebase] ◄────── [ESP32]
       │                                   │
   (Kirim data)                      (Ambil data & kontrol)
                                           │
                                  [LED]  [Buzzer]

                          +──── Web Dashboard (Next.js) ────+
                          |     Tampilkan data sensor        |
                          +─────────────────────────────────+
```

## 📦 Komponen yang Digunakan

### 🔧 Hardware

* ESP8266 NodeMCU (1x)
* ESP32 DevKit (1x)
* Sensor PIR (1x)
* Sensor DHT11 (1x)
* Sensor LDR (1x)
* LED dan Buzzer (output aktuator)
* Breadboard, jumper kabel, power supply

### 🧠 Software & Tools

* Arduino IDE (pemrograman mikrokontroler)
* Firebase Realtime Database (cloud backend)
* Next.js (web frontend)
* JavaScript & CSS (frontend)
* Firebase SDK (integrasi data)

## 🚀 Cara Kerja

1. **ESP8266** membaca data dari sensor (gerakan, suhu, cahaya).
2. Data dikirim secara berkala ke **Firebase**.
3. **ESP32** membaca nilai terbaru dari Firebase:

   * Jika mendeteksi kondisi tertentu (misalnya suhu tinggi atau ada gerakan malam hari), maka menyalakan LED dan buzzer.
4. **Web dashboard** mengambil data dari Firebase dan menampilkannya secara real-time.

---

## 📊 Metodologi Pengujian

### 🔍 Analisis PEAS (Performance, Environment, Actuator, Sensor)

| Komponen        | Deskripsi                                                                          |
| --------------- | ---------------------------------------------------------------------------------- |
| **Performance** | Sistem mampu mengirim dan merespons data dalam waktu < 2 detik secara real-time    |
| **Environment** | Lingkungan indoor dengan pencahayaan alami dan suhu normal (25–35°C)               |
| **Actuator**    | LED menyala saat mendeteksi gerakan pada malam hari, buzzer aktif saat suhu tinggi |
| **Sensor**      | PIR untuk gerakan, DHT11 untuk suhu & kelembapan, LDR untuk cahaya                 |

### 🧪 Running Test

| No | Kondisi Pengujian                             | Output Sistem                | Status |
| -- | --------------------------------------------- | ---------------------------- | ------ |
| 1  | Tidak ada gerakan, suhu normal, cahaya terang | Semua indikator mati         | ✅      |
| 2  | Gerakan terdeteksi di malam hari              | LED menyala                  | ✅      |
| 3  | Suhu > 35°C                                   | Buzzer aktif                 | ✅      |
| 4  | Semua kondisi aktif                           | LED & buzzer aktif bersamaan | ✅      |
| 5  | Perubahan data ditampilkan di web dashboard   | Data tampil secara real-time | ✅      |

### ✅ Kesimpulan Pengujian

Berdasarkan pengujian yang dilakukan, sistem berhasil bekerja secara responsif dan stabil. Komunikasi data antara ESP8266, Firebase, dan ESP32 berjalan dengan baik, dengan latensi minimal. Web dashboard mampu menampilkan perubahan data secara real-time. Dengan ini, sistem dinyatakan **berfungsi sesuai tujuan monitoring dan kontrol otomatis** berbasis kondisi lingkungan.

---

## 📚 Daftar Istilah

| Istilah       | Definisi                                                                                        |
| ------------- | ----------------------------------------------------------------------------------------------- |
| **IoT**       | Internet of Things: Konsep menghubungkan perangkat ke internet agar dapat saling berkomunikasi. |
| **ESP8266**   | Mikrokontroler yang digunakan sebagai node pengirim data sensor ke Firebase.                    |
| **ESP32**     | Mikrokontroler yang berfungsi sebagai node penerima dan pengontrol LED & buzzer.                |
| **PIR**       | Sensor gerak yang mendeteksi adanya pergerakan manusia di dalam ruangan.                        |
| **LDR**       | Light Dependent Resistor, sensor yang mengukur tingkat pencahayaan.                             |
| **DHT11**     | Sensor suhu dan kelembapan yang digunakan untuk memantau kondisi lingkungan.                    |
| **Firebase**  | Platform cloud milik Google untuk menyimpan dan menyinkronkan data secara real-time.            |
| **Next.js**   | Framework JavaScript untuk membuat web aplikasi berbasis React.                                 |
| **Dashboard** | Tampilan web yang menampilkan data monitoring secara visual dan real-time.                      |





