#include <stdio.h>

typedef struct Building {
    const char *name;
    int built_year;
    int (*age)(struct Building *self, int current_year);
};

int building_age(int current_year) {
    return current_year;
}

int main(void) {
    struct Building b = {
        .name = "Opera House",
        .built_year = 1990,
        .age = building_age
    };

    int a = b.age();
    printf("%s age: %d\n", b.name(), a);
    return 0;
}

