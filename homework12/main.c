#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <locale.h>

struct Date {
    int day;
    int month;
    int year;
};

struct Product {
    char name[50];
    struct Date prod_date;
    int shelf_life;
    float price;
    int serial_number;
};

days_in_month(int month, int year);
date_to_days(int day, int month, int year);
struct Date input_date(const char* prompt);
struct Product input_product(int number);
void print_product(struct Product p);
int check_expiring(struct Product p, int cur_day, int cur_month, int cur_year);


int main() {
    setlocale(LC_ALL, "RUS");

    printf("=== СИСТЕМА УЧЕТА ТОВАРОВ ===\n");
    printf("Введите текущую дату для проверки сроков годности:\n");

    int today_day, today_month, today_year;
    printf("Текущая дата (ДД ММ ГГГГ): ");
    scanf("%d %d %d", &today_day, &today_month, &today_year);

    struct Product prod1 = input_product(1);
    struct Product prod2 = input_product(2);
    struct Product prod3 = input_product(3);

    printf("\n========== ВСЕ ТОВАРЫ НА СКЛАДЕ ==========\n");
    print_product(prod1);
    print_product(prod2);
    print_product(prod3);

    printf("\n========== ТОВАРЫ С ИСТЕКАЮЩИМ СРОКОМ ГОДНОСТИ (В ТЕЧЕНИЕ 5 СУТОК) ==========\n");
    int found = 0;

    if (check_expiring(prod1, today_day, today_month, today_year)) found = 1;
    if (check_expiring(prod2, today_day, today_month, today_year)) found = 1;
    if (check_expiring(prod3, today_day, today_month, today_year)) found = 1;

    if (!found) {
        printf("Товаров, срок годности которых заканчивается в течение 5 суток, не найдено.\n");
    }

    return 0;
}


int days_in_month(int month, int year) {
    switch (month) {
    case 1: return 31;
    case 2:
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
            return 29;
        else
            return 28;
    case 3: return 31;
    case 4: return 30;
    case 5: return 31;
    case 6: return 30;
    case 7: return 31;
    case 8: return 31;
    case 9: return 30;
    case 10: return 31;
    case 11: return 30;
    case 12: return 31;
    default: return 30;
    }
}

int date_to_days(int day, int month, int year) {
    int days = year * 365 + day;

    for (int i = 1; i < month; i++) {
        days += days_in_month(i, year);
    }

    if (month > 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
        days += 1;
    }

    return days;
}

struct Date input_date(const char* prompt) {
    struct Date d;
    printf("%s (ДД ММ ГГГГ): ", prompt);
    scanf("%d %d %d", &d.day, &d.month, &d.year);
    return d;
}

struct Product input_product(int number) {
    struct Product p;
    printf("\n--- Ввод товара %d ---\n", number);
    printf("Введите наименование продукта: ");
    scanf(" %[^\n]", p.name);

    p.prod_date = input_date("Введите дату производства");

    printf("Введите срок годности (в днях): ");
    scanf("%d", &p.shelf_life);

    printf("Введите цену (в рублях): ");
    scanf("%f", &p.price);

    printf("Введите номер серии: ");
    scanf("%d", &p.serial_number);

    return p;
}

void print_product(struct Product p) {
    printf("Наименование: %s\n", p.name);
    printf("Дата производства: %02d.%02d.%04d\n", p.prod_date.day, p.prod_date.month, p.prod_date.year);
    printf("Срок годности: %d дн.\n", p.shelf_life);
    printf("Цена: %.2f\n", p.price);
    printf("Номер серии: %d\n", p.serial_number);
    printf("-------------------------\n");
}

int check_expiring(struct Product p, int cur_day, int cur_month, int cur_year) {
    int current_days = date_to_days(cur_day, cur_month, cur_year);
    int exp_days = date_to_days(p.prod_date.day, p.prod_date.month, p.prod_date.year) + p.shelf_life;
    int diff = exp_days - current_days;

    if (diff <= 5) {
        if (diff < 0) {
            printf("СРОК ГОДНОСТИ ИСТЕК! Товар \"%s\" просрочен на %d дн.\n", p.name, -diff);
        }
        else if (diff == 0) {
            printf("ВНИМАНИЕ! Срок годности товара \"%s\" истекает СЕГОДНЯ!\n", p.name);
        }
        else {
            printf("ВНИМАНИЕ! Срок годности товара \"%s\" истекает через %d дн.\n", p.name, diff);
        }
        print_product(p);
        return 1;
    }
    return 0;
}
