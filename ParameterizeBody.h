#include <box2d.h>
#include "Force.h"
#include <map>
#include <vector>
#include "logger.h"
#include <string>

/// @brief Обёртка над b2Body с управлением силами и вычислением ускорений.
class ParamBody {
private:
	std::string name; ///< Имя тела.

	std::map<std::string, Force*> permanentForces; ///< Постоянные силы (пользовательские).
	std::map<std::string, Force*> instantForces;   ///< Мгновенные силы (возникают во время симуляции).

	float prevAV;        ///< Предыдущая угловая скорость.
	Force totalForce;    ///< Суммарная сила (permanent + instant).

	b2World* world;      ///< Указатель на мир Box2D.

	/// @brief Пересчитывает суммарную силу из permanentForces и instantForces.
	void UpdateTotalForces();

	/// @brief Обновляет сохранённую угловую скорость текущим значением из Box2D.
	void UpdatePrevAV();

public:
	b2Body* body; ///< Указатель на тело Box2D.

	/// @brief Конструктор ParamBody.
	/// @param world_ Указатель на мир Box2D.
	/// @param name_ Имя тела.
	/// @param type_ Тип тела (b2_staticBody, b2_dynamicBody, b2_kinematicBody).
	/// @param position_ Начальная позиция тела.
	/// @param fixtures Вектор фикстур для прикрепления к телу.
	ParamBody(b2World* world_,
		std::string name_,
		b2BodyType type_,
		b2Vec2 position_,
		std::vector<b2FixtureDef*> fixtures);

	/// @brief Деструктор ParamBody. Удаляет тело из мира и очищает силы.
	~ParamBody();

	/// @brief Возвращает имя тела.
	/// @return Имя тела.
	std::string GetName();

	/// @brief Проверяет наличие силы в permanentForces.
	/// @param force_name Имя силы.
	/// @return true — сила найдена, false — не найдена.
	bool isForceInPermanent(std::string force_name);

	/// @brief Проверяет наличие силы в instantForces.
	/// @param force_name Имя силы.
	/// @return true — сила найдена, false — не найдена.
	bool isForceInInstant(std::string force_name);

	/// @brief Возвращает указатель на силу по имени (ищет в permanent и instant).
	/// @param force_name Имя силы.
	/// @return Указатель на Force или nullptr, если сила не найдена.
	Force* GetForceByName(std::string force_name);

	/// @brief Возвращает суммарную силу (permanent + instant).
	/// @return Копия totalForce.
	Force GetTotalForce();

	/// @brief Возвращает список всех постоянных сил.
	/// @return Вектор указателей на Force.
	std::vector<Force*> GetListPermanentForces();

	/// @brief Возвращает список всех мгновенных сил.
	/// @return Вектор указателей на Force.
	std::vector<Force*> GetListInstantForces();

	/// @brief Применяет все постоянные силы к телу.
	void ApplyPermanentForces();

	/// @brief Добавляет постоянную силу.
	/// @param force Указатель на Force.
	void AddPermanentForce(Force* force);

	/// @brief Добавляет мгновенную силу.
	/// @param force Указатель на Force.
	void AddInstantForce(Force* force);

	/// @brief Добавляет импульс скорости к телу.
	/// @param v Вектор скорости.
	void AddVelosity(b2Vec2 v);

	/// @brief Удаляет силу по имени (из permanent или instant).
	/// @param force_name Имя силы.
	void DeleteForceByName(std::string force_name);

	/// @brief Очищает все мгновенные силы.
	void ClearInstantForces();

	/// @brief Обновляет состояние тела (суммарная сила и угловая скорость).
	void Update();

	/// @brief Вычисляет угловое ускорение.
	/// @param dt Шаг времени (секунды).
	/// @return Угловое ускорение (рад/с²).
	float CountAngularAceleration(float dt);

	/// @brief Вычисляет линейное ускорение на основе суммарной силы.
	/// @param dt Шаг времени (секунды).
	/// @return Линейное ускорение (м/с²).
	float CountLinearAcceleration(float dt);
};