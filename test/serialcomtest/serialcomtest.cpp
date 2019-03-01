#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include "../../IOListener.h"
#include "../../IOProvider.h"
#include "../../SerialPort.h"
#include "../../UniqueReadBuffer.h"
#include "../../UniqueWriteBuffer.h"

namespace ayif = ayisakov::framework;

const std::string argDev("--device");
const std::string argBaud("--baud-rate");
const std::string cmdQuit("quit");
const std::string terminator("\r");

int parseArgs(int argc, char **argv, std::string &name, int &baud)
{
    if(argc < 5) {
        std::cout << "Usage:\nserialcomtest --device <device> "
                     "--baud-rate <rate>"
                  << std::endl;
        return -1;
    }

    for(int i = 1; i < argc; i++) {
        char *arg = argv[i];
        if(!argDev.compare(arg)) {
            name = argv[++i];
            continue;
        }
        if(!argBaud.compare(arg)) {
            std::istringstream(argv[++i]) >> baud;
            continue;
        }
        std::cout << "Unknown argument " << arg << " provided" << std::endl;
        return -6;
    }
    return 0;
}

int main(int argc, char **argv, char *envp[])
{
    // Get port name, baud rate from arguments
    std::string portName;
    int baudRate;
    if(parseArgs(argc, argv, portName, baudRate)) {
        std::cout << "Could not parse arguments!" << std::endl;
        return -1;
    }
    // Instantiate IOProvider
    ayif::IOProvider provider;

    // Instantiate IOListener and subscribe
    ayif::IOListener listener;
    listener.subscribe(&provider);
    // Obtain a serial port
    ayif::ISerialPort *port = provider.getSerialPort();
    if(!port) {
        std::cout << "Unable to obtain serial port!" << std::endl;
        return -2;
    }
    // Open the port
    if(port->open(portName)) {
        std::cout << "Unable to open serial port "
                  << portName << std::endl;
        return -3;
    }

    // Queue up ^c
    ayif::IWriteBufferPtr writebuf(new ayif::UniqueWriteBuffer("\x03"));
    // Enter communication loop:
    bool bTimedOut = false;
    bool bWriteSuccess = false;
    bool bReadSuccess = false;
    while(true) {
        //     Set timeout timer
        ayif::TimerHandler onWriteTimeout = [&](ayif::TimerRetCode ret) {
            if(!bWriteSuccess) {
                bTimedOut = true;
            }
        };

        ayif::ITimerPtr writeTimeout =
            provider.setTimer(5000, onWriteTimeout);
        writeTimeout->start();
        //     Begin async write
        port->writeAsync(writebuf, [&](ayif::IWriteBufferPtr &buf) {
            writeTimeout->cancel();
            bWriteSuccess = !buf->error();
        });
        //     Dispatch a round of events
        provider.dispatchEvents(&listener, false);
        //     if(write successful)
        if(!bTimedOut && bWriteSuccess) {
            //     Set timeout timer
            ayif::TimerHandler onReadTimeout = [&](ayif::TimerRetCode ret) {
                if(!bReadSuccess) {
                    bTimedOut = true;
                }
            };
            ayif::ITimerPtr readTimeout =
                provider.setTimer(5000, onReadTimeout);
            readTimeout->start();
            //         Begin async read
            ayif::IReadBufferPtr readbuf(new ayif::UniqueReadBuffer(1024));
            port->readAsync(readbuf, [&](ayif::IReadBufferPtr &pBuf) {
                readTimeout->cancel();
                bReadSuccess = !pBuf->error();
            });
            //         Dispatch a round of events
            provider.dispatchEvents(&listener, false);
            //         if(read successful)
            if(!bTimedOut && bReadSuccess) {
                //             display result
                std::cout << "Received: "
                          << std::string(reinterpret_cast<const char *>(
                                             readbuf->contents()),
                                         readbuf->bytesRead())
                          << std::endl;
                // prompt user for input
                std::string input;
                std::cout << "> ";
                std::cin >> input;
                //             if(input == quit command) exit
                if(!cmdQuit.compare(input)) {
                    break; // finished!
                } else {
                    // queue up input for next iteration
                    writebuf = ayif::IWriteBufferPtr(
                        new ayif::UniqueWriteBuffer(input + terminator));
                }
            } else {
                std::cout << "Read timeout!" << std::endl;
                return -5;
            }
        } else {
            std::cout << "Write timeout!" << std::endl;
            return -4;
        }
    }
    return 0;
}
