#include <QCoreApplication>
#include "pcapreader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    pcapReader reader("record.pcap");
    std::cout << "Количество пакетов: " << reader.getCountPackages() << std::endl;
    std::cout << "LinkType: " << reader.getLinkType() << std::endl;

    return a.exec();
}
