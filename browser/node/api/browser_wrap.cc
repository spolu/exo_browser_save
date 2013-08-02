// Copyright (c) 2013 Stanislas Polu.
// See the LICENSE file.

#define BUILDING_NODE_EXTENSION

#include "breach/browser/node/api/browser_wrap.h"

#include "base/command_line.h"
#include "net/base/net_util.h"
#include "content/public/browser/browser_thread.h"
#include "breach/browser/ui/browser.h"
#include "breach/browser/breach_content_browser_client.h"
#include "breach/browser/breach_browser_context.h"

using namespace v8;

namespace breach {

/* TODO(spolu) Code check */
static GURL GetStartupURL() {
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  const CommandLine::StringVector& args = command_line->GetArgs();

  if (args.empty())
    return GURL("http://www.google.com/");

  GURL url(args[0]);
  if (url.is_valid() && url.has_scheme())
    return url;

  return net::FilePathToFileURL(base::FilePath(args[0]));
}

BrowserWrap::BrowserWrap()
{
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&BrowserWrap::CreateTask, this));

}

BrowserWrap::~BrowserWrap()
{
  LOG(INFO) << "BrowserWrap Destructor";

  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&BrowserWrap::DeleteTask, browser_));
}

void 
BrowserWrap::Init(
    Handle<Object> exports) 
{
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Browser"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("close"),
      FunctionTemplate::New(Close)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("showDevTools"),
      FunctionTemplate::New(ShowDevTools)->GetFunction());

  exports->Set(String::NewSymbol("Browser"), tpl->GetFunction());
}

void 
BrowserWrap::New(
    const v8::FunctionCallbackInfo<v8::Value>& args)
{
  HandleScope handle_scope(Isolate::GetCurrent());
  BrowserWrap* browser_wrap = new BrowserWrap();
  browser_wrap->Wrap(args.This());

  args.GetReturnValue().Set(args.This());
}

void
BrowserWrap::Close(
    const v8::FunctionCallbackInfo<v8::Value>& args)
{
  BrowserWrap* obj = ObjectWrap::Unwrap<BrowserWrap>(args.This());
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&BrowserWrap::CloseTask, obj));
}

void
BrowserWrap::ShowDevTools(
    const v8::FunctionCallbackInfo<v8::Value>& args)
{
  BrowserWrap* obj = ObjectWrap::Unwrap<BrowserWrap>(args.This());
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&BrowserWrap::ShowDevToolsTask, obj));
}

/******************************************************************************/
/*                                  TASKS                                     */
/******************************************************************************/
void
BrowserWrap::CreateTask()
{
  browser_ = Browser::CreateNewWindow(
                   BreachContentBrowserClient::Get()->browser_context(),
                   GetStartupURL(),
                   NULL,
                   MSG_ROUTING_NONE,
                   gfx::Size());
}

void
BrowserWrap::DeleteTask(
    Browser* browser)
{
  LOG(INFO) << "DeleteBrowserTask";
  if(!browser->IsClosed())
    /* will cause browser deletion */
    browser->Close();
}

void
BrowserWrap::CloseTask()
{
  if(!browser_->IsClosed())
    browser_->Close();
}

void
BrowserWrap::ShowDevToolsTask()
{
  if(!browser_->IsClosed())
    browser_->ShowDevTools();
}

    
} // namespace breach
