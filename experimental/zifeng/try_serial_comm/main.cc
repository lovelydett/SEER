//
// Created by tt on 2/24/23.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <exception>
#include <cassert>

#include "serial/serial.h"

int main() {
//    try{
//        serial::Serial mother_board("/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));
//    } catch (std::exception &e) {
//        std::cout << e.what();
//    }

    serial::Serial mother_board("/ttyUSB0", 9600, serial::Timeout::simpleTimeout(1000));


    if (mother_board.isOpen())
    {
        std::cout << "Serial port opened successfully!" << std::endl;
    }
    else
    {
        std::cout << "Failed to open serial port." << std::endl;
        return  1;
    }

    std::string message = "Hello, this is a message.";
    auto control = static_cast<uint8_t *>(malloc(1024));
    int data_len = 12;
    *(uint8_t*)(control) = 0x5A;
    *(uint8_t*)(control + 1) = data_len;
    *(uint8_t*)(control + 2) = 1;
    *(uint8_t*)(control + 3) = 1;
    *(short*)(control + 4) = 0x0064;
    *(short*)(control + 6) = 0x0000;
    *(short*)(control + 8) = 0x0000;
    *(uint8_t*)(control + 10) = 0x00;
    *(uint8_t*)(control + 11) = 0xFF;

    // 345(7) = 3 * 7 * 7 + 4 * 7 + 5 * 1
    // 1F4(16) = 1 * 16 * 16 + 15 * 16 + 4 * 1
    // 01000001(2) = 64 + 1

    int total_len = 1 + 1 + 1 + 1 + data_len + 1 + 1;

    auto ret = mother_board.write((uint8_t*)control, total_len);
    assert(ret == total_len);

    std::string response = mother_board.readline();
    std::cout << "Hello, this is a response: " << response << std::endl;

//    uint8_t data[8] = "Hi baby";
//    mother_board.write(data);

    uint8_t received_data[8];
    mother_board.read(reinterpret_cast<uint8_t*>(received_data), sizeof(received_data));
    std::cout << "Received data: ";
    for (int i = 0; i < 8; i++)
    {
        std::cout << received_data[i] << " ";
    }
    std:: cout << std::endl;

    mother_board.close();
    return 0;
//    serial::Serial Construct data packet
//
//    uint8_t data[1000] = "Hello, this is new data.";
//    mother_board.write(data);
//    printf("hhh");
}