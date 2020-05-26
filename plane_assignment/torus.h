#include <iostream>
#include <Vector.h>


class Torus {
private:
	Vector3f position;
	Vector2f radius;
public:
	Torus();
	~Torus();
	void draw();

	friend istream& operator>>(istream &in, Torus &torus);
};
