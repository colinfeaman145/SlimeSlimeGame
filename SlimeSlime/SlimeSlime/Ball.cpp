#include "Ball.hpp"
#include "game.hpp"
#include <random>


using namespace std;

Ball::Ball() {
	static mt19937 gen(random_device{}());
	uniform_int_distribution<int> sizeDist(5, 30);
	uniform_int_distribution<int> colorDist(0, 1);
	uniform_real_distribution<double> vecDist(-.1, .1);

	int size = sizeDist(gen);
	uniform_int_distribution<int> XDist(size, X - size);

	dest.w = dest.h = size; //size
	isGreen = colorDist(gen); //color

	if (isGreen)			//location
		dest.y = vec.y = 0 + size;
	else
		dest.y = vec.y = Y - size;
	dest.x = vec.x = XDist(gen);

	//direciton
	vec.vx = vecDist(gen);
	vec.vy = vecDist(gen);
}

void Ball::UpdateBall() {
	if (UpdateBallPosition()) {
		//delete ball
		//make new one
	}

}

bool Ball::UpdateBallPosition() {
	CheckBounce();
	CheckCollision();
}

bool Ball::CheckCollision() {

}

void Ball::CheckBounce()
{
	if (vec.x <= 0) {//outside left
		vec.x = 0;
		vec.vx *= -1;//flip trajectory
	}
	else if (vec.x >= X - dest.x) {//outside right
		vec.x = X - dest.x;
		vec.vx *= -1;//flip trajectory
	}

	if (vec.y <= 0) {//outside top
		vec.y = 0;
		vec.vy *= -1;//flip trajectory
	}
	else if (vec.y >= Y - dest.x) {//outside bottom
		vec.y = Y - dest.x;
		vec.vy *= -1;//flip trajectory
	}
}