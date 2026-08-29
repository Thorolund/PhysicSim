#include <box2d.h>
#include <map>
#include <vector>
#include "logger.h"

/// @brief Обёртка над b2Body с вычислением линейного и углового ускорения.
class ParamBody {
private:
	b2Vec2 prevLV;       ///< Предыдущая линейная скорость.
	float prevAV;        ///< Предыдущая угловая скорость.
	b2World* world; ///< Указатель на box2d мир.
public:
	b2Body* body; ///< Указатель на тело Box2D.

	/// @brief Конструктор ParamBody.
	/// @param world Указатель на мир Box2D. (Сохраняется в обертке).
	/// @param type Тип тела (b2_staticBody, b2_dynamicBody, b2_kinematicBody).
	/// @param position Начальная позиция тела.
	ParamBody(b2World* world_,
		b2BodyType type,
		b2Vec2 position);

	/// @brief Деструктор ParamBody. Удаляет body из world.
	~ParamBody();

	/// @brief Добавление Fixture к телу.
	/// @param fixtDef определитель фикстуры.
	void AddFixture(b2FixtureDef* fixtDef);

	/// @brief Вычисляет линейное ускорение тела.
	/// @param timestep Шаг времени (секунды).
	/// @return Линейное ускорение (м/с²).
	b2Vec2 GetLinearAcceleration(float timestep);

	/// @brief Вычисляет угловое ускорение тела.
	/// @param timestep Шаг времени (секунды).
	/// @return Угловое ускорение (рад/с²).
	float GetAngleAcceleration(float timestep);

	/// @brief Обновляет сохранённые скорости текущими значениями из Box2D.
	void UpdatePrevVelocites();
};
