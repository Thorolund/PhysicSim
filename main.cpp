#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <box2d.h>
#include <vector>
#include <cmath>
#include <iostream>

/**
 * Простые функции для отрисовки Box2D тел с помощью SDL2 и SDL2_gfx
 */

 // Константа: пикселей на метр
const float PPM = 30.0f;

// Структура цвета (RGBA)
struct Color {
    uint8_t r, g, b, a;
};

// Предопределенные цвета
const Color COLOR_WHITE = { 255, 255, 255, 255 };
const Color COLOR_GREEN = { 0, 255, 0, 255 };
const Color COLOR_RED = { 255, 0, 0, 255 };
const Color COLOR_BLUE = { 0, 100, 255, 255 };
const Color COLOR_GRAY = { 150, 150, 150, 255 };
const Color COLOR_YELLOW = { 255, 200, 0, 255 };

/**
 * Преобразует координаты Box2D (метры) в экранные координаты (пиксели)
 */
void WorldToScreen(float worldX, float worldY, int& screenX, int& screenY) {
    screenX = static_cast<int>(worldX * PPM + 400);  // +400 для центрирования
    screenY = static_cast<int>(300 - worldY * PPM);   // 300 для центрирования
}

/**
 * Отрисовывает круглую фикстуру (контур)
 *
 * @param renderer SDL_Renderer
 * @param body Физическое тело
 * @param fixture Фикстура
 * @param color Цвет
 */
void DrawCircle(SDL_Renderer* renderer, b2Body* body, b2Fixture* fixture, Color color) {
    // Получаем форму
    b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

    // Позиция тела
    b2Vec2 pos = body->GetPosition();

    // Смещение круга (обычно (0,0), но может быть сдвинут)
    b2Vec2 offset = circle->m_p;

    // Мировая позиция центра круга
    float worldX = pos.x + offset.x;
    float worldY = pos.y + offset.y;

    // Преобразуем в пиксели
    int screenX, screenY;
    WorldToScreen(worldX, worldY, screenX, screenY);

    // Радиус в пикселях
    int radius = (int)(circle->m_radius * PPM);
    if (radius < 1) radius = 1;

    // Рисуем окружность (только контур) - ИСПРАВЛЕНО
    circleRGBA(renderer, screenX, screenY, radius,
        color.r, color.g, color.b, color.a);
}

/**
 * Отрисовывает полигональную фикстуру (контур)
 *
 * @param renderer SDL_Renderer
 * @param body Физическое тело
 * @param fixture Фикстура
 * @param color Цвет
 */
void DrawPolygon(SDL_Renderer* renderer, b2Body* body, b2Fixture* fixture, Color color) {
    // Получаем форму
    b2PolygonShape* polygon = (b2PolygonShape*)fixture->GetShape();

    // Количество вершин
    int count = polygon->m_count;
    if (count < 3) return;

    // Позиция и угол тела
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle();

    // Массивы для точек
    std::vector<Sint16> pointsX(count);
    std::vector<Sint16> pointsY(count);

    // Преобразуем вершины
    for (int i = 0; i < count; i++) {
        // Вершина в локальных координатах
        b2Vec2 v = polygon->m_vertices[i];

        // Поворачиваем на угол тела
        float rotatedX = v.x * cos(angle) - v.y * sin(angle);
        float rotatedY = v.x * sin(angle) + v.y * cos(angle);

        // Смещаем на позицию тела
        float worldX = pos.x + rotatedX;
        float worldY = pos.y + rotatedY;

        // Преобразуем в пиксели
        int screenX, screenY;
        WorldToScreen(worldX, worldY, screenX, screenY);

        pointsX[i] = (Sint16)screenX;
        pointsY[i] = (Sint16)screenY;
    }

    // Рисуем полигон (только контур) - ИСПРАВЛЕНО
    polygonRGBA(renderer, pointsX.data(), pointsY.data(), count,
        color.r, color.g, color.b, color.a);
}

/**
 * Отрисовывает все фикстуры тела
 *
 * @param renderer SDL_Renderer
 * @param body Физическое тело
 * @param color Цвет
 */
void DrawBody(SDL_Renderer* renderer, b2Body* body, Color color) {
    // Проходим по всем фикстурам
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
        if (f->GetType() == b2Shape::e_circle) {
            DrawCircle(renderer, body, f, color);
        }
        else if (f->GetType() == b2Shape::e_polygon) {
            DrawPolygon(renderer, body, f, color);
        }
    }
}

/**
 * Отрисовывает все тела в мире
 *
 * @param renderer SDL_Renderer
 * @param world Физический мир
 */
void DrawWorld(SDL_Renderer* renderer, b2World* world) {
    // Проходим по всем телам
    for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {
        // Выбираем цвет по типу тела
        Color color;
        if (body->GetType() == b2_staticBody) {
            color = COLOR_GRAY;
        }
        else if (body->GetType() == b2_dynamicBody) {
            color = COLOR_GREEN;
        }
        else { // kinematic
            color = COLOR_BLUE;
        }

        DrawBody(renderer, body, color);
    }
}
int main(int argc, char* argv[]) {
    // --- ИНИЦИАЛИЗАЦИЯ SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // --- СОЗДАНИЕ ОКНА ---
    SDL_Window* window = SDL_CreateWindow(
        "Box2D Physics Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // --- СОЗДАНИЕ РЕНДЕРЕРА ---
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // --- СОЗДАНИЕ МИРА BOX2D ---
    b2Vec2 g(0.0f, -10.0f);
    b2World world(g);

    b2BodyDef wall_def;
    wall_def.type = b2_staticBody;
    wall_def.position = b2Vec2(0, 8);
    b2Body* wall = world.CreateBody(&wall_def);

    b2PolygonShape wall_shape;
    wall_shape.SetAsBox(5, 0.5);

    b2FixtureDef wallf;
    wallf.shape = &wall_shape;
    wallf.restitution = 1.0f;

    wall->CreateFixture(&wallf);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    bodyDef.position.Set(0, 0);

    b2Body* body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.restitution = 0.8f;

    body->CreateFixture(&fixtureDef);
    body->SetLinearVelocity(b2Vec2(3, -3));
   
    b2DistanceJointDef djointDef;
    djointDef.bodyA = wall;
    djointDef.bodyB = body;
    djointDef.localAnchorA = b2Vec2(0, 0);  // локальная точка на стене
    djointDef.localAnchorB = b2Vec2(0, 0);  // локальная точка на теле
    djointDef.length = 8.0f;  // явно задаём длину
    djointDef.stiffness = 5.0f;
    djointDef.damping = 0;

    auto djoint = (b2DistanceJoint*)world.CreateJoint(&djointDef);



    // --- ИГРОВОЙ ЦИКЛ ---
    bool running = true;
    SDL_Event event;

    while (running) {
        // --- ОБРАБОТКА СОБЫТИЙ ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // --- ШАГ ФИЗИКИ ---
        world.Step(1.0f / 60.0f, 16, 6);

        // --- ОЧИСТКА ЭКРАНА (ОБЯЗАТЕЛЬНО ПЕРВЫМ!) ---
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);

        // --- ОТЛАДОЧНАЯ СЕТКА (опционально) ---
        // Рисуем оси координат
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

        // Горизонтальная линия (ось X)
        SDL_RenderDrawLine(renderer, 0, 300, 800, 300);

        // Вертикальная линия (ось Y)
        SDL_RenderDrawLine(renderer, 400, 0, 400, 600);

        // Красная точка в центре (пиксель)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, 400, 300);

        // --- ОТРИСОВКА МИРА ---
        DrawWorld(renderer, &world);

        // --- ОБНОВЛЕНИЕ ЭКРАНА ---
        SDL_RenderPresent(renderer);

        // --- ЗАДЕРЖКА ---
        SDL_Delay(16);
        b2Vec2 pos = body->GetPosition();
        float length = djoint->GetLength();
        float force = djoint->GetReactionForce(60.0f).Length();
    }

    // --- ОЧИСТКА РЕСУРСОВ ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
