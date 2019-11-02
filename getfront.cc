#define NAPI_VERSION 3
#include <windows.h>
#include <assert.h>
#include <node_api.h>
#include <string>
#include <vector>

std::string utf8_encode(const std::wstring &wstr)
{
    if( wstr.empty() ) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte                  (CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}




napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value world;
  HWND handler = GetForegroundWindow();
  int len = GetWindowTextLengthW(handler) + 1;
  std::vector<wchar_t> buf(len);
  GetWindowTextW(handler, &buf[0], len);
  std::wstring wide = &buf[0];
  //std::string s(wide.begin(),wide.end());
  //size_t length = s.size();
  //char* char_arrays = new char [length];
  //strcpy(char_arrays, wide.c_str()); 
  


  std::string s = utf8_encode(wide);
  size_t length = s.size();
  char* char_arrays = new char [length];
  strcpy(char_arrays, s.c_str()); 
  

  
  
  
  status = napi_create_string_utf8(env,char_arrays, length, &world);
  assert(status == napi_ok);
  return world;
}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("gfwiname", Method);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)