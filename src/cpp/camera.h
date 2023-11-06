#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// �����ƶ�����
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Ĭ�����ֵ
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	// ���캯������
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// ������ͼ����
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	float Zoom;
private:
	// �������
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// ŷ����
	float Yaw;
	float Pitch;
	// ���ѡ��
	float MovementSpeed = 1.0;
	float MouseSensitivity = 0.1;


	void updateCameraVectors();
};


