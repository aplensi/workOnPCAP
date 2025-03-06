#include <QCoreApplication>
#include "ownpcapreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "\033c";
    ownPcapReader ownreader("record.pcap");
    ownreader.readFile("2bytes.bin", FileType::TwoBytes);
    ownreader.readFile("4bytes.bin", FileType::FourBytes);
    std::cout << "Количество пакетов: " << ownreader.getCountPackages() << std::endl;
    std::cout << "LinkType: " << ownreader.getLinkType() << std::endl;
    std::cout << "LinkType name: " << ownreader.getLinkTypeName() << std::endl;

    return a.exec();
}
