#include "Renderer.h"



int main(int argc, char *argv[]) {

	Model *model = loadOBJIntoModel("TreeThing.obj");

	Renderer renderer(1920, 1080);

	red::Vector3 v1(-0.5f, -0.0f, 0);
	red::Vector3 v2(0, -0.5f, -0.5f);
	red::Vector3 v3(0, 0, 0);
	

	StaticShader shader;

	red::Matrix4 proj;
	proj.toProjectionMatrix(renderer.getWidth(), renderer.getHeight(), 70, renderer.getDepth(), 0.5);
	red::Matrix4 transform;
	red::Quaternion tquat;
	red::Matrix4 viewMatrix;
	red::Quaternion cquat;


	float delta = 0.0f;
	red::Vector3 playerPosition;

	

	while(true) {
		playerPosition = playerPosition + movement_direction * 0.05f;

		delta += 0.1f;
		tquat.fromAxis(VECTOR3_Y, delta);
		transform.toTransformationMatrix(red::Vector3(0, 0, 1), tquat, 1);

		cquat.fromAxis(VECTOR3_Y, 0);
		viewMatrix.toTransformationMatrix(playerPosition, cquat, 0.5f);

		renderer.SetShader(&shader);
		shader.loadMatrices(&proj, &transform, &viewMatrix);
		
		renderer.RenderModel(model);

		renderer.RenderScreen();
	}

	return 0;
}
