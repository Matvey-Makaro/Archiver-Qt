#include"Archiver.h"
#include<fstream>
#include<iostream>


Archiver::Archiver()
{
    huffTable.resize(vocabularySize);
    frequency.resize(vocabularySize, 0);
}

int Archiver::inCompressFile(std::string& fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Error! File doesn't open!" << std::endl;
        return Errors::FILE_DOES_NOT_OPEN;
    }
    calculateFreq(file);
    for (unsigned int i = 0; i < frequency.size(); ++i)	//удалить
        if (frequency[i] != 0)
            std::cout << (char)i << " " << frequency[i] << std::endl;

    unsigned short numUnicChar = createHuffTree();
    unsigned short serviceInfSize = numUnicChar * servInfSizeOfOneChar + 2 * sizeof(short);	//две short переменный в начале и конце файла
    createHuffTable();
    std::string outputFileName;
    getZipFileName(fileName, outputFileName);
    std::ofstream zipFile(outputFileName, std::ios::binary | std::ios::trunc);
    if (!zipFile.is_open())
    {
        std::cerr << "Error! Zip file doesn't open!" << std::endl;
        return Errors::FILE_DOES_NOT_OPEN;
    }

    Errors errorCode = (Errors)writeFileExtension(zipFile, fileName);
    if(errorCode == Errors::FILE_EXTENSION_NOT_SUPPORTED)
        return Errors::FILE_EXTENSION_NOT_SUPPORTED;
    zipFile.write((const char*)&serviceInfSize, sizeof(serviceInfSize));
    writeServiceInf(zipFile);
    compress(file, zipFile);

    clearFrequency();
    clearHuffTree();
    clearHuffTable();
    file.close();
    zipFile.close();
    return Errors::NO_ERRORS;
}

int Archiver::inCompressFiles(std::vector<std::string>& fileNames)
{
    std::ifstream file;
    for (unsigned int i = 0; i < fileNames.size(); ++i)
    {
        for (unsigned int j = 0; j < frequency.size(); ++j)
            frequency[j] = 0;
        calculateFreq(file);
        //обнулить таблицу частот
    }
    return 1;
}


int Archiver::outCompressArchive(std::string& zipFileName)
{
    std::ifstream zipFile(zipFileName, std::ios::binary | std::ios::in);	//файл с заархивированными данными
    if (!zipFile.is_open())
    {
        std::cerr << "Error! Zip file doesn't open!" << std::endl;
        return Errors::FILE_DOES_NOT_OPEN;
    }
    short serviceInfSize = 0, numUnicChar = 0, numUselessBits = 0;
    unsigned char extensionIndex = 0;
    long long int fileSize;
    zipFile.seekg(0, std::ios::end);
    fileSize = zipFile.tellg();
    zipFile.seekg(-2, std::ios::end);
    zipFile.read((char*)&numUselessBits, sizeof(numUselessBits));
    zipFile.seekg(0, std::ios::beg);
    zipFile.read((char*)&extensionIndex, sizeof(extensionIndex));
    zipFile.read((char*)&serviceInfSize, sizeof(serviceInfSize));

    numUnicChar = (serviceInfSize - 4) / servInfSizeOfOneChar;	//4 байта служебной информации это 2 shot переменные, в начале и конце файла
    long long int compressInfSize = fileSize - serviceInfSize;
    unsigned char tmpChar= 0;
    int bufferFrequency = 0;
    for (int i = 0; i < numUnicChar; ++i)
    {
        zipFile.read((char*)&tmpChar, sizeof(tmpChar));
        zipFile.read((char*)&bufferFrequency, sizeof(bufferFrequency));
        huffTree.push(tmpChar, bufferFrequency);
    }
    createHuffTree();  //исправить этот метод, он подходит для архивации, а для разархивации делает лишние пуши

    std::string outputFileName;
    Errors errorCode = (Errors)getUnzipFileName(zipFileName, extensionIndex, outputFileName);
    if(errorCode == Errors::FILE_EXTENSION_NOT_SUPPORTED)
        return Errors::FILE_EXTENSION_NOT_SUPPORTED;

    std::ofstream file(outputFileName, std::ios::trunc | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error! Output file doesn't open!" << std::endl;
        return Errors::FILE_DOES_NOT_OPEN;
    }
    Node<unsigned char>* huffTreeRoot = huffTree.getRoot();
    Node<unsigned char>* currNode = huffTreeRoot;
    if (huffTreeRoot != nullptr)
    {
        unsigned char buffer = 0;
        unsigned char mask;
        for (int i = 0; i < compressInfSize - 1; ++i)
        {
            mask = 1 << (byteSize - 1);  //двоичный код маски 10000000, чтобы считать последний бит
            zipFile.read((char*)&buffer, sizeof(buffer));
            for (int j = 0; j < byteSize; ++j)
            {
                if (buffer & mask)
                    currNode = currNode->right;
                else
                    currNode = currNode->left;

                if (huffTree.isLeaf(currNode))
                {
                    file.write((char*)&currNode->data, sizeof(currNode->data));
                    currNode = huffTreeRoot;
                }
                mask >>= 1;
            }
        }

        mask = 1 << (byteSize - 1);	//двоичный код маски 10000000, чтобы считать последний бит
        zipFile.read((char*)&buffer, sizeof(buffer));
        for (int j = 0; j < byteSize - numUselessBits; ++j)
        {
            if (buffer & mask)
                currNode = currNode->right;
            else
                currNode = currNode->left;

            if (huffTree.isLeaf(currNode))
            {
                file.write((char*)&currNode->data, sizeof(currNode->data));
                currNode = huffTreeRoot;
            }
            mask >>= 1;
        }
    }
    clearHuffTree();
    file.close();
    zipFile.close();
    return Errors::NO_ERRORS;
}

Archiver::~Archiver() {}


void Archiver::calculateFreq(std::ifstream& file)
{
    long long int fileSize;
    file.clear();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    unsigned char tmp;
    for (long long int i = 0; i < fileSize; ++i)
    {
        file.read((char*)&tmp, sizeof(tmp));
        ++frequency[tmp];
    }
}


short Archiver::createHuffTree()
{
    short counter = 0;
    for (unsigned int i = 0; i < frequency.size(); ++i)
        if (frequency[i] != 0)
        {
            huffTree.push((unsigned char)i, frequency[i]);
            ++counter;
        }
    huffTree.create();
    return counter;
}

int Archiver::createHuffTable() { return huffTree.createHuffTable(huffTable); }

int Archiver::writeFileExtension(std::ofstream& zipFile, std::string& zipFileName)
{
    std::string zipFileExtension;
    getFileExtension(zipFileName, zipFileExtension);
    unsigned char extensionIndex = Errors::FILE_EXTENSION_NOT_SUPPORTED;
    for(unsigned int i = 0; i < availableExtensions.size(); ++i)
    {
        if(availableExtensions[i] == zipFileExtension)
        {
            extensionIndex = i;
            break;
        }
    }
    if(extensionIndex == Errors::FILE_EXTENSION_NOT_SUPPORTED)
        return Errors::FILE_EXTENSION_NOT_SUPPORTED;
    zipFile.write((const char*)&extensionIndex, sizeof(extensionIndex));
    return Errors::NO_ERRORS;
}

void Archiver::writeServiceInf(std::ofstream& zipFile)
{
    for (unsigned int i = 0; i < frequency.size(); ++i)
    {
        if (frequency[i] != 0)
        {
            zipFile << (unsigned char)i;
            zipFile.write((const char*)&frequency[i], sizeof(frequency[i]));
        }
    }
}

void Archiver::compress(std::ifstream& file, std::ofstream& zipFile)
{
    long long int fileSize;
    file.clear();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    short counter = 0;
    unsigned char buffer = 0, newChar = 0;
    std::string newCode;
    for (int i = 0; i < fileSize; ++i)
    {
        file.read((char*)&buffer, sizeof(buffer));
        newCode = huffTable[buffer];
        for (unsigned int j = 0; j < newCode.size(); ++j)
        {
            if (newCode[j] == '1')
                newChar |= 1;
            ++counter;
            if (counter == byteSize)
            {
                zipFile.write((const char*)&newChar, sizeof(newChar));
                counter = 0;
                newChar = 0;
            }
            newChar <<= 1;
        }
    }

    if (counter != 0)  //если кол-во новых бит не кратно 8
    {
        newChar <<= (byteSize - counter - 1);    //дополняем последний символ до 8 бит нулями, -1 потому что при выходе из цикла сверху newСhar уже сместился на 1 бит влево
        zipFile.write((const char*)&newChar, sizeof(newChar));
    }
    short numUselessBits = byteSize - counter;
    zipFile.write((const char*)&numUselessBits, sizeof(numUselessBits));  //записываем в конец файла кол-во не значимых бит в последнем символе
}

void Archiver::getFileExtension(std::string& fileName, std::string& outExtension)
{
    int pos = fileName.find_last_of(".");
    outExtension = fileName.substr(pos);
}

int Archiver::getZipFileName(std::string& fileName, std::string& zipFileName)
{
    zipFileName = fileName;
    zipFileName.erase(zipFileName.find_last_of("."));
    zipFileName.append(".txt");
    return Errors::NO_ERRORS;
}

int Archiver::getUnzipFileName(std::string& zipFileName, unsigned char extensionIndex, std::string& outUnzipFileName)
{
    if(extensionIndex >= availableExtensions.size())
        return Errors::FILE_EXTENSION_NOT_SUPPORTED;
    outUnzipFileName = zipFileName;
    outUnzipFileName.erase(outUnzipFileName.find_last_of("."));
    outUnzipFileName.append(availableExtensions[extensionIndex]);
    return Errors::NO_ERRORS;
}

void Archiver::clearFrequency()
{
    for (unsigned int i = 0; i < frequency.size(); ++i)
        frequency[i] = 0;
}

void Archiver::clearHuffTree() { huffTree.clear(); }

void Archiver::clearHuffTable()
{
    for (unsigned int i = 0; i < huffTable.size(); ++i)
        huffTable[i] = "";
}
