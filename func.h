#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

std::string getPathToInputFile();

std::string getPathToOutputFile();


class BMPFile {
    std::string path_to_file;

    tagBITMAPFILEHEADER FileHeader;
    tagBITMAPINFOHEADER InfoHeader;

    int sizeArrPalette, sizeArrImg;
    BYTE* arrPalette;
    BYTE* arrImg;

    int WIDTH, HEIGHT;
    unsigned short bitPerPix;

public:


    void readBMPFile(std::string path_to_file) {
        this->path_to_file = path_to_file;
        std::ifstream in(path_to_file, std::ios::binary);
        if (in.is_open()) {
            this->FileHeader = getFileHeaderFromFile(in);
            this->InfoHeader = getInfoHeaderFromFile(in);

            this->WIDTH = this->InfoHeader.biWidth;
            this->HEIGHT = this->InfoHeader.biHeight;
            this->bitPerPix = this->InfoHeader.biBitCount;

            this->arrPalette = getPaletteFromFile(in);

            this->arrImg = getArrayBytesImg(in);
        }
        else {
            std::cout << "Failed to open file";
        }
        in.close();
    }


    void deleteRes() {
        delete[] this->arrPalette;
        delete[] this->arrImg;
        this->arrPalette = nullptr;
        this->arrImg = nullptr;
    }

	tagBITMAPFILEHEADER getFileHeaderFromFile(std::ifstream& in) {
        tagBITMAPFILEHEADER FileHeader;

        unsigned short bfType;
        in.read((char*)&bfType, sizeof(bfType));

        unsigned int bfSize;
        in.read((char*)&bfSize, sizeof(bfSize));

        unsigned short bfReserved1, bfReserved2;
        in.read((char*)&bfReserved1, sizeof(bfReserved1));
        in.read((char*)&bfReserved2, sizeof(bfReserved2));

        unsigned int bfOffBits;
        in.read((char*)&bfOffBits, sizeof(bfOffBits));


        FileHeader.bfType = bfType;
        FileHeader.bfSize = bfSize;
        FileHeader.bfReserved1 = bfReserved1;
        FileHeader.bfReserved2 = bfReserved2;
        FileHeader.bfOffBits = bfOffBits;

        return FileHeader;
	}

	tagBITMAPINFOHEADER getInfoHeaderFromFile(std::ifstream& in) {
        tagBITMAPINFOHEADER InfoHeader;

        unsigned int biSize;
        in.read((char*)&biSize, sizeof(biSize));

        int biWidth, biHeight;
        in.read((char*)&biWidth, sizeof(biWidth));
        in.read((char*)&biHeight, sizeof(biHeight));

        unsigned short biPlanes;
        in.read((char*)&biPlanes, sizeof(biPlanes));

        unsigned short biBitCount;
        in.read((char*)&biBitCount, sizeof(biBitCount));

        unsigned int biCompression;
        in.read((char*)&biCompression, sizeof(biCompression));

        unsigned int biSizeImage;
        in.read((char*)&biSizeImage, sizeof(biSizeImage));

        int biXPelsPerMeter, biYPelsPerMeter;
        in.read((char*)&biXPelsPerMeter, sizeof(biXPelsPerMeter));
        in.read((char*)&biYPelsPerMeter, sizeof(biYPelsPerMeter));

        unsigned int biClrUsed;
        in.read((char*)&biClrUsed, sizeof(biClrUsed));

        unsigned int biClrImportant;
        in.read((char*)&biClrImportant, sizeof(biClrImportant));

        InfoHeader.biSize = biSize;
        InfoHeader.biWidth = biWidth;
        InfoHeader.biHeight = biHeight;
        InfoHeader.biPlanes = biPlanes;
        InfoHeader.biBitCount = biBitCount;
        InfoHeader.biCompression = biCompression;
        InfoHeader.biSizeImage = biSizeImage;
        InfoHeader.biXPelsPerMeter = biXPelsPerMeter;
        InfoHeader.biYPelsPerMeter = biYPelsPerMeter;
        InfoHeader.biClrUsed = biClrUsed;
        InfoHeader.biClrImportant = biClrImportant;

        return InfoHeader;
	}

    BYTE* getPaletteFromFile(std::ifstream& in) {
        this->sizeArrPalette = this->FileHeader.bfOffBits - 54;
        BYTE* arr = new BYTE[this->sizeArrPalette];

        for (int i = 0; i < this->sizeArrPalette; i++) {
            BYTE c;
            in.read((char*)&c, sizeof(c));
            arr[i] = c;
        }

        return arr;
    }


    BYTE* getArrayBytesImg(std::ifstream& in) {
        this->sizeArrImg = this->FileHeader.bfSize - this->FileHeader.bfOffBits;
        BYTE* arr = new BYTE[this->sizeArrImg];
        BYTE c;
        for (int i = 0; i < this->sizeArrImg; i++) {
            in.read((char*)&c, sizeof(c));
            arr[i] = c;
        }

        return arr;
    }


    void printImgInConsole() const {
        for (int i = this->HEIGHT - 1; i >= 0; i--) {
            for (int j = 0; j < this->WIDTH; j++) {
                if (this->bitPerPix == 32) {
                    BYTE c[4] = { this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 4], this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 4 + 1],
                    this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 4 + 2], this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 4 + 3] };
                    if (c[0] == 255 && c[1] == 255 && c[2] == 255) {
                        std::cout << ' ';
                    }
                    else {
                        std::cout << '#';
                    }
                }
                else {
                    BYTE c[3] = { this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 3], this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 3 + 1],
                    this->arrImg[(this->sizeArrImg / this->HEIGHT) * i + j * 3 + 2] };
                    if (c[0] == 255 && c[1] == 255 && c[2] == 255) {
                        std::cout << ' ';
                    }
                    else {
                        std::cout << '#';
                    }
                }
            }
            std::cout << '\n';
        }
        std::cout << "\n\n";
    }

    bool createBMPFile(std::string file_path) const {

        std::ofstream out(file_path, std::ios::out | std::ios::binary);
        if (out.is_open()) {
            out.write((char*)&this->FileHeader.bfType, sizeof(this->FileHeader.bfType));
            out.write((char*)&this->FileHeader.bfSize, sizeof(this->FileHeader.bfSize));
            out.write((char*)&this->FileHeader.bfReserved1, sizeof(this->FileHeader.bfReserved1));
            out.write((char*)&this->FileHeader.bfReserved2, sizeof(this->FileHeader.bfReserved2));
            out.write((char*)&this->FileHeader.bfOffBits, sizeof(this->FileHeader.bfOffBits));

            out.write((char*)&this->InfoHeader.biSize, sizeof(this->InfoHeader.biSize));
            out.write((char*)&this->InfoHeader.biWidth, sizeof(this->InfoHeader.biWidth));
            out.write((char*)&this->InfoHeader.biHeight, sizeof(this->InfoHeader.biHeight));
            out.write((char*)&this->InfoHeader.biPlanes, sizeof(this->InfoHeader.biPlanes));
            out.write((char*)&this->InfoHeader.biBitCount, sizeof(this->InfoHeader.biBitCount));
            out.write((char*)&this->InfoHeader.biCompression, sizeof(this->InfoHeader.biCompression));
            out.write((char*)&this->InfoHeader.biSizeImage, sizeof(this->InfoHeader.biSizeImage));
            out.write((char*)&this->InfoHeader.biXPelsPerMeter, sizeof(this->InfoHeader.biXPelsPerMeter));
            out.write((char*)&this->InfoHeader.biYPelsPerMeter, sizeof(this->InfoHeader.biYPelsPerMeter));
            out.write((char*)&this->InfoHeader.biClrUsed, sizeof(this->InfoHeader.biClrUsed));
            out.write((char*)&this->InfoHeader.biClrImportant, sizeof(this->InfoHeader.biClrImportant));

            for (int i = 0; i < this->sizeArrPalette; i++) {
                out.write((char*)&this->arrPalette[i], sizeof(this->arrPalette[i]));
            }

            for (int i = 0; i < this->sizeArrImg; i++) {
                out.write((char*)&this->arrImg[i], sizeof(this->arrImg[i]));
            }
            out.close();
            return true;
        }
        else {
            std::cout << "File for writing the image could not be opened.";
            out.close();
            return false;
        }

    }


    void createXInImg() {
        if (WIDTH >= HEIGHT) {
            createLineFromLeftToRightX(1, 1, this->WIDTH, this->HEIGHT);
            createLineFromRightToLeftX(this->WIDTH, 1, 1, this->HEIGHT);
        }
        else {
            createLineY(1, 1, this->WIDTH, this->HEIGHT);
            createLineY(this->WIDTH, 1, 1, this->HEIGHT);
        }
    }

    void createLineFromRightToLeftX(int x1, int y1, int x2, int y2) {
        x1--;
        x2--;
        y1 = this->HEIGHT - y1;
        y2 = this->HEIGHT - y2;
        int BLACK32[4]{ 0, 0, 0, 255 };
        int BLACK24[3]{ 0, 0, 0 };
        int deltax = abs(x2 - x1);
        int deltay = abs(y2 - y1);
        int error = 0;
        int deltaerr = deltay + 1;
        int y = y1;
        int diry = y2 - y1;
        if (diry > 0) {
            diry = 1;
        }
        if (diry < 0) {
            diry = -1;
        }
        for (int x = x1; x >= x2; x--) {
            if (this->bitPerPix == 32) {
                setColorToPix32Bit(x, y, BLACK32);
            }
            else {
                setColorToPix24Bit(x, y, BLACK24);
            }

            error += deltaerr;
            if (error >= deltax + 1) {
                y += diry;
                error -= deltax + 1;
            }
        }
    }


    void createLineFromLeftToRightX(int x1, int y1, int x2, int y2) {
        x1--;
        x2--;
        y1 = this->HEIGHT - y1;
        y2 = this->HEIGHT - y2;
        int BLACK32[4]{ 0, 0, 0, 255 };
        int BLACK24[3]{ 0, 0, 0 };
        int deltax = abs(x2 - x1);
        int deltay = abs(y2 - y1);
        int error = 0;
        int deltaerr = deltay + 1;
        int y = y1;
        int diry = y2 - y1;
        if (diry > 0) {
            diry = 1;
        }
        if (diry < 0) {
            diry = -1;
        }
        for (int x = x1; x <= x2; x++) {
            if (bitPerPix == 32) {
                setColorToPix32Bit(x, y, BLACK32);
            }
            else {
                setColorToPix24Bit(x, y, BLACK24);
            }
            error += deltaerr;
            if (error >= deltax + 1) {
                y += diry;
                error -= deltax + 1;
            }
        }
    }


    void createLineY(int x1, int y1, int x2, int y2) {
        x1--;
        x2--;
        y1 = this->HEIGHT - y1;
        y2 = this->HEIGHT - y2;
        int BLACK32[4]{ 0, 0, 0, 255 };
        int BLACK24[3]{ 0, 0, 0 };
        int deltax = abs(x2 - x1);
        int deltay = abs(y2 - y1);
        int error = 0;
        int deltaerr = deltax + 1;
        int x = x1;
        int dirx = x2 - x1;
        if (dirx > 0) {
            dirx = 1;
        }
        if (dirx < 0) {
            dirx = -1;
        }
        for (int y = y1; y >= y2; y--) {
            if (this->bitPerPix== 32) {
                setColorToPix32Bit(x, y, BLACK32);
            }
            else {
                setColorToPix24Bit(x, y, BLACK24);
            }
            error += deltaerr;
            if (error >= deltay + 1) {
                x += dirx;
                error -= deltay + 1;
            }
        }
    }


    void setColorToPix32Bit(int x, int y, int color[4]) {
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 4] = color[0];
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 4 + 1] = color[1];
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 4 + 2] = color[2];
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 4 + 3] = color[3];
    }


    void setColorToPix24Bit(int x, int y, int color[3]) {
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 3] = color[0];
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 3 + 1] = color[1];
        this->arrImg[(this->sizeArrImg / this->HEIGHT) * y + x * 3 + 2] = color[2];
    }
};