#ifndef READANDWRITE_H
#define READANDWRITE_H

#include "ownpcapreader.h"
#include <iostream>
#include <fstream>
#include <vector>

class Field {
public:
    virtual std::vector<uint8_t> read() = 0;
    virtual ~Field() = default;
};

template <typename T>
class TypedField : public Field {
public:
    std::vector<uint8_t> read() override
    {
        T value;
        std::vector<uint8_t> buffer(sizeof(value));
        return buffer;
    }
};

class SizedField : public Field {
public:
    SizedField(size_t size);
    std::vector<uint8_t> read() override;
private:
    size_t m_size;
};

void processRead(const std::string& filename, Field& reader);

void processWrite(std::vector<uint8_t> buffer, Field& reader);

#endif