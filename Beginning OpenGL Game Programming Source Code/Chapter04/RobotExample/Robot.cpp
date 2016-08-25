#ifdef _WINDOWS
    #include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>

#include "Robot.h"

Robot::Robot()
{
    armAngles[LEFT] = 0.0;
    armAngles[RIGHT] = 0.0;
    legAngles[LEFT] = 0.0;
    legAngles[RIGHT] = 0.0;
    armStates[LEFT] = FORWARD_STATE;
    armStates[RIGHT] = BACKWARD_STATE;
    legStates[LEFT] = FORWARD_STATE;
    legStates[RIGHT] = BACKWARD_STATE;
}

Robot::~Robot()
{
}

void Robot::DrawCube(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, 0.0f, 0.0f);   // top face
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   // front face
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   // right face
    glVertex3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);  // left face
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   // bottom face
    glVertex3f(0.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);   // back face
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(0.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

void Robot::DrawArm(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);    // red
    glTranslatef(xPos, yPos, zPos);
    glScalef(1.0f, 4.0f, 1.0f);     // arm is a 1x4x1 cube
    DrawCube(0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Robot::DrawHead(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);    // white
    glTranslatef(xPos, yPos, zPos);
    glScalef(2.0f, 2.0f, 2.0f);     // head is a 2x2x2 cube
    DrawCube(0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Robot::DrawTorso(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);    // blue
    glTranslatef(xPos, yPos, zPos);
    glScalef(3.0f, 5.0f, 2.0f);     // torso is a 3x5x2 cube
    DrawCube(0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Robot::DrawLeg(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    // draw the foot
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    DrawFoot(0.0f, -5.0f, 0.0f);
    glPopMatrix();
    glScalef(1.0f, 5.0f, 1.0f);     // leg is a 1x5x1 cube
    glColor3f(1.0f, 1.0f, 0.0f);    // yellow
    DrawCube(0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Robot::DrawFoot(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xPos, yPos, zPos);
    glScalef(1.0f, 0.5f, 3.0f);
    DrawCube(0.0f, 0.0f, 0.0f);
    glPopMatrix();
}

void Robot::DrawRobot(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos); // draw robot at desired coordinates
    // draw head and torso parts
    DrawHead(1.0f, 2.0f, 0.0f);
    DrawTorso(1.5f, 0.0f, 0.0f);
    // move the left arm away from the torso and rotate it to give "walking" effect
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(armAngles[LEFT], 1.0f, 0.0f, 0.0f);
    DrawArm(2.5f, 0.0f, -0.5f);
    glPopMatrix();
    // move the right arm away from the torso and rotate it to give "walking" effect
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(armAngles[RIGHT], 1.0f, 0.0f, 0.0f);
    DrawArm(-1.5f, 0.0f, -0.5f);
    glPopMatrix();
    // move the left leg away from the torso and rotate it to give "walking" effect
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(legAngles[LEFT], 1.0f, 0.0f, 0.0f);
    DrawLeg(-0.5f, -5.0f, -0.5f);
    glPopMatrix();
    // move the right leg away from the torso and rotate it to give "walking" effect
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(legAngles[RIGHT], 1.0f, 0.0f, 0.0f);
    DrawLeg(1.5f, -5.0f, -0.5f);
    glPopMatrix();
    glPopMatrix();  // pop back to original coordinate system
}

void Robot::Prepare(float dt)
{
    // if leg is moving forward, increase angle, else decrease angle
    for (char side = 0; side < 2; side++) {
        // arms
        if (armStates[side] == FORWARD_STATE)
            armAngles[side] += 20.0f * dt;
        else
            armAngles[side] -= 20.0f * dt;

        // change state if exceeding angles
        if (armAngles[side] >= 15.0f)
            armStates[side] = BACKWARD_STATE;
        else if (armAngles[side] <= -15.0f)
            armStates[side] = FORWARD_STATE;

        // legs
        if (legStates[side] == FORWARD_STATE)
            legAngles[side] += 20.0f * dt;
        else
            legAngles[side] -= 20.0f * dt;

        // change state if exceeding angles
        if (legAngles[side] >= 15.0f)
            legStates[side] = BACKWARD_STATE;
        else if (legAngles[side] <= -15.0f)
            legStates[side] = FORWARD_STATE;
    }
}