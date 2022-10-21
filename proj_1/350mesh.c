/*
	350mesh.c
	Creates the meshMesh struct and defines methods to implement it, including meshRender. 
	Differs from 330mesh.c by implementing near-plane clipping.
	Edited by Cole Weinstein and Robbie Young. Written by Josh Davis for Carleton College's CS311 - Computer Graphics.
*/


/*** Creating and destroying ***/

/* Feel free to read the struct's members, but don't write them, except through 
the accessors below such as meshSetTriangle, meshSetVertex. */
typedef struct meshMesh meshMesh;
struct meshMesh {
	int triNum, vertNum, attrDim;
	int *tri;						/* triNum * 3 ints */
	double *vert;					/* vertNum * attrDim doubles */
};

/* Initializes a mesh with enough memory to hold its triangles and vertices. 
Does not actually fill in those triangles or vertices with useful data. When 
you are finished with the mesh, you must call meshFinalize to deallocate its 
backing resources. */
int meshInitialize(meshMesh *mesh, int triNum, int vertNum, int attrDim) {
	mesh->tri = (int *)malloc(triNum * 3 * sizeof(int) +
		vertNum * attrDim * sizeof(double));
	if (mesh->tri != NULL) {
		mesh->vert = (double *)&(mesh->tri[triNum * 3]);
		mesh->triNum = triNum;
		mesh->vertNum = vertNum;
		mesh->attrDim = attrDim;
	}
	return (mesh->tri == NULL);
}

/* Sets the trith triangle to have vertex indices i, j, k. */
void meshSetTriangle(meshMesh *mesh, int tri, int i, int j, int k) {
	if (0 <= tri && tri < mesh->triNum) {
		mesh->tri[3 * tri] = i;
		mesh->tri[3 * tri + 1] = j;
		mesh->tri[3 * tri + 2] = k;
	}
}

/* Returns a pointer to the trith triangle. For example:
	int *triangle13 = meshGetTrianglePointer(&mesh, 13);
	printf("%d, %d, %d\n", triangle13[0], triangle13[1], triangle13[2]); */
int *meshGetTrianglePointer(const meshMesh *mesh, int tri) {
	if (0 <= tri && tri < mesh->triNum)
		return &mesh->tri[tri * 3];
	else
		return NULL;
}

/* Sets the vertth vertex to have attributes attr. */
void meshSetVertex(meshMesh *mesh, int vert, const double attr[]) {
	int k;
	if (0 <= vert && vert < mesh->vertNum)
		for (k = 0; k < mesh->attrDim; k += 1)
			mesh->vert[mesh->attrDim * vert + k] = attr[k];
}

/* Returns a pointer to the vertth vertex. For example:
	double *vertex13 = meshGetVertexPointer(&mesh, 13);
	printf("x = %f, y = %f\n", vertex13[0], vertex13[1]); */
double *meshGetVertexPointer(const meshMesh *mesh, int vert) {
	if (0 <= vert && vert < mesh->vertNum)
		return &mesh->vert[vert * mesh->attrDim];
	else
		return NULL;
}

/* Deallocates the resources backing the mesh. This function must be called 
when you are finished using a mesh. */
void meshFinalize(meshMesh *mesh) {
	free(mesh->tri);
}



/*** Writing and reading files ***/

/* Helper function for meshInitializeFile. */
int meshFileError(
        meshMesh *mesh, FILE *file, const char *cause, const int line) {
	fprintf(stderr, "error: meshInitializeFile: %s at line %d\n", cause, line);
	fclose(file);
	meshFinalize(mesh);
	return 3;
}

/* Initializes a mesh from a mesh file. The file format is documented at 
meshSaveFile. This function does not do as much error checking as one might 
like. Use it only on trusted, non-corrupted files, such as ones that you have 
recently created using meshSaveFile. Returns 0 on success, non-zero on failure. 
Don't forget to invoke meshFinalize when you are done using the mesh. */
int meshInitializeFile(meshMesh *mesh, const char *path) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		fprintf(stderr, "error: meshInitializeFile: fopen failed\n");
		return 1;
	}
	int year, month, day, triNum, vertNum, attrDim;
	// Future work: Check version.
	if (fscanf(file, "Carleton College CS 311 mesh version %d/%d/%d\n", &year, 
			&month, &day) != 3) {
		fprintf(stderr, "error: meshInitializeFile: bad header at line 1\n");
		fclose(file);
		return 1;
	}
	if (fscanf(file, "triNum %d\n", &triNum) != 1) {
		fprintf(stderr, "error: meshInitializeFile: bad triNum at line 2\n");
		fclose(file);
		return 2;
	}
	if (fscanf(file, "vertNum %d\n", &vertNum) != 1) {
		fprintf(stderr, "error: meshInitializeFile: bad vertNum at line 3\n");
		fclose(file);
		return 3;
	}
	if (fscanf(file, "attrDim %d\n", &attrDim) != 1) {
		fprintf(stderr, "error: meshInitializeFile: bad attrDim at line 4\n");
		fclose(file);
		return 4;
	}
	if (meshInitialize(mesh, triNum, vertNum, attrDim) != 0) {
		fclose(file);
		return 5;
	}
	int line = 5, *tri, j, check;
	if (fscanf(file, "%d Triangles:\n", &check) != 1 || check != triNum)
		return meshFileError(mesh, file, "bad header", line);
	for (line = 6; line < triNum + 6; line += 1) {
		tri = meshGetTrianglePointer(mesh, line - 6);
		if (fscanf(file, "%d %d %d\n", &tri[0], &tri[1], &tri[2]) != 3)
			return meshFileError(mesh, file, "bad triangle", line);
		if (0 > tri[0] || tri[0] >= vertNum || 0 > tri[1] || tri[1] >= vertNum 
				|| 0 > tri[2] || tri[2] >= vertNum)
			return meshFileError(mesh, file, "bad index", line);
	}
	double *vert;
	if (fscanf(file, "%d Vertices:\n", &check) != 1 || check != vertNum)
		return meshFileError(mesh, file, "bad header", line);
	for (line = triNum + 7; line < triNum + 7 + vertNum; line += 1) {
		vert = meshGetVertexPointer(mesh, line - (triNum + 7));
		for (j = 0; j < attrDim; j += 1) {
			if (fscanf(file, "%lf ", &vert[j]) != 1)
				return meshFileError(mesh, file, "bad vertex", line);
		}
		if (fscanf(file, "\n") != 0)
			return meshFileError(mesh, file, "bad vertex", line);
	}
	// Future work: Check EOF.
	fclose(file);
	return 0;
}

/* Saves a mesh to a file in a simple custom format (not any industry 
standard). Returns 0 on success, non-zero on failure. The first line is a 
comment of the form 'Carleton College CS 311 mesh version YYYY/MM/DD'.

I now describe version 2019/01/15. The second line says 'triNum [triNum]', 
where the latter is an integer value. The third and fourth lines do the same 
for vertNum and attrDim. The fifth line says '[triNum] Triangles:'. Then there 
are triNum lines, each holding three integers between 0 and vertNum - 1 
(separated by a space). Then there is a line that says '[vertNum] Vertices:'. 
Then there are vertNum lines, each holding attrDim floating-point numbers 
(terminated by a space). */
int meshSaveFile(const meshMesh *mesh, const char *path) {
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		fprintf(stderr, "error: meshSaveFile: fopen failed\n");
		return 1;
	}
	fprintf(file, "Carleton College CS 311 mesh version 2019/01/15\n");
	fprintf(file, "triNum %d\n", mesh->triNum);
	fprintf(file, "vertNum %d\n", mesh->vertNum);
	fprintf(file, "attrDim %d\n", mesh->attrDim);
	fprintf(file, "%d Triangles:\n", mesh->triNum);
	int i, j;
	int *tri;
	for (i = 0; i < mesh->triNum; i += 1) {
		tri = meshGetTrianglePointer(mesh, i);
		fprintf(file, "%d %d %d\n", tri[0], tri[1], tri[2]);
	}
	fprintf(file, "%d Vertices:\n", mesh->vertNum);
	double *vert;
	for (i = 0; i < mesh->vertNum; i += 1) {
		vert = meshGetVertexPointer(mesh, i);
		for (j = 0; j < mesh->attrDim; j += 1)
			fprintf(file, "%f ", vert[j]);
		fprintf(file, "\n");
	}
	fclose(file);
	return 0;
}



/*** Rendering ***/

void newVertex
	(int varyDim, const double a[], const double b[], double ab[]) {
	double t = (a[2] + a[3]) / (a[2] + a[3] - b[2] - b[3]);
	
	// new vertex ab = a + t * (a - b).
	vecSubtract(varyDim, b, a, ab);
	vecScale(varyDim, t, ab, ab);
	vecAdd(varyDim, a, ab, ab);
}

void renderClippedTriangle(
		depthBuffer *buf, const double viewport[4][4], const shaShading *sha, const double unif[], 
		const texTexture *tex[], double a[], double b[], double c[]) {
	double varyTransformed[sha->varyDim];
	// performs the viewport transformation and the homogeneous division on 
	// the X, Y, Z, W coordinates of each vertex.
	vecCopy(sha->varyDim, a, varyTransformed);
	mat441Multiply(viewport, a, varyTransformed);
	vecScale(sha->varyDim, 1/varyTransformed[3], varyTransformed, a);

	vecCopy(sha->varyDim, b, varyTransformed);
	mat441Multiply(viewport, b, varyTransformed);
	vecScale(sha->varyDim, 1/varyTransformed[3], varyTransformed, b);

	vecCopy(sha->varyDim, c, varyTransformed);
	mat441Multiply(viewport, c, varyTransformed);
	vecScale(sha->varyDim, 1/varyTransformed[3], varyTransformed, c);

	// renders the current triangle.
	triRender(sha, buf, unif, tex, a, b, c);
}
		
void clipTriangle
		(depthBuffer *buf, const double viewport[4][4], const shaShading *sha, const double unif[], 
		const texTexture *tex[], double a[], double b[], double c[]) {
	// 3 vertices are clipped: Call triRender 0 times
	// 2 vertices are clipped: Call triRender 1 time
	// 1 vertex is clipped: Call triRender 2 times
	// 0 vertices are clipped: Call triRender 1 time

	// case when a is clipped
	if (a[3] <= 0 || a[3] < (-1) * a[2]) {

		// case when a and b are clipped
		if (b[3] <= 0 || b[3] < (-1) * b[2]) {
			
			// case when a, b, and c are clipped
			if (c[3] <= 0 || c[3] < (-1) * c[2]) {
				// DONT RENDER
				return;
			}
			
			// case when a and b are clipped, but c is not
			else {
				// FIND ONE TRIANGLE WHERE AC AND BC ARE ON THE NEAR PLANE
				double ac[sha->varyDim], bc[sha->varyDim];
				newVertex(sha->varyDim, a, c, ac);
				newVertex(sha->varyDim, b, c, bc);
				renderClippedTriangle(buf, viewport, sha, unif, tex, bc, c, ac);
			}
		}
		
		// case when a is clipped, but b is not
		else {
			// case when a and c are clipped, but b is not
			if (c[3] <= 0 || c[3] < (-1) * c[2]) {
				// FIND ONE TIRANGLE WHERE AB and BC ARE ON THE NEAR PLANE
				double ab[sha->varyDim], bc[sha->varyDim];
				newVertex(sha->varyDim, a, b, ab);
				newVertex(sha->varyDim, b, c, bc);
				renderClippedTriangle(buf, viewport, sha, unif, tex, ab, b, bc);
			}
			
			// case when a is clipped, but b and c are not
			else {
				// FIND TWO TRIANGLES:
				// 	AB, B, AC
				// 	AC, B, C
				// AB and AC are on the near plane
				double ab[sha->varyDim], ac[sha->varyDim];
				newVertex(sha->varyDim, a, b, ab);
				newVertex(sha->varyDim, a, c, ac);
				renderClippedTriangle(buf, viewport, sha, unif, tex, ab, b, ac);
				renderClippedTriangle(buf, viewport, sha, unif, tex, ac, b, c);
			}
		}
	}

	// case when a is not clipped
	else {

		// case when a is not clipped, but b is
		if (b[3] <= 0 || b[3] < (-1) * b[2]) {
			
			// case when b and c are clipped, but a is not
			if (c[3] <= 0 || c[3] < (-1) * c[2]) {
				// FIND ONE TRIANGLE WHERE AB AND AC ARE ON THE NEAR PLANE
				double ab[sha->varyDim], ac[sha->varyDim];
				newVertex(sha->varyDim, a, b, ab);
				newVertex(sha->varyDim, a, c, ac);
				renderClippedTriangle(buf, viewport, sha, unif, tex, ac, a, ab);
			}
			
			// case when b is clipped, but a and c are not
			else {
				// FIND TWO TRIANGLES:
				// 	BC, C, AB
				// 	AB, C, A
				// BC and AB are on the near plane
				double ab[sha->varyDim], bc[sha->varyDim];
				newVertex(sha->varyDim, a, b, ab);
				newVertex(sha->varyDim, b, c, bc);
				renderClippedTriangle(buf, viewport, sha, unif, tex, bc, c, ab);
				renderClippedTriangle(buf, viewport, sha, unif, tex, ab, c, a);
			}
			
		// case when neither a nor b are clipped
		} else {
			
			// case when c is clipped, but a and b are not
			if (c[3] <= 0 || c[3] < (-1) * c[2]) {
				// FIND TWO TRIANGLES:
				// 	AC, A, BC
				// 	BC, A, B
				// AC and BC are on the near plane
				double ac[sha->varyDim], bc[sha->varyDim];
				newVertex(sha->varyDim, a, c, ac);
				newVertex(sha->varyDim, b, c, bc);
				renderClippedTriangle(buf, viewport, sha, unif, tex, ac, a, bc);
				renderClippedTriangle(buf, viewport, sha, unif, tex, bc, a, b);
			} 
			
			// case when neither a, b, nor c are clipped
			else {
				renderClippedTriangle(buf, viewport, sha, unif, tex, a, b, c);
			}
		}
	}
}

/* Renders the mesh. If the mesh and the shading have differing values for 
attrDim, then prints an error message and does not render anything. */

/* Now makes call to clipTriangle() to clip triangles outside or partially 
the viewing volume. Rendering happens in clipTriangle(). */
void meshRender(
        const meshMesh *mesh, depthBuffer *buf, const double viewport[4][4],
		const shaShading *sha, const double unif[], const texTexture *tex[]) {
	double *attr[3], vary[3][sha->varyDim];
	int *currTriangle;
	/* loops through all of the triangles in mesh->tri, grabbing the attributes of each of that triangles vertices */
	for (int i = 0 ; i < mesh->triNum ; i ++) {
		currTriangle = meshGetTrianglePointer(mesh, i);

		for (int j = 0 ; j < 3 ; j ++) {
			// gets the next vertex and shades it.
			attr[j] = meshGetVertexPointer(mesh, currTriangle[j]);
			sha->shadeVertex(sha->unifDim, unif, sha->attrDim, attr[j], sha->varyDim, vary[j]);
		}
		clipTriangle(buf, viewport, sha, unif, tex, vary[0], vary[1], vary[2]);
	}
}