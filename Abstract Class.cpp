#include <iostream>
using namespace std;

class Abstract
{
    public:
    
    // Pure Virtual Function
    virtual void print()=0;
    
    // Implement Methods
    
    void save()
    {
        //-----
    }
    
    
};

class Child:public Abstract
{
    public:
    
    // Inherting virtual 
    
    void save();
    
    
};

int main() {
    
    // Throw error
	Base *b=new Base();
	
	//throw error as child not inherted pure virtual
	Child *c =new Child();
	
}
