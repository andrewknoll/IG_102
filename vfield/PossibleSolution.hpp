#ifndef __POSSIBLESOLUTION__
#define __POSSIBLESOLUTION__

template <typename T>
struct PossibleSolution{
    private:
        bool exists = false;
        T solution;

    public:
        void setSolution(T s){
            exists = true;
            solution = s;
        }
        T getSolution(){
            return solution;
        }
        bool doesExist(){
            return exists;
        }
        void setNotReal(){
            exists = false;
        }
};

#endif