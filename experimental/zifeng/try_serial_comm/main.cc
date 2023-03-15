//
// Created by tt on 2/24/23.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <exception>
#include <cassert>

#include <serial/serial.h>

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

    //The upper computer(computer) sends speed control instructions to the lower computer(model car)
    auto control = static_cast<uint8_t *>(malloc(1024)); //create a control data arr
    int data_len = 12; //The length of frame 0x0C
    *(uint8_t*)(control) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(control + 1) = data_len; // The length of frame
    *(uint8_t*)(control + 2) = 0x01; // The ID number
    *(uint8_t*)(control + 3) = 0x01; // The function code
    *(short*)(control + 4) = 0x0064; // The X axis speed 1000(int16_t) for 0.1 m/s 16*6+4=100ms=0.1
    *(short*)(control + 6) = 0x0000; // The Y axis speed 1000(int16_t) for 0
    *(short*)(control + 8) = 0x0000; // The Z axis speed 1000(int16_t) for 0
    *(uint8_t*)(control + 10) = 0x00; // reserve space, 0
    *(uint8_t*)(control + 11) = 0xFF; // CRC checking, FF for no check
    int total_len = 1 + 1 + 1 + 1 + data_len + 1 + 1;
    try{
        auto ret = mother_board.write((uint8_t*)control, total_len);
        assert(ret == total_len);
        std::cout << "Speed set to " << control[4-8];
    }catch(const std::exception& e) {
        std::cerr << "Fail to set up the speed" << e.what() << std::endl;
    };

    //The upper computer sends a speed query command to the lower computer
    auto check_speed = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_speed) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_speed + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_speed + 2) = 0x01; // The ID number
    *(uint8_t*)(check_speed + 3) = 0x03; // The function code
    *(uint8_t*)(check_speed + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_speed + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_speed, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the speed" << e.what() << std::endl;
    };

    //The upper computer queries the IMU data from the lower computer
    auto check_IMU_data = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_IMU_data) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_IMU_data + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_IMU_data + 2) = 0x01; // The ID number
    *(uint8_t*)(check_IMU_data + 3) = 0x05; // The function code
    *(uint8_t*)(check_IMU_data + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_IMU_data + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_IMU_data, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the IMU data" << e.what() << std::endl;
    };

    //The upper computer queries the battery information from the lower computer
    auto check_battery_info = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_battery_info) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_battery_info + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_battery_info + 2) = 0x01; // The ID number
    *(uint8_t*)(check_battery_info + 3) = 0x07; // The function code
    *(uint8_t*)(check_battery_info + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_battery_info + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_battery_info, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the battery information" << e.what() << std::endl;
    };

    //The upper computer obtains the odometer information from the lower computer
    auto check_odm_info = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_odm_info) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_odm_info + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_odm_info + 2) = 0x01; // The ID number
    *(uint8_t*)(check_odm_info + 3) = 0x09; // The function code
    *(uint8_t*)(check_odm_info + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_odm_info + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_odm_info, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the odometer information" << e.what() << std::endl;
    };

    //The upper computer obtains the odometer information from the lower computer
    // but add the Y-axis speed, in order to meet the needs of omni-directional mobile chassis
    auto check_odm_info_addY = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_odm_info_addY) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_odm_info_addY + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_odm_info_addY + 2) = 0x01; // The ID number
    *(uint8_t*)(check_odm_info_addY + 3) = 0x11; // The function code
    *(uint8_t*)(check_odm_info_addY + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_odm_info_addY + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_odm_info_addY, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the odometer information" << e.what() << std::endl;
    };

    //The upper computer queries the original IMU data from the lower computer
    auto check_origin_IMU_data = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_origin_IMU_data) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_origin_IMU_data + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_origin_IMU_data + 2) = 0x01; // The ID number
    *(uint8_t*)(check_origin_IMU_data + 3) = 0x13; // The function code
    *(uint8_t*)(check_origin_IMU_data + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_origin_IMU_data + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_origin_IMU_data, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the original IMU data" << e.what() << std::endl;
    };

    //The upper computer(computer) sends speed control instructions(ackermann) to the lower computer(model car)
    auto akm_control = static_cast<uint8_t *>(malloc(1024)); //create a control data arr
    int akm_data_len = 12; //The length of frame 0x0C
    *(uint8_t*)(akm_control) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(akm_control + 1) = data_len; // The length of frame
    *(uint8_t*)(akm_control + 2) = 0x01; // The ID number
    *(uint8_t*)(akm_control + 3) = 0x15; // The function code
    *(short*)(akm_control + 4) = 0x00C8; // The X axis speed 1000(int16_t) for 0.2 m/s 16*12+8=200m/ms=0.2m/s
    *(short*)(akm_control + 6) = 0x0000; // The X-axis direction acceleration 1000 (int16_t) for 0
    *(short*)(akm_control + 8) = 0x00C8; // The Steering angle * 1000 (int16_t) (the angle is in radians) for 0.2rad
    *(uint8_t*)(akm_control + 10) = 0x00; // reserve space, 0
    *(uint8_t*)(akm_control + 11) = 0xFF; // CRC checking, FF for no check
    int akm_total_len = 1 + 1 + 1 + 1 + data_len + 1 + 1;
    try{
        auto ret = mother_board.write((uint8_t*)akm_control, akm_total_len);
        assert(ret == akm_total_len);
        std::cout << "Speed and the radians angle set to " << control[4-8];
    }catch(const std::exception& e) {
        std::cerr << "Fail to set up the speed and angle" << e.what() << std::endl;
    };

    //上位机向下位机获取ADC接口的ADC值，ADC采样范围为0~6.6V，ADC值和电压的换算关系为((电压/2)/3.3V)*4095
    auto check_ADC_data = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_ADC_data) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_ADC_data + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_ADC_data + 2) = 0x01; // The ID number
    *(uint8_t*)(check_ADC_data + 3) = 0x17; // The function code
    *(uint8_t*)(check_ADC_data + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_ADC_data + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_ADC_data, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the ADC data" << e.what() << std::endl;
    };

    //上位机向下位机获取超声波测距值，单位为cm
    auto check_sonic_data = static_cast<uint8_t *>(malloc(0));
    *(uint8_t*)(check_sonic_data) = 0x5A; // The Frame head--const 0x5A
    *(uint8_t*)(check_sonic_data + 1) = 0x06; // The length of frame
    *(uint8_t*)(check_sonic_data + 2) = 0x01; // The ID number
    *(uint8_t*)(check_sonic_data + 3) = 0x19; // The function code
    *(uint8_t*)(check_sonic_data + 4) = 0x00; // reserve space, 0
    *(uint8_t*)(check_sonic_data + 6) = 0xFF; // CRC checking, FF for no check
    try{
        auto order = mother_board.write((uint8_t*)check_sonic_data, 0);
        assert(order = 0);
    }catch(const std::exception& e) {
        std::cerr << "Fail to check the ADC data" << e.what() << std::endl;
    };




//    std::string response = mother_board.readline();
//    std::cout << "Hello, this is a response: " << response << std::endl;


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