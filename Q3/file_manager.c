#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct Employee {
    int id;
    char name[50];
    float salary;
};

int main() {
    int fd;
    struct Employee emp;

    fd = open("employees.dat", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }
    printf("File created successfully\n");

    struct Employee records[] = {
        {1, "Alice", 50000},
        {2, "Bob", 60000},
        {3, "Charlie", 55000}
    };

    for (int i = 0; i < 3; i++) {
        write(fd, &records[i], sizeof(struct Employee));
        printf("Written: ID=%d, Name=%s, Salary=%.2f\n", records[i].id, records[i].name, records[i].salary);
    }

    printf("\nUpdating record 2 (Bob) salary to 75000\n");
    lseek(fd, 1 * sizeof(struct Employee), SEEK_SET);
    read(fd, &emp, sizeof(struct Employee));
    emp.salary = 75000;
    lseek(fd, 1 * sizeof(struct Employee), SEEK_SET);
    write(fd, &emp, sizeof(struct Employee));
    printf("Updated: ID=%d, Name=%s, Salary=%.2f\n", emp.id, emp.name, emp.salary);

    printf("\nRetrieving record 3 (Charlie)\n");
    lseek(fd, 2 * sizeof(struct Employee), SEEK_SET);
    read(fd, &emp, sizeof(struct Employee));
    printf("Retrieved: ID=%d, Name=%s, Salary=%.2f\n", emp.id, emp.name, emp.salary);

    printf("\nAll records:\n");
    lseek(fd, 0, SEEK_SET);
    while (read(fd, &emp, sizeof(struct Employee)) > 0) {
        printf("ID=%d, Name=%s, Salary=%.2f\n", emp.id, emp.name, emp.salary);
    }

    close(fd);
    printf("\nFile closed\n");
    return 0;
}
