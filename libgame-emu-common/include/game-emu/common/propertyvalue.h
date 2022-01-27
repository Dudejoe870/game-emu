#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	class PropertyValue
	{
	public:
		enum class Type
		{
			Integer32,
			Integer64,
			Float,
			Double,
			String
		};
	private:
		union valueStorage
		{
			int _int;
			long long _long;
			float _float;
			double _double;
			std::string _string;

			~valueStorage() { }
		};

		Type type;
		valueStorage value;
	public:
		~PropertyValue() { }

		Type getType() { return type; }

		int getInt() { return (type == Type::Integer32) ? value._int : 0; }
		void setInt(int val) 
		{
			value._int = val;
			type = Type::Integer32;
		}

		long long getLong() { return (type == Type::Integer64) ? value._long : 0LL; }
		void setLong(long long val) 
		{
			value._long = val;
			type = Type::Integer64;
		}

		float getFloat() { return (type == Type::Float) ? value._float : 0.0f; }
		void setFloat(float val) 
		{ 
			value._float = val;
			type = Type::Float;
		}

		double getDouble() { return (type == Type::Double) ? value._double : 0.0; }
		void setDouble(double val)
		{
			value._double = val;
			type = Type::Double;
		}

		std::string getString() { return (type == Type::String) ? value._string : ""; }
		void setString(std::string val) 
		{
			value._string = val;
			type = Type::String;
		}

		PropertyValue& operator=(int val)
		{
			setInt(val);
			return *this;
		}

		PropertyValue& operator=(long long val)
		{
			setLong(val);
			return *this;
		}

		PropertyValue& operator=(float val)
		{
			setFloat(val);
			return *this;
		}

		PropertyValue& operator=(double val)
		{
			setDouble(val);
			return *this;
		}

		PropertyValue& operator=(std::string val)
		{
			setString(val);
			return *this;
		}
	};
}
