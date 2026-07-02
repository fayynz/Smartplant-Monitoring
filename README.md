# Smartplant-Monitoring

Sistem pemantauan kondisi tanaman secara *real-time* berbasis IoT (Internet of Things) yang menggunakan mikrokontroler ESP32 dan *dashboard* berbasis web.

## Deskripsi Proyek
Proyek ini dikembangkan untuk memudahkan pemantauan kelembaban tanah, suhu, dan kelembaban udara di sekitar tanaman. Sistem membaca data dari sensor dan mengirimkannya ke Firebase Realtime Database, yang kemudian ditampilkan melalui *dashboard* web responsif. Fitur otomatisasi pompa air memastikan tanaman mendapatkan air sesuai dengan kebutuhan kelembaban tanah.

## Fitur Utama
- **Monitoring Real-time**: Memantau kelembaban tanah, suhu lingkungan, kelembaban udara, dan intensitas cahaya (*LDR*).
- **Sistem Otomasi**: Pompa air otomatis menyala/mati berdasarkan ambang batas (*threshold*) kelembaban tanah.
- **Kontrol Manual**: Pengguna dapat menyalakan atau mematikan pompa air secara manual melalui *dashboard* web.
- **Responsive Dashboard**: Antarmuka web yang menyesuaikan tampilan pada perangkat *mobile* maupun *desktop*.

## Teknologi yang Digunakan
- **Hardware**: ESP32, Sensor Kelembaban Tanah, Sensor DHT11, Sensor LDR, Relay, Pompa Air.
- **Backend**: Firebase Realtime Database.
- **Frontend**: HTML5, CSS3, JavaScript.

## Konfigurasi Pin (Hardware)
| Komponen | Pin ESP32 |
| :--- | :--- |
| Sensor Kelembaban Tanah | GPIO 34 (ADC) |
| Sensor DHT11 (Suhu & Kelembaban) | GPIO 13 |
| Sensor LDR | GPIO 35 (ADC) |
| Relay (Pompa Air) | GPIO 14 |

## Cara Penggunaan
1. Pastikan perangkat ESP32 telah diprogram dengan *source code* yang sesuai.
2. Pastikan Firebase sudah terhubung dengan perangkat ESP32 dan web *dashboard*.
3. Buka file `index.html` pada browser untuk mengakses *dashboard* pemantauan.

---
*Proyek ini merupakan pemenuhan tugas mata kuliah IoT (Internet of Things).*
