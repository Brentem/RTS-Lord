#include "2DMap.h"

const int backgroundWith = 3000;
const int backgroundHeight = 2000;

Texture2D Map2DGetBackground(){

    // make a huge image for the map, a checkerboard with a red outline

	Image img = GenImageChecked(backgroundWith, backgroundHeight, 128, 128, BLACK, DARKBLUE);
	ImageDrawRectangleLines(&img, (Rectangle) { 0, 0, backgroundWith, backgroundHeight }, 10, RED);
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

