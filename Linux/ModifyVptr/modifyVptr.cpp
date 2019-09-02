#include <iostream>


using namespace std;


class A
{
public:
    A():m_a(0){}
    virtual void print()
    {
      cout << "A"<<endl;
    }
private:
  int m_a;    
};

class B
{
  public:
    B():m_b(0){}
    virtual void print()
    {
      cout << "B "<<endl;
    }
private:
    int m_b;
};

int main()
{
  A* pa = new  A;
  pa->print();
  cout << sizeof(pa)<<endl;
  int * vptr = reinterpret_cast<int*>(pa);
  cout << hex << vptr <<endl;
  B* pb  =static_cast<B*>(static_cast<void*>(pa));
      pb->print();

  cout <<hex << sizeof(pb)<<endl;

  B* pbb = new B;
  pbb ->print();
  *vptr = * reinterpret_cast<int*>(pbb);
  pbb = static_cast<B*>(static_cast<void*>(pa));
  pbb ->print();
  pa->print();
  return 0;
}
