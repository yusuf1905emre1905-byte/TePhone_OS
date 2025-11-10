// tehome.c — TePhone OS Ana Arayüz (TeHome UI)
// Gerçek çalışan kod — TeOS Project
// Version: 0.1-alpha

#include <stdio.h>
#include <unistd.h>
#include <time.h>

void draw_ui() {
    // Temsili basit arayüz (ASCII ekran)
    printf("\033[1;34m"); // Mavi renk
    printf("========================================\n");
    printf("           TePhone OS - TeHome          \n");
    printf("========================================\n");
    printf("\033[0m");
}

void draw_status_bar() {
    // Saat ve şarj durumu
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int battery_percent = 85; // Örnek
    printf("\n[TeHome] Saat: %02d:%02d:%02d | Şarj: %d%%\n",
           tm.tm_hour, tm.tm_min, tm.tm_sec, battery_percent);
}

void draw_icons() {
    printf("\n[TeHome] Uygulamalar:\n");
    printf(" 1. TeSettings  2. TeFiles  3. TeStore  4. TeAI\n");
    printf("Seçmek için rakam girin: ");
}

int main() {
    int choice = 0;

    while(1) {
        // Temizle
        printf("\033[2J\033[H"); // Terminal temizleme

        draw_ui();
        draw_status_bar();
        draw_icons();

        if(scanf("%d", &choice) == 1) {
            switch(choice) {
                case 1:
                    printf("[TeHome] TeSettings açılıyor...\n");
                    sleep(2); // Simülasyon
                    break;
                case 2:
                    printf("[TeHome] TeFiles açılıyor...\n");
                    sleep(2);
                    break;
                case 3:
                    printf("[TeHome] TeStore açılıyor...\n");
                    sleep(2);
                    break;
                case 4:
                    printf("[TeHome] TeAI başlatılıyor...\n");
                    sleep(2);
                    break;
                default:
                    printf("[TeHome] Geçersiz seçim!\n");
                    sleep(1);
            }
        } else {
            // Hatalı giriş
            while(getchar() != '\n'); // Temizle
        }
    }

    return 0;
}
