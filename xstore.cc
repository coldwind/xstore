#include <node.h>
#include <v8.h>
#include <map>

using namespace std;

namespace xstore {

    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Number;
    using v8::Boolean;

    map<string, string> sMap;
    map<string, double> nMap;
    map<string, v8::Local<v8::Object>> oMap;

    void Set(const FunctionCallbackInfo < Value > &args)
    {
        Isolate * isolate = args.GetIsolate();

        // check parameter
        if (args.Length() != 2)
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments, example:(key,value)")));
            return;
        }

        if (!args[0]->IsString())
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The first parameter must be string")));
            return;
        }

        v8::String::Utf8Value key(args[0]->ToString());
        string from = std::string(*key);

        // 校验KEY是否存在
        if (sMap.find(from) != sMap.end())
        {
            sMap.erase(from);
        }
        else if (nMap.find(from) != nMap.end())
        {
            nMap.erase(from);
        }

        if (args[1]->IsString())
        {
            v8::String::Utf8Value value(args[1]->ToString());
            string strVal = std::string(*value);
            sMap[from] = strVal;
        }
        else if (args[1]->IsNumber())
        {
            nMap[from] = args[1]->NumberValue();
        }
        else if (args[1]->IsObject())
        {
            // TODO 遍历OBJECT
            //oMap[from] = args[1]->ToObject();
        }
        else
        {
            args.GetReturnValue().Set(Boolean::New(isolate, false));
            return;
        }

        args.GetReturnValue().Set(Boolean::New(isolate, true));
    }

    void Get(const FunctionCallbackInfo < Value > &args)
    {
        Isolate * isolate = args.GetIsolate();

        // check parameter
        if (args.Length() != 1)
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments, example:(key)")));
            return;
        }

        if (!args[0]->IsString())
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The parameter must be string")));
            return;
        }

        v8::String::Utf8Value key(args[0]->ToString());
        string from = std::string(*key);

        if (sMap.find(from) != sMap.end())
        {
            args.GetReturnValue().Set(String::NewFromUtf8(isolate, (sMap.find(from)->second).c_str()));
        }
        else if (nMap.find(from) != nMap.end())
        {
            args.GetReturnValue().Set(Number::New(isolate, nMap.find(from)->second));
        }
        else if (oMap.find(from) != oMap.end())
        {
            // TODO
            //args.GetReturnValue().Set(oMap.find(from)->second);
        }
        else
        {
            args.GetReturnValue().Set(Boolean::New(isolate, false));
        }
    }

    void Del(const FunctionCallbackInfo < Value > &args)
    {
        Isolate * isolate = args.GetIsolate();

        // check parameter
        if (args.Length() != 1)
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments, example:(key)")));
            return;
        }

        if (!args[0]->IsString())
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The parameter must be string")));
            return;
        }

        v8::String::Utf8Value key(args[0]->ToString());
        string from = std::string(*key);

        // 校验KEY是否存在
        if (sMap.find(from) != sMap.end())
        {
            sMap.erase(from);
            args.GetReturnValue().Set(Boolean::New(isolate, "true"));
        }
        else if (nMap.find(from) != nMap.end())
        {
            nMap.erase(from);
            args.GetReturnValue().Set(Boolean::New(isolate, "true"));
        }

        args.GetReturnValue().Set(Boolean::New(isolate, "false"));
    }

    void Init(Local <Object>exports)
    {
        NODE_SET_METHOD(exports, "set", Set);
        NODE_SET_METHOD(exports, "get", Get);
        NODE_SET_METHOD(exports, "del", Del);
    }

    NODE_MODULE(xstore, Init)
}
