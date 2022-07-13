/*
Author: Xin Xia
Class: ECE 6122
Last Date Modified: 11/06/2021
Description:
This is a client console program for TCP client that takes as a command line argument the IP Address and port number of the server
as shown: ./client localhost 61717. The program continuously prompts the user "Please enter a message:" for messages to send to the server.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <SFML/Network.hpp>

using namespace sf;
using namespace std;

// Verify the input port number
int verifyPort(const string inputPort, int& portnum)
{

    int portF = 1;
    unsigned long portNum;
    stringstream(inputPort) >> portNum;
    // Find if there is any non-numeric value in input port num
    for (auto x : inputPort)
    {
        if (!isdigit(x))
        {
            portF = 0;
            return portF;
        }
    }
    // Find if the port number outside the range 61000-65535
    if (61000 <= portNum <= 65535)
    {
        portnum = portNum;
        portF = 1;
    }
    else
    {
        portF = 0;
    }
    return portF;
}

// Verify the input ip address
int verifyAddr(const string inputAddr, IpAddress& addr)
{
    stringstream(inputAddr) >> addr;
    if (addr != IpAddress::None)
        return 1;
    else
        return 0;
}

// Run client and TCP connection between clients and the server
void startConnection(IpAddress addr, int port)
{

    TcpSocket newTC;
    Time toTime = sf::milliseconds(2500);
    string cInfo;

    if (newTC.connect(addr, port, toTime) != sf::Socket::Done)
    {
        cout << "Failed to connect to the server at " << addr.toString() << " on " << to_string(port)
            << ".\nPlease check your values and press any key to end the program!" << endl;
        return;
    }
    std::cout << "Connected to server " << addr << std::endl;

    while (1)
    {
        cout << "Please enter a message: ";
        getline(cin, cInfo);
        newTC.send(cInfo.c_str(), cInfo.size());
    }
}


// The main function for the client console program
int main(int argc, char* argv[])
{
    sf::IpAddress address;
    int port;
    fstream output("server.log", ios::out | ios::app);

    if (verifyAddr(argv[1], address) && verifyPort(argv[2], port))
    {
        startConnection(address, port);
    }
    else
    {
        cout << "Invalid command line argument detected: " << argv[1] << " " << argv[2] << endl
            << "Please check your values and press any key to end the program!\n";
    }

    std::cin.ignore(20000, '\n');
    return EXIT_SUCCESS;
}

