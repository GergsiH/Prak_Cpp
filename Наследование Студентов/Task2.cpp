#include <iostream>
#include <algorithm>
#include <csignal>
#include "ClassStudent.h"

/*void handler(int signal){
    //std::cout<<"Hiiiiiiii\n";
    std::exit(0);
}*/

int main() {

    //signal(SIGINT, handler);
    
    int count1,count2;
    StudGroup groupStud;
    GradGroup groupGrad;

    std::cout<<"There is one student group studying at this university and it is graduating."<<std::endl;
    std::cout<<"How many people are in the student group? : ";
    std::cin>>count1;
    for ( int i = 0; i < count1; i++){
        std::cout<<"["<<i+1<<"]";
        groupStud.addStudent();
    }

    std::cout<<"How many people are in the graduation group? :";
    std::cin>>count2;
    for ( int i = 0; i < count2; i++){
        std::cout<<"["<<i+1<<"]";
        groupGrad.addStudent();
    }

    std::cout<<"\nThese are the students who study in these groups:"<<std::endl;

    groupStud.outputStudent();
    std::cout<<std::endl;
    groupGrad.outputStudent();




    int id,n,n2;
    std::cout<<"\nWhich group will we work with?\n"<<"1 - Stud Group\n2 - Grad Group\n";
    std::cin>>n;
    switch(n){
        case 1:
            std::cout<<"\nWhich student ID will we delete? : ";
            std::cin>>id;
            groupStud.kickStudent(id);
            groupStud.outputStudent();
            std::cout<<"\nWhich student ID will we display on the screen? : ";
            std::cin>>id;
            try {
                std::cout << groupStud[id] << std::endl;
            }
            catch (const char *s)
            {
                std::cout << s;
            }

            //std::cout << groupStud[id] << std::endl;

            /*std::cout<<"\nAnd now I'll output GROUP to show that assignment overloading works. \n";
            GROUP.outputStudent();*/

            break;
        case 2:
            std::cout<<"\nWhich student ID will we delete? : ";
            std::cin>>id;
            groupGrad.kickStudent(id);
            groupGrad.outputStudent();
            std::cout<<"\nWhich student ID will we display on the screen? : ";
            std::cin>>id;
            try {
                std::cout << groupGrad[id] << std::endl;
            }
            catch (const char *s)
            {
                std::cout << s;
            }
            break;
        default:
            break;

    }


    std::cout<<"\nAnd now I'll output GROUP to show that assignment overloading works. \n";
    //Group* GROUP = &groupStud;
    Group* GROUP[2] = {&groupGrad, &groupStud};
    
    for (int i = 0; i < 2; i++){
        GROUP[i] -> outputStudent(); 
    }
    //GROUP.addStudent();


   //std::cout<<"And now I'll output GROUP to show that assignment overloading works. \n";
    //GROUP.outputStudent();


    return 0;
}