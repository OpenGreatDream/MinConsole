﻿#pragma once

#include "MinDefines.h"
#include <string>

namespace MinConsoleNative
{
    extern const std::wstring DNULL;

    class Database
    {
    public:
        std::wstring savePath;

    public:
        Database();

        Database(const std::wstring& savePath);

        int GetInt(const std::wstring& key, int defaultVal);

        float GetFloat(const std::wstring& key, float defaultVal);

        bool GetBool(const std::wstring& key, bool defaultVal);

        std::wstring GetString(const std::wstring& key, const std::wstring& defaultVal);

        void SetInt(const std::wstring& key, int value);

        void SetFloat(const std::wstring& key, float value);

        void SetBool(const std::wstring& key, bool value);

        void SetString(const std::wstring& key, const std::wstring& value);
    };
}