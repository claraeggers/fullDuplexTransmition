#include <b15f/b15f.h>

#include <thread>
#include <chrono>
#include <mutex>
#include <unordered_map>

namespace {

class B15FImpl: public B15F {
    std::unique_lock<std::mutex> guardDriver() {
        std::unique_lock lock(mtx);
        while (lastAccessID == std::this_thread::get_id()) {
            lock.unlock();
            std::this_thread::yield();
            lock.lock();
        }
        lastAccessID = std::this_thread::get_id();
        return lock;
    }
    
    void setRegister(uint8_t volatile* adr, uint8_t val) override {
        if (adr == &DDRA) {
            DDRA = val;
            return;
        }
        std::unique_lock lock = guardDriver();
        if (adr == &PORTA) {
            if (rand() % noise == 0) {
                val ^= rand() % 0x100;
            }
            PINA = val & DDRA;
        }
        else if (adr == &PINA) {
            assert(false);
        }
        else {
            assert(false);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
    
    uint8_t getRegister(uint8_t volatile* adr) override {
        std::unique_lock lock = guardDriver();
        if (adr == &DDRA) {
            assert(false);
        }
        else if (adr == &PORTA) {
            assert(false);
        }
        else if (adr == &PINA) {
            return PINA;
        }
        else {
            assert(false);
        }
    }
    
    std::thread::id lastAccessID;
    std::mutex mtx;
    int noise = 10;
};

} // namespace

B15F& B15F::getInstance() { return *inst; }

B15F* B15F::inst = new B15FImpl();

thread_local extern uint8_t volatile DDRA = 0;
uint8_t volatile PORTA = 0;
uint8_t volatile PINA = 0;
