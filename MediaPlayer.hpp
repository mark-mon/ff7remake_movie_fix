#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class MediaPlayer : public API::UObject
{
  public:
	using API::UObject::get_full_name;

	static API::UClass *static_class()
	{
		static API::UClass *result = nullptr;
		if(!result) {
			result = API::get()->find_uobject<API::UClass>(L"Class /Script/MediaAssets.MediaPlayer");
		}
		return result;
	}

	static MediaPlayer *get_instance()
	{
		auto klass = MediaPlayer::static_class();
        MediaPlayer* Object = nullptr;
        
		if(klass) {
            std::vector<MediaPlayer*> List = klass->get_objects_matching<MediaPlayer>();
            for(size_t i = 0; i < List.size(); i++) {
                Object = List[i];
                
                std::wstring ObjName = Object->get_full_name();
                //API::get()->log_info("MediaPlayer: Object %d of %d, Object name: %ls",i, List.size(), ObjName.c_str());
                
                // Skip anything with GEN_VARIABLE
                //if(ObjName.size() > 0 && ObjName.find(L"MenuInGame_C") != std::wstring::npos) {
                if(i = List.size() - 1) {
                    //API::get()->log_info("MediaPlayer: returning instance %d, %ls", i, ObjName.c_str());
                    break;
                }
            }
			return Object;
		}
		return nullptr;
	}

	bool is_in_movie()
	{
        bool return_value = false;
        
		static const auto func = MediaPlayer::static_class()->find_function(L"IsPlaying");
		if(!func) {
			return false;
		} 
        
		struct
		{
			bool res;
		} params{0};

        process_event(func, &params);

		return params.res;
	}

};