#pragma once
#include "ParameterizeBody.h"
#include "Force.h"
#include "logger.h"

/// @brief Связка между телом и силой.
class BodyForce {
private:
	ParamBody* bodyPtr;  ///< Указатель на тело.
	Force* forcePtr;     ///< Указатель на силу.

public:
	/// @brief Конструктор BodyForce.
	/// @param body Ссылка на тело.
	/// @param force Ссылка на силу.
	BodyForce(ParamBody& body, Force& force);

	/// @brief Возвращает указатель на силу.
	/// @return Указатель на Force.
	Force* GetForce();

	/// @brief Возвращает указатель на тело.
	/// @return Указатель на ParamBody.
	ParamBody* GetBody();

	/// @brief Применяет силу к телу в точке приложения.
	void ApplyForce();
};
