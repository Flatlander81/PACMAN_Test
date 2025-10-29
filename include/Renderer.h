#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h>
#include <string>

class Renderer {
public:
    static void Initialize(int windowWidth, int windowHeight);

    // Drawing primitives
    static void DrawRect(float x, float y, float width, float height,
                        float r, float g, float b);
    static void DrawCircle(float x, float y, float radius,
                          float r, float g, float b, int segments = 20);
    static void DrawPacman(float x, float y, float radius, float angle,
                          float mouthAngle, float r, float g, float b);
    static void DrawGhost(float x, float y, float size,
                         float r, float g, float b, bool scared = false);

    // Text rendering (simple, using shapes)
    static void DrawDigit(int digit, float x, float y, float size);
    static void DrawNumber(int number, float x, float y, float size);
    static void DrawText(const std::string& text, float x, float y, float size);

    // Utility
    static void Clear(float r, float g, float b);
    static void SetProjection(int width, int height);

private:
    static int windowWidth;
    static int windowHeight;
};

#endif // RENDERER_H
