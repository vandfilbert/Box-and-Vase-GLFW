#include<iostream>
#include<stdlib.h>
#include<glew.h>
#include<glfw3.h>
#include<math.h>
#include "glm/ext.hpp"
using namespace std;

const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;

class Vertex {
public:
	GLfloat x, y, z;
	Vertex(GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vertex operator-(const Vertex& vertex) {
		Vertex temp(x, y, z);
		temp.x -= vertex.x;
		temp.y -= vertex.y;
		temp.z -= vertex.z;
		return temp;
	}
	Vertex operator+(const Vertex& vertex) {
		Vertex temp(x, y, z);
		temp.x += vertex.x;
		temp.y += vertex.y;
		temp.z += vertex.z;
		return temp;
	}
	void operator=(const Vertex& vertex) {
		x = vertex.x;
		y = vertex.y;
		z = vertex.z;
	}
	void normalize() {
		float length = pow(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0), 0.5);
		x /= length;
		y /= length;
		z /= length;
	}
};

Vertex getRotationResult(const Vertex& pivot, const Vertex& vector, float angle, Vertex point, bool isEuler = false) {
	Vertex temp, newPosition;
	temp = point - pivot;
	if (isEuler)
		temp = point;
	else
		temp = point - pivot;

	newPosition.x = temp.x * (cos(angle) + pow(vector.x, 2.0f) * (1.0f - cos(angle))) + temp.y * (vector.x * vector.y * (1.0f - cos(angle)) - vector.z * sin(angle)) + temp.z * (vector.x * vector.z * (1.0f - cos(angle)) + vector.y * sin(angle));
	newPosition.y = temp.x * (vector.x * vector.y * (1.0f - cos(angle)) + vector.z * sin(angle)) + temp.y * (cos(angle) + pow(vector.y, 2.0f) * (1.0f - cos(angle))) + temp.z * (vector.y * vector.z * (1.0f - cos(angle)) - vector.x * sin(angle));
	newPosition.z = temp.x * (vector.x * vector.z * (1.0f - cos(angle)) - vector.y * sin(angle)) + temp.y * (vector.y * vector.z * (1.0f - cos(angle)) + vector.x * sin(angle)) + temp.z * (cos(angle) + pow(vector.z, 2.0f) * (1.0f - cos(angle)));

	if (isEuler)
		temp = newPosition;
	else
		temp = newPosition + pivot;

	temp = newPosition + pivot;
	return temp;
}

int getPascal(int row, int col) {
	if (col > row)
		return 0;
	else if (col == 0 || row == 0)
		return 1;
	return getPascal(row - 1, col - 1) + getPascal(row - 1, col);
}

class Shape {
protected:
	int pointSize; //Menghitung jumlah titik
	Vertex* points; //Menyimpan koordinat setiap titik
	Vertex position; //titik tengah shape yang dibuat
	Vertex euler[3];
	GLuint buffer, shader, outlineShader;
	GLuint MatrixID;
	glm::mat4 mvp;
	float Angle;
public:
	Shape(GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0) {
		position = Vertex(_x, _y, _z);
		euler[0] = Vertex(1, 0, 0);
		euler[1] = Vertex(0, 1, 0);
		euler[2] = Vertex(0, 0, 1);
		Angle = 0;
	}
	Vertex getPosition() {
		return position;
	}
	int getPointSize() {
		return pointSize;
	}
	Vertex* getPoints() {
		return points;
	}
	void showPoints() {
		for (int i = 0; i < pointSize; i++) {
			printf("%f, %f, %f", points[i].x, points[i].y, points[i].z);
		}
	}
	void initiateBuffer() {
		glGenBuffers(1, &buffer);
		setArrayBuffer();
	}
	void setArrayBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, getPointSize() * sizeof(GL_FLOAT) * 3,
			getPoints(), GL_STATIC_DRAW);
	}
	void bindBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			0            // array buffer offset
		);
	}
	void initiateShader(char vertex[], char fragment[]) {
		shader = LoadShaders(vertex, fragment);
		glm::mat4 Projection = glm::perspective(90.0f * DEG_TO_RAD, 1.0f, 0.1f, 100.0f);

		// Camera matrix
		glm::mat4 View = glm::lookAt(
			//glm::vec3(1, 3, 1), //Eye position at (0, 1, -1)
			glm::vec3(1.75, 2, 1),
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);

		// Our ModelViewProjection : multiplication of our 3 matrices
		mvp = Projection * View * Model;

		MatrixID = glGetUniformLocation(shader, "MVP");
	}
	void initiateOutlineShader(char vertex[], char fragment[]) {
		outlineShader = LoadShaders(vertex, fragment);
	}
	void drawPolygon()
	{
		glUseProgram(shader);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		bindBuffer();
		glDrawArrays(GL_TRIANGLES, 0, getPointSize());
	}
	void drawPolyline()
	{
		glUseProgram(outlineShader);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		bindBuffer();
		glDrawArrays(GL_LINES, 0, getPointSize());
	}
	void rotate(Vertex pivot, Vertex vector, float angle)
	{
		angle = angle * DEG_TO_RAD;

		for (int i = 0; i < pointSize; i++) {
			points[i] = getRotationResult(pivot, vector, angle, points[i]);
		}
		for (int i = 0; i < 3; i++) {
			euler[i] = getRotationResult(pivot, vector, angle, euler[i]);
		}
		position = getRotationResult(pivot, vector, angle, position);
		setArrayBuffer();
	}
	void translate(const Vertex& movement) {
		for (int i = 0; i < pointSize; i++) {
			points[i] = points[i] + movement;
		}
		position = position + movement;
		setArrayBuffer();
	}
	float getAngle() {
		return Angle;
	}
	Vertex getEuler(int index) {
		return euler[index];
	}
	void resetEuler() {
		euler[0] = Vertex(1, 0, 0);
		euler[1] = Vertex(0, 1, 0);
		euler[2] = Vertex(0, 0, 1);
	}
	~Shape() {
		delete points;
		glDeleteBuffers(1, &buffer);
	}
};

class Triangle : public Shape {
public:
	Triangle() {
		pointSize = 3;
		points = new Vertex[pointSize];
	}
	Triangle(Vertex _points[3], GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0) : Shape(_x, _y, _z) {
		pointSize = 3;
		points = new Vertex[pointSize];
		for (int i = 0; i < pointSize; i++) {
			points[i] = _points[i];
		}
	}
	void setPoints(const Vertex pts[3]) {
		for (int i = 0; i < 3; i++) {
			points[i] = pts[i];
		}
	}
	void setPosition(const Vertex& _position) {
		position = _position;
	}
};

class Circle : public Shape {
	float radius;
	float step;
	float scale;
public:
	Circle(float _x = 0, float _y = 0, float _z = 0, int _pointSize = 1, float _radius = 1, float _scale = 1) : Shape(_x, _y, _z) {
		scale = _scale;
		pointSize = _pointSize * 3;
		points = new Vertex[pointSize];
		radius = _radius;
		step = 2 * PI * scale / _pointSize;
		generate();
	}
	void generate() {
		float i = -PI;
		float end = i + 2 * PI * scale;
		int j = 0;
		for (; i <= end; i += step, j += 3) {
			float x = cos(i) * radius + position.x;
			float y = sin(i) * radius + position.y;
			float z = 0;

			float next_x = cos(i + step) * radius + position.x;
			float next_y = sin(i + step) * radius + position.y;
			float next_z = 0;

			points[j] = Vertex(x, y, z);
			points[j + 1] = position;
			points[j + 2] = Vertex(next_x, next_y, next_z);
		}
	}
};

class Box : public Shape {
	Vertex position;
	Vertex pts[12][3];
	Triangle* triangles;
	float length, width, height;
	float scale;
	int smoothing, baris, kolom;
public:
	Box(float _x = 0, float _y = 0, float _z = 0, float _length = 0.12, float _width = 0.12, float _height = 0.12) : Shape(_x, _y, _z) {
		position = Vertex(_x, _y, _z);
		length = _length;
		width = _width;
		height = _height;
		triangles = new Triangle[12];
		scale = 1.0;
		pointSize = 36;
		points = new Vertex[pointSize];
		smoothing = 10;
		Vertex pts[12][3] = {
			{//Front - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Front - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Back - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Back - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Top - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2)
			},
			{//Top - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2)
			},
			{//Bottom - 1
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Bottom - 2
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Left - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Left - 2
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Right - 1
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Right - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
			}
		};
		int index = 0;
		for (int i = 0; i < 12; i++) {
			for (int j = 0; j < 3; j++)
				points[index++] = pts[i][j];
		}
	}
	Vertex getPosition() {
		return position;
	}
	void initiateBuffer() {
		for (int i = 0; i < 12; i++)
			triangles[i].initiateBuffer();
	}
	void initiateShader(char vertex[], char fragment[]) {
		for (int i = 0; i < 12; i++)
			triangles[i].initiateShader(vertex, fragment);
	}
	void initiateOutlineShader(char vertex[], char fragment[]) {
		for (int i = 0; i < 12; i++)
			triangles[i].initiateOutlineShader(vertex, fragment);
	}
	void drawPolygon() {
		for (int i = 0; i < 12; i++)
			triangles[i].drawPolygon();
	}
	void drawPolyline() {
		for (int i = 0; i < 12; i++)
			triangles[i].drawPolyline();
	}
	void rotate(const Vertex& pivot, const Vertex& vector, float angle) {
		for (int i = 0; i < 12; i++)
			triangles[i].rotate(pivot, vector, angle);
	}
};

class Box1 : public Shape {
	Vertex position;
	Vertex pts[12][3];
	Triangle* triangles;
	float length, width, height;
	float step, stepInner;
	float scale;
	int smoothing, baris, kolom;
public:
	Box1(float _x = 0, float _y = 0, float _z = 0, float _length = 0.37, float _width = 0.37, float _height = 0.37) : Shape(_x, _y, _z) {
		position = Vertex(_x, _y, _z);
		length = _length;
		width = _width;
		height = _height;
		triangles = new Triangle[12];
		scale = 2.5;
		pointSize = 12 * 10 * 3.0f * 2.0f;
		points = new Vertex[pointSize];
		step = 2.0 * PI * scale / (float)25;
		smoothing = 10;
		stepInner = PI / (float)smoothing;
		//Vertex pts[12][3] = {
		//	{//Front - 1
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
		//	},
		//	{//Front - 2
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
		//	},
		//	{//Back - 1
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2)
		//	},
		//	{//Back - 2
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2)
		//	},
		//	{//Top - 1
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2)
		//	},
		//	{//Top - 2
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2)
		//	},
		//	{//Bottom - 1
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
		//	},
		//	{//Bottom - 2
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
		//	},
		//	{//Left - 1
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2)
		//	},
		//	{//Left - 2
		//		Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
		//		Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
		//	},
		//	{//Right - 1
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2)
		//	},
		//	{//Right - 2
		//		Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
		//		Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
		//	}
		//};

		for (int i = 0; i < 12; i++) {
			triangles[i].setPoints(pts[i]);
			triangles[i].setPosition(position);
		}
		generate();
	}
	void generate() {
		float i = -PI;
		float end = i + 2.0 * PI * scale;
		int l = 0, j = 1;
		Vertex pts[12][3] = {
			{//Front - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Front - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Back - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Back - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Top - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2)
			},
			{//Top - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2)
			},
			{//Bottom - 1
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Bottom - 2
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Left - 1
				Vertex(position.x - length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Left - 2
				Vertex(position.x - length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x - length / 2, position.y - height / 2, position.z - width / 2)
			},
			{//Right - 1
				Vertex(position.x + length / 2, position.y + height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2)
			},
			{//Right - 2
				Vertex(position.x + length / 2, position.y + height / 2, position.z - width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z + width / 2),
				Vertex(position.x + length / 2, position.y - height / 2, position.z - width / 2)
			}
		};
		for (baris = 0; i < end && l < pointSize; i += step, j++, baris++) {
			float k = -PI;
			float endInner = k + PI;
			for (kolom = 0; k <= endInner; k += stepInner, l += 6, kolom++) {
				/*float cur_x = cos(k) * 0.5 + position.x;
				float cur_y = sin(k) * 0.5 + position.y;
				float cur_z = position.z;*/
				float cur_x = cos(k) * 0.3 + pts[baris][kolom].x;
				float cur_y = sin(k) * 0.3 + pts[baris][kolom].y;
				float cur_z = pts[baris][kolom].z;

				/*float next_x = cos(k + stepInner) * 0.5 + position.x;
				float next_y = sin(k + stepInner) * 0.5 + position.y;
				float next_z = position.z;*/
				float next_x = cos(k + stepInner) * 0.3 + pts[baris][kolom].x;
				float next_y = sin(k + stepInner) * 0.3 + pts[baris][kolom].y;
				float next_z = pts[baris][kolom].z;

				points[l] = getRotationResult(position, Vertex(0, 1, 0), i, Vertex(cur_x, cur_y, cur_z));
				points[l + 1] = getRotationResult(position, Vertex(0, 1, 0), i, Vertex(next_x, next_y, next_z));
				points[l + 2] = getRotationResult(position, Vertex(0, 1, 0), i + step, Vertex(cur_x, cur_y, cur_z));
				points[l + 3] = getRotationResult(position, Vertex(0, 1, 0), i, Vertex(next_x, next_y, next_z));
				points[l + 4] = getRotationResult(position, Vertex(0, 1, 0), i + step, Vertex(cur_x, cur_y, cur_z));
				points[l + 5] = getRotationResult(position, Vertex(0, 1, 0), i + step, Vertex(next_x, next_y, next_z));
			}
			if (!(l <= 6 * smoothing * j))
				l = l - 6;
		}
		pointSize = l;
	}
	Vertex getPosition() {
		return position;
	}
	void initiateBuffer() {
		for (int i = 0; i < 12; i++)
			triangles[i].initiateBuffer();
	}
	void initiateShader(char vertex[], char fragment[]) {
		for (int i = 0; i < 12; i++)
			triangles[i].initiateShader(vertex, fragment);
	}
	void initiateOutlineShader(char vertex[], char fragment[]) {
		for (int i = 0; i < 12; i++)
			triangles[i].initiateOutlineShader(vertex, fragment);
	}
	void drawPolygon() {
		for (int i = 0; i < 12; i++)
			triangles[i].drawPolygon();
	}
	void drawPolyline() {
		for (int i = 0; i < 12; i++)
			triangles[i].drawPolyline();
	}
	void rotate(const Vertex& pivot, const Vertex& vector, float angle) {
		for (int i = 0; i < 12; i++)
			triangles[i].rotate(pivot, vector, angle);
	}
};

class Ovaloid : public Shape {
	Vertex radius;
	float step, stepInner;
	float scale;
	int smoothing;
public:
	Ovaloid(float _x = 0, float _y = 0, float _z = 0, int _pointSize = 10, Vertex _radius = Vertex(0.3, 0.3), float _scale = 1.0, float _smoothing = 10) : Shape(_x, _y, _z) {
		scale = _scale;
		smoothing = _smoothing;
		radius = _radius;
		pointSize = _pointSize * _smoothing * 3.0f * 2.0f;
		points = new Vertex[pointSize];
		step = 2.0 * PI * scale / (float)_pointSize;
		stepInner = PI / (float)smoothing;
		generate();
	}
	void generate() {
		float i = -PI;
		float end = i + 2.0 * PI * scale;
		int l = 0, j = 1;
		for (; i < end && l < pointSize; i += step, j++) {
			float k = -PI;
			float endInner = k + PI;
			for (; k < endInner; k += stepInner, l += 6) {
				float cur_x = cos(k) * radius.x + position.x;
				float cur_y = sin(k) * radius.y + position.y;
				float cur_z = position.z;

				float next_x = cos(k + stepInner) * radius.x + position.x;
				float next_y = sin(k + stepInner) * radius.y + position.y;
				float next_z = position.z;

				points[l] = getRotationResult(position, Vertex(1, 0, 0), i, Vertex(cur_x, cur_y, cur_z));
				points[l + 1] = getRotationResult(position, Vertex(1, 0, 0), i, Vertex(next_x, next_y, next_z));
				points[l + 2] = getRotationResult(position, Vertex(1, 0, 0), i + step, Vertex(cur_x, cur_y, cur_z));
				points[l + 3] = getRotationResult(position, Vertex(1, 0, 0), i, Vertex(next_x, next_y, next_z));
				points[l + 4] = getRotationResult(position, Vertex(1, 0, 0), i + step, Vertex(cur_x, cur_y, cur_z));
				points[l + 5] = getRotationResult(position, Vertex(1, 0, 0), i + step, Vertex(next_x, next_y, next_z));
			}
			if (!(l <= 6 * smoothing * j))
				l = l - 6;
		}
		pointSize = l;
	}
};

class Vase : public Shape {
	Vertex* pts;
	int ptsCount;
	float* berzierConst, step, stepInner;
	float scale;
	int smoothing;
public:
	Vase(Vertex _pts[], int _ptsCount, float _x = 0, float _y = 0, float _z = 0, int _pointSize = 10, float _scale = 1.0, float _smoothing = 10) : Shape(_x, _y, _z) {
		scale = _scale;
		smoothing = _smoothing;
		pointSize = _pointSize * smoothing * 3.0 * 2.0;
		points = new Vertex[pointSize];
		step = 2.0 * PI * scale / (float)_pointSize;
		stepInner = 1.0 / (float)(smoothing - 1);

		//Control points
		ptsCount = _ptsCount;
		pts = new Vertex[ptsCount];
		for (int i = 0; i < ptsCount; i++)
			pts[i] = _pts[i] + position;
		berzierConst = new float[ptsCount];
		for (int i = 0; i < ptsCount; i++)
			berzierConst[i] = getPascal(ptsCount - 1, i);
		generate();
	}
	void generate() {
		float i = -PI;
		float end = i + 2.0 * PI * scale;
		int l = 0, j = 1;
		for (int; i < end && l < pointSize; i += step, j++) {
			float k = 0;
			float endInner = 1.0;
			for (; k <= endInner; k += stepInner, l += 6) {
				float cur_x = 0, cur_y = 0, cur_z = 0;
				for (int a = 0; a < ptsCount; a++) {
					float multiplier = pow(1.0 - k, ptsCount - a - 1) * pow(k, a) * berzierConst[a];
					cur_x += multiplier * pts[a].x;
					cur_y += multiplier * pts[a].y;
					cur_z += multiplier * pts[a].z;
				}

				float next_x = 0, next_y = 0, next_z = 0;
				for (int a = 0; a < ptsCount; a++) {
					float multiplier = pow(1.0 - (k + stepInner), ptsCount - a - 1) * pow((k + stepInner), a) * berzierConst[a];
					next_x += multiplier * pts[a].x;
					next_y += multiplier * pts[a].y;
					next_z += multiplier * pts[a].z;
				}

				points[l] = getRotationResult(position, Vertex(0, 1, 0), i, Vertex(cur_x, cur_y, cur_z));
				points[l + 1] = getRotationResult(position, Vertex(0, 1, 0), i, Vertex(next_x, next_y, next_z));
				points[l + 2] = getRotationResult(position, Vertex(0, 1, 0), i + step, Vertex(cur_x, cur_y, cur_z));
				points[l + 3] = getRotationResult(position, Vertex(0, 1, 0), i, Vertex(next_x, next_y, next_z));
				points[l + 4] = getRotationResult(position, Vertex(0, 1, 0), i + step, Vertex(cur_x, cur_y, cur_z));
				points[l + 5] = getRotationResult(position, Vertex(0, 1, 0), i + step, Vertex(next_x, next_y, next_z));
			}
			if (!(l <= 6 * smoothing * j))
				l = l - 6;
		}
		pointSize = l;
	}
};

class Hierarchy {
	Shape* parent;
	Hierarchy** children;
	int childCount;
	float Angle;
public:
	Hierarchy(Shape* _parent = NULL) {
		parent = _parent;
		childCount = 0;
		Angle = 0;
	}
	void setParent(Shape* _parent) {
		parent = _parent;
	}
	void addChild(Hierarchy* child) {
		Hierarchy** temp = children;
		children = new Hierarchy * [childCount + 1];
		for (int i = 0; i < childCount; i++)
			children[i] = temp[i];
		children[childCount] = child;
		childCount++;
	}
	void translate(const Vertex& movement) {
		parent->translate(movement);
		for (int i = 0; i < childCount; i++)
			children[i]->translate(movement);
	}
	void rotate(const Vertex& pivot, const Vertex& vector, float angle) {
		parent->rotate(pivot, vector, angle);
		for (int i = 0; i < childCount; i++)
			children[i]->rotate(pivot, vector, angle);
		this->Angle += angle;
	}
	void drawPolygon() {
		parent->drawPolygon();
		for (int i = 0; i < childCount; i++)
			children[i]->drawPolygon();
	}
	void drawPolyline() {
		parent->drawPolyline();
		for (int i = 0; i < childCount; i++)
			children[i]->drawPolyline();
	}
	void initiateBuffer() {
		parent->initiateBuffer();
		for (int i = 0; i < childCount; i++)
			children[i]->initiateBuffer();
	}
	void initiateShader(char vertex[], char fragment[]) {
		parent->initiateShader(vertex, fragment);
		for (int i = 0; i < childCount; i++)
			children[i]->initiateShader(vertex, fragment);
	}
	void initiateOutlineShader(char vertex[], char fragment[]) {
		parent->initiateOutlineShader(vertex, fragment);
		for (int i = 0; i < childCount; i++)
			children[i]->initiateOutlineShader(vertex, fragment);
	}
	void resetEuler() {
		parent->resetEuler();
		for (int i = 0; i < childCount; i++)
			children[i]->resetEuler();
	}
	float getAngle() {
		return Angle;
	}
	Shape* getParent() {
		return parent;
	}
	Hierarchy* getChild(int index) {
		return children[index];
	}
};