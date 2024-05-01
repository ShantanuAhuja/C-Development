#include <iostream>
using namespace std;

class Interface
{
    public:
    
    // Pure Virtual Function
    virtual void print()=0;
    

    
    
};

class Child:public Interface
{
    public:
    
    // Inherting virtual 
    
    void save();
    
    
};

int main() {
    
    // Throw error
	Interface *b=new Interface();
	
	//throw error as child not inherted pure virtual
	Child *c =new Child();
	
}
