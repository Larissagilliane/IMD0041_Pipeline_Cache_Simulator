 #include "mips.h"
 #include <iostream>
 #include <cassert>

 using namespace std;

 int main (int argc, char * argv[]) {

     int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

     cout << "Testando add..." << endl;
     assert(add(2,3) == 5);
     assert(add(11,11) == 22);
     assert(add(0,3) == 3);

     cout << "Testando sub..." << endl;
     assert(sub(10,4) == 6);
     assert(sub(11,11) == 0);
     assert(sub(0,3) == -3);

     cout << "Testando slt..." << endl;
     assert(slt(2,3) == 1);
     assert(slt(4,3) == 0);
     assert(slt(3,3) == 0);

     cout << "Testando e..." << endl;
     assert(e(92,101) == 68);

     cout << "Testando ou..." << endl;
     assert(ou(92,101) == 125);

     cout << "Testando zeros..." << endl;
     assert(zeros(v) == 0);

     cout << "Testando ones..." << endl;
     assert(ones(v) == 1);

     cout << "Testando min..." << endl;
     assert(min(v) == 1);

     cout << "Testando max..." << endl;
     assert(max(v) == 9);

     return 0;

}
