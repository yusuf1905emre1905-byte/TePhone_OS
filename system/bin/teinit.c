// teinit.c — TePhone OS Initial System Process
// Author: TeOS Foundation
// Version: 0.0.1-alpha

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void start_service(const char *name, const char *cmd) {
    printf("[TeInit] Starting %s...\n", name);
    pid_t pid = fork();
    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd, NULL);
        perror("execl");
        exit(1);
    }
}

void teos_logo() {
    printf("\033[1;36m");
    printf("\n====================================\n");
    printf("        Welcome to TePhone OS       \n");
    printf("          powered by TeOS           \n");
    printf("====================================\n");
    printf("\033[0m\n");
}

int main() {
    teos_logo();
    printf("[TeInit] Boot sequence started.\n");

    // Sistem dizinlerini kontrol et
    mkdir("/data", 0755);
    mkdir("/tmp", 0755);
    mkdir("/system", 0755);

    // Log dosyası
    int fd = open("/data/boot.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd >= 0) dup2(fd, STDOUT_FILENO);

    // Servisleri başlat
    start_service("TeCore", "/system/bin/tecored &");
    start_service("TeDaemon", "/system/bin/tedaemon &");
    start_service("TeUI", "/system/bin/teui_service &");

    // Sistem hazır
    printf("[TeInit] All core services started.\n");
    printf("[TeInit] System entering idle mode.\n");

    // Ana döngü — sistem canlı tut
    while (1) {
        sleep(10);
    }

    return 0;
}
