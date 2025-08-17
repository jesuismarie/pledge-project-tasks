#include <stdio.h>
#include <stdint.h>

int8_t building_age_years(int8_t current_year_2d, int8_t built_year_2d, int8_t months_since_anniv)
{
    int8_t years = current_year_2d - built_year_2d;
    int8_t age_months = years * 12 + months_since_anniv;
    return age_months / 12;
}

int main(void) {
    int8_t current = 25, built = 90, months = 0;
    int8_t age = building_age_years(current, built, months);
    printf("Building age: %d years\n", age);
    return 0;
}

