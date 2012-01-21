#version 330

/* Non- textured shading */

out vec3 N;
out vec3 v;

void main() {
	v = vec3(sModelViewMatrix * sVertex);
	N = normalize(sNormalMatrix * sNormal);

	gl_Position = sModelViewProjectionMatrix * sVertex;
}
