# Shaders uniforms and ins #

To make shaders making easier and faster, Skylium offers some built-in uniforms and ins variables instead of those offered by OpenGL (they are deprecated, anyway).


**Variables available for both - vertex and fragment shaders:**
```
uniform vec4 sDefColor;
```
It is colour of the vertex, it is the same for whole mesh. Skylium does not support color matrices, because it is not needed, since we have textures;

---

```
uniform mat4 sModelViewMatrix;
```
It is model-view matrix, after whole transformations - translate, scale, rotate.

---

```
uniform mat4 sProjectionMatrix;
```
Projection matrix. Generated after calling `MatricesManager::sPerspective()`.

---

```
uniform mat4 sModelViewProjectionMatrix;
```
Result of multiplying `sModelViewMatrix` by `sProjectionMatrix`. However, it is calculated in skylium, before calling `Mesh::draw()` to avoid counting the same matrix hundreds of times.
To have your vertex on the correct position in 3D view, you have to multiply vertex's position (given to shader as `sVertex` - see below) by this matrix.

---

```
uniform mat3 sNormalMatrix;
```
This matrix is generated after calling `MatricesManager::sLookAt()`, but before any Object-related transformations. It is transpose of the inverse of `sModelViewMatrix`.
For explanation, see http://www.cs.uaf.edu/2007/spring/cs481/lecture/01_23_matrices.html.

---

```
struct sMaterialParams {
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform sMaterialParams sFrontMaterial;
```
In this variable you have access to any of the material's parameters. Where does Skylium take these parameters from? From .mtl connected with .obj file.

---

```
struct sLightParams {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
};

uniform sLightParams sLightSource[7];
```
There are some lights' params. I know it is not everything that OpenGL offers, but it has to be enough, for now.

---

```
struct sLightModelParameters {
	vec4 ambient;
};

uniform sLightModelParameters sLightModel;
```
This is being set by `Scene` class. Useful.

---


**Variables available only for vertex shaders:**
```
in vec4 sVertex;
```
Vertex position. It is (x, y, z, 1) - as OpenGL specifies.

---

```
in vec3 sNormal;
```
Normal vector. **When using it, remember to multiply it by `sNormalMatrix`!** Skylium won't do that for you.

---

```
in vec2 sTexCoords;
```
Current texture coordinates. As for now, Skylium does not support more than texture for mesh. But it will, for sure!

---

```
smooth out vec2 sVaryingTexCoords;
```
If you want to use texture coords in fragment shader, you have to use this variable. Just write
```
sVaryingTexCoords = sTexCoords;
```
and that's it. Nothing more is needed.

---


**Variables available only for fragment shaders:**
```
uniform sampler2D textureUnit;
```
This is current texture color map. Set by `Texture::setTexture()`.

---

```
uniform sampler2D normalMap;
```
There you can find the normal map (if available).

---

```
smooth in vec2 sVaryingTexCoords;
```
Texture coordinates given by vertex shader.

---

```
out vec4 sFragColor;
```
It defines the final fragment colour. Use it instead of `gl_FragColor`.

---


Skylium does not support any "derived states" in shaders (except the sNormalMatrix). So, if you want such parameter, ie
```
gl_FrontLightModelProduct.sceneColor
```
just go to GLSL spec, find where does it come from, and get that manually.
```
sFrontMaterial.emission + sFrontMaterial.ambient * sLightModel.ambient
```