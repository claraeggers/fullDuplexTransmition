#include <iostream>

#include <b15f/b15f.h>

#include "HelperFunctions.h"
#include "Sender.h"
#include "Receiver.h"

enum class Mode {
	send, receive, fullDuplex
};

static void send(B15F& drv, Device device, std::string message) {
    Sender sender(drv, device, message);
    for (;;) {
        sender.send();
    }
}

static void receive(B15F& drv, Device device) {
    Receiver receiver(drv, device);
    for (;;) {
        receiver.receive();
    }
}

/// Fullduplex
static void fullDuplex(B15F& drv, Device device, std::string message) {
	Sender sender(drv, device, message);
	Receiver receiver(drv, device);
	for (;;) {
		sender.send();
		receiver.receive();	
	}
}

static void printUsage(std::string arg1, std::string arg2) {
    std::cout << "Bitte geben sie als Argument 1 --send, --receive oder --full-duplex und als Argument 2 --deviceA oder --deviceB an\n";
    if (arg1.empty()) { arg1 = "<leer>"; }
    if (arg2.empty()) { arg2 = "<leer>"; }
    std::cout << "Argumente sind: " << arg1 << " und " << arg2 << std::endl;
}

static std::pair<Mode, Device> parseArguments(int argc, char* argv[]) {
    const std::string modeArg(argv[1]);
    const std::string deviceArg(argv[2]);
    Mode mode;
    if (modeArg == "--send") {
        mode = Mode::send;
    }
    else if (modeArg == "--receive") {
        mode = Mode::receive;
    }
    else if (modeArg == "--full-duplex") {
        mode = Mode::fullDuplex;
    }
    else {
        printUsage(modeArg, deviceArg);
        std::exit(EXIT_FAILURE);
    }
    Device device;
    if (deviceArg == "--deviceA") {
        device = Device::A;
    }
    else if (deviceArg == "--deviceB") {
        device = Device::B;
    }
    else {
        printUsage(modeArg, deviceArg);
        std::exit(EXIT_FAILURE);
    }
    return { mode, device };
}

static B15F& loadDriver() {
    try {
        return B15F::getInstance();
    }
    catch (...) {
        std::cout << "Hardware Fehler" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#ifdef TEST_MAIN

#include <future>

static void deviceA(B15F& drv, bool fullDuplex) {
    if (fullDuplex) {
        ::fullDuplex(drv, Device::A, "Hello from device A");
    }
    else {
        send(drv, Device::A, "Hello!");
    }
}

static void deviceB(B15F& drv, bool fullDuplex) {
    if (fullDuplex) {
        ::fullDuplex(drv, Device::B, "Hello from device B");
    }
    else {
        receive(drv, Device::B);
    }
}

int main() {
    bool const fullDuplex = false;
    B15F& drv = loadDriver();
    auto aft = std::async(deviceA, std::ref(drv), fullDuplex);
    auto bft = std::async(deviceB, std::ref(drv), fullDuplex);
}

#else

/// Das ist dann die eigentliche main Funktion die wir mit dem Board ausführen.
int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage({}, {});
        return EXIT_FAILURE;
    }
    auto const [mode, device] = parseArguments(argc, argv);
    B15F& drv = loadDriver();
	switch (mode) {
    case Mode::send: {
        std::string message = readUserInput();
        send(drv, device, message);
        break;
    }
    case Mode::receive: {
        receive(drv, device);
        break;
    }
    case Mode::fullDuplex: {
        std::string message = readUserInput();
        fullDuplex(drv, device, message);
        break;
    }
	}
	return 0;	
}

#endif
