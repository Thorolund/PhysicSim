#pragma once
#include <box2d.h>
#include <string>

/// @brief Описание силы для приложения к телу.
class Force {
private:
	std::string name; ///< Идентификатор силы.
public:
	bool isLocal;       ///< true — сила действует на одно тело, false — на все тела симуляции.
	b2Vec2 fVec;        ///< Вектор силы.
	b2Vec2 pointAttach; ///< Точка приложения силы (в локальных координатах тела).

	/// @brief Конструктор по умолчанию (нулевая сила).
	Force();

	/// @brief Конструктор Force с параметрами.
	/// @param vec Вектор силы.
	/// @param isLocal_ Флаг типа действия (локально или глобально).
	/// @param name_ Имя силы.
	/// @param pointAttach_ Точка приложения (по умолчанию (0,0)).
	Force(b2Vec2 vec,
		bool isLocal_,
		std::string name_,
		b2Vec2 pointAttach_ = b2Vec2(0, 0));

	/// @brief Возвращает имя силы.
	/// @return Имя силы.
	std::string GetName();
};