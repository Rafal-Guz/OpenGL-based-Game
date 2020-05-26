#include <iostream>
#include <Vector.h>


class Torus {
private:
	Vector3f position;
	Vector2f radius;
public:
	Torus();
	Torus(Vector2f in_radius, Vector3f in_position);
	~Torus();
	void draw();

	//friend std::istream& operator>>(std::istream &in, Torus &torus);
};
