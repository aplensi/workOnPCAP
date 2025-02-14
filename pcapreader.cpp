#include "pcapreader.h"

pcapReader::pcapReader(const char *file) : file(file){}

pcapReader::~pcapReader()
{
    pcap_close(pcap);
}

int pcapReader::getCountPackages()
{
    int packet_count = 0;
    struct pcap_pkthdr header;
    const u_char *packet;
    while ((packet = pcap_next(pcap, &header))) {
        packet_count++;
    }
    return packet_count;
}

int pcapReader::getLinkType()
{
    return pcap_datalink(pcap);
}

std::string pcapReader::getLinkTypeName()
{
    return pcap_datalink_val_to_name(pcap_datalink(pcap));
}
