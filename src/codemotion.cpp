#include <node.h>
#include <unistd.h>

using namespace v8;

// Simple task ---------------------------------------------------------------

Handle<Value> SimpleTask(const Arguments& args) {
    HandleScope scope;

    Local<Integer> integer = args[0]->ToInteger();

    sleep(integer->Value());

    return scope.Close(Integer::New(42));
}


// Callback ------------------------------------------------------------------

Handle<Value> CallbackTask(const Arguments& args) {
    HandleScope scope;

    Local<Function> callback = Local<Function>::Cast(args[1]);

    Local<Integer> integer = args[0]->ToInteger();
    sleep(integer->Value());

    const unsigned argc = 2;
    Local<Value> argv[argc] = {
        Local<Value>::New(Null()),
        Local<Value>::New(Integer::New(42))
    };

    callback->Call(Context::GetCurrent()->Global(), argc, argv);

    return Undefined();
}


// Async ---------------------------------------------------------------------

Handle<Value> AsyncTask(const Arguments& args);
void AsyncTaskWork(uv_work_t* req);
void AsyncTaskAfterWork(uv_work_t* req);

struct AsyncTaskBaton {
    Persistent<Function> callback;
    int32_t seconds;
    int32_t result;
};

Handle<Value> AsyncTask(const Arguments& args) {
    HandleScope scope;

    Local<Function> callback = Local<Function>::Cast(args[1]);

    Local<Integer> integer = args[0]->ToInteger();

    AsyncTaskBaton* asyncTaskBaton = new AsyncTaskBaton();
    asyncTaskBaton->seconds = integer->Value();
    asyncTaskBaton->callback = Persistent<Function>::New(callback);

    uv_work_t *req = new uv_work_t();
    req->data = asyncTaskBaton;

    int status = uv_queue_work(uv_default_loop(), req, AsyncTaskWork, (uv_after_work_cb)AsyncTaskAfterWork);
    assert(status == 0);

    return Undefined();
}

void AsyncTaskWork(uv_work_t* req) {
    AsyncTaskBaton* asyncTaskBaton = static_cast<AsyncTaskBaton*>(req->data);

    sleep(asyncTaskBaton->seconds);
    asyncTaskBaton->result = 42;
}

void AsyncTaskAfterWork(uv_work_t* req) {
    HandleScope scope;
    AsyncTaskBaton* asyncTaskBaton = static_cast<AsyncTaskBaton*>(req->data);

    const unsigned argc = 2;
    Local<Value> argv[argc] = {
        Local<Value>::New(Null()),
        Local<Value>::New(Integer::New(asyncTaskBaton->result))
    };

    TryCatch try_catch;
    asyncTaskBaton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
    if (try_catch.HasCaught()) {
        node::FatalException(try_catch);
    }

    asyncTaskBaton->callback.Dispose();

    delete asyncTaskBaton;
    delete req;
}


// Register module functions -------------------------------------------------

void RegisterModule(Handle<Object> target) {
    target->Set(String::NewSymbol("simpleTask"),
        FunctionTemplate::New(SimpleTask)->GetFunction());

    target->Set(String::NewSymbol("callbackTask"),
        FunctionTemplate::New(CallbackTask)->GetFunction());

    target->Set(String::NewSymbol("asyncTask"),
        FunctionTemplate::New(AsyncTask)->GetFunction());
}



NODE_MODULE(codemotion, RegisterModule);
