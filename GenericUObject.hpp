#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class GenericUObject : public API::UObject
{
  public:
    API::UClass* mStaticClass;
    wchar_t mClassName[256];
    
	using API::UObject::get_full_name;

    // pass in a class like L"Class /Script/MediaAssets.MediaPlayer"
	static API::UClass *static_class_by_name(wchar_t* Name)
	{
		static API::UClass *result = API::get()->find_uobject<API::UClass>(Name);
		return result;
	}

    static GenericUObject *get_instance_at_with_skip(int index, wchar_t* name, wchar_t* skip)
    {
        auto klass = GenericUObject::static_class_by_name(name);
        if (klass) 
        {
            std::vector<GenericUObject*> List = klass->get_objects_matching<GenericUObject>();

            // Remove elements that contain the substring skip
            List.erase(
                std::remove_if(List.begin(), List.end(),
                               [skip](GenericUObject* Object) {
                                   std::wstring ObjName = Object->get_full_name();
                                   return ObjName.find(skip) != std::wstring::npos;
                               }),
                List.end()
            );

            // Now check the index after removal
            if (index < List.size()) 
            {
                //GenericUObject* Object = List[index];
                //std::wstring ObjName = Object->get_full_name();
                //API::get()->log_info("GenericUObject: Returning Object %d of %d, Object name: %ls", index, List.size(), ObjName.c_str());

                return List[index];
            }
        }
        return nullptr;
    }

    // pass in a class like L"Class /Script.Endgame.Menu"
    static GenericUObject *get_instance_at(int index, wchar_t* name)
    {
		auto klass = GenericUObject::static_class_by_name(name);
		if(klass) 
        {
            std::vector<GenericUObject*> List = klass->get_objects_matching<GenericUObject>();
            if(index < List.size()) 
            {
                return List[index];
            }
		}
		return nullptr;
    }
    
    static void log_instances_of(wchar_t* name)
    {
		auto klass = GenericUObject::static_class_by_name(name);
        GenericUObject* Object = nullptr;
        
		if(klass) 
        {
            std::vector<GenericUObject*> List = klass->get_objects_matching<GenericUObject>();
            for(size_t i = 0; i < List.size(); i++) 
            {
                Object = List[i];
                
                std::wstring ObjName = Object->get_full_name();
                API::get()->log_info("GenericUObject: Object %d of %d, Object name: %ls",i, List.size(), ObjName.c_str());
            }
		}
    }
    
	static GenericUObject *get_instance()
	{
		auto klass = GenericUObject::static_class();
        GenericUObject* Object = nullptr;
        
		if(klass) {
            std::vector<GenericUObject*> List = klass->get_objects_matching<GenericUObject>();
            for(size_t i = 0; i < List.size(); i++) {
                Object = List[i];
                
                //std::wstring ObjName = Object->get_full_name();
                //API::get()->log_info("GenericUObject: Object %d of %d, Object name: %ls",i, List.size(), ObjName.c_str());
                
                // Skip anything with GEN_VARIABLE
                //if(ObjName.size() > 0 && ObjName.find(L"GenericUObjectInGame_C") != std::wstring::npos) {
                if(i = List.size() - 1) {
                    //API::get()->log_info("GenericUObject: returning instance %d, %ls", i, ObjName.c_str());
                    break;
                }
            }
			return Object;
		}
		return nullptr;
	}

    
	bool call_bool_function(wchar_t* FuncName, wchar_t* ClassName)
	{
		static const auto func = GenericUObject::static_class_by_name(ClassName)->find_function(FuncName);
		if(!func) 
        {
			return false;
		} 
        
        bool return_val = false;
        
        process_event(func, &return_val);

		return return_val;
	}

};