#version 330 core

layout (location = 0) in vec3 aPos;   // ��ġ ������ attribute position 0�� �����ϴ�.
layout (location = 1) in vec3 aNormal;

void main()
{
	gl_Position = vec4 (aPos, 1.0);
}  