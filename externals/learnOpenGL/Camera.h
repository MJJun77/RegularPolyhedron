#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
using namespace std;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    DOWN,
    UP,

    ROTATE_LEFT,
    ROTATE_RIGHT,
    ROLL_UP,
    ROLL_DOWN,
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  0.0002f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;
const float ROTATE_ANGLE=  3.0f;
const float ROTATE_SPEED=  10.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float mRotateSum;   // Object rotation sum by Left, Right key click
    float mRollSum;     // Object roll sum by Up, Down key click

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), mRotateSum(0.0f), mRollSum(0.0f)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), mRotateSum(0.0f), mRollSum(0.0f)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    void InitStatus(glm::vec3 position, glm::vec3 up)
    {
        Position = position;
        WorldUp = up;

        Yaw = YAW;
        Pitch = PITCH;
        Front = -Position;

        mRotateSum = 0.0f;
        mRollSum = 0.0f;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetRotateMatrix() const
    {
        glm::mat4 retMat(1.0f);
        retMat = glm::rotate(retMat, glm::radians(mRotateSum), Up);
        retMat = glm::rotate(retMat, glm::radians(mRollSum), glm::vec3(1.0f, 0.0f, 0.0f));

        return retMat;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;     // On my laptop, approx 7e-05

        if (direction == FORWARD)
            Position += Front * velocity;
        else if (direction == BACKWARD)
            Position -= Front * velocity;
        else if (direction == LEFT)
            Position -= Right * velocity;
        else if (direction == RIGHT)
            Position += Right * velocity;
        else if (direction == DOWN)
            Position -= Up * velocity;
        else if (direction == UP)
            Position += Up * velocity;
        else if (direction == ROTATE_LEFT)
        {
            mRotateSum -= ROTATE_ANGLE * velocity * ROTATE_SPEED;
            if (mRotateSum < -360.0f)
                mRotateSum += 360.0f;
        }
        else if (direction == ROTATE_RIGHT)
        {
            mRotateSum += ROTATE_ANGLE * velocity * ROTATE_SPEED;
            if (mRotateSum > 360.0f)
                mRotateSum -= 360.0f;

        }
        else if (direction == ROLL_UP)
        {
            mRollSum += ROTATE_ANGLE * velocity * ROTATE_SPEED;
            if (mRollSum > 360.0f)
                mRollSum -= 360.0f;
        }
        else if (direction == ROLL_DOWN)
        {
            mRollSum -= ROTATE_ANGLE * velocity * ROTATE_SPEED;
            if (mRollSum < -360.0f)
                mRollSum += 360.0f;
        }

        /*
        else if ((direction == ROTATE_LEFT) || (direction == ROTATE_RIGHT) ||
                 (direction == CLIMB_UP) || (direction == CLIMB_DOWN))
        {
            glm::vec3 rot_axis = ((direction == ROTATE_LEFT) || (direction == ROTATE_RIGHT)) ?
                                    Up : Right;
            float dir_sign = ((direction == ROTATE_RIGHT) || (direction == CLIMB_UP)) ?
                            1.0f : -1.0f;

            Position = glm::rotate(Position, glm::radians(ROTATE_ANGLE) * velocity * ROTATE_SPEED, glm::normalize(dir_sign * rot_axis));
            // cout << dbgLocal << " th rot val: " << (dir_sign * glm::radians(ROTATE_ANGLE) * velocity * ROTATE_SPEED) << endl;
            Front = -Position;

            updateCameraVectors(true);
        }
        */
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
    /*
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    */
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f; 
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors(bool bSkipFrontCalc = false)
    {
        if (!bSkipFrontCalc)
        {
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
        }
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
