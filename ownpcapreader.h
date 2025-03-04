#ifndef OWNPCAPREADER_H
#define OWNPCAPREADER_H
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

#pragma pack(push, 1)
struct PcapGlobalHeader {
    uint32_t magic_number;
    uint16_t version_minor;
    int32_t thiszone;
    uint32_t sigfigs;
    uint16_t version_major;
    uint32_t snaplen;
    uint32_t network;
};
struct PcapPacketHeader {
    uint32_t ts_sec;
    uint32_t ts_usec;
    uint32_t incl_len;
    uint32_t orig_len;
};
struct Packet {
    uint32_t size;
    const uint8_t* data;
};
#pragma pack(pop)

class ownPcapReader
{
public:
    ownPcapReader(const char* file);
    int getLinkType();
    std::string getLinkTypeName();
    int getCountPackages();
private:
    PcapGlobalHeader globalHeader;
    void countPackets();
    void readPcapToBuffer();
    void createListOfPackages();
    void writePacketsToFile();
    const char* file;
    std::vector<char> buffer;
    std::vector<Packet> packages;
    std::string getLinkTypeName(uint32_t linktype) const;
};

#endif // OWNPCAPREADER_H
