#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>


typedef struct coords{
	int x;
	int y;
}coords;

typedef struct Matrix2D{
	coords i;
	coords j;
} Matrix2D;

typedef struct screen{
	uint64_t pixels[64];
} screen;

typedef struct object{
	uint64_t pixels[64];
	coords position;
} object;

void makeSquare(object* obj, coords start, coords end){
	if(start.x > end.x){
		uint8_t ph = start.x;
		start.x = end.x;
		end.x = ph;
	}
	if(start.y > end.y){
		uint8_t ph = start.y;
		start.y = end.y;
		end.y = ph;
	}
	for(int i = start.y;i < end.y;i++){
		for(int j = start.x;j < end.x; j++){
			obj -> pixels[i] ^= 1ULL << j;
		}
	}

}

void drawObject(object obj){
	for(int i = 0; i < 64; i++){
		printf("\n");
		for(int j = 0; j < 64; j++){
			if(i+obj.position.y > 64 || i+obj.position.y < 0 || j+obj.position.x > 64 || j+obj.position.x < 0){
				continue;
			}
			if(obj.pixels[i] & (1ULL<<j)){
				printf("[]");
			}else{
				printf("..");
			}
		}
	}
}

void addObject(object obj,screen *scr){
	for(int i = 0; i < 64; i++){
		uint64_t row;
		if(i + obj.position.y > 64){
			break;
		}else if (i + obj.position.y < 0) {
			continue;
		}

		if(obj.position.x < 0){
			row = obj.pixels[i] << -obj.position.x;
		}else{
			row = obj.pixels[i] >> obj.position.x;
		}
		scr -> pixels[i + obj.position.y] |= row;
	}
}

void drawScreen(screen obj){
	for(int i = 0; i < 64; i++){
		printf("\n");
		for(int j = 0; j < 64; j++){
			if(obj.pixels[i] & (1ULL<<j)){
				printf("[]");
			}else{
				printf("..");
			}
		}
	}
}

void moveObject(object *obj,int x,int y){
	obj ->position.x += x;
	obj ->position.y += y;
}
void transform(screen *obj,Matrix2D m){
	const int a = -32;
	//printf("i:[%d %d] j:[%d %d]", m.i.x,m.i.y,m.j.x,m.j.y);
	uint64_t* result = calloc(64,sizeof(uint64_t));
	coords point;
	for(int i = 0; i < 64;i++){
		for(int j = 0; j < 64; j++){
			if(!(obj -> pixels[i] & (1ULL << j))){
				continue;
			}
			point.x = (j+a)*m.i.x + (i+a)*m.j.x - a;
			point.y = (j+a)*m.i.y + (i+a)*m.j.y - a;
			if(point.y < 0 || point.y >64 || point.x < 0 || point.x > 64){
				continue;
			}
			result[point.y] |= 1ULL << point.x;
		}
	}
	memcpy(obj->pixels,result,sizeof(uint64_t) * 64);
}
void gameLoop(){
}

int main(){
	bool gameOn =true;
	const int width = 64;
	const int height = 64;
//	for(int i = 0; i < height; i++){
//		printf("\n");
//		for(int j = 0; j < width; j++){
//			printf("..");
//		}
//	}
	Matrix2D turn = {(coords){0,-1},(coords){1,0}};
	screen* Screen =  malloc(sizeof(screen));
	object* Square = malloc(sizeof(object));
	object* Square2 = malloc(sizeof(object));
	Square -> position = (coords){-12,-12};
	Square2 -> position = (coords){0,0};
	coords start = {22,22};
	coords end = {42,42};
	makeSquare(Square,start ,end);
	makeSquare(Square2,start ,end);
	addObject(*Square, Screen);
	addObject(*Square2,Screen);
	drawScreen(*Screen);
	//for(int i = 0; i < 10; i++){
		sleep(3);
		system("clear");
//		moveObject(Square,2,2);
//		moveObject(Square2,-2,-2);
//		addObject(*Square2, Screen);
//		addObject(*Square, Screen);
		transform(Screen,turn);
		drawScreen(*Screen);
		//memset(Screen,0,sizeof(screen));
	//}
	//drawObject(*Square);
}
