#ifndef B15F_H
#define B15F_H

#include <stdint.h>

class B15F {
public:
    static B15F& getInstance();
    
    virtual ~B15F() = default;
    
    virtual void setRegister(uint8_t volatile* adr, uint8_t val) = 0;

    virtual uint8_t getRegister(uint8_t volatile* adr) = 0;
    
protected:
    B15F() = default;
    
private:
    static B15F* inst;
};

thread_local extern uint8_t volatile DDRA;
extern uint8_t volatile PORTA;
extern uint8_t volatile PINA;

#endif // B15F_H
