#ifndef READFILE_H
#define READFILE_H

#include <iostream>
#include <fstream>
#include <vector>

class FieldReader {
public:
    virtual std::vector<uint8_t> read() = 0;
    virtual ~FieldReader() = default;
};

template <typename T>
class TypedFieldReader : public FieldReader {
public:
    std::vector<uint8_t> read() override;
};

class SizedFieldReader : public FieldReader {
public:
    SizedFieldReader(size_t size);
    std::vector<uint8_t> read() override;
private:
    size_t m_size;
};

void processFile(const std::string& filename, FieldReader& reader);

#endif