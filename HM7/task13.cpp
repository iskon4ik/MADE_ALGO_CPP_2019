/*
Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
 [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку.
 Достаточно найти хотя бы какое-то решение. Число перемещений костяшек не обязано быть минимальным.

Ввод
1 2 3 0 5 6 7 4 9 10 11 8 13 14 15 12
Вывод
3

Ввод
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0
Вывод
0

14 5 4 8 2 12 9 3 0 10 13 5 7 11 15 1
*/

#include <iostream>
#include <vector>
#include <set>

class State {
private:
    std::vector<int> nums;
    int G;
    int H;
    int F;
    State * parent;
    char move;

    void SetG(int g);
    void SetH(int h);
//    void SetF();

public:

    State();
    explicit State(std::vector<int> a);

    int GetG();
    int GetH();
    int GetF();
    char GetMove();
    State * GetParent();

    std::vector<int> GetNums();


    void SetParent(State * p);
    void SetMove(char m);

    int GetAmountOfWrongPlaced();
};

int State::GetAmountOfWrongPlaced() {
    int n_wrong = 0;
    for(int i=0; i<nums.size(); ++i) {
        if(nums[i]) {
            if(nums[i] != i+1) {
                n_wrong++;
            }
        }
    }
    return n_wrong;
}

void State::SetG(int g) {
    G = g;
    F = G + H;
}

void State::SetH(int h) {
    H = h;
    F = G + H;
}

State::State(): nums(std::vector<int>(16, 0)), G(0), H(0), F(0), parent(nullptr), move('n') {}

State::State(std::vector<int> a) {
    for(int a_i: a) {
        nums.push_back(a_i);
    }
    SetG(0);
    SetH(GetAmountOfWrongPlaced());
    parent = nullptr;
    move = 'n';
}

void State::SetParent(State * p) {
    parent = p;
    SetG(parent->G + 1);
}

void State::SetMove(char m) {
    move = m;
}

State* State::GetParent() {
    return parent;
}

int State::GetG() {
    return G;
}

int State::GetH() {
    return H;
}

int State::GetF() {
    return F;
}

char State::GetMove() {
    return move;
}

std::vector<int> State::GetNums() {
    return nums;
}

int AmountOfInversions(std::vector<int> a) {
    int inv = 0;
    for (int i=0; i<16; ++i){
        if (a[i]) {
            for (int j=0; j<i; ++j){
                if (a[j] > a[i]) {
                    ++inv;
                }
            }
        }
    }
    return inv;
}

int index_of_zero(std::vector<int> a) {
    for(int i=0;i<a.size();++i) {
        if(a[i] == 0) {
            return i;
        }
    }
    return -1;
}

void add_replace_or_leave(std::set<State*>& my_set, State* state) {
    bool flag = true;
    for(auto st: my_set){
        if(std::equal(st->GetNums().begin(), st->GetNums().end(), state->GetNums().begin())) {
            if(st->GetF() > state->GetF()) {
                st = state;
            } else {
                flag = false;
            }
            break;
        }
    }
    if(flag){
        my_set.insert(state);
    }
}

bool state_in_set(std::set<State*> my_set, State* state) {
    bool flag = true;
    for(State* st: my_set){
        if(std::equal(st->GetNums().begin(), st->GetNums().end(), state->GetNums().begin())) {
            if(st->GetF() > state->GetF()) {
                st = state;
            } else {
                flag = false;
            }
            break;
        }
    }
    return !flag;
}

int main(){

    std::vector<int> a(16, 0);

//    std::vector<int> a1(16, 0);
//
//    std::cout<<std::equal(a.begin(), a.end(), a1.begin())<<std::endl;

    for (int i=0; i<16; ++i){
        std::cin >> a[i];
    }

    int inv = AmountOfInversions(a);

    for (int i=0; i<16; ++i) {
        if (a[i] == 0){
            inv += 1 + i / 4;
        }
    }

    if(inv%2 !=0) {
        std::cout<<-1;
        return 0;
    }

    std::set<State*> closed_states;
    std::set<State*> open_states;

    State * begin = new State(a);
    closed_states.insert(begin);

    State * curr = begin;

    int min_F = 0;

    while(curr->GetH() != 0) {

        for(State * st: closed_states) {
            std::vector<int> closed_a = st->GetNums();
//            std::cout<<"clos ";
//            for(int a_i: closed_a) {
//                std::cout<<a_i<<' ';
//            }
//            std::cout<<std::endl;
            int z = index_of_zero(closed_a);

            bool up = true, down = true, left = true, right = true;

            if(z < 4) {
                up = false;
            }

            if(z > 11) {
                down = false;
            }

            if(z == 0 || z == 4 || z == 8 || z == 12) {
                left = false;
            }

            if(z == 3 || z == 7 || z == 11 || z == 15) {
                right = false;
            }

            if(up) {
//                std::vector<int> new_a = std::move(closed_a);
                std::swap(closed_a[z], closed_a[z-4]);
                State * new_state = new State(closed_a);
                new_state->SetMove('D');
                new_state->SetParent(st);
//                std::cout<<"up ";
//                for(int a_i: new_state->GetNums()) {
//                    std::cout<<a_i<<' ';
//                }
//                std::cout<<std::endl;
                add_replace_or_leave(open_states, new_state);
                std::swap(closed_a[z], closed_a[z-4]);

                if(new_state->GetH() == 0) {
                    break;
                }
            }

            if(down) {
//                std::vector<int> new_a = std::move(closed_a);
                std::swap(closed_a[z], closed_a[z+4]);
                State * new_state = new State(closed_a);
                new_state->SetMove('U');
                new_state->SetParent(st);
//                std::cout<<"down ";
//                for(int a_i: new_state->GetNums()) {
//                    std::cout<<a_i<<' ';
//                }
//                std::cout<<std::endl;
                add_replace_or_leave(open_states, new_state);
                std::swap(closed_a[z], closed_a[z+4]);

                if(new_state->GetH() == 0) {
                    break;
                }
            }

            if(left) {
//                std::vector<int> new_a = std::move(closed_a);
                std::swap(closed_a[z], closed_a[z-1]);
                State * new_state = new State(closed_a);
                new_state->SetMove('R');
                new_state->SetParent(st);
//                std::cout<<"left ";
//                for(int a_i: new_state->GetNums()) {
//                    std::cout<<a_i<<' ';
//                }
//                std::cout<<std::endl;
                add_replace_or_leave(open_states, new_state);
                std::swap(closed_a[z], closed_a[z-1]);

                if(new_state->GetH() == 0) {
                    break;
                }
            }

            if(right) {
//                std::vector<int> new_a = std::move(closed_a);
                std::swap(closed_a[z], closed_a[z+1]);
                State * new_state = new State(closed_a);
                new_state->SetMove('L');
                new_state->SetParent(st);
//                std::cout<<"right ";
//                for(int a_i: new_state->GetNums()) {
//                    std::cout<<a_i<<' ';
//                }
//                std::cout<<std::endl;
                add_replace_or_leave(open_states, new_state);
                std::swap(closed_a[z], closed_a[z+1]);

                if(new_state->GetH() == 0) {
                    break;
                }
            }
        }

        min_F = -1;
        State *min_st = nullptr;

//        std::cout<<std::endl;

        for(State * st: open_states) {
//            if(closed_states.find(st) == closed_states.end()) {
//            if(!state_in_set(closed_states, st)) {
//                std::cout<<"open ";
//                for(int a_i: st->GetNums()) {
//                    std::cout<<a_i<<' ';
//                }
//                std::cout<<" F"<<st->GetF()<<" H"<<st->GetH()<<" M:"<<st->GetMove()<<std::endl;
                if(min_F == -1 || min_F > st->GetF() || (min_F == st->GetF() && st->GetH() < min_st->GetH()) ) {
                    min_st = st;
                    min_F = min_st->GetF();
                }
        }


        curr = min_st;
        closed_states.insert(min_st);
        open_states.erase(min_st);

//        std::cout<<"curr ";
//        for(int a_i: curr->GetNums()) {
//            std::cout<<a_i<<' ';
//        }
//        std::cout<<" F"<<curr->GetF()<<" G"<<curr->GetG()<<" H"<<curr->GetH()<<" M:"<<curr->GetMove()<<std::endl;
//        std::cout<<std::endl;
//        std::cout<<"###############################################################################"<<std::endl;
    }


    std::cout << curr->GetG()<<std::endl;
    std::vector<char> ans;
    ans.push_back(curr->GetMove());
    while(curr->GetParent()->GetParent()) {
        curr = curr->GetParent();
        ans.push_back(curr->GetMove());
    }
    for(int i=ans.size()-1; i>=0; --i) {
        std::cout<<ans[i]<<' ';
    }

    return 0;
}