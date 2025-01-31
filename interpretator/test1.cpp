#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "student.h"

int main() {
    Group123 group1;
    Group4 group2;

    group1.add();
    //group1.add();
    //group1.add();

    Group123& group3 = group1;

    group2.add();
    //group2.add();

    std::cout << "Исходная группа:\n";
    group1.output();
    group3.output();
    group2.output();
    int id;
    printf("Студента с каким id будем удалять?\n");
    scanf("%d", &id);
    group1.remove(id);
    group1.output();
    
    printf("Студента с каким id вывести на экран? ");
    scanf("%d", &id);
    std::cout << group1[id] << std::endl;

    return 0;
}
