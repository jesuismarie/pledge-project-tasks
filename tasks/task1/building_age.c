#include <stdio.h>
#include <stdint.h>

int8_t building_age_years(int8_t current_year_2d, int8_t built_year_2d, int8_t months_since_anniv)
{
	int8_t years = current_year_2d - built_year_2d;
	if (years < 0)
		years += 100;
	years += months_since_anniv / 12;
	if (months_since_anniv < 0)
		years -= 1;
	return years;
}

int main(void) {
	int8_t current = 25, built = 90, months = -11;
	int8_t age = building_age_years(current, built, months);
	printf("Building age: %d years\n", age);
	return 0;
}
