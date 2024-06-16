#pragma once
#include "SKSEMenuFramework.h"
#include "Configuration.h"
namespace UI {
    void Register();
    namespace Example1 {
        inline uint32_t AddFormId = 0x64B33;
        inline RE::TESBoundObject* AddBoundObject = nullptr;
        void LookupForm();
        void __stdcall Render();
    }
    namespace Example2{
        void __stdcall Render();
        void __stdcall RenderWindow();
        inline MENU_WINDOW ExampleWindow;
    }
    namespace Example3 {
        void __stdcall Render();
    }
    namespace Example4 {
        inline std::string TitleText = "This is an " + FontAwesome::UnicodeToUtf8(0xf2b4) + " Font Awesome usage example";
        inline std::string Button1Text = FontAwesome::UnicodeToUtf8(0xf0e9) + " Umbrella";
        inline std::string Button2Text = FontAwesome::UnicodeToUtf8(0xf06e) + " Eye";
        void __stdcall Render();
    }
};