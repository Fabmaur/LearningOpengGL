#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* vertices, const unsigned int size); //Size of vertices array (in bytes)
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int id;
};
