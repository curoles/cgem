/**
 *
 * Resources:
 *  - http://www.drdobbs.com/cpp/mixin-based-programming-in-c/184404445
 *  - http://cgi.di.uoa.gr/~smaragd/practical-fmtd.pdf
 */
#include "cgem/mixin.h"

#include <iostream>
using namespace std;

mixin_class(A)
{
protected:
    int m_data;

    void call() { cout << "A calls " << Super::name() << " data=" << m_data << "\n"; }

public:
    void i_am_A(){ cout << "I am A\n"; }
};

mixin_class(B)
{
protected:
    double m_data;

    void call() { cout << "B calls " << Super::name() << " data=" << m_data << "\n"; }

public:
    void i_am_B(){cout << "I am B\n"; }
};

struct Super
{
    string m_name;
    Super(const string& name="super"):m_name(name){}
    const string& name()const{return m_name;}
};

template<class Super>
class Mixer : public A<Super>, public B<Super>
{
    string m_name;
public:
    Mixer(const string& name="super-duper"):m_name(name){}

    void run(){
        A<Super>::m_data = 7;
        B<Super>::m_data = 7.7;
        A<Super>::call();
        B<Super>::call();
    }

    string name(){return m_name;}
};

int main()
{
    Mixer<Super> test;
    test.run();
    test.i_am_A();
    test.i_am_B();
    cout << "my name is " << test.name() << "\n";

    cout << "mixin: TEST PASSED\n";

    return 0;
}

