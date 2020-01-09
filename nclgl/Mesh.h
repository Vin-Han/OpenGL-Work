#pragma once
#include "../../NCLGL/OGLRenderer.h"

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, 
	NORMAL_BUFFER, TANGENT_BUFFER,INDEX_BUFFER, MAX_BUFFER
};
class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	static Mesh* GenerateQuad();

	virtual void Draw();
	static Mesh* GenerateTriangle();

	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() { return texture; }
	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap() { return bumpTexture; }

protected:
	void BufferData();
	void GenerateNormals();

	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3& a, const Vector3& b,
		const Vector3& c, const Vector2& ta,
		const Vector2& tb, const Vector2& tc);

	//bisic data of array number ,VAO and drawing type
	GLuint bufferObject[MAX_BUFFER];
	GLuint type;
	GLuint arrayObject;

	//data of the array for each trangles 
	GLuint numVertices;
	Vector3* vertices;
	Vector4* colours;

	//data of the array for its textures 
	GLuint texture;
	Vector2 * textureCoords;

	//data of the array for its bump textures 
	GLuint bumpTexture;
	Vector3* normals;
	Vector3* tangents;

	//data of the array of EBO
	GLuint numIndices;
	unsigned int* indices;


};