#include <SDL.h>
#include <box2d.h>
#include <iostream>
#include <vector>
#include <string>

// Константы для преобразования физических координат в пиксели
const float SCALE = 30.0f; // 1 метр = 30 пикселей

// Структура для хранения данных отрисовки
struct DrawData {
    float x, y;          // позиция в метрах
    float angle;         // угол поворота
    float width, height; // размеры в метрах
    SDL_Color color;
};

// Функция для преобразования Box2D координат в SDL координаты (центр окна)
SDL_FPoint Box2DToSDL(float x, float y, int windowWidth, int windowHeight) {
    return {
        x * SCALE + windowWidth / 2.0f,
        -y * SCALE + windowHeight / 2.0f  // инвертируем Y, так как SDL считает Y вниз
    };
}

int main(int argc, char* argv[]) {
    // ====== ИНИЦИАЛИЗАЦИЯ SDL ======
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Box2D + SDL2 Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    std::cout << "Box2D + SDL2 успешно инициализированы!" << std::endl;

    // ====== ИНИЦИАЛИЗАЦИЯ BOX2D ======
    // Создаем физический мир с гравитацией (0, -10) м/с²
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    // ====== СОЗДАЕМ ЗЕМЛЮ (статическое тело) ======
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -5.0f); // немного ниже центра
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(10.0f, 1.0f); // 10 метров в ширину, 1 метр в высоту

    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundBox;
    groundFixtureDef.friction = 0.5f;
    groundBody->CreateFixture(&groundFixtureDef);

    std::cout << "Земля создана" << std::endl;

    // ====== СОЗДАЕМ ПАДАЮЩИЙ КВАДРАТ (динамическое тело) ======
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // динамическое тело
    bodyDef.position.Set(0.0f, 3.0f); // выше земли
    bodyDef.angle = 10;
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.5f, 0.5f); // 0.5x0.5 метра

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;     // плотность
    fixtureDef.friction = 0.3f;    // трение
    fixtureDef.restitution = 1.0; // упругость (отскок)
    body->CreateFixture(&fixtureDef);
    body->SetLinearVelocity(b2Vec2(5.0f, 0.0f));

    std::cout << "Падающий квадрат создан" << std::endl;

    // ====== ГЛАВНЫЙ ЦИКЛ ======
    bool quit = false;
    SDL_Event e;
    const float timeStep = 1.0f / 60.0f;
    const int velocityIterations = 6;
    const int positionIterations = 2;

    // Для измерения FPS
    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();

    while (!quit) {
        // Обработка событий
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // Нажатие пробела для сброса позиции
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                body->SetTransform(b2Vec2(0.0f, 3.0f), 0.0f);
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                body->SetAngularVelocity(0.0f);
                std::cout << "Позиция сброшена!" << std::endl;
            }
        }

        // Шаг физики
        world.Step(timeStep, velocityIterations, positionIterations);

        // Получаем позицию и угол тела
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();

        // Очищаем экран
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderClear(renderer);

        // ====== ОТРИСОВКА ЗЕМЛИ ======
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_FPoint groundPos = Box2DToSDL(0.0f, -5.0f, windowWidth, windowHeight);
        SDL_Rect groundRect = {
            (int)(groundPos.x - 10.0f * SCALE),
            (int)(groundPos.y - 1.0f * SCALE),
            (int)(20.0f * SCALE),
            (int)(2.0f * SCALE)
        };
        SDL_RenderFillRect(renderer, &groundRect);

        // ====== ОТРИСОВКА КВАДРАТА ======
        // Преобразуем координаты Box2D в SDL
        SDL_FPoint sdlPos = Box2DToSDL(position.x, position.y, windowWidth, windowHeight);

        // Создаем матрицу поворота для отрисовки
        SDL_FPoint center = { 0.5f * SCALE, 0.5f * SCALE };
        SDL_FPoint corners[4] = {
            { -0.5f * SCALE, -0.5f * SCALE },
            {  0.5f * SCALE, -0.5f * SCALE },
            {  0.5f * SCALE,  0.5f * SCALE },
            { -0.5f * SCALE,  0.5f * SCALE }
        };

        // Поворачиваем и смещаем углы
        SDL_FPoint rotatedCorners[4];
        for (int i = 0; i < 4; i++) {
            float cosA = cosf(angle);
            float sinA = sinf(angle);
            rotatedCorners[i].x = sdlPos.x + (corners[i].x * cosA - corners[i].y * sinA);
            rotatedCorners[i].y = sdlPos.y + (corners[i].x * sinA + corners[i].y * cosA);
        }

        // Рисуем квадрат
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_RenderDrawLineF(renderer, rotatedCorners[0].x, rotatedCorners[0].y,
            rotatedCorners[1].x, rotatedCorners[1].y);
        SDL_RenderDrawLineF(renderer, rotatedCorners[1].x, rotatedCorners[1].y,
            rotatedCorners[2].x, rotatedCorners[2].y);
        SDL_RenderDrawLineF(renderer, rotatedCorners[2].x, rotatedCorners[2].y,
            rotatedCorners[3].x, rotatedCorners[3].y);
        SDL_RenderDrawLineF(renderer, rotatedCorners[3].x, rotatedCorners[3].y,
            rotatedCorners[0].x, rotatedCorners[0].y);

        // Заливаем квадрат цветом
        SDL_Vertex vertices[4];
        SDL_Color color = { 255, 100, 100, 180 };
        for (int i = 0; i < 4; i++) {
            vertices[i].position = rotatedCorners[i];
            vertices[i].color = color;
        }
        SDL_RenderGeometry(renderer, nullptr, vertices, 4, nullptr, 0);

        // ====== ОТРИСОВКА ИНФОРМАЦИИ ======
        // Показываем позицию и FPS в заголовке окна
        frameCount++;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - startTime >= 1000) {
            std::string title = "Box2D + SDL2 | FPS: " + std::to_string(frameCount) +
                " | Pos: (" + std::to_string(position.x).substr(0, 4) +
                ", " + std::to_string(position.y).substr(0, 4) + ")";
            SDL_SetWindowTitle(window, title.c_str());
            frameCount = 0;
            startTime = currentTime;
        }

        // Обновляем экран
        SDL_RenderPresent(renderer);

        // Небольшая задержка для экономии CPU
        SDL_Delay(16);
    }

    // ====== ОЧИСТКА ======
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Программа завершена" << std::endl;
    return 0;
}