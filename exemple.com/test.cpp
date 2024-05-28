#include <iostream>
#include <fstream>
#include <vector>

void saveBinaryDataToFile(const std::string& filename, const std::vector<char>& binaryData) {
    // Open a file stream in binary mode
    std::ofstream file(filename, std::ios::binary);

    // Check if the file stream is open
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // Write the binary data to the file
    file.write(binaryData.data(), binaryData.size());

    // Check if any error occurred while writing
    if (!file) {
        std::cerr << "Error occurred while writing to file: " << filename << std::endl;
    }

    // Close the file stream
    file.close();
}

int main() {
    // Example binary data (you would replace this with actual binary data)
    std::vector<char> binaryData = {'\x48', '\x65', '\x6c', '\x6c', '\x6f', '\x20', '\x57', '\x6f', '\x72', '\x6c', '\x64'};

    // Save the binary data to a file
    saveBinaryDataToFile("example.bin", binaryData);

    return 0;
}

