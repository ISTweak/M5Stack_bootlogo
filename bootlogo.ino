#include <M5Stack.h>
#include <M5StackUpdater.h>
#include <Wire.h>
#include <math.h>

#include "png_m5stack.h"
#include "png_five.h"

//x,y,角度,長さ,大きさ,色,遅延,戻りx,戻りy
void drawDiagonalLine(uint16_t x, uint16_t y, uint16_t a, uint16_t l, uint16_t s, uint16_t c, uint16_t d, uint16_t* rx, uint16_t* ry) {
	uint16_t len;
	uint16_t dx, dy;
	uint16_t kx = 1;
	uint16_t ky = 1;
	uint8_t an = 0;
	if ( a < 91 ) {
		an = a;
		ky = -1;
	} else if ( a < 181 ) {
		an = 180 - a;
		kx = -1;
		ky = -1;
	} else if ( a < 271 ) {
		an = a - 180;
		kx = -1;
	} else {
		an = 360 - a;
	}
	double xr = cos(an * M_PI / 180);
	double yr = sin(an * M_PI / 180);
	
	for ( len = 0; len < l; len++ ) {
		dx = int(len * xr);
		dy = int(len * yr);
		*rx = x + dx * kx;
		*ry = y + dy * ky;
		M5.Lcd.fillCircle(*rx, *ry, s, c);
		delay(d);
	}
}

void drawDiagonalLine2(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t s, uint16_t c, uint16_t d) {
	float sx = (x2 - x1) / 2.0;
	float sy = (y2 - y1) / 2.0;
	int stp = abs(sx) > abs(sy) ? abs(sx) : abs(sy);
	float dx = float(x2 - x1) / stp;
	float dy = float(y2 - y1) / stp;
	for (int j = 1; j <= stp; j++) {
		M5.Lcd.fillCircle(x1 + dx * j, y1 + dy * j, s, c);
		delay(d);
	}
}

//弧の起点から中心点を算出
void askCenterXY(uint16_t x, uint16_t y, uint16_t r, uint16_t a, uint16_t* cx, uint16_t* cy) {
	uint16_t dx, dy;
	dx = int(r * cos(a * M_PI / 180));
	dy = int(r * sin(a * M_PI / 180));
	*cx = x - dx;
	*cy = y - dy;
}

//x,y,半径,開始角度,終了角度,回転方向,大きさ,色,遅延,戻りx,戻りy
void drawCircleLine(uint16_t x, uint16_t y, uint16_t r, uint16_t sa, uint16_t ea, uint8_t lr, uint8_t s, uint16_t c, uint16_t d, uint16_t* rx, uint16_t* ry) {
	uint16_t tx, ty;
	if ( lr == 0 ) {
		drawCircleLineL(x, y, r, sa, ea, s, c, d, &tx, &ty);
	} else {
		drawCircleLineR(x, y, r, sa, ea, s, c, d, &tx, &ty);
	}
	*rx = tx;
	*ry = ty;
}

//左回りの弧　x,y,半径,開始角度,終了角度,大きさ,色,遅延,戻りx,戻りy
void drawCircleLineL(uint16_t x, uint16_t y, uint16_t r, uint16_t sa, uint16_t ea, uint8_t s, uint16_t c, uint16_t d, uint16_t* rx, uint16_t* ry) {
	uint16_t cx, cy, tx, ty;
	askCenterXY(x, y, r, sa, &cx, &cy);
	
	while ( ea >= sa ) {
		drawCircleLineS(cx, cy, r, sa, s, c, d, &tx, &ty);
		sa++;
	}
	*rx = tx;
	*ry = ty;
}

//右回りの弧　x,y,半径,開始角度,終了角度,大きさ,色,遅延,戻りx,戻りy
void drawCircleLineR(uint16_t x, uint16_t y, uint16_t r, uint16_t sa, uint16_t ea, uint8_t s, uint16_t c, uint16_t d, uint16_t* rx, uint16_t* ry) {
	uint16_t cx, cy, tx, ty;
	askCenterXY(x, y, r, sa, &cx, &cy);
	
	while ( sa >= ea ) {
		drawCircleLineS(cx, cy, r, sa, s, c, d, &tx, &ty);
		sa--;
	}
	*rx = tx;
	*ry = ty;
}

void drawCircleLineS(uint16_t x, uint16_t y, uint16_t r, uint16_t a, uint8_t s, uint16_t c, uint16_t d, uint16_t* rx, uint16_t* ry) {
	uint16_t sx = x + int(r * cos(a * M_PI / 180));
	uint16_t sy = y + int(r * sin(a * M_PI / 180));
	if ( *rx == sx && *ry == sy ) {
		return;
	}
	
	M5.Lcd.fillCircle(sx, sy, s, c);
	delay(d);
	*rx = sx;
	*ry = sy;
}

//x,y,上下左右,長さ,大きさ,色,遅延,戻りx,戻りy
void drawStraightLine(uint16_t x, uint16_t y, uint8_t u, int16_t l, int16_t s, uint16_t c, uint16_t d, uint16_t* rx, uint16_t* ry) {
	uint16_t len;
	int16_t p;
	
	for ( len = 0; len < l; len++ ) {
		p = len;
		if ( u == 0 || u == 2 ) {
			p *= -1;
		}
		if ( u == 0 || u == 1 ) {
			*rx = x;
			*ry = y + p;
		} else {
			*rx = x + p;
			*ry = y;
		}
		M5.Lcd.fillCircle(*rx, *ry, s, c);
		delay(d);
	}
}

void bootlogo() {
	uint16_t x, y, i;
	uint8_t d = 5;
	uint8_t s = 1;
	uint16_t c = getColor(0, 100, 160);
	
	M5.Lcd.fillScreen(WHITE);
	M5.Lcd.pushImage(130, 68, 58, 30, (uint16_t *)gimp_image.pixel_data);
	delay(300);
	
	drawDiagonalLine(203, 77, 154, 45, s, c, d, &x, &y);
	drawCircleLine(x, y, 15, 285, 255, 1, s, c, 1, &x, &y);
	drawDiagonalLine(x, y, 206, 58, s, c, d, &x, &y);
	drawDiagonalLine(x, y, 335, 56, s, c, d, &x, &y);
	drawCircleLine(x, y, 15, 105, 75, 1, s, c, 1, &x, &y);
	drawDiagonalLine(x, y, 26, 56, s, c, d, &x, &y);
	drawStraightLine(x, y, 1, 38, s, c, d, &x, &y);
	drawDiagonalLine(x, y, 208, 56, s, c, d, &x, &y);
	drawCircleLine(x, y, 15, 75, 105, 0, s, c, 1, &x, &y);
	drawDiagonalLine(x, y, 156, 55, s, c, d, &x, &y);
	drawStraightLine(x, y, 0, 28, s, c, d, &x, &y);
	delay(200);

	for ( i=240; i>159; i--) {
		M5.Lcd.pushImage(92, i, 138, 25, (uint16_t *)GIMP_IMAGE_PIXEL_DATA);
	}
}

uint16_t getColor(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);
}

void setup() {
	M5.begin();
	Wire.begin();
	if(digitalRead(BUTTON_A_PIN) == 0) {
		Serial.println("Will Load menu binary");
		updateFromFS(SD);
		ESP.restart();
	}
	bootlogo();
}

void loop() {
	M5.update();
	if ( M5.BtnC.wasReleased() ) {
		bootlogo();
	}

}
