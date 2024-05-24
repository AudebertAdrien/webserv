#include <iostream>

int main() {
    // Print the HTTP headers
    std::cout << "Content-Type: text/html\n\n";
    
    // Generate HTML content
    std::cout << "<html>\n";
    std::cout << "<head>\n";
    std::cout << "<title>CGI Script Example</title>\n";
    std::cout << "</head>\n";
    std::cout << "<body>\n";
    std::cout << "<h1>Hello, World!</h1>\n";
    std::cout << "<p>This is a simple CGI script written in C++.</p>\n";
    std::cout << "</body>\n";
    std::cout << "</html>\n";

    return 0;
}
