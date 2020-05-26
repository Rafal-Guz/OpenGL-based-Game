#include "torus.h"

Torus::Torus()
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

friend istream& operator>>(istream &in, Torus &torus){
	in >> radius.x >> radius.y >> position.x >> position.y >> position.z;
	return in;
};
