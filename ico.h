using namespace std;

class ICO{
    public:
        int* widths;
        int* heights;
        int* offsets;
        int* colorPaletes;
        int* colorPanes;
        int* bitsPerPixel;
        int* sizes;
        int numberOfImages;
        int imageType;
        char* fileName;
        unsigned char* header;
        unsigned char* icoDirectories;
        unsigned char* fileData;
        int sizeOfBytes = -1;

        ICO(char* fileName);
        int getWidth(int index);
        int getHeight(int index);
        int getOffset(int index);
        int getColorPalete(int index);
        int getColorPane(int index);
        int getBitsPerPixel(int index);
        int getSize(int index);
        unsigned char* getImageData(int index);
        unsigned char* getPixelData(int index, int width, int height, int bitsPerPixel);
        unsigned char* getMonochromeDataIntermediate(int index, int green, int red, int blue, int alpha, bool flipRight, bool flipDown);
        unsigned char* getMonoChromeRepresentation(int index, int green, int red, int blue, int alpha, bool flipRight, bool flipDown);

};