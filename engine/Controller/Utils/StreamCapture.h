#pragma once
#include <stdio.h>
#include <sstream>
#include <iostream>

class StreamCapture
{
public:
    StreamCapture(std::ostream& stream) {
        oldBuffer = std::cout.rdbuf(redirectStream.rdbuf());
    }
    ~StreamCapture() { Release(); }

    void Release() {
        std::cout.rdbuf(oldBuffer); // Return control of the stream to the system
    }

    std::string GetBuffer() const { return redirectStream.str(); }

private:
    std::stringstream redirectStream;
    std::streambuf* oldBuffer;
};