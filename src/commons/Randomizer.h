#pragma once

#include <random>

struct Randomizer
{
public:

private:
	static inline std::random_device rd;
	static inline std::mt19937       gen;

public:
	static void CheckDeviceExistence()
	{
		gen = std::mt19937(rd());
		//gen.seed(rd());
	}

	template <typename Container>
	static void ShuffleVector(Container& _container)
	{
		std::shuffle(_container.begin(), _container.end(), gen);
	}

	template <typename Container>
	static typename Container::value_type GetRandIt(const Container& _container)
	{
		int randIndex = std::uniform_int_distribution<int>(0, _container.size() - 1)(gen);
		return _container[randIndex];
	}

	template <typename Container>
	static int GetRandIndex(const Container& _container)
	{
		int randIndex = std::uniform_int_distribution<int>(0, _container.size() - 1)(gen);
		return randIndex;
	}

	/// @brief Get probability from percentage
	/// @param _probabilityPercentage 100% means always true, 0% will always false
	/// @return 
	static bool GetRandPercentChange(float _probabilityPercentage)
	{
		//Set limit
		float clampedProbability = std::clamp(_probabilityPercentage, 0.0f, 100.0f);

		float randomValue = GetRandF(0.0f, 100.0f);

		return randomValue <= clampedProbability;
	}

	static int GetRandI(int minInclusion, int maxInclusion)
	{
		return std::uniform_int_distribution<int>(minInclusion, maxInclusion)(gen);
	}

	template<typename EnumType>
	static EnumType GetRandomEnum()
	{
		static_assert(std::is_enum<EnumType>::value, "Template parameter must be an enum type");

		// Get the minimum and maximum values of the enum
		auto min = static_cast<typename std::underlying_type<EnumType>::type>(std::numeric_limits<EnumType>::min());
		auto max = static_cast<typename std::underlying_type<EnumType>::type>(std::numeric_limits<EnumType>::max());

		return static_cast<EnumType>(GetRandI(min, max));
	}

	static float GetRandF(float minInclusion, float maxInclusion)
	{
		static constexpr float EPSILON = std::numeric_limits<float>().epsilon();
		return std::uniform_real_distribution<float>(minInclusion, maxInclusion + EPSILON)(gen);
	}

	static bool GetRandB(float _trueProbabilityRatio = 0.5f)
	{
		// std::bernoulli_distribution(1.0)(gen); //100% true
		return std::bernoulli_distribution(_trueProbabilityRatio)(gen);
	}

	static int GetRandSign(float _probability) //ratio
	{
		// Generate a boolean based on the probability
		bool isPositive = GetRandB(_probability);

		// Return 1 if true, -1 if false
		return isPositive ? 1 : -1;
	}


private:

};
