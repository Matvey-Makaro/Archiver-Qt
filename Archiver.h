#ifndef ARCHIVER_H
#define ARCHIVER_H

#include<vector>
#include<string>
#include<fstream>
#include"HuffmansTree.h"

class Archiver
{
public:
    Archiver();
    int inCompressFile(std::string& fileName);
    int outCompressArchive(std::string& zipFileName);
    ~Archiver();

public:
    enum Errors : char
    {
        NO_ERRORS = 0,
        FILE_DOES_NOT_OPEN = 1,
        FILE_EXTENSION_NOT_SUPPORTED = 100,
        UNKNOWN = 3
    };

private:
    static const int vocabularySize = 256; // кол-во символов, которые можно зашифровать в одном байте
    static constexpr int servInfSizeOfOneChar = sizeof(char) + sizeof(int);  // служебная информация - символ + его частота
    static constexpr int byteSize = sizeof(char) * 8;  //размер байта в битах
    HuffmansTree huffTree;
    std::vector<int> frequency;
    std::vector<std::string> huffTable;
    std::vector<std::string> availableExtensions = {".txt", ".pdf", ".doc", ".docx", ".mp4"};

private:
    void calculateFreq(std::ifstream&);
    void createHuffTree();
    int createHuffTable();
    int writeFileExtension(std::ofstream& zipFile, std::string& zipFileName);
    void writeServiceInf(std::ofstream&);
    void compress(std::ifstream&, std::ofstream&);
    void getFileExtension(std::string& fileName, std::string& outExtension);
    int getZipFileName(std::string& fileName, std::string& outZipFileName);
    int getUnzipFileName(std::string& zipFileName, unsigned char extensionIndex, std::string& outUnzipFileName);
    void clearFrequency();
    void clearHuffTree();
    void clearHuffTable();
};
#endif // ARCHIVER_H
