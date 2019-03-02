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
const std::string cmdHelp("help");
const std::string terminator("\r");
const unsigned timeout = 15000; //ms

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
    std::cout << "Attempting to open serial port " << portName << std::endl;
    if(port->open(portName)) {
        std::cout << "Unable to open serial port "
                  << portName << std::endl;
        return -3;
    }

    // Queue up ^c
    ayif::IWriteBufferPtr writebuf(new ayif::UniqueWriteBuffer("\x03"));
    if(!writebuf) {
        std::cout << "Failed to construct the write buffer!" << std::endl;
    }
    std::string writeBufferContents(reinterpret_cast<const char *>(
                                        writebuf->contents()),
                                    writebuf->length());
    std::cout << "Write buffer contents:\n"
              << writeBufferContents << std::endl;
    // Enter communication loop:
    std::cout << "Entering write-read loop." << std::endl;
    try {
    while(true) {
		bool bReadTimedOut = false;
		bool bWriteTimedOut = false;
		bool bWriteSuccess = false;
		bool bReadSuccess = false;
        //     Set timeout timer
        std::cout << "Setting write timeout timer." << std::endl;
        ayif::TimerHandler onWriteTimeout = [&](ayif::TimerRetCode ret) {
            std::cout << "Entered write timeout handler." << std::endl;
            if(!ret && !bWriteSuccess) {
                bWriteTimedOut = true;
            }
        };

        ayif::ITimerPtr writeTimeout =
            provider.setTimer(timeout, onWriteTimeout);
        writeTimeout->start();
        //     Begin async write
        std::cout << "Starting asynchronous write." << std::endl;
        try {
			port->writeAsync(writebuf, [&](ayif::IWriteBufferPtr &buf) {
				std::cout << "Entered write handler." << std::endl;
				writeTimeout->cancel();
				bWriteSuccess = !buf->error();
			}
			);
        } catch(std::exception &e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
		}
        //     Dispatch a round of events
        std::cout << "Dispatching events." << std::endl;
        int dispRet = provider.dispatchEvents(&listener, false);
        std::cout << "provider.dispatchEvents returned " << dispRet << std::endl;
        //     if(write successful)
        if(!bWriteTimedOut && bWriteSuccess) {
            //     Set timeout timer
            std::cout << "Setting read timeout timer." << std::endl;
            ayif::TimerHandler onReadTimeout = [&](ayif::TimerRetCode ret) {
                std::cout << "Entered read timeout handler." << std::endl;
                if(!ret && !bReadSuccess) {
                    bReadTimedOut = true;
                }
            };
            ayif::ITimerPtr readTimeout =
                provider.setTimer(timeout, onReadTimeout);
            readTimeout->start();
            //         Begin async read
            ayif::IReadBufferPtr readbuf(new ayif::UniqueReadBuffer(1024));
            std::cout << "Starting asynchronous read." << std::endl;
            int readRet = port->readAsync(readbuf, [&](ayif::IReadBufferPtr &pBuf) {
                std::cout << "Entered read handler." << std::endl;
                readTimeout->cancel();
                bReadSuccess = !pBuf->error();
				if(bReadSuccess) {
					std::cout << "Reported successful read." << std::endl;
                    pBuf.swap(readbuf);
				}
            });
            if(readRet) {
                std::cout << "Failed to initiate asynchronous "
                             "read operation!"
                          << std::endl;
                return -8;
            }
            //         Dispatch a round of events
            std::cout << "Dispatching events." << std::endl;
            dispRet = provider.dispatchEvents(&listener, false);
			std::cout << "provider.dispatchEvents returned " << dispRet << std::endl;
            //         if(read successful)
            if(!bReadTimedOut && bReadSuccess) {
                //             display result
                std::cout << "Received: "
                          << std::string(reinterpret_cast<const char *>(
                                             readbuf->contents()),
                                         readbuf->bytesRead())
                          << std::endl;
                // prompt user for input
                bool again = true;
                bool exit = false;
                while(again) {
                    again = false;
					std::string input;
					std::cout << "> ";
					std::getline(std::cin, input);
					//             if(input == quit command) exit
					if(!cmdQuit.compare(input)) {
						std::cout << "Terminating session per user's request." << std::endl;
                        exit = true;
					} else if(!cmdHelp.compare(input)) {
						std::cout
							<< "Enter the command you wish to send "
							   "and press [Return/Enter].\n"
							<< "Enter 'quit' to exit." << std::endl;
                        again = true;
					} else {
						// queue up input for next iteration
						writebuf = ayif::IWriteBufferPtr(
							new ayif::UniqueWriteBuffer(input + terminator));
					}
				}
                if(exit) {
					break; // finished!
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
    } catch(...) {
        std::cout << "Unhandled exception!" << std::endl;
        return -7;
	}
    return 0;
}