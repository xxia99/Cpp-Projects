/*
Author: Xin Xia
Class: ECE 6122
Last Date Modified: 11/06/2021
Description:
This is a client console program for TCP server that takes as a command line argument
the port number of the server as shown: ./client 61717. 
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

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

// Run server and TCP connection between clients and the server
void startConnection(int port)
{
    SocketSelector newSS;
    TcpListener newTL;
    vector<TcpSocket*> TSs;
    int totalSum = 0;
    fstream output("server.log", ios::out | ios::app);

    if (newTL.listen(port) != sf::Socket::Done)
        return;
    newSS.add(newTL);

    while (1)
    {
        if (newSS.wait() && newSS.isReady(newTL))
        {
            TcpSocket *newTS = new TcpSocket;
            if (newTL.accept(*newTS) != Socket::Done)
            {
                delete newTS;
            }
            else
            {
                totalSum++;
                TSs.push_back(newTS);
                newSS.add(*newTS);
                output << "Client successfully connected" << endl;
            }           
        }
    }
}

int main(int argc, char *argv[])
{
    int port;  
    if (verifyPort(argv[1], port))
    {
        startConnection(port);
    }
    else
    {
        cout << "Invalid command line argument detected: " << argv[1] << endl
            << "Please check your values and press any key to end the program!\n";
    }    
    std::cin.ignore(20000, '\n');
    return EXIT_SUCCESS;
}
