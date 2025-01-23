#include "bmp.cpp"
#include "ico.cpp"

int main()
{
    ICO myImage((char*) "whatsapp.ico");

    for(int a = 0; a < myImage.numberOfImages; a++)
    {
        cout << "Number Of Images: " << myImage.numberOfImages << endl;
        cout << "Image " << a << ": " << endl;
        cout << "\tWidth: " << myImage.getWidth(a) << endl;
        cout << "\tHeight: " << myImage.getHeight(a) << endl;
        cout << "\tColor Palettes: " << myImage.getColorPalete(a) << endl;
        cout << "\tColorPanes: " << myImage.getColorPane(a) << endl;
        cout << "\tBits Per Pixel: " << myImage.getBitsPerPixel(a) << endl;
        cout << "\tSizes: " << myImage.getSize(a) << endl;
        cout << "\tOffsets: " << myImage.getOffset(a) << endl;
    }

    //myImage.getPixelData(0, myImage.getWidth(0), myImage.getHeight(0), myImage.getBitsPerPixel(0));

    myImage.getMonochromeDataIntermediate(0, 255, 255, 80, 255, false, true);

    //myImage.getMonoChromeRepresentation(0);
    
    return 1;
}