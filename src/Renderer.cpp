#include "Renderer.h"
#include <cmath>

int Renderer::windowWidth = 800;
int Renderer::windowHeight = 600;

void Renderer::Initialize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    SetProjection(width, height);
}

void Renderer::SetProjection(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::Clear(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawRect(float x, float y, float width, float height,
                       float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Renderer::DrawCircle(float x, float y, float radius,
                         float r, float g, float b, int segments) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * float(i) / float(segments);
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void Renderer::DrawPacman(float x, float y, float radius, float angle,
                         float mouthAngle, float r, float g, float b) {
    glColor3f(r, g, b);

    // Convert angle to radians
    float angleRad = angle * 3.14159f / 180.0f;
    float mouthRad = mouthAngle * 3.14159f / 180.0f;

    // Draw Pacman as a partial circle (with mouth)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Center

    int segments = 30;
    float startAngle = angleRad - mouthRad / 2;
    float endAngle = angleRad + mouthRad / 2;

    // Draw from start to end going the long way around
    for (int i = 0; i <= segments; i++) {
        float t = float(i) / float(segments);
        float currentAngle = startAngle + (2.0f * 3.14159f - mouthRad) * t;
        float dx = radius * cosf(currentAngle);
        float dy = radius * sinf(currentAngle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void Renderer::DrawGhost(float x, float y, float size,
                        float r, float g, float b, bool scared) {
    if (scared) {
        // Blue color when scared
        r = 0.0f;
        g = 0.0f;
        b = 1.0f;
    }

    glColor3f(r, g, b);

    // Draw ghost body (semicircle top + rectangle bottom)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Center top
    for (int i = 0; i <= 20; i++) {
        float angle = 3.14159f * float(i) / 20.0f;  // 0 to PI (top half)
        float dx = size * cosf(angle);
        float dy = size * sinf(angle);
        glVertex2f(x + dx, y - dy);
    }
    glEnd();

    // Draw rectangle body
    DrawRect(x - size, y, size * 2, size, r, g, b);

    // Draw wavy bottom
    glBegin(GL_TRIANGLES);
    int waves = 4;
    for (int i = 0; i < waves; i++) {
        float waveWidth = (size * 2) / waves;
        float x1 = x - size + i * waveWidth;
        float x2 = x - size + (i + 0.5f) * waveWidth;
        float x3 = x - size + (i + 1) * waveWidth;

        glVertex2f(x1, y + size);
        glVertex2f(x2, y + size + size * 0.3f);
        glVertex2f(x3, y + size);
    }
    glEnd();

    // Draw eyes (white with pupils)
    if (!scared) {
        DrawCircle(x - size * 0.3f, y + size * 0.2f, size * 0.25f, 1.0f, 1.0f, 1.0f);
        DrawCircle(x + size * 0.3f, y + size * 0.2f, size * 0.25f, 1.0f, 1.0f, 1.0f);
        DrawCircle(x - size * 0.3f, y + size * 0.2f, size * 0.15f, 0.0f, 0.0f, 1.0f);
        DrawCircle(x + size * 0.3f, y + size * 0.2f, size * 0.15f, 0.0f, 0.0f, 1.0f);
    } else {
        // Scared face
        DrawRect(x - size * 0.5f, y + size * 0.3f, size * 0.3f, size * 0.1f, 1.0f, 1.0f, 1.0f);
        DrawRect(x + size * 0.2f, y + size * 0.3f, size * 0.3f, size * 0.1f, 1.0f, 1.0f, 1.0f);
    }
}

void Renderer::DrawDigit(int digit, float x, float y, float size) {
    // Simple 7-segment style digit rendering
    // This is a simplified version - draws rectangles to form digits
    float segWidth = size * 0.6f;
    float segHeight = size * 0.1f;

    bool segs[7] = {false};

    // Define which segments are on for each digit (7-segment display)
    switch (digit) {
        case 0: segs[0]=true; segs[1]=true; segs[2]=true; segs[3]=true; segs[4]=true; segs[5]=true; break;
        case 1: segs[1]=true; segs[2]=true; break;
        case 2: segs[0]=true; segs[1]=true; segs[6]=true; segs[4]=true; segs[3]=true; break;
        case 3: segs[0]=true; segs[1]=true; segs[6]=true; segs[2]=true; segs[3]=true; break;
        case 4: segs[5]=true; segs[6]=true; segs[1]=true; segs[2]=true; break;
        case 5: segs[0]=true; segs[5]=true; segs[6]=true; segs[2]=true; segs[3]=true; break;
        case 6: segs[0]=true; segs[5]=true; segs[4]=true; segs[3]=true; segs[2]=true; segs[6]=true; break;
        case 7: segs[0]=true; segs[1]=true; segs[2]=true; break;
        case 8: segs[0]=true; segs[1]=true; segs[2]=true; segs[3]=true; segs[4]=true; segs[5]=true; segs[6]=true; break;
        case 9: segs[0]=true; segs[1]=true; segs[2]=true; segs[3]=true; segs[5]=true; segs[6]=true; break;
    }

    // Draw segments
    if (segs[0]) DrawRect(x, y, segWidth, segHeight, 1.0f, 1.0f, 1.0f);  // top
    if (segs[1]) DrawRect(x + segWidth, y, segHeight, size * 0.4f, 1.0f, 1.0f, 1.0f);  // top-right
    if (segs[2]) DrawRect(x + segWidth, y + size * 0.5f, segHeight, size * 0.4f, 1.0f, 1.0f, 1.0f);  // bottom-right
    if (segs[3]) DrawRect(x, y + size * 0.9f, segWidth, segHeight, 1.0f, 1.0f, 1.0f);  // bottom
    if (segs[4]) DrawRect(x, y + size * 0.5f, segHeight, size * 0.4f, 1.0f, 1.0f, 1.0f);  // bottom-left
    if (segs[5]) DrawRect(x, y, segHeight, size * 0.4f, 1.0f, 1.0f, 1.0f);  // top-left
    if (segs[6]) DrawRect(x, y + size * 0.45f, segWidth, segHeight, 1.0f, 1.0f, 1.0f);  // middle
}

void Renderer::DrawNumber(int number, float x, float y, float size) {
    if (number == 0) {
        DrawDigit(0, x, y, size);
        return;
    }

    int temp = number;
    int digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    float digitWidth = size * 0.8f;
    float startX = x;

    for (int i = digits - 1; i >= 0; i--) {
        int digit = (number / int(pow(10, i))) % 10;
        DrawDigit(digit, startX, y, size);
        startX += digitWidth;
    }
}

void Renderer::DrawText(const std::string& text, float x, float y, float size) {
    // Very basic text rendering - just for simple messages
    float charWidth = size * 0.6f;
    for (size_t i = 0; i < text.length(); i++) {
        // For now, just draw rectangles as placeholders for letters
        DrawRect(x + i * charWidth, y, charWidth * 0.8f, size, 1.0f, 1.0f, 1.0f);
    }
}
