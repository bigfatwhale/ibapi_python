#pragma once

class TWSAPIDLLEXP SoftDollarTier
{
	std::string m_name, m_val, m_displayName;

public:
	SoftDollarTier(const std::string& name = "", const std::string& val = "", const std::string& displayName = "");

	std::string name() const;
	std::string val() const;
	std::string displayName() const;
    static void export_to_python();
};

