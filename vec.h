#pragma once
#include <cmath>
#include "logger.h"

/// <summary>Двумерный вектор.</summary>
struct vec {
private:
	float x, y; ///< Координата вектора
	float self_len; ///< Кэшированная длина вектора
public:
	/// <param name="x_">Координата X (по умолч. 0)</param>
	/// <param name="y_">Координата Y (по умолч. 0)</param>
	vec(float x_ = 0, float y_ = 0);

	/// <returns>Координата x</returns>
	float getx() const;
	/// <returns>Координата y</returns>
	float gety() const;
	/// <returns>Координата x(целая часть)</returns>
	int getxint() const;
	/// <returns>Координата y(целая часть)</returns>
	int getyint() const;

	/// <param name="other_vec">Второе слагаемое.</param>
	/// <returns>Покомпонентная сумма.</returns>
	vec operator+(vec other_vec) const;
	/// <param name="other_vec">Вычитаемое.</param>
	/// <returns>Покомпонентная разность.</returns>
	vec operator-(vec other_vec) const;
	/// <param name="a">Скалярный множитель.</param>
	/// <returns>Покомпонентное произведение.</returns>
	vec operator*(float a) const;
	/// <param name="a">Скалярный делитель. При 0 логирует ошибку и возвращает (0,0).</param>
	/// <returns>Покомпонентное частное.</returns>
	vec operator/(float a) const;
	/// <param name="other_vec">Второй вектор.</param>
	/// <returns>Скалярное произведение (x*x2 + y*y2).</returns>
	float operator*(vec other_vec) const;
	/// <param name="other_vec">Второй вектор.</param>
	/// <returns>Сравнение по координатам.</returns>
	bool operator==(vec other_vec) const;
	/// <param name="other_vec">Второй вектор.</param>
	/// <returns>Сравнение по координатам.</returns>
	bool operator!=(vec other_vec) const;

	/// <returns>Кэшированная длина (вычислена в конструкторе).</returns>
	float len() const;
	/// <returns>Нормализованный вектор. При нулевой длине логирует ошибку и возвращает (0,0).</returns>
	vec len1() const;
	
	/// <returns>Правая нормаль (y, -x). Не нормализуется.</returns>
	vec rnormal() const;
	/// <returns>Левая нормаль (-y, x). Не нормализуется.</returns>
	vec lnormal() const;
};
