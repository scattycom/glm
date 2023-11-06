#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 定义移动方向
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// 默认相机值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	// 构造函数声明
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// 返回视图矩阵
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	float Zoom;
private:
	// 相机属性
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// 欧拉角
	float Yaw;
	float Pitch;
	// 相机选项
	float MovementSpeed = 1.0;
	float MouseSensitivity = 0.1;


	void updateCameraVectors();
};


