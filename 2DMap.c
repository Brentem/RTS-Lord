#include "2DMap.h"

const int backgroundWith = 3200;
const int backgroundHeight = 1216;

Texture2D Map2DGetBackground(){

    // make a huge image for the map, a checkerboard with a red outline

	Image img = GenImageChecked(backgroundWith, backgroundHeight, 32, 32, LIGHTGRAY, BEIGE);
	ImageDrawRectangleLines(&img, (Rectangle) { 0, 0, backgroundWith, backgroundHeight }, 10, RED);
	ImageDrawCircle(&img, backgroundWith/2, backgroundHeight/2, 50, GREEN);
	Texture2D background = LoadTextureFromImage(img);
	UnloadImage(img);

    return background;

}

int Map2dGetWidth(){
	return backgroundWith;
}

int Map2dGetHeight(){
	return backgroundHeight;
}

