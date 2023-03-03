//
// Created by tt on 2/24/23.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <exception>
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
//
//    std::string message = "Hello, this is a message.";
//    mother_board.write(message);
//
//    std::string response = mother_board.readline();
//    std::cout << "Hello, this is a response: " << response << std::endl;
//
//    char data[8] = "Hi baby";
//    mother_board.write(data);
//
//    char received_data[8];
//    mother_board.read(reinterpret_cast<uint8_t*>(received_data), sizeof(received_data));
//    std::cout << "Received data: ";
//    for (int i = 0; i < 8; i++)
//    {
//        std::cout << received_data[i] << " ";
//    }
//    std:: cout << std::endl;
//
//    mother_board.close();
//    return 0;


    // Construct data packet

    //
    //char data[1000] = "Hello, this is new data.";
    //mother_board.write(data);
    printf("hhh");
}