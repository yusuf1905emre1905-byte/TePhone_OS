// tecored.c — TePhone OS Core Service Manager
// Gerçek çalışan kod — TeOS Project
// Version: 0.1-alpha

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SERVICES 10

typedef struct {
    const char *name;
    const char *cmd;
    pid_t pid;
} service_t;

service_t services[MAX_SERVICES];
int service_count = 0;

void add_service(const char *name, const char *cmd) {
    if(service_count >= MAX_SERVICES) {
        fprintf(stderr, "Max service limit reached\n");
        return;
    }
    services[service_count].name = name;
    services[service_count].cmd = cmd;
    services[service_count].pid = 0;
    service_count++;
}

void start_service(service_t *svc) {
    pid_t pid = fork();
    if(pid == 0) {
        // Child process
        execl("/bin/sh", "sh", "-c", svc->cmd, NULL);
        perror("Failed to start service");
        exit(1);
    } else if(pid > 0) {
        // Parent
        svc->pid = pid;
        printf("[TeCore] Started %s (PID: %d)\n", svc->name, pid);
    } else {
        perror("fork failed");
    }
}

void monitor_services() {
    int status;
    for(int i = 0; i < service_count; i++) {
        if(services[i].pid > 0) {
            pid_t ret = waitpid(services[i].pid, &status, WNOHANG);
            if(ret == 0) continue; // still running
            printf("[TeCore] Service %s exited. Restarting...\n", services[i].name);
            start_service(&services[i]);
        }
    }
}

int main() {
    printf("[TeCore] TePhone OS Core Service Manager starting...\n");

    // Temel servisleri ekle
    add_service("TeDaemon", "/system/bin/tedaemon &");
    add_service("TeUI", "/system/bin/teui_service &");
    add_service("TeAI", "/system/bin/teai &");

    // Başlat
    for(int i = 0; i < service_count; i++)
        start_service(&services[i]);

    // Sürekli çalışacak ana döngü
    while(1) {
        monitor_services();
        sleep(5);
    }

    return 0;
}
  
