#include <game-emu/common/util.h>
#include <game-emu/common/propertyvalue.h>

#if defined(_WIN32) || defined(__CYGWIN__)
#include <Windows.h>
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
#elif
#error "Platform not supported!"
#endif

namespace GameEmu::Common
{
	std::filesystem::path Util::GetExecutablePath()
	{
#if defined(_WIN32) || defined(__CYGWIN__)
		wchar_t path[MAX_PATH] = { 0 };
		GetModuleFileNameW(NULL, path, MAX_PATH);
		return path;
#elif defined(__linux__)
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
#endif
	}

	Util::PropertyValueType Util::GetPropertyValueType(const PropertyValue& value)
	{
		if (std::holds_alternative<s32>(value))
			return PropertyValueType::Integer32;
		else if (std::holds_alternative<s64>(value))
			return PropertyValueType::Integer64;
		else if (std::holds_alternative<s8>(value))
			return PropertyValueType::Integer8;
		else if (std::holds_alternative<s16>(value))
			return PropertyValueType::Integer16;
		else if (std::holds_alternative<float>(value))
			return PropertyValueType::Float;
		else if (std::holds_alternative<double>(value))
			return PropertyValueType::Double;
		else if (std::holds_alternative<bool>(value))
			return PropertyValueType::Bool;
		else if (std::holds_alternative<std::string>(value))
			return PropertyValueType::String;

		return PropertyValueType::Unknown;
	}

	std::string Util::GetPropertyValueTypeDisplayName(PropertyValueType type)
	{
		switch (type)
		{
		default:
			return "Unknown";
		case PropertyValueType::Integer32:
			return "Integer 32-bit";
		case PropertyValueType::Integer64:
			return "Integer 64-bit";
		case PropertyValueType::Integer8:
			return "Integer 8-bit";
		case PropertyValueType::Integer16:
			return "Integer 16-bit";
		case PropertyValueType::Float:
			return "Float 32-bit";
		case PropertyValueType::Double:
			return "Float 64-bit";
		case PropertyValueType::Bool:
			return "Bool";
		case PropertyValueType::String:
			return "String";
		}
	}
}
