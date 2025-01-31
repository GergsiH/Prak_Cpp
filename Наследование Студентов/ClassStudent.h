#include <iostream>
#define M 50

class Student {

    public:
        char *name = new char[M];
        int id;
        double score;

        Student ( char* _name = new char[M]{0}, int _id = 0, double _score = 0) :
            id(_id), score(_score), name(_name){}
        Student ( const Student& stud ){
            name = stud.name;
            id = stud.id;
            score = stud.score;
        }

        Student& operator =(const Student& stud) {
            name = stud.name;
            id = stud.id;
            score = stud.score;
            return *this;
        }
        ~Student() {
            delete [] name;
        }
};

class Graduates : public Student{
    public:
        char* diplom = new char[M];
        char* scientist = new char[M];
        Graduates(char* _name = new char[M]{0}, int _id = 0, double _score = 0, char* _diplom = new char[M]{0}, char* _scientist = new char[M]{0}) \
            : Student (_name, _id, _score), diplom(_diplom), scientist(_scientist) {}
        ~Graduates()
        {
            delete [] diplom;
            delete [] scientist;
        }
};

class Group {
    public:
        virtual ~Group() {}
        virtual void kickStudent (int studID) = 0;
        virtual void addStudent () = 0;
        virtual void outputStudent () const = 0;
        //virtual int getcount () const = 0;
        virtual const char* operator[](int studentId) const = 0;
};

class StudGroup : public Group {
    static const int MaxStud = 30;
    Student** arrStud;
    int count;
    public:
        StudGroup() : count(0) {
            arrStud = new Student*[MaxStud];
            for ( int i = 0; i < MaxStud; i++) 
                arrStud[i] = nullptr;
        }

        StudGroup( const StudGroup& group){
            for (int i = 0; i < count; i++){
                delete arrStud[i];
            }

            count = group.count;
            for( int i = 0; i < count; i++){
                arrStud[i] = new Student;
                arrStud[i] -> name = group.arrStud[i] -> name;
                arrStud[i] -> id = group.arrStud[i] -> id;
                arrStud[i] -> score = group.arrStud[i] -> score;
             }
        }

        StudGroup(StudGroup&& group){
            for ( int i = 0; i < count; i++){
                delete arrStud[i];
            }

            count = group.count;
            for ( int i = 0; i < count; i++){
                arrStud[i] = group.arrStud[i];
                group.arrStud[i] = nullptr;
            }

            group.count = 0;
            group.arrStud = nullptr;
        }

        ~StudGroup() {
            for ( int i = 0; i < MaxStud; i++) {
                delete arrStud[i];
            }
            delete [] arrStud;
        }

        StudGroup& operator = ( const StudGroup& group) {
            if ( this == &group)
                return *this;
            for ( int i = 0; i < count; i++){
                delete arrStud[i];
            }

            count = group.count;
            for ( int i = 0; i < count; i++){
                arrStud[i] = new Student(group.arrStud[i] -> name, group.arrStud[i] -> id, group.arrStud[i] -> score);
            }
            return *this;
        }


        StudGroup& operator = ( StudGroup&& rather) {
            std::swap(arrStud, rather.arrStud);
            std::swap(count, rather.count);
            return *this;
        }

        /*int getcount() const {
            return count;
        }*/

        friend std::ostream& operator << (std::ostream& os, const StudGroup& group) {
            os << "Students in the group: \n";
            for (int i = 0; i < group.count; ++i) {
                os <<"Student's name: " << group.arrStud[i]->name << ", ID: " << group.arrStud[i]->id << \
                ", Score: " << group.arrStud[i] -> score << std::endl;
            }
            return os;
        }

        const char* operator [] (int _id) const override {
            for (int i = 0; i < count; i++) {
                if (arrStud[i] -> id == _id) {
                    return arrStud[i] -> name;
                }
            }
            throw "There is no student with this ID";
        }


        void addStudent() override {
            char* name = new char[M];
            double score;
            int id;
            std::cout<<std::endl;
            std::cout << "Student's name: ";
            std::cin >> name;
            std::cout << "ID: ";
            std::cin >> id;
            std::cout << "Score: ";
            std::cin >> score;
            if (count < MaxStud) {
                arrStud[count++] = new Student(name, id, score);
            } else {
                std::cout << "This Group is full" << std::endl;
            }
            std::cout<<std::endl;
        }

        void kickStudent(int _id) override {
            int j;
            for (int i = 0; i < count; i++) {
                if (arrStud[i] -> id == _id) {
                    delete arrStud[i];
                    for (j = i; j < count - 1; j++) {
                        arrStud[j] = arrStud[j + 1];
                    }
                    arrStud[j] = nullptr;
                    count--;
                    return;
                }
            }
            std::cout << "There is no student with this ID : " << _id << std::endl;
        }

        void outputStudent() const override {
            std::cout << *this;
        }
};

class GradGroup: public Group {
    static const int MaxStud = 30;
    Graduates** arrStud;
    int count;
public:
    GradGroup () : count(0) {
        arrStud = new Graduates*[MaxStud];
        for (int i = 0; i < MaxStud; i++) arrStud[i] = nullptr;
    }

    GradGroup (const GradGroup& group) {
        for (int i = 0; i < count; i++) {
            delete arrStud[i];
        }
        count = group.count;
        for (int i = 0; i < count; i++) {
            arrStud[i] = new Graduates;
            arrStud[i] -> name = group.arrStud[i]->name;
            arrStud[i] -> id = group.arrStud[i] -> id;
            arrStud[i] -> score = group.arrStud[i] -> score; 
            arrStud[i] -> diplom = group.arrStud[i] -> diplom;
            arrStud[i] -> scientist = group.arrStud[i] -> scientist;    
        }
    }

    GradGroup (GradGroup&& group) {
        for (int i = 0; i < count; i++) {
            delete arrStud[i];
        }
        count = group.count;
        for (int i = 0; i < count; i++) {
            arrStud[i] =  group.arrStud[i];
            group.arrStud[i] = nullptr;
        }
        group.count = 0;
        group.arrStud = nullptr;
    }

    ~GradGroup() {
        for (int i = 0; i < count; i++) {
            delete arrStud[i];
        }
        delete[] arrStud;
    }

    GradGroup& operator = (const GradGroup& group) {
        if (this == &group)
            return *this;
        for (int i = 0; i < count; i++) {
            delete arrStud[i];
        }
        count = group.count;
        for (int i = 0; i < count; i++) {
            arrStud[i] = new Graduates(group.arrStud[i] -> name, group.arrStud[i] -> id, group.arrStud[i] -> score,group.arrStud[i] -> diplom,group.arrStud[i] -> scientist);
        }
        return *this;
    }

    GradGroup& operator = (GradGroup&& rather) {
		std::swap(arrStud, rather.arrStud);
		std::swap(count, rather.count);
		return *this;
	}

    /*int getcount() const {
        return count;
    }*/

    friend std::ostream& operator<<(std::ostream& os, const GradGroup& group) {
        os << "Students in the graduation group:\n";
        for (int i = 0; i < group.count; i++) {
            os << "Student's name: " << group.arrStud[i] -> name << ", ID: " << group.arrStud[i] -> id << std::endl;
        }
        return os;
    }

    const char* operator [] (int _id) const override {
        for (int i = 0; i < count; i++) {
            if (arrStud[i] -> id == _id) {
                return arrStud[i] -> name;
            }
            //throw "There is no student with this ID.";
        }
        throw "There is no student with this ID.";
    }

    void addStudent() {
        if (count < MaxStud) {
            char* name = new char[M];
            char* diplom = new char[M];
            char* scientist = new char[M];
            double score;
            int id;
            std::cout<<std::endl;
            std::cout << "Student's name: ";
            std::cin >> name;
            std::cout << "ID: ";
            std::cin >> id;
            std::cout << "Score: ";
            std::cin >> score;
            std::cout << "Diplom: ";
            std::cin >> diplom;
            std::cout << "Scientific supervisor:  ";
            std::cin >> scientist;
            arrStud[count++] = new Graduates(name, id, score, diplom, scientist);
        } else {
            std::cout << "This Group is full" << std::endl;
        }
        std::cout<<std::endl;
    }

    void kickStudent(int _id) override {
        for (int i = 0; i < count; i++) {
            if (arrStud[i] -> id == _id) {
                delete arrStud[i];
                int j;
                for (j = i; j < count - 1; j++) {
                    arrStud[j] = arrStud[j + 1];
                }
                arrStud[j] = nullptr;
                count--;
                return;
            }
        }
        std::cout << "There is no student with this ID: " << _id  << std::endl;
    }

    void outputStudent() const override {
        std::cout << *this;
    }
};


