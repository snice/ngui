/* ***** BEGIN LICENSE BLOCK *****
 * Distributed under the BSD license:
 *
 * Copyright (c) 2015, xuewen.chu
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of xuewen.chu nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL xuewen.chu BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END LICENSE BLOCK ***** */

#include <ngui/base/util.h>
#include <ngui/base/http.h>
#include <ngui/base/string.h>
#include <ngui/base/fs.h>

using namespace ngui;

class MyClient: public HttpClientRequest, HttpClientRequest::Delegate {
public:
  MyClient(): HttpClientRequest( ), count(0) {
    set_delegate(this);
  }
  
  virtual void trigger_http_error(HttpClientRequest* req, cError& error) {
    LOG("trigger_http_error, %s", *error.message());
  }
  virtual void trigger_http_write(HttpClientRequest* req) {
    LOG("Write, %d/%d, %d/%d", download_size(), download_total(), upload_size(), upload_total());
  }
  virtual void trigger_http_header(HttpClientRequest* req) {
    LOG("Header: %d", status_code());
    for ( auto& i : get_all_response_headers() ) {
      LOG("  %s: %s", i.key().c(), i.value().c());
    }
    LOG("");
  }
  virtual void trigger_http_data(HttpClientRequest* req, Buffer buffer) {
    LOG("Read, %d/%d, %d/%d", download_size(), download_total(), upload_size(), upload_total());
    LOG( String(buffer.value(), buffer.length()) );
  }
  virtual void trigger_http_end(HttpClientRequest* req) {
    LOG("http_end, status: %d, %s", status_code(), url().c());
    // LOG( FileHelper::read_file_sync(Path::documents("http.cc")) );
    release();
    //RunLoop::current()->stop();
  }
  virtual void trigger_http_readystate_change(HttpClientRequest* req) {
    LOG( "http_readystate_change, %d", ready_state() );
  }
  virtual void trigger_http_timeout(HttpClientRequest* req) {
    LOG( "trigger_http_timeout" );
  }
  virtual void trigger_http_abort(HttpClientRequest* req) {
    LOG( "trigger_http_abort" );
  }
  
  int count;
  
};

void test_http2() {
  
  MyClient* cli = new MyClient();
  
  String url = "https://www.baidu.com";
  cli->set_method(HTTP_METHOD_GET);
  cli->set_url(url);
  //cli->set_keep_alive(false);
  //req->disable_cache(true);
  //req->disable_cookie(true);
  //req->set_save_path(Path::documents("http.cc"));
  cli->send();
  
  RunLoop::current()->run();
}

