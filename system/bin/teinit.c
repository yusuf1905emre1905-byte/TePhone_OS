// teui_service.c
// TePhone OS – İlk Görsel Arayüz Servisi
// © 2025 TeOS Project

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

int main() {
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Ekran cihazı açılamadı (/dev/fb0)");
        return 1;
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Ekran bilgileri alınamadı");
        close(fbfd);
        return 1;
    }

    long screensize = vinfo.yres_virtual * vinfo.xres_virtual * vinfo.bits_per_pixel / 8;
    char *fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((intptr_t)fbp == -1) {
        perror("Ekran belleği eşlenemedi");
        close(fbfd);
        return 1;
    }

    // Ekranı siyah yap
    memset(fbp, 0x00, screensize);

    // Ortada beyaz bir "TePhone OS" yazısı (piksel bazlı)
    int x, y;
    int cx = vinfo.xres / 2;
    int cy = vinfo.yres / 2;

    // Basit bir yazı efekti (beyaz karelerle)
    for (y = -40; y < 40; y++) {
        for (x = -100; x < 100; x++) {
            long location = (cx + x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                            (cy + y + vinfo.yoffset) * vinfo.line_length;

            if (x % 10 == 0 || y % 20 == 0) { // Basit desen
                *(fbp + location) = 0xFF;        // Blue
                *(fbp + location + 1) = 0xFF;    // Green
                *(fbp + location + 2) = 0xFF;    // Red
            }
        }
    }

    printf("TeUI: TePhone OS açılış ekranı çizildi.\n");

    sleep(3); // 3 saniye göster
    munmap(fbp, screensize);
    close(fbfd);

    return 0;
}
