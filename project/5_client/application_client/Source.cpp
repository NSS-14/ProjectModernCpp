#include <iostream>
#include <numeric>
#include <sstream>
#include <regex>
#include <string>

#include <cpr/cpr.h>
#include <crow.h>

int main()
{
    auto response = cpr::Put(
        cpr::Url{ "http://localhost:18080/login" },
        cpr::Payload{
            { "name", "testname" },
            { "password", "p12345" }
        }
    );

    if (response.status_code == 200 || response.status_code == 201) {
        std::cout << "Logged in. :)\n";
    }
    else {
        std::cout << "There was a problem that occured in the logging operation. :(\n";
    }

	return 0;
}