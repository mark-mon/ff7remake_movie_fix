#define  _CRT_SECURE_NO_WARNINGS 1
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <memory>

#include "uevr/Plugin.hpp"
#include "MediaPlayer.hpp"
#include "main.h"



#define PAGE_MAP            0
#define PAGE_EQUIPMENT      1
#define PAGE_TALENTS        2
#define PAGE_JOURNAL        3
#define PAGE_SPELLS         4
#define PAGES               5


void DebugPrint(char* Format, ...);
using namespace uevr;

#define PLUGIN_LOG_ONCE(...) \
    static bool _logged_ = false; \
    if (!_logged_) { \
        _logged_ = true; \
        API::get()->log_info(__VA_ARGS__); \
    }
    
class FF7MovieFix : public uevr::Plugin {
public:
    FF7MovieFix() = default;
	
    const UEVR_PluginInitializeParam* m_Param;
    const UEVR_VRData* m_VR;
	
    void on_dllmain() override {
    }

    void on_initialize() override {
      // This shows how to get to the API functions.
      m_Param = API::get()->param();
      m_VR = m_Param->vr;
    }

    //*******************************************************************************************
    // This is the controller input routine. Everything happens here.
    //*******************************************************************************************
    void on_xinput_get_state(uint32_t* retval, uint32_t user_index, XINPUT_STATE* state) 
    {
    }
    
    void on_pre_engine_tick(API::UGameEngine* engine, float delta) 
    {
        
        static bool RenderingMovie = false;

        bool IsInMovie = false;
        const auto MP = MediaPlayer::get_instance();
        if(MP && MP->is_in_movie()) {
            IsInMovie = true;
        }
        //API::get()->log_info("**Currently In Cinematic: %d", IsInMovie);
        if(IsInMovie == true && RenderingMovie == false)
        {
            RenderingMovie = true;
            API::get()->log_info("**Currently In Cinematic: %d", IsInMovie);
            m_VR->set_mod_value("VR_RenderingMethod", "1");
            m_VR->set_mod_value("VR_2DScreenMode", "true");
        }
        else if(IsInMovie == false && RenderingMovie == true)
        {
            RenderingMovie = false;
            API::get()->log_info("**Currently In Cinematic: %d", IsInMovie);
            m_VR->set_mod_value("VR_RenderingMethod", "0");
            m_VR->set_mod_value("VR_2DScreenMode", "false");
        }
    }
    
};
// Actually creates the plugin. Very important that this global is created.
// The fact that it's using std::unique_ptr is not important, as long as the constructor is called in some way.
std::unique_ptr<FF7MovieFix> g_plugin{new FF7MovieFix()};

void DebugPrint(char* Format, ...)
{
  char FormattedMessage[512];    
  va_list ArgPtr = NULL;  
  
  /* Generate the formatted debug message. */        
  va_start(ArgPtr, Format);        
  vsprintf(FormattedMessage, Format, ArgPtr);        
  va_end(ArgPtr); 

  OutputDebugString(FormattedMessage);
}

