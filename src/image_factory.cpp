#include <lms/imaging/image_factory.h>
#include <lms/imaging/image.h>
#include <cmath>
namespace lms {
namespace imaging {

namespace op{

void gaussPxl(const Image &input,Image &output,int x, int y){
    //TODO check if input and output are the same formats
    if(input.format() == Format::GREY){
        int gauss = gaussGrey(input,x,y);
        // set pixel
        *(output.data()+input.width()*y + x ) = gauss;
    }else{
        //ERROR not implemented yetgaussPxlGrey
        return;
    }
}

int gaussGrey(const Image &input, int x, int y){
    //TODO nur vorrübergehend
    const std::uint8_t *IMAGE = input.data();
    int IMAGE_WIDTH = input.width();
    int IMAGE_HEIGHT = input.height();

    int valueGauss = 0;
    int x_index = 0;
    int y_index = 0;

    y_index = IMAGE_WIDTH * (validateY(y + (0-1),IMAGE_HEIGHT,true));

    x_index = validateX(x + (0-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;
    x_index = validateX(x+(1-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;
    x_index = validateX(x + (2-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;

    y_index = IMAGE_WIDTH * (validateY(y + (1-1),IMAGE_HEIGHT,true));
    x_index = validateX(x + (0-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;
    x_index = validateX(x + (1-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 2;
    x_index = validateX(x + (2-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;

    y_index = IMAGE_WIDTH * (validateY(y + (2-1),IMAGE_HEIGHT,true));
    x_index = validateX(x + (0-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;
    x_index = validateX(x + (1-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;
    x_index = validateX(x + (2-1),IMAGE_WIDTH,true);
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;

    return valueGauss;
}

void gauss(const Image &input,Image &output){
    for(int x = 0; x < input.width(); x++){
        for(int y = 0; y < input.height(); y++){
            gaussPxl(input,output,x,y);
        }
    }
}

void sobelX(const Image &input, Image &output) {
    //TODO error checking
    for(int x = 0; x < input.width();x++){
        for(int y = 0; y< input.height(); y++){
            //normalize img
            *(output.data()+y*input.width()+x)=sobelX(x,y,input)/8+128;
        }
    }
}
void sobelY(const Image &input, Image &output) {
    //TODO error checking
    for(int x = 0; x < input.width();x++){
        for(int y = 0; y< input.height(); y++){
            *(output.data()+y*input.width()+x)=sobelY(x,y,input)/8+128;
        }
    }
}
void imageOperator(const Image &input,Image &output,const double *mat,int matRows, int matCols,bool reflectBorders,int devisor) {
    //TODO error checking
    for(int x = 0; x < input.width();x++){
        for(int y = 0; y< input.height(); y++){
            *(output.data()+y*input.width()+x)=(int)imageOperator(input,x,y,mat,matRows,matCols,reflectBorders,devisor);
        }
    }
}

double imageOperator(const Image &image,int x, int y,const double *mat,int matRows, int matCols,bool reflectBorders,int devisor){
    //TODO error checking
    int x_index = 0;
    int y_index = 0;

    double result= 0;
    int x_start = x-matCols/2;
    int y_start = y-matRows/2;
    //y-axis
    for(int k = 0; k < matRows; k++) {
        y_index = image.width()*(validateY(y_start+k,image.height(),reflectBorders));
        if(y_index < 0){
            continue;
        }
        for(int j = 0; j < matCols; j++) {
            x_index = validateX(x_start + j,image.width(),reflectBorders);
            if(x_index < 0){
                continue;
            }
            result += *(image.data() + y_index + x_index)* *(mat+k*matCols+j);
        }
    }
    return result/devisor;

}




int validateX(int x,int width,bool reflect){
    if(x < 0){
        if(reflect){
            return - x - 1;
        }else
            return 0;
    }
    if(x >= width){
        if(reflect){
            return 2*width-x-1;
        }else
            return width -1;
    }
    return x;
}
int validateY(int y,int height,bool reflect){
    if(y < 0){
        if(reflect){
            return - y - 1;
        }else
            return -1;
    }
    if(y >= height){
        if(reflect){
            return 2*height-y-1;
        }else
            return -1;
    }
    return y;
}


int sobelX(int x, int y, const Image &image) {
    //TODO error checking
    const std::uint8_t* data = image.data();
    int height = image.height();
    int width = image.width();

    int valueSobelX = 0;
    int y_index = width * (validateY(y + (0 - 1),height,true));

    valueSobelX -= *(data + y_index + validateX(x + (0-1),width,true));
    valueSobelX += *(data + y_index + validateX(x + (2-1),width,true));

    y_index = width * (validateY(y + (1 - 1),height,true));

    valueSobelX -= *(data + y_index + validateX(x + (0-1),width,true)) << 1;
    valueSobelX += *(data + y_index + validateX(x + (2-1),width,true)) << 1;

    y_index = width * (validateY(y + (2 - 1),height,true));

    valueSobelX -= *(data + y_index + validateX(x + (0-1),width,true));
    valueSobelX += *(data + y_index + validateX(x + (2-1),width,true));
    return valueSobelX;
}

int sobelY(int x, int y,const Image &image) {
    //TODO error checking
    const std::uint8_t* data = image.data();
    int height = image.height();
    int width = image.width();

    int valueSobelY = 0;
    int y_index = width * (validateY(y + (0 - 1),height,true));

    valueSobelY += *(data + y_index + validateX(x + (0-1),width,true));
    valueSobelY += *(data + y_index + validateX(x + (1-1),width,true)) << 1;
    valueSobelY += *(data + y_index + validateX(x + (2-1),width,true));

    y_index = width * (validateY(y + (2 - 1),height,true));

    valueSobelY -= *(data + y_index + validateX(x + (0-1),width,true));
    valueSobelY -= *(data + y_index + validateX(x + (1-1),width,true)) << 1;
    valueSobelY -= *(data + y_index + validateX(x + (2-1),width,true));

    return valueSobelY;

}


void gaussBox(const Image &input,Image &output,int xMin, int yMin,int xMax, int yMax){
    for(;xMin <= xMax;xMin++){
        for(;yMin <= yMax;yMin++){
            gaussPxl(input,output,xMin,yMin);
        }
    }
}


void subtract(const Image &input1, const Image &input2, Image &output, int minVal, int maxVal){
    //TODO errorchecking
    int value = 0;
    if(input1.format() == Format::GREY){
        for(int x = 0; x < input1.width(); x++){
            for(int y = 0; y < input1.height(); y++){
                int v1 = *(input1.data()+y*input1.width()+x);
                int v2 = *(input2.data()+y*input1.width()+x);
                value = abs(v1-v2);
                if(value <= minVal)
                    value = 0;
                if(value >= maxVal){
                    value = 255;
                }
                *(output.data()+y*input1.width()+x) = value;
            }
        }
    }else{
        //not implemented yet
    }
}
} //namespace op
void bresenhamLine(int x0, int y0, int x1, int y1,std::function<bool(int,int)> foundPixel){
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = dx+dy, e2; /* error value e_xy */

  for(;;){  /* loop */
    if(!foundPixel(x0,y0))
        break;
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

void bresenhamLine(int x0, int y0, int x1, int y1, std::vector<int> &vX, std::vector<int> &vY){
    bresenhamLine(x0,y0,x1,y1,[&vX,&vY](int x,int y){
        vX.push_back(x);
        vY.push_back(y);
        return true;
    });
}
}//namespace imaging
}//namespace lms
