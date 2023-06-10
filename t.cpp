#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

// A function to get a 10 digit random number
long long randomDigits()
{
    // Initialize the random number generator with the current time
    srand(time(0));

    // Declare a variable to store the result
    long long result = 0;

    // Loop through 10 digits
    for (int i = 0; i < 10; i++)
    {
        // Generate a random digit from 0 to 9
        int digit = rand() % 10;

        // Append the digit to the result
        result = result * 10 + digit;
    }

    // Return the result
    return result;
}

int main()
{
    // Call the function and print the result
    cout << randomDigits() << endl;

    return 0;
}