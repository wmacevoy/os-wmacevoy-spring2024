#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <array>
#include <algorithm> // For std::fill and std::min
#include <cstring>   // For std::memcpy
#include <sys/mman.h>

#include <cmath>
#include "canary.h"

void cStackHandleIncomingMessage() {
    char stackBuffer[1024]; // Allocate a buffer on the stack

    // Simulate receiving and processing a message
    snprintf(stackBuffer, sizeof(stackBuffer), "Message received on the stack");
    printf("%s\n", stackBuffer);

    // No need to manually free the buffer; it's automatically freed when the function returns
}

void cHeapHandleIncomingMessage() {
    size_t bufferSize = 1024; // Example buffer size; could be determined at runtime
    char* heapBuffer = (char*)malloc(bufferSize); // Allocate a buffer on the heap

    if (heapBuffer != NULL) {
        // Simulate receiving and processing a message
        snprintf(heapBuffer, bufferSize, "Message received on the heap");
        printf("%s\n", heapBuffer);

        free(heapBuffer); // It's crucial to free the allocated memory to avoid leaks
    } else {
        printf("Failed to allocate memory on the heap\n");
    }

}

class CXXHeapMessageBuffer {
public:
    CXXHeapMessageBuffer(size_t size) 
    : bufferSize(size), buffer(new char[size]) {
        std::cout << "MessageBuffer of size " << bufferSize << " allocated." << std::endl;
        // Zero the buffer upon creation
        std::memset(buffer, 0, bufferSize);
    }

    ~CXXHeapMessageBuffer() {
        // Zero the buffer before deallocation for security
        std::memset(buffer, 0, bufferSize);
        delete[] buffer;
        std::cout << "MessageBuffer of size " << bufferSize << " deallocated." << std::endl;
    }

    // Simulate processing an incoming message
    void processMessage(const std::string& message) {
        // Ensure we don't overflow the buffer. Truncate if necessary.
        size_t copySize = std::min(message.size(), bufferSize - 1);
        std::memcpy(buffer, message.c_str(), copySize);
        // Ensure null termination
        buffer[copySize] = '\0';
        std::cout << "Processing message: " << buffer << std::endl;
    }

private:
    size_t bufferSize;
    // This buffer is always on the heap.
    char* buffer;
};

template<size_t Size>
class CXXArrayMessageBuffer {
public:
    CXXArrayMessageBuffer() {
        // Zero the buffer upon creation
        buffer.fill(0);
    }

    ~CXXArrayMessageBuffer() {
        // Zero the buffer before destruction for security
        buffer.fill(0);
    }

    // Simulate processing an incoming message
    void processMessage(const std::string& message) {
        // Ensure we don't overflow the buffer. Truncate if necessary.
        size_t copySize = std::min(message.size(), Size - 1);
        std::memcpy(buffer.data(), message.c_str(), copySize);
        // Ensure null termination
        buffer[copySize] = '\0';
        std::cout << "Processing message: " << buffer.data() << std::endl;
    }

private:
    std::array<char, Size> buffer;
};

class SafeMessage {
  void processMessage(const std::string& message) {
    size_t size;
    bool clipped;
    SecureArray<uint8_t,1024> secure;
    size=message.size();
    if (size > secure.data.size() - 1) {
      size = secure.data.size() - 1;
      clipped = true;
    }
    std::memcpy(&secure.data[0], &message[0], size);
    secure.data[size]='\0';
    process(&secure.data[0],size,clipped);
  }

  void process(uint8_t *buffer, size_t size, bool clipped) {
    // ...
  }
};



int main() {
    cHeapHandleIncomingMessage();
    return 0;
}
