#include <QCoreApplication>
#include "ownpcapreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "\033c";
    ownPcapReader ownreader("record.pcap");

    ownreader.readFile("8bytes.bin", FileType::EightBytes);
    //ownreader.readFile("4bytes.bin", FileType::FourBytes);

    std::map<int, int> sizes = ownreader.getMapOfPackages();
    for(auto [size, count] : sizes){
        std::cout << size << " => " << count << std::endl;
    }

    std::cout << std::endl << "Количество пакетов: " << ownreader.getCountPackages() << std::endl;
    std::cout << "LinkType: " << ownreader.getLinkType() << std::endl;
    std::cout << "LinkType name: " << ownreader.getLinkTypeName() << std::endl;

    return a.exec();
}
