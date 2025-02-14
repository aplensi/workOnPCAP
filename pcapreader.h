#ifndef PCAPREADER_H
#define PCAPREADER_H

#include <string>
#include <pcap.h>
#include <iostream>

class pcapReader {
public:
    pcapReader(const char *file);
    ~pcapReader();
    int getCountPackages();
    int getLinkType();
    std::string getLinkTypeName();
private:
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *file;
    pcap_t *pcap = pcap_open_offline(file, errbuf);
};

#endif
