#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Function to parse multipart/form-data content
void parseMultipartFormData(const std::string& contentType, const std::string& body) {
    // Extract boundary string from Content-Type header
    std::string boundary;
    size_t pos = contentType.find("boundary=");
    if (pos != std::string::npos) {
        boundary = contentType.substr(pos + 9);
    } else {
        std::cerr << "Boundary not found in Content-Type header." << std::endl;
        return;
    }

    // Split body into parts using the boundary string
    std::vector<std::string> parts;
    size_t start = body.find("--" + boundary);
    while (start != std::string::npos) {
        size_t end = body.find("--" + boundary, start + boundary.length() + 2);
        if (end != std::string::npos) {
            parts.push_back(body.substr(start, end - start));
            start = end;
        } else {
            break;
        }
    }

    // Parse each part
    for (const auto& part : parts) {
        // Find headers and body in the part
        size_t headerEnd = part.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            std::string headers = part.substr(0, headerEnd);
            std::string body = part.substr(headerEnd + 4);
            
            // Process headers and body separately (e.g., extract filename, content type, etc.)
            std::string filename;
            size_t filenamePos = headers.find("filename=");
            if (filenamePos != std::string::npos) {
                size_t filenameStart = headers.find("\"", filenamePos);
                size_t filenameEnd = headers.find("\"", filenameStart + 1);
                if (filenameStart != std::string::npos && filenameEnd != std::string::npos) {
                    filename = headers.substr(filenameStart + 1, filenameEnd - filenameStart - 1);
                }
            }
            
            // Save the binary data to a file
            if (!filename.empty()) {
                std::ofstream file(filename, std::ios::binary);
                file.write(body.c_str(), body.size());
                file.close();
                std::cout << "File saved: " << filename << std::endl;
            }
        }
    }
}

int main() {
    // Example Content-Type header and body of a multipart/form-data request
    std::string contentType = "multipart/form-data; boundary=--------------------------1234567890";
    std::string body = "----------------------------1234567890\r\n"
                       "Content-Disposition: form-data; name=\"text_field\"\r\n\r\n"
                       "Text field value\r\n"
                       "----------------------------1234567890\r\n"
                       "Content-Disposition: form-data; name=\"file_field\"; filename=\"example.mp3\"\r\n"
                       "Content-Type: audio/mpeg\r\n\r\n"
                       "(binary data for example.mp3 goes here)\r\n"
                       "----------------------------1234567890--\r\n";

    // Parse the multipart/form-data content
    parseMultipartFormData(contentType, body);

    return 0;
}
