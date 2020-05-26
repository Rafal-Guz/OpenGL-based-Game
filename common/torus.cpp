#include "torus.h"


Torus::Torus(){};

Torus::Torus(Vector2f in_radius, Vector3f in_position)
: radius(in_radius), position(in_position)
{};

Torus::~Torus()
{};

void Torus::draw(){
	TempMatrix = ModelViewMatrix;
	TempMatrix.translate(position.x, position.y, position.z);

	glUniformMatrix4fv(
	MVMatrixUniformLocation,  	//Uniform location
	1,					        //Number of Uniforms
	false,				        //Transpose Matrix
  TempMatrix.getPtr());	        //Pointer to Matrix Values
	glutSolidTorus(radiu.x,radius.y,50,50);
};

/*friend std::istream& operator>>(std::istream &in, Torus &torus){
	in >> radius >> position.x >> position.y >> position.z;
	return in;
};*/
