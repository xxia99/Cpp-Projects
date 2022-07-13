# Problem 1: Prime Factors

### Problem Description

The prime factors of 13195 are 5, 7, 13 and 29. The program takes one command-line argument variable of type unsigned long and contains two functions:

1. int main(int argc, char* argv[]) – parses the input parameter and calls the function GetPrimeFactors(const unsigned long ulInputNumber, string &strOutput) to calculate the prime factors of ulInputNumber.

2. bool GetPrimeFactors (const unsigned long ulInputNumber, string &strOutput) – determines the prime factors of ulInputNumber. If there are no prime factors then the function returns false, otherwise the function returns true. Any prime factors are concatenated into the function parameter strOutput using a comma to separate the factors.

### Outputs and Examples

For example, if ulInputNumber = 13195, then strOutput = “5,7,13,29”.

1. The program must then output just strOutput to a text file called output1.txt. If the output1.txt previously existed, overwrite any existing information in the file. 
2. If there are no prime factors then your code needs to output “No prime factors”.
