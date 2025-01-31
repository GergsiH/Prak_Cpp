#include <iostream>
#define M 50



class Set {
    friend std::ostream& operator << (std::ostream & os, const Set & set1);
    friend std::istream& operator >> (std::istream & in, Set & set1);
    //friend void Sort_Repeat(Set & set);
    //friend void write (Set & set);
    //friend void print (Set & set);
    //friend void sort(int set[], int l);

    //friend const Set & operator | (Set & set1, Set & set2);
    //friend const Set & operator & (Set & set1, Set & set2);
    //friend const Set & operator - (Set & set1, Set & set2);
    //friend const Set & operator - (Set & set);

    int *elems;
    int len;

    static void sort(int set[], int l){
            int n,m,b;
            for (n = 0; n < l; n++){
                for (m = (n+1); m < l; m++){
                    if (set[m]<set[n]){
                    b = set[m];
                    set[m]=set[n];
                    set[n]=b;
                }
            }
        }
        return;
    }

    public:
        Set (int lenght){ len = lenght;};
        //Set(){};
        Set(){elems = new int[M];};
        //Set(int lenght){ len = lenght;};
        Set( Set & set_help, int l){
            for ( int i = 0; i < l; i++){
                elems[i] = set_help.elems[i];
            }
        }

        /*void sort(int set[], int l){
            int n,m,b;
            for (n = 0; n < l; n++){
                for (m = (n+1); m < l; m++){
                    if (set[m]<set[n]){
                    b = set[m];
                    set[m]=set[n];
                    set[n]=b;
                }
            }
        }
        return;
        }*/

        /*void Repeat (){
            int j = 0;
            for ( int i = 0; i < len; i++){
                if(elems[i] != elems[i+1]){
                    elems[j] = elems[i+1];
                    j++; 
                }
            }

        }*/

        void AND ( Set & set_1, Set & set_2){
            sort(set_1.elems, set_1.len);
            sort(set_2.elems, set_2.len);

            //normal(set_1);
            //normal(set_2);
            int l1, l2, n = 0;

            if (set_1.len > set_2.len){
                l1 = set_1.len;
                l2 = set_2.len;
            }
            else{
                l2 = set_1.len;
                l1 = set_2.len;
            }
            
            for ( int i = 0; i < l1; i++){
                for ( int j = 0; j < l2; j++){
                    if (set_1.elems[i] == set_2.elems[j]){
                        elems[n] = set_1.elems[i];
                        n++;
                    }
                }
            }
            //normal(elems, len);
            len = n;
            sort(elems,len);
            return;
        }

        void OR ( Set & set_1, Set & set_2){
            int n = 0;
            for (int i = 0; i < set_1.len; i++){
                elems[n] = set_1.elems[i];
                n++;
            }
            for (int i = 0; i < set_2.len; i++){
                 elems[n] = set_2.elems[i];
                 n++;
            }
            len = n;
            sort(elems, len);

            return;
        }

        void DIFF ( Set & set_1, Set & set_2){
            //sort(set_1.elems, set_1.len);
            //sort(set_2.elems, set_2.len);
            int n = 0;
            bool t;
            for(int i = 0; i < set_1.len; i++){
                t = true;
                for (int j = 0; j < set_2.len; j++){
                    if( set_1.elems[i] == set_2.elems[j]){
                        t = false;
                    }
                }
                if (t){
                    elems[n] = set_1.elems[i];
                    n++;
                }
                
            }
            len = n;
            return;
        }

        bool FoundElem(Set & set, int el){
            bool flag = false;
            int left = 0;
            int right = set.len - 1;
            int mid;

            while ((left <= right) && (flag != true)){
                mid = (left + right) / 2;
                if (set.elems[mid] == el) {
                    flag = true;
                }
                if ( set.elems[mid] > el){
                    right = mid - 1;
                }
                else {
                    left = mid + 1;
                }
            }
            return flag;
        }

        void ADD (int elem){

            sort(elems,len);
            //std::cout<<"What element are we adding? : ";
        
            bool t = FoundElem(*this, elem);

            if( t ){
                std::cout<<"This element is already in the set";
            }
            else{
                elems[len] = elem;
                len++;
            }
            sort(elems,len);
            return;
           
        }

        void KICK(int elem){
            int n = 0;
            sort(elems,len);
    
    

            bool t = FoundElem(*this,elem);

            if( t ){
                for ( int i = 0; i < len; i++){
                    if( elems[n] != elem){
                        elems[i] = elems[i];
                    }
                    else{
                        elems[i] = elems[n+1];
                        n++;
                    }
                    n++;
                }
                len = n - 2;
            }
            else{
                std::cout<<"There is no such element in the set";
            }
            return;


        }
    /*Set operator & (const Set & set2){
        int lenght = len+set2.len;

        
        return set3;
    }*/
        
        ~Set(){delete [] elems;}
};

/*void write (Set & set){
    int n = 0;
    int elem;
    std::cin >> elem;
    while ( elem != -1){
        if (n > M)
        {
            Set* set1 = new Set[M+10];
            for (int i = 0; i < M; i++)
            {
                set1[i] = set.elems[i];
            }
            delete [] set.elems;
           // set.elems = set1;
        }
        set.elems[n] = elem;
        n++;
        std::cin >> elem;
    }
    set.len = n;
    return;
}*/

/*void print (Set & set){
    int l = set.len;
    if ( l != 0){
        std::cout<<"[ ";
        for ( int i = 0; i < l; i++ ){
            std::cout<<set.elems[i]<<' ';
        }
        std::cout<<']';
    }
    else{
        std::cout<<"Zero elem"<<std::endl;
    }

    std::cout << std::endl;
    return;
}*/


/*const Set & operator + (Set & set_1, Set & set_2){

    return Set(set_1)  = set_2;
}*/

std::ostream& operator << (std::ostream & os, const Set & set1)
{
    //os<<'Res: ';
    for ( int j = 0;  j < set1.len; j++){
        os<<set1.elems[j]<<' ';
    }
    os << std::endl;
    return os;
}

std::istream& operator >> (std::istream & in, Set & set1)
{
        set1.len = 0;
        int elem;
        in >> elem;
        while (elem != -1){
            set1.elems[set1.len] = elem;
            set1.len++;
            in >> elem;
        }
        return in;
        
}

/*void Sort_Repeat(Set & set){
    int i = set.len, n = 0;
    while (  i != 1 ){
        if(set.elems[i-1] == set.elems[i-2]){
            set.elems[n] = set.elems[i-2];
            n++;
        }
        n++;
        i--;
    }
    set.len = n;
}*/

int main(){
    int action,elem;
   
    Set Set1;
    Set Set2;
    Set Set3;

    while(1){
        std::cout<<"Finish the set -1\n";
        std::cout<<" 0 - Exit\n 1 - AND\n 2 - OR\n 3 - DIFF\n 4 - ADD\n 5 - KICK\n";
        std::cin>>action;
        switch (action){
            case 0:
                return 0;
            case 1:
                std::cout<<"First Set: ";
                std::cin>>Set1;
                std::cout<<"\nSecond Set: ";
                std::cin>>Set2;

                Set3.AND(Set1, Set2);
                //Sort_Repeat(Set3);

                std::cout<<"Res: "<<Set3;
                break; 
            case 2:
                std::cout<<"First Set: ";
                std::cin>>Set1;
                std::cout<<"\nSecond Set: ";
                std::cin>>Set2;

                Set3.OR(Set1, Set2);
                //Sort_Repeat(Set3);

                std::cout<<"Res: "<<Set3;
                break; 
            case 3:
                std::cout<<"What are we subtracting from? ";
                std::cin>>Set1;
                std::cout<<"\nWhat are we subtracting? ";
                std::cin>>Set2;

                Set3.DIFF(Set1, Set2);
                //Sort_Repeat(Set3);

                std::cout<<"Res: "<<Set3;
                break; 
            case 4:
                std::cout<<"Set: ";
                std::cin>>Set1;
                
                std::cout<<"Which element are we add from the set? ";
                std::cin>>elem;
                Set1.ADD(elem);
                //Sort_Repeat(Set1);

                std::cout<<"Res: "<<Set1;
                break;
            case 5:
                std::cout<<"Set: ";
                std::cin>>Set1;

                std::cout<<"Which element are we removing from the set? ";
                std::cin>>elem;
                Set1.KICK(elem);
                //Sort_Repeat(Set3);

                std::cout<<"Res: "<<Set1;
                break;
            default:
                std::cout<<"Command not found" << std::endl;
                break;
        }
    }

   // write(Set1);
   // print (Set1);
    return 0;
}